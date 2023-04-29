#include "pch.h"
#include "log.h"
#include "G3D/FileSystem.h"

#define _SILENCE_EX PERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

std::mutex AzerothLib::Shared::Log::_lockObj;

void AzerothLib::Shared::Log::WriteLine(LogType type, std::string format, std::string prefix, ...)
{
	_lockObj.lock();

	try 
	{
		char timeStr[20];
		std::time_t now = std::time(nullptr);
		std::tm timeInfo;
		localtime_s(&timeInfo, &now);
		std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeInfo);

		format = "[" + std::string(timeStr) + "]" + "[" + std::to_string(static_cast<unsigned long long>(type)) + "] " + format;

		char buffer[1024];
		va_list args;
		va_start(args, prefix);
		std::vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
		va_end(args);

		if (true)//Config::LogToFile) 
		{
			std::string filePath;
			switch (type)
			{
			case LogType::Packet:
				filePath = "logs/" + prefix + "_packet_log.txt";
				break;
			case LogType::Network:
				filePath = "logs/" + prefix + "_network_log.txt";
				break;
			case LogType::Error:
				filePath = "logs/" + prefix + "_error_log.txt";
				break;
			case LogType::Debug:
				filePath = "logs/" + prefix + "_debug_log.txt";
				break;
			case LogType::Chat:
				filePath = "logs/" + prefix + "_chat_log.txt";
				break;
			case LogType::Normal:
				filePath = "logs/" + prefix + "_normal_log.txt";
				break;
			default:
				break;
			}
			if (! G3D::FileSystem::exists("logs", false)) 
        		G3D::FileSystem::createDirectory("logs");
	    	std::ofstream file(filePath, std::ios_base::app);
    		file << buffer << std::endl;
		}
	}
	catch (const std::exception& ex) 
	{
		std::cerr << ex.what() << std::endl;
	}

	_lockObj.unlock();
}


