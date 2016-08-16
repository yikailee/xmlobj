#include "string_source.h"

namespace xml
{

void StringSource::Track(char ch) {
	if (ch == '\n') {
		m_line++;
		m_col = 1;
	} else {
		++m_col;
	}
}
	
char StringSource::Next() {
	char ret = '\0';
	if (m_pc < m_pe) {
		ret = *m_pc++;
		Track(ret);
	}
	return ret;
}

char StringSource::Peek() {
	char ret = '\0';
	if (m_pc < m_pe) {
		ret = *m_pc;
	}
	return ret;
}

void StringSource::PushBack() {
    if (m_pc > m_ps) {
        --m_pc;
		--m_col;
    }
}
	
// speculative look-ahead, advance if matched
bool StringSource::Match(const char *arg) {
	if (m_pc >= m_pe || nullptr == arg || 0 == strlen(arg)) {
		return false;
	}

	size_t len = strlen(arg);
	bool matched = true;
    const char *pc = m_pc;
    while (matched && pc < m_pe && *arg) {
        if (tolower(*arg) != tolower(*pc)) {
            matched = false;
        } else {
            ++arg; ++pc;
        }
    }
	if (*arg != '\0') matched = false;

    if (matched) {
        m_pc = pc;
		m_col += static_cast<int>(len);
    }
    return matched;
}
	
} // end namespace
