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

void Start_of_while(int count){
    std::cout<<"  .Lbeginwhile"<<count<<":\n";
}

void Main_of_while(int count){
    std::cout<<"    pop rax\n";
    std::cout<<"    cmp rax, 0\n";
    std::cout<<"    je  .Lendwhile"<<count<<"\n";
}

void End_of_while(int count){
    std::cout<<"    jmp .Lbeginwhile"<<count<<"\n";
    std::cout<<"  .Lendwhile"<<count<<":\n";
}

void Start_of_for(int count){
    std::cout<<"  .Lbeginfor"<<count<<":\n";
}

void Main_of_for(int count){
    std::cout<<"    pop rax\n";
    std::cout<<"    cmp rax, 0\n";
    std::cout<<"    je  .Lendfor"<<count<<"\n";
}

void End_of_for(int count){
    std::cout<<"    jmp .Lbeginfor"<<count<<"\n";
    std::cout<<"  .Lendfor"<<count<<":\n";
}
#endif