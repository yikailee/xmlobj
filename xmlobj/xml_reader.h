#pragma once

#include "xml_adapter.h"
#include "scanner.h"

#include <memory>
#include <stack>

namespace xml {
	
class Reader : public Adapter {
public:
	Reader(Scanner &scanner) : m_pScanner(&scanner) {}
	~Reader() {}

	// should return false for reading
	bool storing() override;

	TokenType Next();
	void GetNext(TokenType type);
	void GetNext(TokenType type, const std::string &nodeName);
	void GetNext(const std::string& key, TokenType tType, std::string& value, SerlType sType);

	std::string PeekText(TokenType type);
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

	void Serialize(const std::string& key, std::string& value, SerlType type) override;
	void Serialize(const std::string& key, int& value, SerlType type) override;
	void Serialize(const std::string& key, unsigned int& value, SerlType type) override;
	void Serialize(const std::string& key, long& value, SerlType type) override;
	void Serialize(const std::string& key, float& value, SerlType type) override;
	void Serialize(const std::string& key, double& value, SerlType type) override;
	void Serialize(const std::string& key, bool& value, SerlType type) override;

private:
	Scanner *m_pScanner;
	std::stack<Token> m_tokens;
	Token m_token;
	Token m_tokenBuf;

	void throw_if(bool condition, std::string msg);
}; // end class Reader

}// end namespace