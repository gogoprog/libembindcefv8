#include "embindcefv8.h"

namespace embindcefv8
{
    #ifdef CEF
        std::vector<std::function<void(CefV8Context*)>>
            registerers;

        void onContextCreated(CefV8Context* context)
        {
            for(auto func : registerers)
            {
                func(context);
            }
        }

        std::vector<std::function<void(CefV8Context*)>> & getRegisterers()
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
