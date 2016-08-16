#include "scanner.h"
#include "platform_selector.h"
#include "xml_ex.h"

namespace xml {

//-----------------------------------------------------------------------------
// throw with a suitable error message if the condition is true
inline void throw_if(ISource *pSource, bool condition, std::string msg) {
	if (condition) {
		__PLATFORM_THROW(xml_exception(""));
	}
}

/*
 * Read string which start and end with '"' char, or throw a exception.
 */
std::string Scanner::ReadString() {
	std::string ret = "";
	this->SkipBlanks();

	char c = m_pSource->Next();
	if (c != '"') {
		throw_if(m_pSource, true, "no string following");
	}

	c = m_pSource->Next();
	while (c != '"') {
		// it's an escaped character
		if (c == '\\') {
			c = m_pSource->Next();
			if (c == '"') {
				ret += c;
			} else if (c == '\\') {
				ret += c;
			} else if (c == '/') {
				ret += c;
			} else if (c == 'b') {
				ret += '\b';
			} else if (c == 'f') {
				ret += '\f';
			} else if (c == 'n') {
				ret += '\n';
			} else if (c == 'r') {
				ret += '\r';
			} else if (c == 't') {
				ret += '\t';
			} 
		} else if (c == '\0') {
			// run out of road ...
			throw_if(m_pSource, true, "unterminated string");
		} else {
			ret += c;
		}

		c = m_pSource->Next();
	}

	if (ret.empty()) {
		throw_if(m_pSource, true, "invalid attribute value");
	}

	return ret;
}

/*
 * Read text which end with blank char or ">", "=", "/>" or "?>"
 */
std::string Scanner::ReadText() {
	this->SkipBlanks();

	std::string ret = "";
	if (isalpha(m_pSource->Peek())) {
		char c = m_pSource->Next();
		while (c != '\0') {
			if (IsBlank(c)) break;
			ret += c;

			if (m_pSource->Match(">") || m_pSource->Match("=")) {
				m_pSource->PushBack();
				break;
			}
			if (m_pSource->Match("/>") || m_pSource->Match("?>")) {
				m_pSource->PushBack();
				m_pSource->PushBack();
				break;
			}

			c = m_pSource->Next();
		}
		if (c == '\0') {
			throw_if(m_pSource, true, "Unterminated node");
		}
	}
	
	if (ret.empty()) {
		throw_if(m_pSource, true, "invalid node name or attribute name");
	}

	return ret;
}

/*
 * Read the innter text inner start and end of a node, e.g
 * <NODE>inner text </NODE>
 */
Token Scanner::ReadInnerText() {
	Token ret;

	ret.type = T_STRING;
	while (IsBlank(m_pSource->Peek())) {
		m_pSource->Next();
	}

	char c = m_pSource->Next();
	while (!IsBlank(c) && c != '<' && c != '\0') {
		ret.text += c; 
		c = m_pSource->Next();
	}
	if (c == '<') {
		m_pSource->PushBack();
	}
	if (c == '\0') {
		throw_if(m_pSource, true, "Unterminated node");
	}
	if (ret.text.empty()) {
		throw_if(m_pSource, true, "invalid attribute value");
	}

	return ret;
}

/*
 * Skip read comment which start with "<!--" and end with "-->"
 */
void Scanner::SkipComment() {
	bool matched = false;
	if (m_pSource->Match("--")) {
		while (m_pSource->Peek() != '\0') {
			if (m_pSource->Match("-->")) {
				return;
			}
			m_pSource->Next(); 
		}
	} else {
		throw_if(m_pSource, true, "illegal format");
	}

	if (m_pSource->Peek() == '\0' && !matched) {
		throw_if(m_pSource, true, "mismatch comment");
	}
}

/*
 * Skip read blank chars ' ', '\t', '\r' and '\n'
 */
void Scanner::SkipBlanks() {
	while (IsBlank(m_pSource->Peek())) {
		m_pSource->Next();
	}	
}

/*
 * Meet a start of xml node, read info of start node into a Token.
 * if meet one of input "endstrs", teminate read.
 */
std::string Scanner::ReadAttrs(Token &token, std::vector<std::string> endstrs) {
	token.text = ReadText();
	std::string matchedStr = "";

	while (true) {
		for (auto &str : endstrs) {
			if (m_pSource->Match(str.c_str())) {
				matchedStr = str;
				break;
			}
		}

		if (!matchedStr.empty() || m_pSource->Peek() == '\0') break;

		if (IsBlank(m_pSource->Peek())) {
			m_pSource->Next();
			continue;
		}

		// read node attribute key
		std::string key = ReadText();
		this->SkipBlanks();
		
		// should be a '=' next by attribute key
		char c = m_pSource->Next();
		if (c != '=') {
			throw_if(m_pSource, true, "no '=' after attribute name");
		}

		// next should be a attribute value which start and end with '"'
		std::string value = ReadString();

		token.attrs[key] = value;
	}

	if (matchedStr.empty()) {
		throw_if(m_pSource, true, "Unterminated xml");
	}

	return matchedStr;
}

/* 
 * If meet a '<' char, should be a xml node, read it
 */
Token Scanner::ReadNode() {
	// case "<!--", "<?", "</" or "<"
	Token token;

	char c = m_pSource->Next();
	if (c == '?') {
		std::vector<std::string> matchedstrs;
		matchedstrs.push_back("?>");
		this->ReadAttrs(token, matchedstrs);
		token.type = T_PROCINST;

	} else if (c == '/') {
		// end of node
		std::vector<std::string> matchedstrs;
		matchedstrs.push_back(">");
		this->ReadAttrs(token, matchedstrs);
		if (!token.attrs.empty()) {
			throw_if(m_pSource, true, "invalid xml");
		}
		token.type = T_NODE_END;

	} else if (c == '!') {
		// comment
		this->SkipComment();
		token.type = T_COMMENT;

	} else if (isalpha(c)) {
		// start of node
		m_pSource->PushBack();
		std::vector<std::string> matchedstrs;
		matchedstrs.push_back(">");
		matchedstrs.push_back("/>");
		std::string matchedstr = this->ReadAttrs(token, matchedstrs);
		token.type = T_NODE_START;
		// if end with />, should save a T_NODE_END token to m_nextToken
		if (matchedstr == "/>") { 
			m_nextToken.text = token.text;
			m_nextToken.type = T_NODE_END;
		}

	} else {
		// exception
		throw_if(m_pSource, true, "illegal format");
	}

	return token;
}

bool Scanner::IsBlank(char c) {
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

/*
 * call Scan() once, will return a token
 */
Token Scanner::Scan() {
	Token token;

	if (m_nextToken.type != T_UNKNOWN) {
		token = m_nextToken;
		m_nextToken.type = T_UNKNOWN;
		return token;
	}

	char c = '\0';
	while (token.type == T_UNKNOWN) {
		c = m_pSource->Next();
		if (IsBlank(c)) continue;

		c = static_cast<char>(tolower(c));
		if (c == '\0') {
			token.text = "EOF";
			token.type = T_EOF;

		} else if (c == '<') {
			token = this->ReadNode();

		} else {
			m_pSource->PushBack();
			token = this->ReadInnerText();
		}
	}

	return token;
}

} // end namespace


