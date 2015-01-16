#pragma once

#include <SFML\Window.hpp>

#include <map>


typedef struct InputKey_stuct{
	sf::Keyboard::Key Key;
	std::string Action;
	std::string Description;
}InputKey;

class ConfigManager
{

public:
	~ConfigManager(void);
	static ConfigManager * GetInstance();

	InputKey * GetInputKey(std::string action);
	std::string GetPath(std::string folder);

private:
	ConfigManager(void);

private:
	// File Paths
	// Key - Path name;
	// Stored - Path from root directory "..//"
	std::map<std::string, std::string> mFilePaths;

	// Keys
	// Key - Action. Ex "Left"
	// Stored - SFML key enum and description of action
	std::map<std::string, InputKey *> mActions;
};

