#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdarg>
#include <mutex>
#include "define.h"

namespace AzerothLib
{
    namespace Shared
    {
        enum class LogType : uint64
        {
            Command = 0x1000000000000000,
            Normal = 0x0100000000000000,
            Success = 0x0010000000000000,
            Error = 0x0001000000000000,
            Debug = 0x0000100000000000,
            Test = 0x0000010000000000,
            Chat = 0x0000001000000000,
            Terrain = 0x0000000100000000,
            Network = 0x0000000010000000,
            Packet = 0x0000000001000000,
        };

        class Log
        {
        private:
            static std::mutex _lockObj;

        public:
            static void WriteLine(LogType type, std::string format, std::string prefix, ...);
        };
    }
}
#endif