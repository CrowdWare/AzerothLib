#include "pch.h"
#include "framework.h"
#include <iostream>
#include "log.h"

namespace AzerothLib
{
    int main()
    {
        Shared::Log::WriteLine(AzerothLib::Shared::LogType::Normal, "Hello, world!", "Main");

        return 0;   
    }
}