/*
 * Define interface for a xml class 
 * a xml class should inherit iXmlDoc interface
 * a inner xml node should iherit iXmlNode 
 * Parse a xml object to xml string should call static function:
 * xml::iXmlDoc::Marshal<classname>(xml_object) to return a xml string
 * or call xml::iXmlDoc::SaveToXml(save_path, xml_object) to save to xml file
 *
 * Parse a xml string to a xml object should define a xml class, then call
 * xml::iXmlDoc::UnMarshal<xml_object>(xml_string, xml_object) to init a xml 
 * object by corresponding xml string
 * or call xml::iXmlDoc::ReadXml(xml_file_path, xml_object) to init a xml object
 */

#pragma once

#include "xml_adapter.h"
#include "string_source.h"
#include "scanner.h"
#include "xml_reader.h"
#include "xml_writer.h"

namespace xml {

class iXmlDoc {
public:
	virtual void Serialize(Adapter&) = 0;
	//virtual bool operator==(const iXmlDoc&)const;

	template<typename T> 
	static std::string Marshal(T &t) throw(xml::xml_exception) {
		std::stringstream ss;
		StringSink sink(&ss);
		// create a writer
		xml::Writer writer(&sink);
		// serialize the instance
		t.Serialize(writer);

		return ss.str();
	}

	template<typename T> 
	static void SaveToXml(const std::string &file, T &t) throw(xml::xml_exception) {
		std::ofstream outfile(file.c_str(), std::ofstream::binary);
		outfile << iXmlDoc::Marshal(t);
		outfile.close();	
	}

	template<typename T> 
	static void UnMarshal(const std::string &xmlStr, T &t) {
		StringSource ss(xmlStr.c_str());
		xml::Scanner scanner(ss);
		xml::Reader reader(scanner);

		t.Serialize(reader);
	}

	template<typename T> 
	static void ReadXml(const std::string &file, T &t) {
		std::ifstream infile(file.c_str(), std::ifstream::in);
		std::stringstream buffer;
		buffer << infile.rdbuf();

		iXmlDoc::UnMarshal(buffer.str(), t);

		infile.close();
	}
};

class iXmlNode {
public:
	virtual void Serialize(Adapter&) = 0;
	//virtual bool operator==(const iXmlNode&)const;
};

}
