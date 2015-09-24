#include "embindcefv8.h"
#include <iostream>
#include "cef.h"

#ifdef EMSCRIPTEN
    #define EXECUTE_JS      EM_ASM
#else
    #define EXECUTE_JS(src) \
        executeJs( #src "\nstop();" );
#endif

struct TestStruct
{
    TestStruct()
        :
        floatMember(123)
    {
    }

    float
        floatMember;
};

int main(int argc, char* argv[])
{
    (*(new embindcefv8::ValueObject<TestStruct>("TestStruct")))
        .constructor()
        .field("floatMember", &TestStruct::floatMember)
        ;

    #ifdef CEF
        initCef(argc, argv);
    #endif

    std::cout << "embindcefv8 - tests" << std::endl;

    EXECUTE_JS(
        console.log("Test");
        var test = TestStruct();
        console.log(test);
        console.log(test.floatMember);
    );

    return 0;
}
