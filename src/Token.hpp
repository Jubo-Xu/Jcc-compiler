#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <cstdarg>
#include <cstring>

typedef enum{
    TK_OPERATOR,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;

//typedef struct Token Token;
struct Token{
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

template <typename T>
class TOKEN{
    protected:
        // typedef enum{
        //     TK_OPERATOR,
        //     TK_NUM,
        //     TK_EOF,
        // } TokenKind;

        // //typedef struct Token Token;
        // struct Token{
        //     TokenKind kind;
        //     Token *next;
        //     T val;
        //     char *str;
        // };
        // Token head;
        // Token *token;
        // char *user_input;
        

    public:
        // typedef enum{
        //     TK_OPERATOR,
        //     TK_NUM,
        //     TK_EOF,
        // } TokenKind;

        // //typedef struct Token Token;
        // struct Token{
        //     TokenKind kind;
        //     Token *next;
        //     T val;
        //     char *str;
        // };
        Token head;
        Token *token;
        char *user_input;
        int pos = 0;
        TOKEN(char *p){
            user_input = p;
            head.next = NULL;
            Token *cur = &head;
            try{
            while(*p){
                if(std::isspace(*p)){
                    p++;
                    continue;
                }

                if(*p == ';'){
                    Token *tok = new Token();
                    tok->kind = TK_OPERATOR;
                    tok->str = p;
                    tok->len = 1;
                    cur->next = tok;
                    cur = cur->next;
                    p++;
                    continue;
                }
                
                if(*p == '+' || *p == '-'|| *p == '*' || *p == '/' || *p == '(' || *p == ')'){
                    Token *tok = new Token();
                    tok->kind = TK_OPERATOR;
                    tok->str = p;
                    tok->len = 1;
                    cur->next = tok;
                    cur = cur->next;
                    p++;
                    continue;
                }

                if((*p == '=' && *(p+1) == '=')|| (*p == '!' && *(p+1) == '=') || (*p == '>' && *(p+1) == '=') || (*p == '<' && *(p+1) == '=')){
                    Token *tok = new Token();
                    tok->kind = TK_OPERATOR;
                    tok->str = p;
                    tok->len = 2;
                    cur->next = tok;
                    cur = cur->next;
                    p+=2;
                    continue;
                }
                
                if(*p == '<' || *p == '>'){
                    Token *tok = new Token();
                    tok->kind = TK_OPERATOR;
                    tok->str = p;
                    tok->len = 1;
                    cur->next = tok;
                    cur = cur->next;
                    p++;
                    continue;
                }

                if('a' <= *p && *p <= 'z'){
                    Token *tok = new Token();
                    tok->kind = TK_IDENT;
                    tok->str = p;
                    tok->len = 1;
                    cur->next = tok;
                    cur = cur->next;
                    p++;
                    continue;
                }

                if(*p == '='){
                    Token *tok = new Token();
                    tok->kind = TK_OPERATOR;
                    tok->str = p;
                    tok->len = 1;
                    cur->next = tok;
                    cur = cur->next;
                    p++;
                    continue;
                }

                

                

                if(std::isdigit(*p)){
                    Token *tok = new Token();
                    tok->kind = TK_NUM;
                    tok->str = p;
                    cur->next = tok;
                    cur = cur->next;
                    cur->val = std::strtol(p, &p, 10);
                    continue;
                }
                error("Error: cannot tokenize\n");
            }} catch (const std::exception& e) {
              std::cerr << e.what() << std::endl;
              exit(1);
            }
            Token *tok = new Token();
            tok->kind = TK_EOF;
            tok->str = p;
            cur->next = tok;
            cur = cur->next;

            token = head.next;
        }

        template<typename... Args>
        void error(const char* fmt, Args&&... args) {
            std::ostringstream oss;
            ((oss << args), ...);
            throw std::runtime_error(std::string(fmt) + oss.str());
        }

        void error_at(const char* loc, const char* fmt, ...) {
            va_list ap;
            va_start(ap, fmt);

            //int pos = loc - user_input;

            // Print the user_input string
            std::cerr << user_input << std::endl;

            // Print spaces up to the error location
            //std::cerr << std::string(pos, ' ');
            for (int i = 0; i < pos; i++) {
                std::cerr << ' ';
            }

            // Print an arrow marker (^) at the error location
            std::cerr << "^ ";

            // Print the error message using the provided format and arguments
            vfprintf(stderr, fmt, ap);
            std::cerr << std::endl;
            // Cleanup
            va_end(ap);

            // Exit the program with a non-zero status code to indicate an error
            std::exit(1);
        }

        bool consume_old(char op){
            if(token->kind != TK_OPERATOR || token->str[0] != op)
                return false;
            token = token->next;
            pos++;
            return true;
        }
        // bool consume(char *op){
        //     if(token->kind != TK_OPERATOR || (sizeof(*op)/sizeof(char)) != token->len
        //         || std::memcmp(token->str, op, token->len))
        //         return false;
        //     token = token->next;
        //     pos++;
        //     return true; 
        // }
        bool consume(char *op){
            if(token->len == 1){
                if(token->kind != TK_OPERATOR || (sizeof(*op)/sizeof(char)) != token->len || std::memcmp(token->str, op, token->len)){
                    return false;}
                token = token->next;
                pos++;
                return true; 
            }
            else{
                if(token->kind != TK_OPERATOR || (sizeof(*op)/sizeof(char)-1) != token->len || ((token->str[0] != op[0]) && (token->str[1] != op[1]))){
                    return false;}
                token = token->next;
                pos++;
                return true; 
            }
            // token = token->next;
            // pos++;
            // return true; 
        }

        bool consume_str(std::string op){
            if(token->kind != TK_OPERATOR || op.size() != token->len || (token->str[0] != op[0]))
                return false;
            token = token->next;
            pos++;
            return true;
        }

        char *consume_ident(){
            char *out;
            if(token->kind != TK_IDENT || token->len != 1){
                out[0] = '0';
                return out;
            }
            out[0] = token->str[0];
            token = token->next;
            pos++;
            return out;
        }

        void expect(char op){
            // try {
            //     if(token->kind != TK_OPERATOR || token->str[0] != op)
            //         error("Error: not equal to %c\n", op);
            //         error_at(token->str, "value not correct");
            //     token = token->next;
                
            // } catch (const std::exception& e) {
            //   std::cerr << e.what() << std::endl;
            // }
            // pos++;
            if(token->kind != TK_OPERATOR || token->str[0] != op)
                error_at(token->str, "value not correct");
            token = token->next;
            pos++;
        }

        T expect_number(){
            // try {
            //     if(token->kind != TK_NUM)
            //         error("Error: not a number\n");
            //         error_at(token->str, "not a number");
            // } catch (const std::exception& e) {
            //   std::cerr << e.what() << std::endl;
            // }
            // T val = token->val;
            // token = token->next;
            // pos++;
            // return val;
            if(token->kind != TK_NUM)
                error_at(token->str, "not a number");
            T val = token->val;
            token = token->next;
            pos++;
            return val;
        }

        bool at_eof(){
            return token->kind == TK_EOF;
        }

        void Execute(){
            std::cout<<"    mov rax, "<<expect_number()<<std::endl;

            while(!at_eof()){
                if(consume('+')) {
                    std::cout<<"    add rax, "<<expect_number()<<std::endl;
                    continue;
                }
                expect('-');
                std::cout<<"    sub rax, "<<expect_number()<<std::endl;
            }

            std::cout<<"    ret\n";
        }

         ~TOKEN(){
            //delete &head;
            delete token;
        }

};


#endif