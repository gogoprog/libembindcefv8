#ifndef EMSCRIPTEN
#include "cef.h"

#include "include/cef_client.h"
#include "embindcefv8.h"

CefRefPtr<Handler>
    handler;
CefRefPtr<CefBrowser>
    browser;
bool
    mustProcess(true);

class StopHandler : public CefV8Handler {
public:
    StopHandler() {}

    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE
    {
        mustProcess = false;
        puts(name.ToString().c_str());
        return true;
    }

    IMPLEMENT_REFCOUNTING(LocalV8Handler);
};

void App::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
    CefRefPtr<CefV8Handler> stop_handler = new StopHandler();
    CefRefPtr<CefV8Value> stop_func = CefV8Value::CreateFunction("stop", stop_handler);
    context->GetGlobal()->SetValue("stop", stop_func, V8_PROPERTY_ATTRIBUTE_NONE);

    embindcefv8::onContextCreated(& * context);
}

void App::OnContextInitialized()
{
    CefWindowInfo window_info;

    window_info.SetAsOffScreen(nullptr);
    window_info.SetTransparentPainting(true);

    CefBrowserSettings browser_settings;
    browser = CefBrowserHost::CreateBrowserSync(window_info, & * handler, "about:blank", browser_settings, nullptr);
}

void App::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
{
}

bool Handler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
    rect = CefRect(0, 0, 128, 128);
    return true;
}

void Handler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
}

bool Handler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line)
{
    puts(message.ToString().c_str());
    return true;
}

void initCef(int argc, char *argv[])
{
    #ifdef _LINUX
        CefMainArgs args(argc, argv);
    #elif defined(_WINDOWS)
        CefMainArgs args(GetModuleHandle(NULL));
    #endif

    CefRefPtr<App> app(new App);

    int exit_code = CefExecuteProcess(args, app.get(), nullptr);

    if (exit_code >= 0)
    {
        exit(exit_code);
        return;
    }

    handler = new Handler();

    CefSettings settings;
    memset(&settings, 0, sizeof(CefSettings));
    settings.single_process = true;
    settings.no_sandbox = true;
    settings.multi_threaded_message_loop = false;
    settings.log_severity = LOGSEVERITY_DISABLE;
    settings.size = sizeof(CefSettings);

    CefInitialize(args, settings, app.get(), nullptr);
}

void executeJs(const char *src)
{
    CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    frame->ExecuteJavaScript(src, frame->GetURL(), 0);

    mustProcess = true;
    while(mustProcess)
    {
        CefDoMessageLoopWork();
    }

    for(int i=0; i<100; i++)
    {
        CefDoMessageLoopWork();
    }
}

void finalizeCef()
{
    CefShutdown();
}

#endif
