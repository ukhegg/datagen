#include <datagen/stl_select_random.hpp>
#include <vector>


int main(int argc, char *argv[])
{
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7};
    auto val = datagen::select_random(values);
    return val;
}