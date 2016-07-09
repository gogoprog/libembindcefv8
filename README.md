# libembindcefv8
Bind C/C++ to Javascript using emscripten embind or chromium embedded framework v8

### How to use

    #include "embindcefv8.h"

    embindcefv8::Class<UserClass>("UserClass")
        .constructor()                              // expose the default constructor
        .constructor<int>()                         // expose the constructor with a integer parameter
        .property("aInt", &UserClass::aInt)         // expose a class attribute
        .method("aMethod", &UserClass::aMethod)     // expose a class method
