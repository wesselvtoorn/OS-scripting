#include <iostream>
#include <string>

std::string criptText(std::string & input, std::string argument){
    std::string result = input;
    for(char& c : result){
        c += stoi(argument);
    }
    return result;
}

int main(int argc, char *argv[]){
    std::string line;
    
    if(argc != 2){
        std::cerr << "Deze functie heeft exact 1 argument nodig" <<  std::ends;
        return -1;
    }
    
    while(std::getline(std::cin, line)){
        std::cout << criptText(line, argv[1]) << std::ends;
        return 0;
    }
}
