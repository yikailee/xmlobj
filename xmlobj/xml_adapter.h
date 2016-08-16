#pragma once

#include <string>
#include <sstream> 
#include <iostream>
#include <unordered_map>

#include "xml_lexer.h"
#include "platform_selector.h"
#include "xml_ex.h"



namespace xml {

/*
 * Serialize type
 * T_ATTR: attribute of node
 * T_NODE: sub node of node 
 * T_VALUE: inner value of node. 
 * e.g 'value' of '<NODE>value</NODE>' or '<NODE attr="val">value</NODE>'
 */
enum SerlType {
	T_ATTR, 
	T_NODE,  
	T_VALUE  
};

class Adapter {
public:
	Adapter() {}

	// must be implemented
	virtual bool storing() = 0;

	virtual bool MoreNode(const std::string&) = 0;

	virtual void Serialize(TokenType) = 0;

	virtual void Serialize(const std::string&) = 0;
	virtual void Serialize(TokenType, const std::string&) = 0;
	
	virtual void Serialize(std::string&) = 0;
	virtual void Serialize(int&) = 0;
	virtual void Serialize(unsigned int&) = 0;
	virtual void Serialize(long&) = 0;
	virtual void Serialize(float&) = 0;
	virtual void Serialize(double&)  = 0;
	virtual void Serialize(bool&) = 0;
	
	// key/value pairs
	virtual void Serialize(const std::string&, std::string&, SerlType) = 0;
	virtual void Serialize(const std::string&, int&, SerlType) = 0;
	virtual void Serialize(const std::string&, unsigned int&, SerlType) = 0;
	virtual void Serialize(const std::string&, long&, SerlType) = 0;
	virtual void Serialize(const std::string&, double&, SerlType) = 0;
	virtual void Serialize(const std::string&, float&, SerlType) = 0;
	virtual void Serialize(const std::string&, bool&, SerlType) = 0;

}; // end class Adapther

/*
 * Every xml node should create a Class object to declare the node name in 
 * virual function Serialize().
 */
class Class {
public:
	Class(Adapter &adapter, const std::string &nodeName) 
		: m_pAdapter(&adapter), m_nodeName(nodeName) {

		m_pAdapter->Serialize(T_NODE_START, nodeName);
	}

	~Class() {
		m_pAdapter->Serialize(T_NODE_END, m_nodeName);
	}

private:
	Adapter *m_pAdapter;
	const std::string m_nodeName;

};

/*
 * root node class of the xml document should create a Procinst class
 * to set xml profile like <?xml version="xxx" encoding="xxx"?>
 */
class Procinst {
public:
	Procinst(Adapter &adapter, const std::string &nodeName, 
		std::unordered_map<std::string, std::string> &attrs) 
		: m_pAdapter(&adapter), m_nodeName(nodeName), m_attrs(attrs) {

		m_pAdapter->Serialize(T_PROCINST, nodeName);
		for (auto &attr : attrs) {
			m_pAdapter->Serialize(attr.first, attr.second, SerlType::T_ATTR);
		}
	}

	~Procinst() {}

private:
	Adapter *m_pAdapter;
	const std::string m_nodeName;
	std::unordered_map<std::string, std::string> m_attrs;
};

//-----------------------------------------------------------------------------
// Some helper macros for brevity
// Serializable element which is attribute of this node 
#define XML_ATTR(xml_adapter, class_member)  xml::stream(xml_adapter, #class_member, class_member, xml::SerlType::T_ATTR)
// Serializable element which is sub node of this node
#define XML_NODE(xml_adapter, class_member)  xml::stream(xml_adapter, #class_member, class_member, xml::SerlType::T_NODE)
// Serializable element which is inner value of this node. e.g. 'inner value' of '<NODE>inner value</NODE>
#define XML_VALUE(xml_adapter, class_member)  xml::stream(xml_adapter, #class_member, class_member, xml::SerlType::T_VALUE)


void stream(Adapter &adapter, TokenType type);
void stream(Adapter &adapter, const std::string &value);

void stream(Adapter &adapter, std::string &value);
void stream(Adapter &adapter, std::wstring &value);
void stream(Adapter &adapter, int &value);
void stream(Adapter &adapter, unsigned int &value);
void stream(Adapter &adapter, long &value);
void stream(Adapter &adapter, unsigned char &value);
void stream(Adapter &adapter, double &value);
void stream(Adapter &adapter, bool &value);

void stream(Adapter &adapter, const std::string &key, std::wstring &value, SerlType type);
void stream(Adapter &adapter, const std::string &key, std::string &value, SerlType type);
void stream(Adapter &adapter, const std::string &key, int &value, SerlType type);
void stream(Adapter &adapter, const std::string &key, unsigned int &value, SerlType type);
void stream(Adapter &adapter, const std::string &key, long &value, SerlType type);
void stream(Adapter &adapter, const std::string &key, unsigned char &value, SerlType type);
void stream(Adapter &adapter, const std::string &key, double &value, SerlType type);
void stream(Adapter &adapter, const std::string &key, bool &value, SerlType type);


//-----------------------------------------------------------------------------
// arbitrary type T. must implement serialize with the correct signature
template <typename T> 
inline void stream(Adapter &adapter, T &arg) {
	arg.Serialize(adapter);
}


template <typename T>
inline void stream(Adapter &adapter, 
				   const std::string& /*key*/, 
				   T &value, 
				   bool more) {

	// handle the value
	value.Serialize(adapter);
}


template <typename T>
inline void stream_primitives(Adapter &adapter, 
							  const std::string &key, 
							  std::vector<T> &value) {

	if (!adapter.storing()) { 
		// read xml iSource
		while (adapter.MoreNode(key)) {
			T t;
			stream(adapter, key, t, SerlType::T_NODE);
			value.push_back(t);
		}
	
	} else {
		// write xml to iSink
		for (auto &it = value.begin(); it != value.end(); ++it) {
			// VC2012 cannot disambiguate the type of T when a vector of bool is used.
			stream(adapter, key, *it, SerlType::T_NODE);
		}
	}
}


template <typename T>
inline void stream_classes(Adapter &adapter, 
						   const std::string &key, 
						   std::vector<T> &value) {

	std::string className = typeid(T).name();
	throw_if(className.find("class ") != 0, "unknown type");

	// get class type name
	className = className.substr(6);
	if (!adapter.storing()) { 
		// read xml iSource
		while (adapter.MoreNode(className)) {
			T t;
			stream(adapter, t);
			value.push_back(t);
		}
	} else {
		// write xml to iSink
		for (auto &it = value.begin(); it != value.end(); ++it) {
			// VC2012 cannot disambiguate the type of T when a vector of bool is used.
			stream(adapter, *it);
		}
	}
}


// Handle of vector data type
template <typename T>
inline void stream(Adapter &adapter, 
				   const std::string &key, 
				   std::vector<T> &value, 
				   SerlType type) {

	throw_if(type != xml::SerlType::T_NODE, 
		"vector member can not set as attribute of xml");

	std::string className = typeid(T).name();
	bool primitive = className.find("class ") != 0;
//	bool primitive = (std::is_same<T,std::string>::value ||
//						std::is_integral<T>::value ||
//						std::is_floating_point<T>::value);

	if (primitive) {
		stream_primitives<T>(adapter, key, value);
	} else {
		stream_classes<T>(adapter, key, value);
	}
}


inline void throw_if(bool condition, std::string msg) {
	if (condition) {
		std::stringstream ss;
		ss << "Error :" << msg;
		__PLATFORM_THROW(xml_exception(ss.str().c_str()));
	}
}

} // end namespace