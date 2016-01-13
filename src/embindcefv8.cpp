#include "embindcefv8.h"

namespace embindcefv8
{
    #ifdef CEF
        std::vector<Registerer>
            registerers;
        CefRefPtr<CefBrowser>
            browser;

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

        void setBrowser(CefRefPtr<CefBrowser> _browser)
        {
            browser = _browser;
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
