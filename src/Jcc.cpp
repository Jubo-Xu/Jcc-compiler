#include <array>
#include <iostream>
#include <string>
#include <vector>
#include "Token.hpp"
#include "Parser.hpp"
#include "utils.hpp"


int main(int argc, char** argv){

    if(argc != 2){
        std::cerr << "there's an error" << std::endl;
        return 1;
    }

    //TOKEN<int> Tok(argv[1]);
    PARSER<int> Parser(argv[1]);

    Init_part();
    Space_for_26_letters();

    // std::cout<<".intel_syntax noprefix\n";
    // std::cout<<".globl main\n";
    // std::cout<<"main:\n";
    // std::cout<<" mov rax, "<<std::atoi(argv[1])<<std::endl;
    // std::cout<<" ret\n";
    //Tok.Execute();
    Parser.stack_gen();
    // std::cout<<"    pop rax\n";
    // std::cout<<"    ret\n";
    End_of_single_character_variable();
    return 0;
}