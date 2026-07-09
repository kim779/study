#pragma once

#include <afxwin.h>
#include <EventToken.h>
#include <functional>
#include <map>

struct ICoreWebView2Environment;
struct ICoreWebView2Controller;

struct CbrowserXImpl;


class CbrowserX : public CWnd
{
public:
	enum class CallbackType
	{
		CreationCompleted,
		NavigationCompleted,
		TitleChanged,
	};

	using CallbackFunc = std::function<void()>;

public:
	CbrowserX();
	virtual ~CbrowserX();

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const	RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* = NULL) override;
	BOOL CreateAsync(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CallbackFunc onCreated);

	void RegisterCallback(CallbackType const type, CallbackFunc callback);

	RECT GetBounds();
	void SetBounds(LONG const width, LONG const height) { Resize(width, height); }
	void Resize(LONG const width, LONG const height);

	CString GetLocationURL();

	void xNavigate(CString const& url, CallbackFunc onComplete);
	void xNavigatePost(CString const& url, CString const& content, CString const& headers, CallbackFunc onComplete = nullptr);
	void GoBack();
	void GoForward();
	void Reload();
	void Stop();
	bool IsNavigating() const { return m_isNavigating; }
	void DisablePopups();

	void PrintDocument();
	CString GetTitle() const { return m_strTitle; }

	void SetParentView(CView* pViewParent) { m_pViewParent = pViewParent; }
	bool IsWebViewCreated() const;

protected:
	DECLARE_DYNCREATE(CbrowserX)
	DECLARE_MESSAGE_MAP()

private:
	CbrowserXImpl* m_pImpl;
	std::map<CallbackType, CallbackFunc> m_callbacks;

	EventRegistrationToken m_navigationCompletedToken = {};
	EventRegistrationToken m_navigationStartingToken = {};
	EventRegistrationToken m_documentTitleChangedToken = {};

	bool	m_isNavigating = false;
	CView* m_pViewParent = nullptr;
	CString m_strTitle;

private:
	void RunAsync(CallbackFunc callback);

	void CloseWebView();
	void RegisterEventHandlers();
	void ResizeToClientArea();
	void NavigateTo(CString url);
	CString NormalizeUrl(CString url);

	static CString GetInstallPath();
	static CString GetInstallPathFromRegistry(bool const searchWebView = true);
	static CString GetInstallPathFromDisk(bool const searchWebView = true);
	static CString GetUserDataFolder();

	HRESULT InitializeWebView();
	HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnCreateWebViewControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);

	static PCTSTR GetWindowClass();
	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result);

	BOOL CreateHostWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
public:
	afx_msg void OnDestroy();
};

