#pragma once
#include <iostream>

namespace xml
{

class xml_exception : public std::exception
{
public:
	xml_exception(const char *arg) : std::exception(arg) {}

	virtual ~xml_exception() throw() {}

	// Returns a C-style character string describing
	// the general cause of the current error.
	virtual const char* what() const throw() {
		return std::exception::what();
	}
}; // end class

} // end namespace