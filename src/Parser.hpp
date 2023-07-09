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
        } NodeKind;

        struct Node{
            NodeKind kind;
            Node *lhs;
            Node *rhs;
            T val;
        };

        Node *node;

        // Token head;
        // Token *token;
        // char *user_input;
        


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
            expr    = mul ('+' mul | '-' mul)*
            mul     = primary ('*' primary | '/' primary)*
            primary = num | '(' expr ')'
        */
        // define the expr
        Node *expr(){
            Node *node_expr = mul();
            for(;;){
                if(this->consume('+'))
                    node_expr = new_node(ND_ADD, node_expr, mul());
                else if(this->consume('-'))
                    node_expr = new_node(ND_SUB, node_expr, mul());
                else
                    return node_expr;
            }
        }

        // define the mul
        Node *mul(){
            Node *node_mul = primary();
            for(;;){
                if(this->consume('*'))
                    node_mul = new_node(ND_MUL, node_mul, primary());
                else if(this->consume('/'))
                    node_mul = new_node(ND_DIV, node_mul, primary());
                else
                    return node_mul;
            }
        }

        // define the primary
        Node *primary(){
            if(this->consume('(')){
                Node *node_primary = expr();
                this->expect(')');
                return node_primary;
            }
            return new_node_num(this->expect_number());
        }

        void Recursive_Descent_Parsing(){
            node = expr();
        }

        // define the generator of the stack
        void gen(Node *node_in){
            if(node_in->kind == ND_NUM){
                std::cout<<"    push "<<node_in->val<<"\n";
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
            
            default:
                break;
            }

            std::cout<<"    push rax\n";
        }

        void stack_gen(){
            Recursive_Descent_Parsing();
            gen(node);
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
            delete node;
        }
};

#endif
