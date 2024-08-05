#include "jcomponent_data.h"

JComponentData::JComponentData(const nlohmann::json& data, const std::string& collectionName) {
	m_name = data["name"];
	m_collectionName = collectionName;
	m_inputCount = data["inputCount"];
	for (auto& expr : data["outputs"]) {
		m_outputs.push_back(expr);
	}
}
