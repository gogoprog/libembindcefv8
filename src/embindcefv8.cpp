#include "embindcefv8.h"

namespace embindcefv8
{
    #ifdef CEF
        std::vector<Registerer>
            registerers;
        CefRefPtr<CefBrowser>
            browser;
        CefRefPtr<CefV8Context>
            context;
        CefRefPtr<CefV8Value>
            moduleObject;

        std::map<std::string, Initializer> & getInitializers()
        {
            static std::map<std::string, Initializer>
                initializers;

            return initializers;
        }

        void onContextCreated(CefV8Context *context_)
        {
            context = context_;
            moduleObject = CefV8Value::CreateObject(nullptr);
            context->GetGlobal()->SetValue("Module", moduleObject, V8_PROPERTY_ATTRIBUTE_NONE);

            for(auto& kv : getInitializers())
            {
                kv.second();
            }

            for(auto func : registerers)
            {
                func(moduleObject);
            }
        }

        void setBrowser(CefRefPtr<CefBrowser> _browser)
        {
            browser = _browser;
        }

        bool hasContext()
        {
            return context;
        }

        CefRefPtr<CefV8Value> & getModuleObject()
        {
            return moduleObject;
        }

        std::vector<Registerer> & getRegisterers()
        {
            return registerers;
        }
    #endif

    void executeJavaScript(const char *str)
    {
        #ifdef EMSCRIPTEN
            emscripten_run_script(str);
        #else
            CefRefPtr<CefFrame> frame = browser->GetMainFrame();
            frame->ExecuteJavaScript(str, frame->GetURL(), 0);
        #endif
    }
}
