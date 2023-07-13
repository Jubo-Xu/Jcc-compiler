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
            ND_ASSIGN,
            ND_LVAR,
        } NodeKind;

        struct Node{
            NodeKind kind;
            Node *lhs;
            Node *rhs;
            T val;
            int offset;
        };

        Node *node;

        // Token head;
        // Token *token;
        // char *user_input;
        Node *code[100];

        int end = 0;
        


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
            stmt       = expr ";"
            expr       = assign
            assign     = equality ("=" assign)?
            equality   = relational ("==" relational | "!=" relational)*
            relational = add ("<" add | "<=" add | ">" add | ">=" add)*
            add        = mul ("+" mul | "-" mul)*
            mul        = unary ("*" unary | "/" unary)*
            unary      = ("+" | "-")? primary
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
        }
        // define the stmt
        Node *stmt(){
            Node *node_stmt = expr();
            this->expect(';');
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
            if(this->consume(&plus))
                return primary();
            if(this->consume(&minus))
                return new_node(ND_SUB, new_node_num(0), primary());
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
            char *ident = this->consume_ident();
            if(ident[0] != '0'){
                Node *node_primary_2 = new Node();
                node_primary_2->kind = ND_LVAR;
                node_primary_2->offset = (ident[0] - 'a' + 1) * 8;
                //std::cerr<<"check: "<<node_primary_2->offset<<std::endl;
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
            for(int i=0; i<end; i++){
                //std::cerr<<"check: "<<i<<std::endl;
                gen(code[i]);
                std::cout<<"    pop rax\n";
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
