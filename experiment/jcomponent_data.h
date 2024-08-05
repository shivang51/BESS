#pragma once

#include <string>
#include <vector>
#include "json.hpp"


class JComponentData {
public:
	JComponentData() = default;
	JComponentData(const nlohmann::json& json, const std::string& collectionName);

	const std::string& name = m_name;
	const std::string& collectionName = m_collectionName;
	const int& inputCount = m_inputCount;
	const std::vector<std::string>& outputs = m_outputs;
		
private:
	std::string m_name = "";
	std::string m_collectionName = "";
	int m_inputCount = 0;
	std::vector<std::string> m_outputs = {};
};
