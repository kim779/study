#pragma once
#include <d2d1.h>
#include <dwrite.h>

class CParam
{
public:
	int		key = 0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point = 0;		// font point
	int		style = 0;		// font style
	DWORD		tRGB = 0;			// text color index
	DWORD		pRGB = 0;			// paint color index
	CString		options;		// properties
};

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

	void parseOptions();
	void SetParam(_param* pParam);
	enum AnimType {
		ANIM_NORMAL   = 0,
		ANIM_SLIDE    = 1,
		ANIM_ODOMETER = 2
	};

	CParam m_Param;
	int m_type{};
private:
	CStringW m_text;          // current display value
	CStringW m_prevText;      // previous value (for animation)
	int      m_dir;           // animation direction: 1=up, -1=down
	int      m_colorDir;      // text color: 1=red(+), -1=blue(-), 0=black
	int      m_prevColorDir;  // previous text color (for animation frame)
	float  m_iInterval = 0.04f;

	float  m_animProg;
	bool   m_bAnimating;
	double m_currVal;
	double m_targetVal;
	bool   m_bCounting;
	int    m_decimalPlaces;
	enum { TIMER_ANIM = 1 };

	// D2D device-independent resources
	ID2D1Factory*      m_pD2DFactory;
	IDWriteFactory*    m_pDWFactory;
	IDWriteTextFormat* m_pFmt;

	// D2D device-dependent resources
	ID2D1HwndRenderTarget* m_pRT;
	ID2D1SolidColorBrush*  m_pBrushUp;
	ID2D1SolidColorBrush*  m_pBrushDown;
	ID2D1SolidColorBrush*  m_pBrushFlat;

	HRESULT  InitD2D();
	HRESULT  CreateDeviceResources();
	void     DiscardDeviceResources();
	void     RecreateTextFormats(float height);
	void     ReleaseTextFormats();
	void     RenderD2D();
	void     RenderOdometer(float W, float H);
	void     DrawSlice(float yOff, const CStringW& text, int dir);
	float    MeasurePrefixX(const CStringW& display, int prefixLen);
	CStringW CountDisplayText() const;

	afx_msg int  OnCreate(LPCREATESTRUCT lpCS);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg SHORT SetData(int gubn, LPCTSTR sVal);
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	enum { dispidSetData = 1L };
};
