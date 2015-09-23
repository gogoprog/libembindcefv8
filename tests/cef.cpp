#ifndef EMSCRIPTEN
#include "cef.h"

#include "include/cef_client.h"

CefRefPtr<Handler>
    handler;
CefRefPtr<CefBrowser>
    browser;

void App::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
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
    settings.size = sizeof(CefSettings);

    CefInitialize(args, settings, app.get(), nullptr);
}

void executeJs(const char *src)
{
    CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    frame->ExecuteJavaScript(src, frame->GetURL(), 0);
}

#endif
