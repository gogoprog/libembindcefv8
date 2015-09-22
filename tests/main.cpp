#include "embindcefv8.h"
#include <iostream>

struct TestStruct
{
    float
        floatMember;
};

int main(int argc, char* argv[])
{
    std::cout << "embindcefv8 - tests" << std::endl;

    embindcefv8::ValueObject<TestStruct>("TestStruct")
        .constructor();


    embindcefv8::execute("var test = Module.TestStruct();");

    return 0;
}
