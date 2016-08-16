/*
 * defines the interface for sinking xml data
 */

#pragma once

#include <string>

namespace xml {

class ISink {
public:
	virtual ISink& operator<<(const char*) = 0;
	virtual ISink& operator<<(const std::string&) = 0;
	virtual ISink& operator<<(const char&) = 0;
	virtual ISink& operator<<(const int&) = 0;
	virtual ISink& operator<<(const unsigned int&) = 0;
	virtual ISink& operator<<(const long&) = 0;
	virtual ISink& operator<<(const double&) = 0;
	virtual ISink& operator<<(const bool&) = 0;
};

}