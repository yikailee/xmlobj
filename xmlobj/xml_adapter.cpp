#include "xml_adapter.h"

namespace xml {

void stream(Adapter &adapter, TokenType type) {
	adapter.Serialize(type);
}

void stream(Adapter &adapter, const std::string &value) {
	adapter.Serialize(value);
}

void stream(Adapter &adapter, std::string &value) {
	adapter.Serialize(value);
}

void stream(Adapter &adapter, int &value) {
	adapter.Serialize(value);
}

void stream(Adapter &adapter, unsigned int &value) {
	adapter.Serialize(value);
}

void stream(Adapter &adapter, long &value) {
	adapter.Serialize(value);
}

void stream(Adapter &adapter, double &value) {
	adapter.Serialize(value);
}

void stream(Adapter &adapter, bool &value) {
	adapter.Serialize(value);
}

void stream(Adapter &adapter, const std::string &key, std::string &value, SerlType type) {
	adapter.Serialize(key, value, type);
}

void stream(Adapter &adapter, const std::string &key, int &value, SerlType type) {
	adapter.Serialize(key, value, type);
}

void stream(Adapter &adapter, const std::string &key, unsigned int &value, SerlType type) {
	adapter.Serialize(key, value, type);
}

void stream(Adapter &adapter, const std::string &key, long &value, SerlType type) {
	adapter.Serialize(key, value, type);
}

void stream(Adapter &adapter, const std::string &key, float &value, SerlType type) {
	adapter.Serialize(key, value, type);
}

void stream(Adapter &adapter, const std::string &key, double &value, SerlType type) {
	adapter.Serialize(key, value, type);
}

void stream(Adapter &adapter, const std::string &key, bool &value, SerlType type) {
	adapter.Serialize(key, value, type);
}

} // end namespace