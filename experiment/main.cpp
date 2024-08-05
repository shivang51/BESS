#include <iostream>
#include <stack>
#include <vector>

#include"json.hpp"
#include <fstream>
#include "jcomponent_data.h"

int applyOperator(char op, int a, int b = 0) {
    switch (op) {
    case '+': return (a + b) >= 1 ? 1 : 0;
    case '*': return a * b;
    case '!': return a == 0 ? 1 : 0;
    default: throw std::invalid_argument("Invalid operator");
    }
}

int evaluateExpression(const std::string& expression, const std::vector<int>& arr) {
    std::stack<int> values;
    std::stack<char> operators;

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (isdigit(c)) {
            int index = c - '0'; // Convert char to int
            values.push(arr[index]);
        } else if (c == '(') {
            operators.push(c);
        } else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                if (operators.top() == '!') {
                    int a = values.top(); values.pop();
                    char op = operators.top(); operators.pop();
                    values.push(applyOperator(op, a));
                } else {
                    int b = values.top(); values.pop();
                    int a = values.top(); values.pop();
                    char op = operators.top(); operators.pop();
                    values.push(applyOperator(op, a, b));
                }
            }
            operators.pop(); // Remove '('
        } else if (c == '+' || c == '*' || c == '!') {
            while (!operators.empty() && operators.top() != '(' &&
                   ((c == '*' && (operators.top() == '*' || operators.top() == '!')) || 
                    (c == '+' && (operators.top() == '+' || operators.top() == '*' || operators.top() == '!')) ||
                    (c == '!' && operators.top() == '!'))) {
                if (operators.top() == '!') {
                    int a = values.top(); values.pop();
                    char op = operators.top(); operators.pop();
                    values.push(applyOperator(op, a));
                } else {
                    int b = values.top(); values.pop();
                    int a = values.top(); values.pop();
                    char op = operators.top(); operators.pop();
                    values.push(applyOperator(op, a, b));
                }
            }
            operators.push(c);
        }
    }

    while (!operators.empty()) {
        if (operators.top() == '!') {
            int a = values.top(); values.pop();
            char op = operators.top(); operators.pop();
            values.push(applyOperator(op, a));
        } else {
            int b = values.top(); values.pop();
            int a = values.top(); values.pop();
            char op = operators.top(); operators.pop();
            values.push(applyOperator(op, a, b));
        }
    }

    return values.top();
}

int main() {
    std::vector<std::vector<int>> arr = { {0, 0} , {0, 1}, {1, 0}, {1, 1} };

    std::ifstream file("./comp.json");
    nlohmann::json data = nlohmann::json::parse(file);

    std::cout << data["collectionName"] << std::endl;
    for (auto& cjson : data["components"]) {
        JComponentData comp(cjson, data["collectionName"]);
		std::cout << comp.name << std::endl;
        for (auto& expr : comp.outputs) {
            for (auto& inp : arr) {
                std::cout << "\t(" << inp[0] << ", " << inp[1] << ") = ";
                std::cout << evaluateExpression(expr, inp) << std::endl;
            }
        }
    }

	return 0; 
}