#pragma once


// CSplashWnd

class CSplashWnd : public CWnd
{
	DECLARE_DYNAMIC(CSplashWnd)

public:
	CSplashWnd();
	virtual ~CSplashWnd();

	int	m_percentage;

	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CWnd* pParentWnd = NULL);
	static void HideSplashScreen();
	static void NextScreen(int nPercentage, int nType = 0);
	static bool IsCreated();

protected:
	CString m_str;
	CBitmap	m_bitmap;

	static BOOL	c_showSplashWnd;
	static CSplashWnd* c_splashWnd;

	BOOL	create(CWnd* pParent = NULL);
	void	drawBitmap(CDC *pDC);
	void	setSize(HBITMAP hBitmap);
	CSize	bitmapSize(HBITMAP hBitmap);
	void	setFont(LOGFONT& lf, CString fName, int size, bool isItalic, bool isBold);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual void PostNcDestroy();
};


