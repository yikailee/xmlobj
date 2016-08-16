/*
 * Scan a ISource and call Scan() to get token one by one
 */

#pragma once

#include "xml_lexer.h"
#include "isource.h"

namespace xml {

class Scanner {
public:
    Scanner(ISource &source) : m_pSource(&source) {}
    
	/*
     * call Scan(), will return a token, 
	 * next time call Scan() will move to next token
	*/
	Token Scan();

    int Line() const { return m_pSource->Line(); }
    int Col() const { return m_pSource->Col(); }

private:
	void		SkipComment();
	void		SkipBlanks();

	Token		ReadInnerText();
	std::string ReadString();
	std::string ReadText();
	Token		ReadNode();
	std::string ReadAttrs(Token &token, std::vector<std::string> endstrs);

	
	// ' ', '\t', '\r', '\n'
	bool IsBlank(char c); 

    // source of characters for scanner
    ISource *m_pSource;
	// reserve for case <NODE .. />, will save a T_NODE_END token nex by T_NODE_START
	Token m_nextToken; 

}; // end Scanner

} // end namespace
