#pragma once
#include <d2d1.h>
#include <dwrite.h>

class ControlWnd : public CWnd
{
	DECLARE_DYNCREATE(ControlWnd)

public:
	ControlWnd();
	ControlWnd(_param* pParam, CWnd* pWizard);

	CWnd*   m_pWizard{};
	CString m_sParamKey{};

	virtual ~ControlWnd();
	virtual void OnFinalRelease();

private:
	// Current ticker data
	CStringW m_label;
	CStringW m_price;
	CStringW m_change;
	int      m_dir;     // 1=up, -1=down, 0=flat

	// Previous frame data kept for slide animation
	CStringW m_prevPrice;
	CStringW m_prevChange;
	int      m_prevDir;

	// Animation state
	float m_animProg;
	bool  m_bAnimating;
	enum { TIMER_ANIM = 1 };

	// D2D device-independent resources
	ID2D1Factory*      m_pD2DFactory;
	IDWriteFactory*    m_pDWFactory;
	IDWriteTextFormat* m_pFmtLabel;
	IDWriteTextFormat* m_pFmtValue;

	// D2D device-dependent resources
	ID2D1HwndRenderTarget* m_pRT;
	ID2D1SolidColorBrush*  m_pBrushLabel;
	ID2D1SolidColorBrush*  m_pBrushUp;
	ID2D1SolidColorBrush*  m_pBrushDown;
	ID2D1SolidColorBrush*  m_pBrushFlat;

	HRESULT InitD2D();
	HRESULT CreateDeviceResources();
	void    DiscardDeviceResources();
	void    RecreateTextFormats(float height);
	void    ReleaseTextFormats();
	void    RenderD2D();
	void    DrawRow(float yOff, const CStringW& price, const CStringW& change, int dir);

	afx_msg int  OnCreate(LPCREATESTRUCT lpCS);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg SHORT SetData(int gubn, BSTR sVal);
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	enum { dispidSetData = 1L };
};
