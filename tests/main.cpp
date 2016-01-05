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
        puts("TestStruct constructor");
    }

    float
        floatMember;
};

int main(int argc, char* argv[])
{
    embindcefv8::ValueObject<TestStruct>("TestStruct")
        .constructor()
        .field("floatMember", &TestStruct::floatMember)
        ;

    #ifdef CEF
        initCef(argc, argv);
    #endif

    std::cout << "embindcefv8 - tests" << std::endl;

    EXECUTE_JS(
        var test = TestStruct();
        console.log(test+'1');
        console.log(test+'2');
        console.log(test+'3');
        console.log(test+'4');
    );


    #ifdef CEF
        finalizeCef();
    #endif

    return 0;
}
