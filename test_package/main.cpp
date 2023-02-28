#include <datagen/stl_select_random.hpp>
#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "hello from conan package" << std::endl;
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7};
    auto val = datagen::select_random(values);
    std::cout << "value selected:" << val << std::endl;
    return 0;
}