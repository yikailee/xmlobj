#include "gtest\gtest.h"
#include "..\xmlobj\scanner.h"
#include "..\xmlobj\string_source.h"
#include "..\xmlobj\xml_ex.h"

#include <unordered_map>

bool EqualToken(const xml::Token &token1, const xml::Token &token2) {
	if (token1.type != token2.type) {
		return false;
	}
	if (token1.text != token2.text) {
		return false;
	}
	if (token1.attrs != token2.attrs) {
		return false;
	}

	return true;
}

TEST(test_Scanner, test_ReadNode) {
	// happy path test
	{
		std::unordered_map<std::string, xml::Token> happyTestData;

		xml::Token expectedToken; 
		// case
		expectedToken.type = xml::T_NODE_START;
		expectedToken.text = "node_name";
		happyTestData["<node_name>"] = expectedToken;
		// case
		happyTestData["<node_name >"] = expectedToken;
		// case
		expectedToken.attrs["attr"] = "val1";
		happyTestData["<node_name attr=\"val1\">"] = expectedToken;
		// case
		happyTestData["<node_name attr = \"val1\" >"] = expectedToken;
		// case
		happyTestData["<node_name attr = \"val1\"	>"] = expectedToken;
		// case
		expectedToken.attrs["attr2"] = "val2";
		happyTestData["<node_name attr=\"val1\" attr2=\"val2\">"] = expectedToken;
		// case
		happyTestData["<node_name attr = \"val1\" attr2=\"val2\">"] = expectedToken;
		// case
		happyTestData["<node_name attr = \"val1\" attr2 = \"val2\">"] = expectedToken;
		// case 
		expectedToken.type = xml::T_NODE_END;
		expectedToken.text = "node_name";
		expectedToken.attrs.clear();
		happyTestData["</node_name>"] = expectedToken;
		// case
		happyTestData["</ node_name >"] = expectedToken;

		for (auto &it : happyTestData) {
			xml::StringSource ss(it.first.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(false, bEx);
			EXPECT_EQ(true, EqualToken(token, it.second));
		}
	}


	// happy path test node end with />
	{
		std::unordered_map<std::string, xml::Token> happyTestData;

		xml::Token expectedToken;
		// case
		expectedToken.type = xml::T_NODE_START;
		expectedToken.text = "node_name";
		happyTestData["<node_name/>"] = expectedToken;
		// case
		happyTestData["<node_name />"] = expectedToken;
		// case
		expectedToken.attrs["attr"] = "val1";
		happyTestData["<node_name attr=\"val1\"/>"] = expectedToken;
		// case
		happyTestData["<node_name attr = \"val1\" />"] = expectedToken;
		// case
		happyTestData["<node_name attr = \"val1\"	/>"] = expectedToken;
		// case
		expectedToken.attrs["attr2"] = "val2";
		happyTestData["<node_name attr=\"val1\" attr2=\"val2\"/>"] = expectedToken;
		// case
		happyTestData["<node_name attr = \"val1\" attr2=\"val2\"/>"] = expectedToken;
		// case
		happyTestData["<node_name attr = \"val1\" attr2 = \"val2\"/>"] = expectedToken;

		for (auto &it : happyTestData) {
			xml::StringSource ss(it.first.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(false, bEx);
			EXPECT_EQ(true, EqualToken(token, it.second));

			// scan next token should be T_NODE_END
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(false, bEx);
			
			// test next token
			xml::Token nextExpectedToken;
			nextExpectedToken.type = xml::T_NODE_END;
			nextExpectedToken.text = it.second.text;

			EXPECT_EQ(true, EqualToken(token, nextExpectedToken));
		}
	}

	// exception test
	{
		std::vector<std::string> xmlstrs;
		// case
		xmlstrs.push_back("<");	
		// case
		xmlstrs.push_back("<node_name");
		// case
		xmlstrs.push_back("<node_name attr>");
		// case
		xmlstrs.push_back("<node_name attr= >");
		// case
		xmlstrs.push_back("<node_name attr = abc >");
		// case
		xmlstrs.push_back("<node_name attr = \"ab");
		// case
		xmlstrs.push_back("<node_name attr = \"abc\"");
		// case
		xmlstrs.push_back("<node_name attr = abc\"");
		// case
		xmlstrs.push_back("<node_name attr=\"val\" \"abc\">");
		// case
		xmlstrs.push_back("<node_name attr=\"val\"=\"val2\">");
		// case
		xmlstrs.push_back("</node_name attr = \"val1\">");

		for (auto &it : xmlstrs) {
			xml::StringSource ss(it.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(true, bEx);
		}
	}
}

TEST(test_Scanner, test_ReadNode_procinst) {
		// happy path test
	{
		std::unordered_map<std::string, xml::Token> happyTestData;

		xml::Token expectedToken; 
		// case
		expectedToken.type = xml::T_PROCINST;
		expectedToken.text = "xml";
		happyTestData["<?xml?>"] = expectedToken;
		// case
		happyTestData["<?xml ?>"] = expectedToken;
		// case
		expectedToken.attrs["version"] = "1.0";
		happyTestData["<?xml version=\"1.0\"?>"] = expectedToken;
		// case
		happyTestData["<?xml version = \"1.0\" ?>"] = expectedToken;
		// case
		happyTestData["<?xml version = \"1.0\"	?>"] = expectedToken;
		// case
		expectedToken.attrs["attr2"] = "val2";
		happyTestData["<?xml version=\"1.0\" attr2=\"val2\"?>"] = expectedToken;
		// case
		happyTestData["<?xml version = \"1.0\" attr2=\"val2\"?>"] = expectedToken;
		// case
		happyTestData["<?xml version = \"1.0\" attr2 = \"val2\"?>"] = expectedToken;

		for (auto &it : happyTestData) {
			xml::StringSource ss(it.first.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(false, bEx);
			EXPECT_EQ(true, EqualToken(token, it.second));
		}
	}

	// exception test
	{
		std::vector<std::string> xmlstrs;
		// case
		xmlstrs.push_back("<?");	
		// case
		xmlstrs.push_back("<?node_name");
		// case
		xmlstrs.push_back("<?node_name attr?>");
		// case
		xmlstrs.push_back("<?node_name attr= ?>");
		// case
		xmlstrs.push_back("<?node_name attr = abc ?>");
		// case
		xmlstrs.push_back("<?node_name attr = \"ab");
		// case
		xmlstrs.push_back("<?node_name attr = \"abc\"");
		// case
		xmlstrs.push_back("<?node_name attr = abc\"");
		// case
		xmlstrs.push_back("<?node_name attr=\"val\" \"abc\"?>");
		// case
		xmlstrs.push_back("<?node_name attr=\"val\"=\"val2\"?>");
		for (auto &it : xmlstrs) {
			xml::StringSource ss(it.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(true, bEx);
		}
	}
}

TEST(test_Scanner, test_ReadNode_comment) {
	// happy path test
	{
		std::unordered_map<std::string, xml::Token> happyTestData;

		xml::Token expectedToken; 
		// case
		expectedToken.type = xml::T_COMMENT;
		expectedToken.text = "comment coment";
		happyTestData["<!-- comment coment -->"] = expectedToken;
		// case
		happyTestData["<!---->"] = expectedToken;
		for (auto &it : happyTestData) {
			xml::StringSource ss(it.first.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(false, bEx);
			EXPECT_EQ(token.type, expectedToken.type);
		}
	}

	// exception test
	{
		std::vector<std::string> xmlstrs;
		// case
		xmlstrs.push_back("<!--");	
		// case
		xmlstrs.push_back("<!-- abcde ");
		// case
		xmlstrs.push_back("<!--abcd");
		// case
		xmlstrs.push_back("<!-- ->");
		// case
		xmlstrs.push_back("<!-- --");
		// case
		xmlstrs.push_back("<!-- -- >");
		for (auto &it : xmlstrs) {
			xml::StringSource ss(it.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(true, bEx);
		}
	}	
}

TEST(test_Scanner, test_Scan) {
	// happy path test
	{
		std::unordered_map<std::string, std::vector<xml::Token>> happyTestData;

		std::vector<xml::Token> expectedTokens;
		// case
		xml::Token expectedToken;
		expectedToken.type = xml::T_PROCINST;
		expectedToken.text = "xml";
		expectedToken.attrs["version"] = "1.0";
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_COMMENT;
		expectedToken.text = "";
		expectedToken.attrs.clear();
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_NODE_START;
		expectedToken.text = "father";
		expectedToken.attrs.clear();
		expectedToken.attrs["attr"] = "val";
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_NODE_START;
		expectedToken.text = "child";
		expectedToken.attrs.clear();
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_NODE_START;
		expectedToken.text = "subChild";
		expectedToken.attrs.clear();
		expectedToken.attrs["a"] = "b";
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_NODE_END;
		expectedToken.text = "subChild";
		expectedToken.attrs.clear();
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_NODE_START;
		expectedToken.text = "attr1";
		expectedToken.attrs.clear();
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_STRING;
		expectedToken.text = "val1";
		expectedToken.attrs.clear();
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_NODE_END;
		expectedToken.text = "attr1";
		expectedToken.attrs.clear();
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_NODE_END;
		expectedToken.text = "child";
		expectedToken.attrs.clear();
		expectedTokens.push_back(expectedToken);

		expectedToken.type = xml::T_NODE_END;
		expectedToken.text = "father";
		expectedToken.attrs.clear();
		expectedTokens.push_back(expectedToken);

		happyTestData["<?xml version=\"1.0\"?>\r\n \
					   <!-- comment here -->\r\n \
					     <father attr=\"val\">\r\n \
						   <child>\r\n \
							 <subChild a=\"b\" /> \
						     <attr1>val1</attr1>\r\n \
						   </child>\r\n \
						 </father>\r\n \
					  "] = expectedTokens;
		for (auto &it : happyTestData) {
			xml::StringSource ss(it.first.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();
				int i = 0;
				while (token.type != xml::T_EOF && i < expectedTokens.size()) {
					EXPECT_EQ(true, EqualToken(token, expectedTokens[i]));

					token = xmlScanner.Scan();
					++i;
				}

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(false, bEx);
		}
	}

	// exception test
	{
		std::vector<std::string> xmlstrs;
		// case
		xmlstrs.push_back("<!--");	
		// case
		xmlstrs.push_back("<!-- abcde ");
		// case
		xmlstrs.push_back("<!--abcd");
		// case
		xmlstrs.push_back("<!-- ->");
		// case
		xmlstrs.push_back("<!-- --");
		// case
		xmlstrs.push_back("<!-- -- >");
		for (auto &it : xmlstrs) {
			xml::StringSource ss(it.c_str());
			xml::Scanner xmlScanner(ss);
			bool bEx = false;
			xml::Token token;
			try {
				token = xmlScanner.Scan();

			} catch (xml::xml_exception &ex) {
				bEx = true;
			}

			EXPECT_EQ(true, bEx);
		}
	}
}
