#include "ConfigManager.h"


ConfigManager::ConfigManager(void)
{
}


ConfigManager::~ConfigManager(void)
{

}


ConfigManager * ConfigManager::GetInstance(){
	static ConfigManager manager;
	return &manager;
}



InputKey * ConfigManager::GetInputKey(std::string action){
	
	// Check if action is being tracked
	if( mActions.find(action) == mActions.end() ){
		return NULL;
	}

	return mActions[action];
}

std::string ConfigManager::GetPath(std::string folder){
	return "";
}
