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

#pragma once
#include "../../Vitruvia.h"
#include "../../Vitruvia/Globals/VitruviaGlobals.h"

namespace Vitruvia
{
	class CrashLogger
	{
	public:
		CrashLogger() {	clear(); }
		void Add(const std::string& log);
		void RemoveLogs();
		void SetWriting(bool enable) { enable_writing = enable; }
	private:
		bool enable_writing;
		std::stringstream stream;
		void clear() { enable_writing = false; stream.clear(); }
	};
	extern CrashLogger g_CrashLog;
}