#pragma once
#include <string>
#include <algorithm> 
#include <cctype>

namespace GeoString
{
	// trim from start (in place)
	static inline void ltrim(std::string &InString)
	{
		InString.erase(InString.begin(), std::find_if(InString.begin(), InString.end(), [](int ch)
		{
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &InString)
	{
		InString.erase(std::find_if(InString.rbegin(), InString.rend(), [](int ch)
		{
			return !std::isspace(ch);
		}).base(), InString.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &InString)
	{
		ltrim(InString);
		rtrim(InString);
	}

	static inline bool ParseBool(std::string InString)
	{
		return (std::stoi(InString)) ? true : false;
	}
}