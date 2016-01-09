#include "embindcefv8.h"

namespace embindcefv8
{
    #ifdef CEF
        std::vector<Registerer>
            registerers;

        std::map<std::string, Initializer> & getInitializers()
        {
            static std::map<std::string, Initializer>
                initializers;

            return initializers;
        }

        void onContextCreated(CefV8Context *context)
        {
            CefRefPtr<CefV8Value> module_object = CefV8Value::CreateObject(nullptr);
            context->GetGlobal()->SetValue("Module", module_object, V8_PROPERTY_ATTRIBUTE_NONE);

            for(auto& kv : getInitializers())
            {
                kv.second();
            }

            for(auto func : registerers)
            {
                func(module_object);
            }
        }

        std::vector<Registerer> & getRegisterers()
        {
            return registerers;
        }
    #endif

    void execute(const char *str)
    {
        #ifdef EMSCRIPTEN
            emscripten_run_script(str);
        #endif
    }
}
