#pragma once

#include "isource.h"
#include <iostream>

namespace xml {

//-----------------------------------------------------------------------------
// A string source
// StringSource does not make a copy of input xml string data
class StringSource : public ISource {
public:
	StringSource(const char *xmlstr) : 
	m_ps(xmlstr), m_pc(xmlstr), m_line(1), m_col(1) {
		size_t size = strlen(xmlstr);
		m_pe = xmlstr + size;
	}

	char Next() override;
	char Peek() override;
	void PushBack() override;
	bool Match(const char*) override;
	
	int Line() const override { return m_line; }
	int Col() const override { return m_col; }

private:
	const char *m_ps;
	const char *m_pc;
	const char *m_pe;

	int m_line;
	int m_col;

	void Track(char ch);
};

}
