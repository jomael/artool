#ifndef ARTERROR_CPP
#define ARTERROR_CPP

//Vereinbarungen zwischen Hornelement und ART-Interface

#include "ARTerror.h"
#include <iostream>

ARTerror::ARTerror(std::string in, std::string des, std::string s1, std::string s2, std::string s3) 
{
	inFunction = in; 
	description = des;
	size_t pos;

	// replace place holders with substrings
	pos = description.find("%s1");
	while (pos != std::string::npos)
	{
		description.replace(pos, 3, s1);
		pos = description.find("%s1");
	}
	
	pos = description.find("%s2");
	while (pos != std::string::npos)
	{
		description.replace(pos, 3, s2);
		pos = description.find("%s2");
	}

	pos = description.find("%s3");
	while (pos != std::string::npos)
	{
		description.replace(pos, 3, s3);
		pos = description.find("%s3");
	}
} 
std::string ARTerror::GetErrorMessage() 
{
	std::string errorst = "Error in function '" + inFunction + "': " + description + "\n";
	return errorst;
}
std::string ARTerror::GetErrorString() 
{
	return description;
}

#endif
