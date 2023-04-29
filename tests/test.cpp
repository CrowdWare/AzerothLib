#include <iostream>
#include "log.h"

void main()
{
    std::cout << "testing started" << std::endl;
    AzerothLib::Shared::Log::WriteLine(AzerothLib::Shared::LogType::Normal, "Hello, world!", "Main");
    std::ifstream file("logs/Main_normal_log.txt", std::ios_base::app);
    std::string line;
    std::getline(file, line);
    if (file.fail())
        std::cerr << "Error reading logfile" << std::endl;
    else
        std::cout << line << std::endl;
    std::cout << "testing ended" << std::endl;
}