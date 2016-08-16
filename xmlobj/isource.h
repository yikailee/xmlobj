#pragma once

//-----------------------------------------------------------------------------
// defines the interface for a TokenType source
class ISource
{
public:
	//
	virtual ~ISource() {}
	// move to next char and return current char
	virtual char Next() = 0;
	// peek the next character
	virtual char Peek() = 0;
    // move back to previous char
	virtual void PushBack() = 0;
	// if matched, advances 
	virtual bool Match(const char*) = 0;
	//
	virtual int Line() const = 0;
	virtual int Col() const = 0;
};
