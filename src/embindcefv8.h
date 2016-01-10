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

#ifdef EMSCRIPTEN
    #define EMBINDCEFV8_BINDINGS EMSCRIPTEN_BINDINGS
#else
    #define EMBINDCEFV8_BINDINGS(name)\
        static struct EmbindCefV8Initializer_##name {\
            EmbindCefV8Initializer_##name() {\
                embindcefv8::getInitializers()[std::string(#name)] = std::function<void()>(&EmbindCefV8Initializer_##name::init);\
            }\
            static void init();\
        } EmbindCefV8Initializer_##name##_instance;\
        void EmbindCefV8Initializer_##name::init()
#endif

namespace embindcefv8
{
    #ifdef CEF
        typedef std::function<void()>
            Initializer;
        typedef std::function<void(CefRefPtr<CefV8Value>&)>
            Registerer;
        typedef std::function<void(CefRefPtr<CefV8Value>&, const CefV8ValueList&)>
            ResultFunction;
        typedef std::function<void(CefRefPtr<CefV8Value>&, void*)>
            GetterFunction;
        typedef std::function<void(CefRefPtr<CefV8Value>&, void*, const CefV8ValueList& arguments)>
            MethodFunction;

        std::map<std::string, Initializer> & getInitializers();
        void onContextCreated(CefV8Context* context);
        std::vector<Registerer> & getRegisterers();

        class FuncHandler : public CefV8Handler
        {
        public:
            FuncHandler(ResultFunction & _func) : CefV8Handler()
            {
                func = _func;
            }

            virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override
            {
                func(retval, arguments);
                return true;
            }

            IMPLEMENT_REFCOUNTING(FuncHandler);
        private:
            ResultFunction
                func;
        };

        template<typename T>
        class Class;

        template<typename T>
        class ClassAccessor : public CefV8Accessor
        {
        public:
            ClassAccessor(T * _object) : CefV8Accessor(), owner(_object)
            {

            }

            virtual bool Get(const CefString& name, const CefRefPtr<CefV8Value> object, CefRefPtr<CefV8Value>& retval, CefString& exception) override
            {
                auto it = Class<T>::getters.find(name);

                if(it != Class<T>::getters.end())
                {
                    it->second(retval, owner);
                    return true;
                }

                return false;
            }

            virtual bool Set(const CefString& name, const CefRefPtr<CefV8Value> object, const CefRefPtr<CefV8Value> value, CefString& exception) override
            {
                return false;
            }

            IMPLEMENT_REFCOUNTING(ClassAccessor);

        private:
            T
                * owner;
        };

        template<typename T>
        struct ValueCreator;

        template<>
        struct ValueCreator<float>
        {
            static void create(CefRefPtr<CefV8Value>& retval, float & value)
            {
                retval = CefV8Value::CreateDouble(value);
            }
        };

        template<>
        struct ValueCreator<int>
        {
            static void create(CefRefPtr<CefV8Value>& retval, int & value)
            {
                retval = CefV8Value::CreateInt(value);
            }
        };

        template<>
        struct ValueCreator<std::string>
        {
            static void create(CefRefPtr<CefV8Value>& retval, std::string & value)
            {
                retval = CefV8Value::CreateString(value);
            }
        };

        template<typename T>
        struct ValueConverter;

        template<>
        struct ValueConverter<int>
        {
            static int get(CefV8Value & v)
            {
                return v.GetIntValue();
            }
        };


        template<typename T, typename ... Args>
        struct MethodInvoker
        {
            static void call(void (T::*field)(Args...), void * object, const CefV8ValueList& arguments)
            {
                ((*(T *) object).*field)();
            }
        };

        template<typename T, typename A0>
        struct MethodInvoker<T, A0 >
        {
            static void call(void (T::*field)(A0), void * object, const CefV8ValueList& arguments)
            {
                ((*(T *) object).*field)(
                    ValueConverter<A0>::get(*arguments[0])
                    );
            }
        };

        template<typename T, typename A0, typename A1>
        struct MethodInvoker<T, A0, A1>
        {
            static void call(void (T::*field)(A0, A1), void * object, const CefV8ValueList& arguments)
            {
                ((*(T *) object).*field)(
                    ValueConverter<A0>::get(*arguments[0]),
                    ValueConverter<A0>::get(*arguments[1])
                    );
            }
        };

        template<typename T, typename A0, typename A1, typename A2>
        struct MethodInvoker<T, A0, A1, A2>
        {
            static void call(void (T::*field)(A0, A1, A2), void * object, const CefV8ValueList& arguments)
            {
                ((*(T *) object).*field)(
                    ValueConverter<A0>::get(*arguments[0]),
                    ValueConverter<A0>::get(*arguments[1]),
                    ValueConverter<A0>::get(*arguments[2])
                    );
            }
        };

        template<typename T, typename A0, typename A1, typename A2, typename A3>
        struct MethodInvoker<T, A0, A1, A2, A3>
        {
            static void call(void (T::*field)(A0, A1, A2, A3), void * object, const CefV8ValueList& arguments)
            {
                ((*(T *) object).*field)(
                    ValueConverter<A0>::get(*arguments[0]),
                    ValueConverter<A0>::get(*arguments[1]),
                    ValueConverter<A0>::get(*arguments[2]),
                    ValueConverter<A0>::get(*arguments[3])
                    );
            }
        };

        template<typename T, typename A0, typename A1, typename A2, typename A3, typename A4>
        struct MethodInvoker<T, A0, A1, A2, A3, A4>
        {
            static void call(void (T::*field)(A0, A1, A2, A3, A4), void * object, const CefV8ValueList& arguments)
            {
                ((*(T *) object).*field)(
                    ValueConverter<A0>::get(*arguments[0]),
                    ValueConverter<A0>::get(*arguments[1]),
                    ValueConverter<A0>::get(*arguments[2]),
                    ValueConverter<A0>::get(*arguments[3])
                    );
            }
        };

    #endif

    template<class T>
    class ValueObject
    {
    public:
        ValueObject(const char *_name)
        {
            name = _name;

            #ifdef EMSCRIPTEN
                emVo = new emscripten::value_object<T>(_name);
            #else
                registerConstructor = false;
            #endif
        }

        ~ValueObject()
        {
            #ifdef EMSCRIPTEN
                delete emVo;
            #else
                if(registerConstructor)
                {
                    auto copied_name = name;

                    getRegisterers().push_back(
                            [copied_name](CefRefPtr<CefV8Value> & module_object)
                            {
                                ResultFunction fc = [](CefRefPtr<CefV8Value>& retval, const CefV8ValueList&) {
                                    T new_object;
                                    ValueCreator<T>::create(retval, new_object);
                                };

                                CefRefPtr<CefV8Value> constructor_func = CefV8Value::CreateFunction(copied_name.c_str(), new FuncHandler(fc));
                                module_object->SetValue(copied_name.c_str(), constructor_func, V8_PROPERTY_ATTRIBUTE_NONE);
                            }
                        );
                }
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
                registerConstructor = true;
            #endif

            return *this;
        }

        template<class F>
        ValueObject & member(const char *name, F (T::*field))
        {
            #ifdef EMSCRIPTEN
                emVo->field(name, field);
            #else
                getters[name] = [field](CefRefPtr<CefV8Value>& retval, void * object) {
                    ValueCreator<F>::create(retval, (*(T *)object).*field);
                };
            #endif

            return *this;
        }

        template<typename C>
        friend class ValueCreator;

    private:
        static std::string
            name;
        #ifdef EMSCRIPTEN
            emscripten::value_object<T>
                * emVo;
        #else
            static std::map<std::string, GetterFunction>
                getters;
            bool
                registerConstructor;
        #endif
    };

    template<class T>
    class Class
    {
    public:
        Class(const char *_name)
        {
            name = _name;

            #ifdef EMSCRIPTEN
                emClass = new emscripten::class_<T>(_name);
            #else
                registerConstructor = false;
            #endif
        }

        ~Class()
        {
            #ifdef EMSCRIPTEN
                delete emClass;
            #else
                if(registerConstructor)
                {
                    auto copied_name = name;

                    getRegisterers().push_back(
                            [copied_name](CefRefPtr<CefV8Value> & module_object)
                            {
                                ResultFunction fc = [](CefRefPtr<CefV8Value>& retval, const CefV8ValueList&) {
                                    T * new_object = new T();
                                    ValueCreator<T>::create(retval, * new_object);
                                };

                                CefRefPtr<CefV8Value> constructor_func = CefV8Value::CreateFunction(copied_name.c_str(), new FuncHandler(fc));
                                module_object->SetValue(copied_name.c_str(), constructor_func, V8_PROPERTY_ATTRIBUTE_NONE);
                            }
                        );
                }
            #endif
        }

        Class & constructor()
        {
            #ifdef EMSCRIPTEN
                T ( * func )() =
                    []() -> T
                    {
                        return T();
                    };

                emscripten::function(name.c_str(), func);
            #else
                registerConstructor = true;
            #endif

            return *this;
        }

        template<class F>
        Class & member(const char *name, F (T::*field))
        {
            #ifdef EMSCRIPTEN
                emClass->property(name, field);
            #else
                getters[name] = [field](CefRefPtr<CefV8Value>& retval, void * object) {
                    ValueCreator<F>::create(retval, (*(T *)object).*field);
                };
            #endif

            return *this;
        }

        template<typename ... Args>
        Class & method(const char *name, void (T::*field)(Args...))
        {
            #ifdef EMSCRIPTEN
                emClass->function(name, field);
            #else
                methods[name] = [field](CefRefPtr<CefV8Value>& retval, void * object, const CefV8ValueList& arguments) {
                    MethodInvoker<T, Args...>::call(field, object, arguments);
                };
            #endif

            return *this;
        }

        template<typename C>
        friend class ValueCreator;
        template<typename C>
        friend class ClassAccessor;

    private:
        static std::string
            name;
        #ifdef EMSCRIPTEN
            emscripten::class_<T>
                * emClass;
        #else
            static std::map<std::string, GetterFunction>
                getters;
            static std::map<std::string, MethodFunction>
                methods;
            bool
                registerConstructor;
        #endif
    };

    template<class T>
    std::string ValueObject<T>::name;

    template<class T>
    std::string Class<T>::name;

    #ifdef CEF
        template<class T>
        std::map<std::string, GetterFunction> ValueObject<T>::getters;

        template<class T>
        std::map<std::string, GetterFunction> Class<T>::getters;
        template<class T>
        std::map<std::string, MethodFunction> Class<T>::methods;

        template<typename T>
        struct ValueCreator
        {
            static void create(CefRefPtr<CefV8Value>& retval, T& value)
            {
                retval = CefV8Value::CreateObject(nullptr);

                if(!ValueObject<T>::name.empty())
                {
                    for(auto& kv : ValueObject<T>::getters)
                    {
                        CefRefPtr<CefV8Value> field_value;
                        kv.second(field_value, (void*) &value);
                        retval->SetValue(kv.first, field_value, V8_PROPERTY_ATTRIBUTE_NONE);
                    }
                }
                else
                {
                    retval = CefV8Value::CreateObject(new ClassAccessor<T>(& value));

                    for(auto& kv : Class<T>::getters)
                    {
                        retval->SetValue(kv.first, V8_ACCESS_CONTROL_DEFAULT, V8_PROPERTY_ATTRIBUTE_NONE);
                    }

                    for(auto& kv : Class<T>::methods)
                    {
                        auto copied_kv = kv;
                        ResultFunction fc = [copied_kv, & value](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
                            CefRefPtr<CefV8Value> field_value;
                            copied_kv.second(field_value, (void*) &value, arguments);
                        };

                        CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(kv.first, new FuncHandler(fc));
                        retval->SetValue(kv.first, func, V8_PROPERTY_ATTRIBUTE_NONE);
                    }
                }
            }
        };
    #endif

    void execute(const char *str);
}
