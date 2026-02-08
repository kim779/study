#pragma once

#include <wrl.h>

#include "WebView2EnvironmentOptions.h"
#include "WebView2.h"
#include <dcomp.h>

class CEdgeBrowser : public CWnd
{
public:
	CEdgeBrowser(CWnd* pWizard);
	~CEdgeBrowser();
public:
	CWnd* m_pWizard;
	CWnd* m_pParent;
	bool m_bFirst{};
	CString m_sUrl{};

	bool m_bParentWnd;
	bool m_bCtrlVisible;

public:
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	void CloseWebView(bool cleanupUserDataFolder = false);
	bool InitializeWebView();
	void ResizeEverything();

	void Navigate(CString sUrl);

	void Print();
	void SetVisible(bool bVisible);

	void SetParentWnd(bool bParentWnd);

	// Implementation
protected:
	Microsoft::WRL::ComPtr<ICoreWebView2Environment> m_webViewEnvironment;
	Microsoft::WRL::ComPtr<ICoreWebView2Controller> m_controller;
	Microsoft::WRL::ComPtr<ICoreWebView2> m_webView;

	HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnAcceleratorKeyPress(ICoreWebView2Controller* pSender, ICoreWebView2AcceleratorKeyPressedEventArgs* pEventArgs);
	HRESULT OnNavigationStarting(ICoreWebView2* pICoreWebView2, ICoreWebView2NavigationStartingEventArgs* pEventArgs);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

