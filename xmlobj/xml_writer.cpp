#include "xml_writer.h"
#include "platform_selector.h"
#include "xml_ex.h"

#include <sstream>

namespace xml {

bool Writer::MoreNode(const std::string& nodeName) {
	return false;
}

void Writer::Serialize(TokenType type) {}
	
void Writer::Serialize(const std::string &value) {}

void Writer::Serialize(std::string &value) {}
	
void Writer::Serialize(int &value) {}
	
void Writer::Serialize(unsigned int &value) {}
	
void Writer::Serialize(long &value) {}
	
void Writer::Serialize(float &value) {}

void Writer::Serialize(double &value) {}
	
void Writer::Serialize(bool &value) {}

void Writer::Serialize(TokenType type, const std::string &nodeName) {
	if (type == T_NODE_START) {
		if (m_curType == T_PROCINST) {
			(*_sink) << "?>\r\n";
		}

		this->Indent();
		++m_indent;
		(*_sink) << "<" << nodeName;
		m_preIsAttr = true;
		m_curType = T_NODE_START;

	} else if (type == T_NODE_END) {
		--m_indent;
		this->Indent();
		(*_sink) << "</" << nodeName << ">" << "\r\n";	

	} else if (type == T_PROCINST) {
		(*_sink) << "<?" << nodeName;
		m_preIsAttr = true;	
		m_curType = T_PROCINST;
	}
}

template<typename T>
void Writer::serialize(const std::string &key, T &value, SerlType type) {
	if (m_preIsAttr) {
		if (type == SerlType::T_NODE)		(*_sink) << ">\r\n"; 	
		else if (type == SerlType::T_VALUE) (*_sink) << ">"; 
	} 

	if (type == SerlType::T_ATTR) {
		(*_sink) << " " << key << "=\"" << value << "\"";
	} else if (type == SerlType::T_NODE) {
		this->Indent();
		(*_sink) << "<" << key << ">" << value << "</" << key << ">\r\n";
	} else if (type == SerlType::T_VALUE) {
		(*_sink) << value;
	}

	m_preIsAttr = (type == SerlType::T_ATTR);		
}

void Writer::Serialize(const std::string &key, std::string &value, SerlType type) {
	this->serialize(key, value, type);
}

void Writer::Serialize(const std::string &key, int &value, SerlType type) {
	this->serialize(key, value, type);
}

void Writer::Serialize(const std::string &key, unsigned int &value, SerlType type) {
	this->serialize(key, value, type);
}

void Writer::Serialize(const std::string &key, long &value, SerlType type) {
	this->serialize(key, value, type);
}

void Writer::Serialize(const std::string &key, float &value, SerlType type) {
	this->serialize(key, value, type);
}

void Writer::Serialize(const std::string &key, double &value, SerlType type) {
	this->serialize(key, value, type);
}

// special handle bool type
void Writer::Serialize(const std::string& key, bool& value, SerlType type) {
	if (m_preIsAttr) {
		if (type == SerlType::T_NODE)		(*_sink) << ">\r\n"; 	
		else if (type == SerlType::T_VALUE) (*_sink) << ">"; 
	} 

	std::string valueStr = value ? "true" : "false";
	if (type == SerlType::T_ATTR) {
		(*_sink) << " " << key << "=\"" << valueStr << "\"";

	} else if (type == SerlType::T_NODE) {
		this->Indent();
		(*_sink) << "<" << key << ">" << valueStr << "</" << key << ">\r\n";

	} else if (type == SerlType::T_VALUE) {
		(*_sink) << valueStr;
	}

	m_preIsAttr = (type == SerlType::T_ATTR);	
}

void Writer::Indent() {
	unsigned int indent = m_indent;
	while (indent-- > 0) {
		(*_sink) << "\t";
	}
}


}