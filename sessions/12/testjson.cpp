#include <iostream>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

const char* myjson = R"(
{
    "age": 30,
    "hello": "world"
}
)";

int main() {
	json j = {{"hello", "world"}};
	cout << j << endl;
	json j2 = json::parse(myjson);
	cout << j2 << endl;
	int age = j2["age"];
	age++;
	cout << "Age: " << age << endl;
}
