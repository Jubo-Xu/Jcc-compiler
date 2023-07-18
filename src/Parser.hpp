#ifndef __PARSER_HPP__
#define __PARSER_HPP__`

#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <cstdarg>
#include "Token.hpp"
#include "utils.hpp"

template <typename T>
class PARSER : public TOKEN<T>{
    protected: 
        typedef enum{
            ND_ADD, // +
            ND_SUB, // -
            ND_MUL, // *
            ND_DIV, // /
            ND_NUM, // number
            ND_EQUAL, // ==
            ND_NOTEQUAL, // !=
            ND_LARGER, // >
            ND_SMALLER, // <
            ND_LARGEREQ, // >=
            ND_SMALLEREQ, // <=
            ND_ASSIGN, // =
            ND_LVAR, // left variable
            ND_RETURN, // return
            ND_IF, // if
            ND_ELSE, // else
            ND_WHILE, // while
            ND_FOR, // for
            ND_BLOCK, // {}
            // ND_BLOCK_START, // {
            // ND_BLOCK_END, // }
            ND_DEREF, // *
            ND_ADDR, // &
        } NodeKind;

        struct Node{
            NodeKind kind;
            NodeKind kind_2;
            Node *lhs;
            Node *rhs;
            Node *for_mid;
            Node *for_rhs;
            Node *block_node;
            Node *block_next;
            T val;
            int offset;
        };

        Node *node;

        // Token head;
        // Token *token;
        // char *user_input;
        Node *code[100];

        int end = 0;
        int if_count = 0;
        int else_count = 0;
        int while_count = 0;
        int for_count = 0;
        


    public:
        
        Node *new_node(NodeKind kind, Node *lhs, Node *rhs){
            Node *node_out = new Node();
            node_out->kind = kind;
            node_out->lhs = lhs;
            node_out-> rhs = rhs;
            
            return node_out;
        }

        Node *new_node_num(int val){
            Node *node_out = new Node();
            node_out->kind = ND_NUM;
            node_out->val = val;
            return node_out;
        }

        void new_node_block(Node *block, Node *next){
            Node *node_out = new Node();
            node_out->kind_2 = ND_BLOCK;
            node_out->block_node = block;
            next->block_next = node_out;
        }

        PARSER(char *p):TOKEN<T>(p){
            // this->user_input = p;
            // this->head.next = NULL;
            // Token *cur = &this->head;
            // try{
            // while(*p){
            //     if(std::isspace(*p)){
            //         p++;
            //         continue;
            //     }
            //     if(*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')'){
            //         Token *tok = new Token();
            //         tok->kind = TK_OPERATOR;
            //         tok->str = p;
            //         cur->next = tok;
            //         cur = cur->next;
            //         p++;
            //         continue;
            //     }

            //     if(std::isdigit(*p)){
            //         Token *tok = new Token();
            //         tok->kind = TK_NUM;
            //         tok->str = p;
            //         cur->next = tok;
            //         cur = cur->next;
            //         cur->val = std::strtol(p, &p, 10);
            //         continue;
            //     }
            //     this->error("Error: cannot tokenize\n");
            // }} catch (const std::exception& e) {
            //   std::cerr << e.what() << std::endl;
            //   exit(1);
            // }
            // Token *tok = new Token();
            // tok->kind = TK_EOF;
            // tok->str = p;
            // cur->next = tok;
            // cur = cur->next;

            // this->token = this->head.next;
        }

        //////////////////////////////////////////////////////
        //////////// RECURSIVE DESCENT PARSING ///////////////
        /*------- EBNF of recursive grammars with precedence parentheses --------
            program    = stmt*
            stmt       = expr ";" | "return" expr ";"
                         | "if" "(" expr ")" stmt ("else" stmt)?
                         | "while" "(" expr ")" stmt
                         | "for" "(" expr? ";" expr? ";" expr? ")" stmt
                         | "{" stmt* "}"
            expr       = assign
            assign     = equality ("=" assign)?
            equality   = relational ("==" relational | "!=" relational)*
            relational = add ("<" add | "<=" add | ">" add | ">=" add)*
            add        = mul ("+" mul | "-" mul)*
            mul        = unary ("*" unary | "/" unary)*
            unary      = ("+" | "-")? primary
                         | "*" unary
                         | "&" unary
            primary    = num | ident | "(" expr ")"
        */
        template <typename Func>
        Node *function(char op_1, char op_2, Func input_function){
            Node *node_op = input_function();
            for(;;){
                if(this->consume(op_1))
                    node_op = new_node(ND_ADD, node_op, input_function());
                else if(this->consume(op_2))
                    node_op = new_node(ND_SUB, node_op, input_function());
                else
                    return node_op;
            }
        }

        // define the program
        void program(){
            int i = 0;
            while(!this->at_eof())
                code[i++] = stmt();
            code[i] = NULL;
            end = i;
            //std::cerr<<"number of nodes: "<<i<<"\n";
        }
        // define the stmt
        // Node *stmt(){
        //     Node *node_stmt = expr();
        //     this->expect(';');
        //     return node_stmt;
        // }
        // Node *stmt(){
        //     Node *node_stmt;
        //     if(this->consume_return()){
        //         node_stmt = new Node();
        //         node_stmt->kind = ND_RETURN;
        //         node_stmt->lhs = expr();
        //     }
        //     else{
        //         node_stmt = expr();
        //     }
        //     this->expect(';');
        //     return node_stmt;
        // }
        Node *stmt(){
            Node *node_stmt;
            if(this->consume_return()){
                node_stmt = new Node();
                node_stmt->kind = ND_RETURN;
                node_stmt->lhs = expr();
                this->expect(';');
                //std::cerr<<"return get\n";
            }
            else if(this->consume_if()){
                this->expect('(');
                node_stmt = new Node();
                node_stmt->kind = ND_IF;
                node_stmt->lhs = expr();
                this->expect(')');
                Node *node_stmt_rhs = stmt();
                if(this->consume_else()){
                    node_stmt_rhs = new_node(ND_ELSE, node_stmt_rhs, stmt());
                }
                node_stmt->rhs = node_stmt_rhs;   
            }
            else if(this->consume_while()){
                this->expect('(');
                node_stmt = new Node();
                node_stmt->kind = ND_WHILE;
                node_stmt->lhs = expr();
                this->expect(')');
                node_stmt->rhs = stmt();
            }
            else if(this->consume_for()){
                this->expect('(');
                node_stmt = new Node();
                node_stmt->kind = ND_FOR;
                node_stmt->lhs = expr();
                this->expect(';');
                //Node *node_stmt_lhs = node_stmt->lhs;
                //node_stmt_lhs->lhs = expr();
                node_stmt->for_mid = expr();
                this->expect(';');
                //node_stmt_lhs->rhs = expr();
                node_stmt->for_rhs = expr();
                //this->expect(';');
                this->expect(')');
                node_stmt->rhs = stmt();
            }
            else if(this->consume_old('{')){
                node_stmt = new Node();
                node_stmt->kind = ND_BLOCK;
                // Node *node_tmp = new Node();
                // node_tmp = node_stmt->block_next;
                // node_tmp->kind_2 = ND_BLOCK;
                Node *node_tmp = node_stmt;
                node_tmp->kind_2 = ND_BLOCK;
                // node_tmp->kind_2 = ND_BLOCK;
                while(!this->consume_old('}')){
                    // node_tmp->kind_2 = ND_BLOCK;
                    // node_tmp->block = stmt();
                    // node_tmp = node_tmp->block;
                    new_node_block(stmt(), node_tmp);
                    node_tmp = node_tmp->block_next;
                    //node_tmp = new_node_block(stmt(), node_tmp);
                    //std::cerr<<"expression finished\n";
                }
                //std::cerr<<"get out of the while loop\n";
                // Node *check = node_stmt;
                // int test = 0;
                // while(check){
                    
                //     check = check->block_next;
                //     std::cerr<<"check whether node_stmt is null: "<<test<<"\n";
                //     test++;
                // }
            }
            // else if(this->consume_old('{')){
            //     node_stmt = new Node();
            //     node_stmt->kind = ND_BLOCK_START;
            //     node_stmt->rhs = stmt();
            // }

            // else if(this->consume_old('}')){
            //     node_stmt = new Node();
            //     node_stmt->kind = ND_BLOCK_END;
            // }
            else{
                node_stmt = expr();
                this->expect(';');
            }
            
            return node_stmt;
        }
       
        // define the expr
        // Node *expr(){
        //     Node *node_expr = mul();
        //     for(;;){
        //         if(this->consume('+'))
        //             node_expr = new_node(ND_ADD, node_expr, mul());
        //         else if(this->consume('-'))
        //             node_expr = new_node(ND_SUB, node_expr, mul());
        //         else
        //             return node_expr;
        //     }
        // }
        Node *expr(){
            return assign();
        }

        // define the assign
        Node *assign(){
            Node *node_assign = equality();
            char equal = '=';
            if(this->consume(&equal))
                node_assign = new_node(ND_ASSIGN, node_assign, assign());
            return node_assign;
        }

        // define the equality
        Node *equality(){
            Node *node_equality = relational();
            for(;;){
                //char equal[2] = {'=', '='};
                //char notequal[2] = {'!', '='};
                char *equal = "==";
                char *notequal = "!=";
                // std::string equal = "==";
                // std::string notequal = "!=";
                if(this->consume_str(equal))
                    node_equality = new_node(ND_EQUAL, node_equality, relational());
                else if(this->consume_str(notequal))
                    node_equality = new_node(ND_NOTEQUAL, node_equality, relational());
                else
                    return node_equality;
            }
        }

        // define the relational
        Node *relational(){
            Node *node_relational = add();
            for(;;){
                char smaller = '<';
                //char smaller_eq[2] = {'<', '='};
                char larger = '>';
                //char larger_eq[2] = {'>', '='};
                std::string smaller_eq = ">=";
                std::string larger_eq = "<=";
                if(this->consume(&smaller))
                    node_relational = new_node(ND_SMALLER, node_relational, add());
                else if(this->consume_str(smaller_eq))
                    node_relational = new_node(ND_SMALLEREQ, node_relational, add());
                else if(this->consume(&larger))
                    node_relational = new_node(ND_LARGER, node_relational, add());
                else if(this->consume_str(larger_eq))
                    node_relational = new_node(ND_LARGEREQ, node_relational, add());
                else
                    return node_relational;
            }
        }

        // define the add
        Node *add(){
            Node *node_add = mul();
            for(;;){
                char plus = '+';
                char minus = '-';
                if(this->consume(&plus))
                    node_add = new_node(ND_ADD, node_add, mul());
                else if(this->consume(&minus))
                    node_add = new_node(ND_SUB, node_add, mul());
                else
                    return node_add;
            }
        }

        // Node *expr(){
        //     //return function(mul);
        //     return function('+', '-', [this]() { return mul(); });
        // }

        // define the mul
        // Node *mul(){
        //     Node *node_mul = primary();
        //     for(;;){
        //         if(this->consume('*'))
        //             node_mul = new_node(ND_MUL, node_mul, primary());
        //         else if(this->consume('/'))
        //             node_mul = new_node(ND_DIV, node_mul, primary());
        //         else
        //             return node_mul;
        //     }
        // }
        Node *mul(){
            Node *node_mul = unary();
            for(;;){
                char mul = '*';
                char div = '/';
                if(this->consume(&mul))
                    node_mul = new_node(ND_MUL, node_mul, unary());
                else if(this->consume(&div))
                    node_mul = new_node(ND_DIV, node_mul, unary());
                else
                    return node_mul;
            }
        }
        // Node *mul(){
        //     //return function(unary);
        //     return function('*', '/', [this]() { return unary(); });
        // }


        // define the unary
        Node *unary(){
            char plus = '+';
            char minus = '-';
            char deref = '*';
            if(this->consume(&plus))
                return primary();
            if(this->consume(&minus))
                return new_node(ND_SUB, new_node_num(0), primary());
            // if(this->consume_deref()){
            //     std::cerr<<"getting into the deref\n";
            //     Node *node_unary = new Node();
            //     node_unary->kind = ND_DEREF;
            //     node_unary->lhs = unary();
            //     return node_unary;
            // }
            if(this->consume(&deref)){
                Node *node_unary = new Node();
                node_unary->kind = ND_DEREF;
                node_unary->lhs = unary();
                return node_unary;
            }
            if(this->consume_addr()){
                Node *node_unary = new Node();
                node_unary->kind = ND_ADDR;
                node_unary->lhs = unary();
                return node_unary;
            }
            return primary();
        }

        // define the primary
        Node *primary(){
            char left = '(';
            char right = ')';
            if(this->consume(&left)){
                Node *node_primary = expr();
                this->expect(right);
                return node_primary;
            }
            // char *ident = this->consume_ident();
            // if(ident[0] != '0'){
            //     Node *node_primary_2 = new Node();
            //     node_primary_2->kind = ND_LVAR;
            //     //node_primary_2->offset = (ident[0] - 'a' + 1) * 8;
            //     node_primary_2->offset = (*ident - 'a' + 1) * 8;
            //     //std::cerr<<"check: "<<node_primary_2->offset<<std::endl;
            //     return node_primary_2;
            // }

            if(this->check_ident()){
                Node *node_primary_2 = new Node();
                node_primary_2->kind = ND_LVAR;
                int check_off = this->find_lvar();
                //node_primary_2->offset = (!check_off) ? this->gen_new_lvar() : check_off;
                if(check_off == 0){
                    node_primary_2->offset = this->gen_new_lvar();
                }
                else{
                    node_primary_2->offset = check_off;
                }
                return node_primary_2;
            }
            //std::cerr<<"check: enter the number"<<std::endl;
            return new_node_num(this->expect_number());
            
        }

        void Recursive_Descent_Parsing(){
            //node = expr();
            program();
            //node = expr(expr(unary()));
            //return function([this]() { return function([this](){ return unary();}); });
        }

        // define the generator of the stack
        void gen_lval(Node *node_in){
            if(node_in->kind != ND_LVAR){
                //this->error("the variable at the left is not a left variable");
                std::cerr<<"the variable at the left is not a left variable\n";
                exit(1);
            }    
            std::cout<<"    mov rax, rbp\n";
            std::cout<<"    sub rax, "<<node_in->offset<<"\n";
            std::cout<<"    push rax\n";
        }
        void gen(Node *node_in){
            // if(node_in->kind == ND_NUM){
            //     std::cout<<"    push "<<node_in->val<<"\n";
            //     return;
            // }
            //std::cout<<"check: into gen"<<std::endl;
            // switch (node_in->kind)
            // {
            // case ND_NUM:
            //     std::cout<<"    push "<<node_in->val<<"\n";
            //     return;
            // case ND_LVAR:
            //     std::cout<<"check: into ND_LVAR"<<std::endl;
            //     gen_lval(node_in);
            //     std::cout<<"    pop rax\n";
            //     std::cout<<"    mov rax, [rax]\n";
            //     std::cout<<"    push rax\n";
            //     return;
            // case ND_ASSIGN:
            //     gen_lval(node_in->lhs);
            //     gen(node_in->rhs);
            //     std::cout<<"    pop rdi\n";
            //     std::cout<<"    pop rax\n";
            //     std::cout<<"    mov [rax], rdi\n";
            //     std::cout<<"    push rdi\n";
            //     return;
            // }
            if(node_in->kind == ND_NUM){
                std::cout<<"    push "<<node_in->val<<"\n";
                return;
            }
            if(node_in->kind == ND_LVAR){
                //std::cout<<"check: into ND_LVAR"<<std::endl;
                gen_lval(node_in);
                std::cout<<"    pop rax\n";
                std::cout<<"    mov rax, [rax]\n";
                std::cout<<"    push rax\n";
                return;
            }
            if(node_in->kind == ND_ASSIGN){
                gen_lval(node_in->lhs);
                gen(node_in->rhs);
                std::cout<<"    pop rdi\n";
                std::cout<<"    pop rax\n";
                std::cout<<"    mov [rax], rdi\n";
                std::cout<<"    push rdi\n";
                return;
            }
            if(node_in->kind == ND_RETURN){
                gen(node_in->lhs);
                std::cout<<"    pop rax\n";
                std::cout<<"    mov rsp, rbp\n";
                std::cout<<"    pop rbp\n";
                std::cout<<"    ret\n";
                return;
            }

            int if_count_2 = if_count;
            if(node_in->kind == ND_IF){
                if_count++;
                gen(node_in->lhs);
                std::cout<<"    pop rax\n";
                std::cout<<"    cmp rax, 0\n";
                Node *tmp = node_in->rhs;
                int else_count_2 = else_count;
                if(tmp->kind == ND_ELSE){
                    else_count++;
                    //std::cout<<"    je  .LelseXXX\n";
                    std::cout<<"    je  .Lelse"<<else_count_2<<"\n";
                    //std::cout<<"    je  .Lelse"<<if_count<<"\n";
                    gen(tmp->lhs);
                    //std::cout<<"    jmp .LendXXX\n";
                    std::cout<<"    jmp .Lendif"<<if_count_2<<"\n";
                    //std::cout<<"  .LelseXXX:\n";
                    std::cout<<"  .Lelse"<<else_count_2<<":\n";
                    //std::cout<<"  .Lelse"<<if_count<<":\n";
                    gen(tmp->rhs);
                    
                }
                else{
                    //std::cout<<"    je  .LendXXX\n";
                    std::cout<<"    je  .Lendif"<<if_count_2<<"\n";
                    gen(node_in->rhs);
                }
                //std::cout<<"  .LendXXX:\n";
                std::cout<<"  .Lendif"<<if_count_2<<":\n";
                return;
            }
            if(node_in->kind == ND_WHILE){
                while_count++;
                Start_of_while(while_count);
                gen(node_in->lhs);
                Main_of_while(while_count);
                gen(node_in->rhs);
                End_of_while(while_count);
                return;
            }

            if(node_in->kind == ND_FOR){
                for_count++;
                gen(node_in->lhs);
                Start_of_for(for_count);
                //Node *tmp_for = node_in->lhs;
                //gen(tmp_for->lhs);
                gen(node_in->for_mid);
                Main_of_for(for_count);
                gen(node_in->rhs);
                //gen(tmp_for->rhs);
                gen(node_in->for_rhs);
                End_of_for(for_count);
                return;
            }

            if(node_in->kind == ND_BLOCK){
                //Node *node_in_2 = node_in->block_next;
                Node *node_in_2 = node_in;
                //std::cerr<<"getting into the block\n";
                int block_itr = 0;
                // if(node_in->kind_2 == ND_BLOCK){
                //     std::cerr<<"kind_2 is block\n";
                // }
                // else{
                //     std::cerr<<"kind_2 is not block\n";
                // }
                while(node_in_2){
                    //std::cerr<<"getting into while loop\n";
                    // gen(node_in_2->lhs);
                    // gen(node_in_2->rhs);
                    if(block_itr==0){
                        block_itr++;
                        node_in_2 = node_in_2->block_next;
                        if(node_in_2 == NULL){
                            //std::cerr<<"now is NULL\n";
                        }
                        //std::cerr<<"finished first iter\n";
                    }
                    else{
                        gen(node_in_2->block_node);
                        //std::cerr<<"gen node finished\n";
                        node_in_2 = node_in_2->block_next;
                        // if(node_in_2 == NULL){
                        //     std::cerr<<"now is NULL\n";
                        // }
                        //gen(node_in_2);
                        //std::cerr<<"block iteration is: "<<block_itr<<"\n";
                        //return;
                        block_itr++;
                    }
                    
                }
                //std::cerr<<"get out of while\n";
                return;
            }

            if(node_in->kind == ND_ADDR){
                gen_lval(node_in->lhs);
                return;
            }

            if(node_in->kind == ND_DEREF){
                //std::cout<<"    pop rax\n";
                gen(node_in->lhs);
                End_of_deref();
                return;
            }
            // if(node_in->kind == ND_BLOCK_START){
                
            // }

            gen(node_in->lhs);
            gen(node_in->rhs);
            
            std::cout<<"    pop rdi\n";
            std::cout<<"    pop rax\n";

            switch (node_in->kind)
            {
            case ND_ADD:
                /* code */
                std::cout<<"    add rax, rdi\n";
                break;
            case ND_SUB:
                std::cout<<"    sub rax, rdi\n";
                break;
            case ND_MUL:
                std::cout<<"    imul rax, rdi\n";
                break;
            case ND_DIV:
                std::cout<<"    cqo\n";
                std::cout<<"    idiv rdi\n";
                break;
            case ND_EQUAL:
                std::cout<<"    cmp rax, rdi\n";
                std::cout<<"    sete al\n";
                std::cout<<"    movzb rax, al\n";
                break;
            case ND_NOTEQUAL:
                std::cout<<"    cmp rax, rdi\n";
                std::cout<<"    setne al\n";
                std::cout<<"    movzb rax, al\n";
                break;
            case ND_LARGER:
                std::cout<<"    cmp rdi, rax\n";
                std::cout<<"    setl al\n";
                std::cout<<"    movzb rax, al\n";
                break;
            case ND_LARGEREQ:
                std::cout<<"    cmp rax, rdi\n";
                std::cout<<"    setle al\n";
                std::cout<<"    movzb rax, al\n";
                break;
            case ND_SMALLER:
                std::cout<<"    cmp rax, rdi\n";
                std::cout<<"    setl al\n";
                std::cout<<"    movzb rax, al\n";
                break;
            case ND_SMALLEREQ:
                std::cout<<"    cmp rdi, rax\n";
                std::cout<<"    setle al\n";
                std::cout<<"    movzb rax, al\n";
                break;
            
            default:
                break;
            }

            

            std::cout<<"    push rax\n";

            
        }

        void stack_gen(){
            Recursive_Descent_Parsing();
            //gen(node);
            //int i=0;
            
            // while(code[i] != NULL){
            //     std::cerr<<"check: "<<i<<std::endl;
            //     gen(code[i++]);
            //     std::cout<<"    pop rax\n";
            // }
            //std::cerr<<"parsing is ok\n";
            for(int i=0; i<end; i++){
                //std::cerr<<"check: "<<i<<std::endl;
                gen(code[i]);
                std::cout<<"    pop rax\n";
                //std::cerr<<"finished expression "<<i<<"\n";
            }
            //std::cout<<"check: now is ok"<<std::endl;
            // gen(code[0]);
            // std::cout<<"    pop rax\n";
            
        }

        void delete_node(Node* node_in) {
            if (node_in == nullptr)
                return;

            delete_node(node_in->lhs);
            delete_node(node_in->rhs);

            delete node_in;
        }


        ~PARSER(){
            //delete this->token;
            //delete node;
            //delete_node(node);
            for(int i=0; i<end+1; i++){
                delete code[i];
            }
            //delete code;
            //delete node;
        }
};

#endif
