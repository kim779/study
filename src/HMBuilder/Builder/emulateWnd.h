#pragma once


// CEmulateWnd 뷰입니다.

class CEmulateWnd : public CView
{
	DECLARE_DYNCREATE(CEmulateWnd)

public:
	CEmulateWnd();
	virtual ~CEmulateWnd();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};


