#include <iostream>
#include <string>



int main() {
    std::cout << "Hello world" << std::endl;

    std::cout << "Size of int: " << sizeof(int) << std::endl;
    std::cout << "Size of char: " << sizeof(char) << std::endl;
    std::cout << "Size of char*: " << sizeof(char*) << std::endl;
    std::cout << "Size of std::string: " << sizeof(std::string) << std::endl;
    
    return 0;
}