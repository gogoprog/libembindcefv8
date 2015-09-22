#pragma once

#ifdef EMSCRIPTEN
    #include <emscripten.h>
    #include <emscripten/bind.h>
#endif

namespace embindcefv8
{
    template<class T>
    class ValueObject
    {
    public:
        ValueObject(const char *_name)
        {
            strcpy(name, _name);

            #ifdef EMSCRIPTEN
                vo = new emscripten::value_object<T>(_name);
            #endif
        }

        ~ValueObject()
        {
            #ifdef EMSCRIPTEN
                delete vo;
            #endif
        }

        ValueObject & constructor()
        {
            T ( * func )() =
                []() -> T
                {
                    return T();
                };

            #ifdef EMSCRIPTEN
                emscripten::function(name, func);
            #endif

            return *this;
        }

        ValueObject & field(const char *fieldName, float (T::*field))
        {
            #ifdef EMSCRIPTEN
                vo->field(fieldName, field);
            #endif

            return *this;
        }

    private:
        #ifdef EMSCRIPTEN
            char
                name[128];
            emscripten::value_object<T>
                * vo;
        #endif
    };

    void execute(const char *str)
    {
        #ifdef EMSCRIPTEN
            emscripten_run_script(str);
        #endif
    }
}
