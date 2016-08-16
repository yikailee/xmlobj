#include "gtest\gtest.h"
#include "..\xmlobj\xml_reader.h"
#include "..\xmlobj\string_source.h"
#include "..\xmlobj\ixml.h"

#include <memory>

class B : public xml::iXmlNode {
public:
	bool good;

	void Serialize(xml::Adapter &adapter){
		xml::Class xml(adapter, "B");
		XML_NODE(adapter, good);
	}

	bool operator == (const B &ref) const {
		return this->good == ref.good;
	}
};

class C : public xml::iXmlDoc {
public:
	std::string attr;
	double subchild;
	std::vector<int> values;
	std::vector<B> bs;

	void Serialize(xml::Adapter &adapter){
		std::unordered_map<std::string, std::string> attrs;
		attrs["version"] = "1.0";
		attrs["encoding"] = "utf-8";
		xml::Procinst(adapter, "xml", attrs);
		xml::Class xml(adapter, "C");
		XML_ATTR(adapter, attr);
		XML_NODE(adapter, subchild);
		XML_NODE(adapter, values);
		XML_NODE(adapter, bs);
	}
};

TEST(test_Reader, test_AllMethod) {
	std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?> \
					   <C attr=\"value\"> \r\n\
						 <subchild>100.0</subchild>\r\n\
						 <values>1</values> \
						 <values>2</values> \
						 <B> \
						   <good>true</good> \
						 </B> \
						 <B> \
						   <good>false</good> \
						 </B> \
					   </C>";

	C test;
	EXPECT_NO_THROW(xml::iXmlDoc::UnMarshal(xml, test));

	C expected;
	expected.attr = "value";
	expected.subchild = 100.0;
	expected.values.push_back(1);
	expected.values.push_back(2);
	B b1, b2;
	b1.good = true;
	b2.good = false;
	expected.bs.push_back(b1);
	expected.bs.push_back(b2);

	EXPECT_EQ(expected.attr, test.attr);
	EXPECT_EQ(expected.subchild, test.subchild);
	EXPECT_EQ(expected.values.size(), test.values.size());
	EXPECT_EQ(expected.values, test.values);
	EXPECT_EQ(expected.bs.size(), test.bs.size());
	EXPECT_EQ(expected.bs.size(), test.bs.size());
}


class D : public xml::iXmlDoc {
public:
	std::string a;
	bool		good;

	void Serialize(xml::Adapter &adapter){
		std::unordered_map<std::string, std::string> attrs;
		attrs["version"] = "1.0";
		attrs["encoding"] = "utf-8";
		xml::Procinst(adapter, "xml", attrs);
		xml::Class xml(adapter, "D");
		XML_ATTR(adapter, a);
		XML_VALUE(adapter, good);
	}
};

TEST(test_Reader, test_AllMethod2) {
	std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?> \
					   <D a=\"value\"> \r\n\
							true \r\n\
					   </D>";

	D test;
	EXPECT_NO_THROW(xml::iXmlDoc::UnMarshal(xml, test));

	D expected;
	expected.a = "value";
	expected.good = true;

	EXPECT_EQ(expected.a, test.a);
	EXPECT_EQ(expected.good, test.good);
}
