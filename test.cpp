#include <iostream>
#include <vector>
#include <string>

int main(int argc, char const *argv[])
{
    std::vector<std::string> fruits;
    fruits.reserve(6);
    fruits.push_back("Apple");
    fruits.push_back("Banana");
    fruits.push_back("Pear");
    fruits.push_back("P1ear");

    std::string &ff = fruits.at(0);

    fruits.push_back("Kiwi");

    std::cout << ff << std::endl;
    
    return 0;
}
