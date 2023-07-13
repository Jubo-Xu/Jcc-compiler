#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

void Init_part(){
    std::cout<<".intel_syntax noprefix\n";
    std::cout<<".globl main\n";
    std::cout<<"main:\n";
}

void Space_for_26_letters(){
    std::cout<<"    push rbp\n";
    std::cout<<"    mov rbp, rsp\n";
    std::cout<<"    sub rsp, 208\n";
}

void End_of_single_character_variable(){
    std::cout<<"    mov rsp, rbp\n";
    std::cout<<"    pop rbp\n";
    std::cout<<"    ret\n";
}
#endif