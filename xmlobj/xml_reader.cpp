#include "xml_reader.h"
#include "platform_selector.h"
#include "xml_ex.h"

#include <sstream> 

namespace xml {

bool Reader::storing() { return false; }

TokenType Reader::Next() {
	bool matched = true;
	if (m_tokenBuf.type == T_UNKNOWN) {
		m_token = m_pScanner->Scan();
	} else {
		m_token = m_tokenBuf;
		m_tokenBuf.type = T_UNKNOWN;
	}

	if (m_token.type == T_NODE_START) {
		m_tokens.push(m_token);

	} else if (m_token.type == T_NODE_END) {
		if (m_tokens.empty()) {
			matched = false;

		} else {
			Token& topToken = m_tokens.top();
			if (topToken.text == m_token.text && 
				topToken.type == T_NODE_START) {
				m_tokens.pop();
			} else {
				matched = false;
			}
		}
	}

	throw_if(!matched, "Next: type mismatch");

	return m_token.type;
}

void Reader::GetNext(TokenType type) {
	TokenType next = this->Next();
	throw_if(next != type, "GetNext: type mismatch");
}

void Reader::GetNext(TokenType type, const std::string &nodeName) {
	TokenType next = this->Next();

	throw_if(next != type, "GetNext: type mismatch");
	throw_if(m_token.text != nodeName, "GetNext: node name mismatch");
}

void Reader::GetNext(const std::string& key, TokenType tType, std::string& value, SerlType sType) {
	if (sType == SerlType::T_ATTR) {
		auto &attr = m_token.attrs;
		throw_if(attr.find(key) == attr.end(), "attribute does not match");
		value = attr[key];

	} else {
		GetNext(tType);
		value = m_token.text;
	}
}

std::string Reader::PeekText(TokenType type) {
	if (m_tokenBuf.type == T_UNKNOWN) {
		m_tokenBuf = m_pScanner->Scan();
	} 

	if (m_tokenBuf.type == type) {
		return m_tokenBuf.text;
	}
	return "";
}

bool Reader::MoreNode(const std::string& nodeName) {
	return (!nodeName.empty()) && (nodeName == PeekText(T_NODE_START));
}

void Reader::throw_if(bool condition, std::string msg) {
	if (condition) {
		std::stringstream ss;
		ss << "Error (" << m_pScanner->Line() << ':' << m_pScanner->Col() << ") " << msg;
		__PLATFORM_THROW(xml_exception(ss.str().c_str()));
	}
}

void Reader::Serialize(TokenType type) {
	GetNext(type);
}

void Reader::Serialize(TokenType type, const std::string &nodeName) {
	GetNext(type, nodeName);
}
	
void Reader::Serialize(const std::string& value) {
	GetNext(T_NODE_START);
}
	

void Reader::Serialize(std::string& value) {
	GetNext(T_STRING);
	value = m_token.text;
}
	
void Reader::Serialize(int& value) {
	GetNext(T_STRING);

	// convert to unsigned int type
	try {
		value = std::stoi(m_token.text.c_str());
	} catch(std::exception &ex) {
		throw_if(true, "data type mismatch");
	}
}
	
void Reader::Serialize(unsigned int& value) {
	GetNext(T_STRING);

	// convert to unsigned int type
	try {
		value = static_cast<unsigned int>(std::stoi(m_token.text.c_str()));
	} catch(std::exception &ex) {
		throw_if(true, "data type mismatch");
	}
}
	
void Reader::Serialize(long& value) {
	GetNext(T_STRING);

	// convert to long type
	try {
		value = std::stol(m_token.text.c_str());
	} catch(std::exception &ex) {
		throw_if(true, "data type mismatch");
	}
}
	
void Reader::Serialize(float& value) {
	GetNext(T_STRING);

	// convert to float type
	try {
		value = std::stof(m_token.text.c_str());
	} catch (std::exception &ex) {
		throw_if(true, "data type mismatch");
	}

}

void Reader::Serialize(double& value) {
	GetNext(T_STRING);

	// convert to double type
	try {
		value = std::stod(m_token.text.c_str());
	} catch (std::exception &ex) {
		throw_if(true, "data type mismatch");
	}
}
	
void Reader::Serialize(bool& value) {
	GetNext(T_STRING);

	//convert ss to lowercase
	std::string str = m_token.text;
	for (auto &c : str) { tolower(c); }
	throw_if(str != "true" && str != "false", "data type mismatch");
	value = (str == "true");
}


void Reader::Serialize(const std::string &key, std::string &value, SerlType type) {
	if (type == SerlType::T_NODE) { // case node start
		GetNext(T_NODE_START, key);
	}

	GetNext(key, T_STRING, value, type);

	if (type == SerlType::T_NODE) { // case node end
		GetNext(T_NODE_END, key);
	}
}

void Reader::Serialize(const std::string &key, int &value, SerlType type) {
	if (type == SerlType::T_NODE) { // case node start
		GetNext(T_NODE_START, key);
	}

	std::string ss;
	GetNext(key, T_STRING, ss, type);

	// convert to int type
	try {
		value = std::stoi(ss);
	} catch(std::exception &ex) {
		throw_if(true, "data type mismatch");
	}

	if (type == SerlType::T_NODE) { // case node end
		GetNext(T_NODE_END, key);
	}
}

void Reader::Serialize(const std::string &key, unsigned int &value, SerlType type) {
	if (type == SerlType::T_NODE) { // case node start
		GetNext(T_NODE_START, key);
	}

	std::string ss;
	GetNext(key, T_STRING, ss, type);

	// convert to unsigned int type
	try {
		value = static_cast<unsigned int>(std::stoi(ss));
	} catch(std::exception &ex) {
		throw_if(true, "data type mismatch");
	}

	if (type == SerlType::T_NODE) { // case node end
		GetNext(T_NODE_END, key);
	}
}

void Reader::Serialize(const std::string &key, long &value, SerlType type) {
	if (type == SerlType::T_NODE) { // case node start
		GetNext(T_NODE_START, key);
	}

	std::string ss;
	GetNext(key, T_STRING, ss, type);

	// convert to long type
	try {
		value = std::stol(ss);
	} catch(std::exception &ex) {
		throw_if(true, "data type mismatch");
	}

	if (type == SerlType::T_NODE) { // case node end
		GetNext(T_NODE_END, key);
	}
}

void Reader::Serialize(const std::string &key, float &value, SerlType type) {
	if (type == SerlType::T_NODE) { // case node start
		GetNext(T_NODE_START, key);
	}

	std::string ss;
	GetNext(key, T_STRING, ss, type);

	// convert to float type
	try {
		value = std::stof(ss.c_str());
	} catch (std::exception &ex) {
		throw_if(true, "data type mismatch");
	}

	if (type == SerlType::T_NODE) { // case node end
		GetNext(T_NODE_END, key);
	}
}

void Reader::Serialize(const std::string &key, double &value, SerlType type) {
	if (type == SerlType::T_NODE) { // case node start
		GetNext(T_NODE_START, key);
	}

	std::string ss;
	GetNext(key, T_STRING, ss, type);

	// convert to double type
	try {
		value = std::stod(ss.c_str());
	} catch (std::exception &ex) {
		throw_if(true, "data type mismatch");
	}

	if (type == SerlType::T_NODE) { // case node end
		GetNext(T_NODE_END, key);
	}
}

void Reader::Serialize(const std::string& key, bool& value, SerlType type) {
	if (type == SerlType::T_NODE) { // case node start
		GetNext(T_NODE_START, key);
	}

	std::string ss;
	GetNext(key, T_STRING, ss, type);

	//convert ss to lowercase
	for (auto &c : ss) { tolower(c); }
	throw_if(ss != "true" && ss != "false", "data type mismatch");
	value = (ss == "true");

	if (type == SerlType::T_NODE) { // case node end
		GetNext(T_NODE_END, key);
	}
}

}