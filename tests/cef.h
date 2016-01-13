#pragma once

#ifndef EMSCRIPTEN

#define CEF_ENABLE_SANDBOX 0

#include "include/cef_app.h"
#include "include/cef_request_handler.h"

class App : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler
{
public:
    App() = default;

    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }
    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
    virtual void OnContextInitialized() override;
    virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) override;

private:

    IMPLEMENT_REFCOUNTING(App);
};

#include "include/cef_client.h"
#include "include/cef_load_handler.h"

class Handler : public CefRenderHandler, public CefClient, public CefRequestHandler, public CefLoadHandler, public CefDisplayHandler
{
public:
    Handler() = default;

    virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
    virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;
    virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line) override;

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
    virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }

    IMPLEMENT_REFCOUNTING(Handler)

private:

    IMPLEMENT_LOCKING(Handler)
};

void initCef(int argc, char *argv[]);
void finalizeCef();
void processLoop();
void executeFile(const char *src);

#endif
