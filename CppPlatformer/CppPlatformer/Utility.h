#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <string>
#include <sstream>
#include <vector>

class Utility
{

public:

	template <typename T>
	static bool Convert(std::string const &input, T &output){
	
		std::stringstream ss(input);
		return (ss >> output);
	}

	template <typename T>
	static T Convert(std::string const &input){
	
		std::stringstream ss(input);

		T val;
		ss >> val;
		return val;
	}


	static void Split(std::string const &input, std::vector<std::string> &elements, char const delim){
		std::stringstream ss(input);
		std::string item;

		while( std::getline(ss, item, delim) ){
			elements.push_back(item);
		}
	}


private:

};

#endif
