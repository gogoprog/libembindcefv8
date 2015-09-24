#pragma once

#include <cstring>

#ifdef EMSCRIPTEN
    #include <emscripten.h>
    #include <emscripten/bind.h>
#else
    #include "include/cef_client.h"
    #include "include/cef_app.h"
    #include <functional>
#endif

namespace embindcefv8
{
    #ifdef CEF
        void onContextCreated(CefV8Context* context);
        std::vector<std::function<void(CefV8Context*)>> & getRegisterers();
    #endif

    template<class T>
    class ValueObject
    #if CEF
        : public CefV8Handler
    #endif
    {
    public:
        ValueObject(const char *_name)
        {
            name = _name;

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
            #ifdef EMSCRIPTEN
                T ( * func )() =
                    []() -> T
                    {
                        return T();
                    };

                emscripten::function(name.c_str(), func);
            #else
                CefRefPtr<CefV8Value> constructor_func = CefV8Value::CreateFunction(name, this);
                std::string name_copy = name;

                getRegisterers().push_back(
                        [name_copy, constructor_func](CefV8Context* context)
                        {
                            context->GetGlobal()->SetValue(name_copy.c_str(), constructor_func, V8_PROPERTY_ATTRIBUTE_NONE);
                        }
                    );
            #endif

            return *this;
        }

        template<class F>
        ValueObject & field(const char *fieldName, F (T::*field))
        {
            #ifdef EMSCRIPTEN
                vo->field(fieldName, field);
            #endif

            return *this;
        }

        #ifdef CEF
            virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override
            {

                return false;
            }

            IMPLEMENT_REFCOUNTING(MyV8Handler);
        #endif

    private:
        std::string
            name;
        #ifdef EMSCRIPTEN
            emscripten::value_object<T>
                * vo;
        #else
            std::map<std::string, std::function<void(T*)>>
                accessors;
        #endif
    };

    void execute(const char *str);
}
