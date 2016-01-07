#include "embindcefv8.h"
#include <iostream>
#include "cef.h"

#ifdef EMSCRIPTEN
    #define EXECUTE_JS      EM_ASM
#else
    #define EXECUTE_JS(src) \
        executeJs( #src "\nstop();" );
#endif

struct AStruct
{
    AStruct()
        :
        floatMember(16.0f),
        intMember(1024),
        stringMember("A sample string")
    {
    }

    float
        floatMember;
    int
        intMember;
    std::string
        stringMember;
};

struct AStructContainer
{
    AStructContainer()
        :
        aMember()
    {
    }

    void modifyMembers()
    {
        aMember.floatMember *= 2;
        aMember.intMember *= 2;
        aMember.stringMember = "Another string";
    }

    void aMethod()
    {
        std::cout << "aMethod() called" << std::endl;
    }

    void aMethod1(const int a)
    {
        std::cout << "aMethod1() called with " << a << std::endl;
    }

    void aMethod2(const int a, const int b)
    {
        std::cout << "aMethod2() called with " << a << ", " << b << std::endl;
    }

    AStruct
        aMember;
};

int main(int argc, char* argv[])
{
    {
        embindcefv8::ValueObject<AStruct>("AStruct")
            .constructor()
            .member("floatMember", &AStruct::floatMember)
            .member("intMember", &AStruct::intMember)
            .member("stringMember", &AStruct::stringMember)
            ;

        embindcefv8::Class<AStructContainer>("AStructContainer")
            .constructor()
            .member("aMember", &AStructContainer::aMember)
            .method("aMethod", &AStructContainer::aMethod)
            .method("aMethod1", &AStructContainer::aMethod1)
            .method("aMethod2", &AStructContainer::aMethod2)
            .method("modifyMembers", &AStructContainer::modifyMembers)
            ;
    }

    #ifdef CEF
        initCef(argc, argv);
    #endif

    EXECUTE_JS(
        var test = Module.AStruct();
        console.log(test.floatMember);
        console.log(test.intMember);
        console.log(test.stringMember);

        test = Module.AStructContainer();
        console.log(test.aMember.floatMember);
        console.log(test.aMember.intMember);
        console.log(test.aMember.stringMember);

        test.modifyMembers();

        console.log(test.aMember.floatMember);
        console.log(test.aMember.intMember);
        console.log(test.aMember.stringMember);

        test.aMethod();
        test.aMethod1(1);
        test.aMethod2(1, 2);
    );


    #ifdef CEF
        finalizeCef();
    #endif

    return 0;
}
