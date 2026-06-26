#pragma once

#include <DispatcherQueue.h>
#include <windows.ui.composition.interop.h>
#include <winrt/Windows.UI.Composition.Desktop.h>

#ifdef DF_USEAPP
	#include "AppWindow.h"
#else
	#include "CBrowserComp.h"
#endif

//#include "WebView2EnvironmentOptions.h"  //frame.h 에 있음
//#include "WebView2Experimental.h"  //이거 그냥 모른다고 한다.... 우선 주석

class CompositionHost
{
public:
	CompositionHost() = default;
	~CompositionHost();
#ifdef DF_USEAPP
	void Initialize(AppWindow* appWindow);
#else
	void Initialize(CBrowserComp* appWindow);
#endif
	void OnMouseMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void SetBounds(RECT bounds);

private:
	void EnsureDispatcherQueue();
	void CreateDesktopWindowTarget(HWND window);
	void CreateCompositionRoot();
	void CreateWebViewVisual();
	void DestroyWinCompVisualTree();
	void AddElement();
	void UpdateVisual(POINT point, UINT message, WPARAM wParam);
	winrt::Windows::UI::Composition::ContainerVisual FindVisual(POINT point);
	void CreateVisuals();
	winrt::Windows::UI::Color RandomBlue();
	void SetWebViewVisualBounds();
	void ResizeAllVisuals();

#ifdef DF_USEAPP
	AppWindow* m_appWindow = nullptr;
#else
	CBrowserComp* m_appWindow = nullptr;
#endif
	wil::com_ptr<ICoreWebView2Controller> m_controller;
	wil::com_ptr<ICoreWebView2CompositionController> m_compositionController;

	winrt::Windows::UI::Composition::Compositor m_compositor{ nullptr };
	winrt::Windows::System::DispatcherQueueController m_dispatcherQueueController{ nullptr };
	winrt::Windows::UI::Composition::Desktop::DesktopWindowTarget m_target{ nullptr };
	winrt::Windows::UI::Composition::ContainerVisual m_rootVisual{ nullptr };
	winrt::Windows::UI::Composition::ContainerVisual m_webViewVisual{ nullptr };
	RECT m_appBounds = {};

};

