#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

const char* myjson = R"(
{
    "age": 30,
    "hello": "world"
}
)";
int main() {
    json j = {{"hello", "world"}};
    std::cout << j << std::endl;
    json j2 = json::parse(myjson);
    std::cout << j2 << std::endl;
    int age = j2["age"];
    age++;
    std::cout << "Age: " << age << std::endl;
}