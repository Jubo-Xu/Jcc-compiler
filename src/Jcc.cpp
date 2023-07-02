#include <array>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char** argv){

    if(argc != 2){
        std::cerr << "there's an error" << std::endl;
        return 1;
    }

    std::cout<<".intel_syntax noprefix\n";
    std::cout<<".globl main\n";
    std::cout<<"main:\n";
    std::cout<<" mov rax, "<<std::atoi(argv[1])<<std::endl;
    std::cout<<" ret\n";
    
    return 0;
}