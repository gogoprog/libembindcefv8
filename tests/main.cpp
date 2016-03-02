#include "embindcefv8.h"
#include <iostream>
#include "cef.h"

#ifdef EMSCRIPTEN
    #define executeFile(file) \
        EM_ASM( global.Module = Module; require('./main.js'); )
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

    AStruct(const int i)
        :
        floatMember(16.0f * i),
        intMember(1024 * i),
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
        aMember(),
        aInt(128)
    {
    }

    AStructContainer(const int a)
        :
        aMember(),
        aInt(a)
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
    }

    void aMethod1(const int a)
    {
    }

    void aMethod2(const int a, const int b)
    {
    }

    void aMethod3(const int a, const int b, const int c)
    {
    }

    AStruct
        aMember;
    int
        aInt;
};

EMBINDCEFV8_BINDINGS(test)
{
    embindcefv8::ValueObject<AStruct>("AStruct")
        .constructor()
        .constructor<int>()
        .property("floatMember", &AStruct::floatMember)
        .property("intMember", &AStruct::intMember)
        .property("stringMember", &AStruct::stringMember)
        ;

    embindcefv8::Class<AStructContainer>("AStructContainer")
        .constructor()
        .constructor<int>()
        .property("aMember", &AStructContainer::aMember)
        .property("aInt", &AStructContainer::aInt)
        .method("aMethod", &AStructContainer::aMethod)
        .method("aMethod1", &AStructContainer::aMethod1)
        .method("aMethod2", &AStructContainer::aMethod2)
        .method("aMethod3", &AStructContainer::aMethod3)
        .method("modifyMembers", &AStructContainer::modifyMembers)
        ;
}

int main(int argc, char* argv[])
{
    #ifdef EMSCRIPTEN
        EM_ASM( global.Module = Module; var test = require('./kludjs.js'); global.test = test; require('./main.js'); );
    #else
        executeFile("kludjs.js");
        executeFile("main.js");
    #endif

    #ifdef CEF
        initCef(argc, argv);
        processLoop();
        finalizeCef();
    #endif

    return 0;
}
