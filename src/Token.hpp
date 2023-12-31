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
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_FOR,
    TK_ADDR,
} TokenKind;

//typedef struct Token Token;
struct Token{
    TokenKind kind;
    bool DEREF = true;
    Token *next;
    int val;
    char *str;
    int len;
};

struct LVar{
    LVar *next;
    char *name;
    int len;
    int offset;
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
        LVar head_lvar;
        LVar *locals;
        char *user_input;
        int pos = 0;
        int iter_of_rval = 0;

        bool is_alnum(char *p){
            if(('a' <= *p && *p <= 'z') ||
               ('A' <= *p && *p <= 'Z') ||
               (*p == '_')){
                    return true;
               } 
            else{
                return false;
            }
        }
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

                // if('a' <= *p && *p <= 'z'){
                //     Token *tok = new Token();
                //     tok->kind = TK_IDENT;
                //     tok->str = p;
                //     tok->len = 1;
                //     cur->next = tok;
                //     cur = cur->next;
                //     p++;
                //     continue;
                // }

                // if('a' <= *p && *p <= 'z'){
                //     Token *tok = new Token();
                //     tok->kind = TK_IDENT;
                //     tok->str = p;

                //     int length = 1;
                //     while('a'<=*(p+1) && *(p+1)<='z'){
                //         length++;
                //         p++;
                //     }
                //     tok->len = length;
                //     cur->next = tok;
                //     cur = cur->next;
                //     p++;
                //     continue;
                // }
                if(*p=='r' && *(p+1)=='e' && *(p+2)=='t' && *(p+3)=='u' && *(p+4)=='r' && *(p+5)=='n' && !is_alnum(p+6)){
                    Token *tok = new Token();
                    tok->kind = TK_RETURN;
                    tok->str = p;
                    tok->len = 6;
                    cur->next = tok;
                    cur = cur->next;
                    p+=6;
                    continue;

                }

                if(*p=='i' && *(p+1)=='f' && !is_alnum(p+2)){
                    Token *tok = new Token();
                    tok->kind = TK_IF;
                    tok->str = p;
                    // p++;
                    // while(std::isspace(*(p+1))){
                    //     p++;
                    //     continue;
                    // }
                    // if(*(p+1)!='('){
                    //     error("Error: cannot tokenize\n");
                    // }
                    // tok->len = 2;
                    // cur->next = tok;
                    // cur = cur->next;
                    // p++;
                    tok->len = 2;
                    cur->next = tok;
                    cur = cur->next;
                    p+=2;
                    continue;

                }

                if(*p=='e' && *(p+1)=='l' && *(p+2)=='s' && *(p+3)=='e' && !is_alnum(p+4)){
                    Token *tok = new Token();
                    tok->kind = TK_ELSE;
                    tok->str = p;
                    tok->len = 4;
                    cur->next = tok;
                    cur = cur->next;
                    p+=4;
                    continue;
                }

                if(*p=='w' && *(p+1)=='h' && *(p+2)=='i' && *(p+3)=='l' && *(p+4)=='e' && !is_alnum(p+5)){
                    Token *tok = new Token();
                    tok->kind = TK_WHILE;
                    tok->str = p;
                    tok->len = 5;
                    cur->next = tok;
                    cur = cur->next;
                    p+=5;
                    continue;
                }

                if(*p=='f' && *(p+1)=='o' && *(p+2)=='r' && !is_alnum(p+3)){
                    Token *tok = new Token();
                    tok->kind = TK_FOR;
                    tok->str = p;
                    tok->len = 3;
                    cur->next = tok;
                    cur = cur->next;
                    p+=3;
                    continue;
                }

                if(is_alnum(p)){
                    Token *tok = new Token();
                    tok->kind = TK_IDENT;
                    tok->str = p;

                    int length = 1;
                    while(is_alnum(p+1)){
                        length++;
                        p++;
                    }
                    tok->len = length;
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

                if(*p == '{' || *p == '}'){
                    Token *tok = new Token();
                    tok->kind = TK_OPERATOR;
                    tok->str = p;
                    tok->len = 1;
                    cur->next = tok;
                    cur = cur->next;
                    p++;
                    continue;
                }

                if(*p == '&' && is_alnum(p+1)){
                    Token *tok = new Token();
                    tok->kind = TK_ADDR;
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

            int pos_2 = loc - user_input;

            // Print the user_input string
            std::cerr << user_input << std::endl;

            // Print spaces up to the error location
            //std::cerr << std::string(pos, ' ');
            for (int i = 0; i < pos_2; i++) {
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

        void error_at_2(const char* fmt, ...) {
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
            if(token->kind != TK_IDENT){
                out[0] = '0';
                return out;
            }
            //out[0] = token->str[0];
            out = token->str;
            token = token->next;
            pos++;
            return out;
        }

        bool check_ident(){
            if(token->kind != TK_IDENT){
                return false;
            }
            Token *tmp = token->next;
            if(tmp->str[0] == '*'){
                tmp->DEREF = false;
            }
            // std::cerr<<"iteration is: "<<iter_of_rval<<std::endl;
            // std::cerr<<"check ident yes"<<std::endl;
            return true;
        }

        int find_lvar(){
            
            if(!iter_of_rval){
                //std::cerr<<"iteration is 0\n";
                return 0;
            }
            LVar *var = locals;
            // std::cerr<<"locals offset is: "<<var->offset<<std::endl;
            // //std::cerr<<"locals name is: "<<var->name<<std::endl;
            // std::cerr<<"locals len is: "<<var->len<<std::endl;
            // std::cerr<<"locals name is: "<<var->name<<std::endl;
            // std::cerr<<"tokens str is: "<<token->str<<std::endl;
            int check = 0;
            while(var){
                if(var->len==token->len && !std::memcmp(token->str, var->name, var->len)){
                    iter_of_rval++;
                    token = token->next;
                    pos++;
                    //std::cerr<<"not a new variable\n";
                    //std::cerr<<"check is: "<<check<<std::endl;
                    return var->offset;
                }
                check++;
                var = var->next;
            }
            //std::cerr<<"new variable\n";
            return 0;
        }

        int gen_new_lvar(){
            //std::cerr<<"getting into gen_new_lvar\n";
            if(!iter_of_rval){
                int ofset;
                locals = new LVar();
                locals->name = token->str;
                locals->len = token->len;
                locals->offset = (*(token->str) - 'a' + 1) * 8;
                ofset = locals->offset;
                token = token->next;
                pos++;
                iter_of_rval++;
                //std::cerr<<"offset of iteration 0 is: "<<ofset<<std::endl;
                return ofset;
            }
            else{
                int ofset;
                LVar *lvar = new LVar();
                lvar->next = locals;
                lvar->name = token->str;
                lvar->len = token->len;
                lvar->offset = locals->offset+8;
                //lvar->offset = (iter_of_rval==0) ? ((*(token->str) - 'a' + 1) * 8) : (locals->offset+8);
                //std::cerr<<"offset is: "<<lvar->offset<<std::endl;
                //lvar->offset = (*(token->str) - 'a' + 1) * 8;
                ofset = lvar->offset;
                locals = lvar;
                token = token->next;
                pos++;
                iter_of_rval++;
                return ofset;
            }
        }

        bool consume_return(){
            if(token->kind != TK_RETURN || token->len != 6)
                return false;
            token = token->next;
            pos++;
            return true;
        }

        bool consume_if(){
            if(token->kind != TK_IF || token->len != 2)
                return false;
            token = token->next;
            pos++;
            return true;
        }

        bool consume_else(){
            if(token->kind != TK_ELSE || token->len != 4)
                return false;
            token = token->next;
            pos++;
            return true;
        }

        bool consume_while(){
            if(token->kind != TK_WHILE || token->len != 5)
                return false;
            token = token->next;
            pos++;
            return true;
        }

        bool consume_for(){
            if(token->kind != TK_FOR || token->len != 3)
                return false;
            token = token->next;
            pos++;
            return true;
        }

        bool consume_deref(){
            if(token->kind == TK_OPERATOR && token->len == 1 && token->str[0] == '*' && token->DEREF == true){
                token = token->next;
                pos++;
                return true;
            }
            return false;
        }

        bool consume_addr(){
            if(token->kind != TK_ADDR || token->len != 1 || token->str[0] != '&')
                return false;
            token = token->next;
            pos++;
            return true;
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
            if(token->kind != TK_OPERATOR || token->str[0] != op){
                if(op == ';')
                    error_at(token->str, "missing ; ");
                if(op == '(')
                    error_at(token->str, "missing ( ");
                if(op == ')')
                    error_at(token->str, "missing ) ");
                if(op == '{')
                    error_at(token->str, "missing { ");
                if(op == '}')
                    error_at(token->str, "missing } ");
                error_at(token->str, "value not correct");}
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
            Token *tmp = token->next;
            if(tmp->str[0] == '*'){
                tmp->DEREF = false;
            }
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
            delete locals;
        }

};


#endif