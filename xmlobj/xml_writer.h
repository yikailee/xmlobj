#pragma once

#include "xml_adapter.h"
#include "string_sink.h"

#include <sstream>

namespace xml {

class Writer : public Adapter {
public:
	Writer(ISink *sink) : _sink(sink) {
		m_preIsAttr = false;
		m_indent = 0;
	}
	~Writer() {}

	// should return true for writing
	bool storing() override { return true; }

	bool MoreNode(const std::string& nodeName) override;

	void Serialize(TokenType type) override;
	void Serialize(TokenType type, const std::string &nodeName) override;
	void Serialize(const std::string &value) override;
	void Serialize(std::string &value) override;
	void Serialize(int &value) override;
	void Serialize(unsigned int &value) override;
	void Serialize(long &value) override;
	void Serialize(float &value) override;
	void Serialize(double &value)  override;
	void Serialize(bool &value) override;

	void Serialize(const std::string &key, std::string &value, SerlType type) override;
	void Serialize(const std::string &key, int &value, SerlType type) override;
	void Serialize(const std::string &key, unsigned int &value, SerlType type) override;
	void Serialize(const std::string &key, long &value, SerlType type) override;
	void Serialize(const std::string &key, float &value, SerlType type) override;
	void Serialize(const std::string &key, double &value, SerlType type) override;
	void Serialize(const std::string &key, bool &value, SerlType type) override;

	template<typename T>
	void serialize(const std::string &key, T &value, SerlType type);

	void Indent();

private:
	ISink *_sink;
	bool m_preIsAttr;
	TokenType m_curType;

	unsigned int m_indent;
};

}