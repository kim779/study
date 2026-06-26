#pragma once


// CBrowserWnd
#include "ComponentBase.h"


class CBrowserWnd : public CWnd
{
	DECLARE_DYNAMIC(CBrowserWnd)

public:
	CBrowserWnd();
	virtual ~CBrowserWnd();

	std::vector<std::unique_ptr<ComponentBase>> m_components;
	template <class ComponentType, class... Args> void NewComponent(Args&&... args);

	wil::com_ptr<ICoreWebView2Controller> m_controller;
	wil::com_ptr<ICoreWebView2> m_webView;

	ICoreWebView2Controller* GetWebViewController()
	{
		return m_controller.get();
	}
	ICoreWebView2* GetWebView()
	{
		return m_webView.get();
	}
private:


	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

template <class ComponentType, class... Args> void CBrowserWnd::NewComponent(Args&&... args)
{
	m_components.emplace_back(new ComponentType(std::forward<Args>(args)...));
}
