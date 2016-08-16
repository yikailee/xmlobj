#include "gtest\gtest.h"
#include "..\xmlobj\xml_writer.h"
#include "..\xmlobj\string_source.h"
#include "..\xmlobj\ixml.h"

#include <memory>

class B2 {
public:
	bool good;

	void Serialize(xml::Adapter &adapter) {
		xml::Class xml(adapter, "B2");
		XML_NODE(adapter, good);
	}
};


class C2 {
public:
	std::string attr;
	double subchild;
	//std::vector<B> bs;
	std::vector<int> values;
	std::vector<B2> bs;

	void Serialize(xml::Adapter &adapter) {
		std::unordered_map<std::string, std::string> attrs;
		attrs["version"] = "1.0";
		attrs["encoding"] = "utf-8";
		xml::Procinst(adapter, "xml", attrs);
		xml::Class xml(adapter, "C2");
		XML_ATTR(adapter, attr);
		XML_NODE(adapter, subchild);
		XML_NODE(adapter, values);
		XML_NODE(adapter, bs);
	}
};


TEST(test_Writer, test_WriterAllMethod) {
	std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<C2 attr=\"value\">\r\n\t<subchild>100</subchild>\r\n\t<values>1</values>\r\n\t<values>2</values>\r\n\t<B2>\r\n\t\t<good>true</good>\r\n\t</B2>\r\n\t<B2>\r\n\t\t<good>false</good>\r\n\t</B2>\r\n</C2>\r\n";

	C2 c;
	c.attr = "value";
	c.subchild = 100.0;
	c.values.push_back(1);
	c.values.push_back(2);

	B2 b1;
	b1.good = true;
	c.bs.push_back(b1);

	B2 b2;
	b2.good = false;
	c.bs.push_back(b2);

	std::string test;
	try {
		test = xml::iXmlDoc::Marshal<C2>(c);

	} catch (xml::xml_exception &ex) {
		std::cout << ex.what() << std::endl;
		EXPECT_TRUE(false);
	}
	
	EXPECT_EQ(expected, test);
}


class D2 : public xml::iXmlDoc {
public:
	std::string a;
	bool		good;

	void Serialize(xml::Adapter& adapter){
		std::unordered_map<std::string, std::string> attrs;
		attrs["version"] = "1.0";
		attrs["encoding"] = "utf-8";
		xml::Procinst(adapter, "xml", attrs);
		xml::Class xml(adapter, "D");
		XML_ATTR(adapter, a);
		XML_VALUE(adapter, good);
	}
};

TEST(test_Writer, test_WriterAllMethod2) {
	std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<D a=\"value\">true</D>\r\n";

	D2 d2;
	d2.a = "value";
	d2.good = true;
	std::string test;
	try {
		test = xml::iXmlDoc::Marshal<D2>(d2);

	} catch (xml::xml_exception &ex) {
		std::cout << ex.what() << std::endl;
		EXPECT_TRUE(false);
	}

	EXPECT_EQ(expected, test);
}
