#pragma once

#include <string>
#include <sstream>

#include "isink.h"

namespace xml {

class StringSink : public ISink
{
public:
	StringSink(std::stringstream* sink) : m_buf(sink)	{}

	ISink& operator<<(const char *arg) override;
	ISink& operator<<(const std::string &arg) override;
	ISink& operator<<(const char &arg) override;
	ISink& operator<<(const int &arg) override;
	ISink& operator<<(const unsigned int &arg) override;
	ISink& operator<<(const long &arg) override;
	ISink& operator<<(const double &arg) override;
	ISink& operator<<(const bool &arg) override;

private:
	std::stringstream *m_buf;
};

}