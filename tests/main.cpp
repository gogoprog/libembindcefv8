#include "embindcefv8.h"
#include <iostream>
#include "cef.h"

#ifdef EMSCRIPTEN
    #define EXECUTE_JS      EM_ASM
#else
    #define EXECUTE_JS(src) \
        executeJs( #src "\nstop();" );
#endif

struct FloatStruct
{
    FloatStruct()
        :
        floatMember(123)
    {
    }

    float
        floatMember;
};

struct StructContainerStruct
{
    StructContainerStruct()
        :
        testMember()
    {
    }

    FloatStruct
        testMember;
};

int main(int argc, char* argv[])
{
    {
        embindcefv8::ValueObject<FloatStruct>("FloatStruct")
            .constructor()
            .field("floatMember", &FloatStruct::floatMember)
            ;

        embindcefv8::ValueObject<StructContainerStruct>("StructContainerStruct")
            .constructor()
            .field("testMember", &StructContainerStruct::testMember)
            ;
    }

    #ifdef CEF
        initCef(argc, argv);
    #endif

    std::cout << "embindcefv8 - tests" << std::endl;

    EXECUTE_JS(
        var test = Module.FloatStruct();
        console.log(test.floatMember);

        test = Module.StructContainerStruct();
        console.log(test.testMember.floatMember);
    );


    #ifdef CEF
        finalizeCef();
    #endif

    return 0;
}
