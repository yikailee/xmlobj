/*
 * XML lexer and token define
 * a Token object should have its text, 
 * and atts(reserve for start of node which has attributes).
 * 1. Xml declaration should be a T_COMMENT token start with "<?" and end with "?>";
 * 2. A line a comment is a T_PROCINST token start with "<!--" and end with "-->";
 * 3. Start of node should be a T_NODE_START token start with "<" and end with ">", 
 *    if end with "/>", will break down into two token, T_NODE_START and T_NODE_END
 * 4. End of node should be a T_NODE_END token 
 * 5. between a start node and end node should be a T_STRING token if there is no 
 *    sub node inside.
 * 6. end of xml should be a T_EOF node.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <stdint.h>

namespace xml 
{
//----------------------------------------------------------------
enum TokenType {
	T_UNKNOWN,
	T_NODE_START, // </ or <
	T_NODE_END, // > or />
	T_COMMENT,
	T_PROCINST, // <? xxxx ?>
	T_STRING,
	T_EOF
};
//----------------------------------------------------------------

struct Token {
	// defalut is a T_UNKNOWN token
	Token() : type(T_UNKNOWN) {}

	// if a T_NODE_START or T_NODE_END, will be node name
	// if T_STRING, will be this string value.
	std::string text;
	// if start of node, save node attributes.
	std::unordered_map<std::string, std::string> attrs;
	// token type of a node
	TokenType type;
};

} // end namespace
