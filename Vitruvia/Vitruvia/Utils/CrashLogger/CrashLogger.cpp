/*
	____   ____.__  __                     .__        
	\   \ /   /|__|/  |________ __ _____  _|__|____   
	 \   Y   / |  \   __\_  __ \  |  \  \/ /  \__  \  
	  \     /  |  ||  |  |  | \/  |  /\   /|  |/ __ \_
	   \___/   |__||__|  |__|  |____/  \_/ |__(____  /
												   \/ 
	Premium Internal Multihack for Counter-Strike: Global Offensive
	Made with love by double v - 2017
*/

#include "CrashLogger.h"

Vitruvia::CrashLogger Vitruvia::g_CrashLog;

void Vitruvia::CrashLogger::Add(const std::string& log)
{
	this->stream << log << std::endl;
	if (enable_writing)
	{
		std::string toWrite = log + enc("\n");
		std::string filePath = global->ConfigFolder() + enc("crash_log.txt");
		std::ofstream of;
		of.open(filePath, std::ios::app | std::ios::out | std::ios::in);
		of.write(toWrite.c_str(), toWrite.length());
		of.close();
	}
}

void Vitruvia::CrashLogger::RemoveLogs()
{
	std::string fullPath = global->ConfigFolder() + enc("crash_log.txt");
	std::remove(fullPath.c_str());
}
