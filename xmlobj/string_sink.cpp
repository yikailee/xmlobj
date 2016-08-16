#include "string_sink.h"

namespace xml {

ISink& StringSink::operator<<(const char *arg) {
	(*m_buf) << arg;
	return *this;
};

ISink& StringSink::operator<<(const std::string &arg) {
	(*m_buf) << arg;
	return *this;
};

ISink& StringSink::operator<<(const char &arg) {
	(*m_buf) << arg;
	return *this;
};

ISink& StringSink::operator<<(const int &arg) {
	(*m_buf) << arg;
	return *this;
};

ISink& StringSink::operator<<(const unsigned int &arg) {
	(*m_buf) << arg;
	return *this;
};

ISink& StringSink::operator<<(const long &arg) {
	(*m_buf) << arg;
	return *this;
};

ISink& StringSink::operator<<(const double &arg) {
	(*m_buf) << arg;
	return *this;
};

ISink& StringSink::operator<<(const bool &arg) {
	(*m_buf) << arg;
	return *this;
};

}