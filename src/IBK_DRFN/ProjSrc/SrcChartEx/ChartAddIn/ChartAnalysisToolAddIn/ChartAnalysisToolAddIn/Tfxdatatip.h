#ifndef _tfxdatatip_h__
    #define _tfxdatatip_h__


class TFXDataTip : public CWnd
{
// Construction
public:
	TFXDataTip();
	virtual ~TFXDataTip();

	virtual bool Create( void);

public:
	void SetFont(CFont* pFont);
	void Initialise(const COLORREF crColor);
	void SetBackgrandColor(const COLORREF crColor);
    void SetSurrounding(const long& x, const long& y);
    void SetSurrounding(const CSize& offset);
    void SetDelay(const short delay);
	bool SetNewTip( CWnd* pParentWnd, CPoint p_ptCursor);

	void ResetText();
	void AddText(const char* szText);

    bool IsOn() const;
    void On(const bool bOn);
    bool Hide();

protected:
	//{{AFX_VIRTUAL(TFXDataTip)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(TFXDataTip)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	static LRESULT CALLBACK KeyboardHookCallback(const int code, WPARAM wParam, LPARAM lParam);
	static HHOOK m_s_hookProc;
	static TFXDataTip *m_s_pCurrent;

private:
	COLORREF m_crBkColor;
    LOGFONT m_lf;
	CFont* m_pFont;
	CStringList	m_listStrings;
	CPoint m_MousePosition;	// Position of mouse when tip is placed
	CSize m_TipSurrounding;	// Moving the mouse so far is ok (no hiding of tip)
	bool m_bReady;
	bool m_bOn;
	bool m_bRegistered;
	UINT m_timer, m_timerToHide;
	short m_sDelay;

	void Display();
	void RegisterWnd();
	void DataTipKillTimer();
	bool IsPointNearOtherPoint(const CPoint& point, const CPoint& test, const CSize& distance);

// (2006/4/11 - Seung-Won, Bae) Manage OCX Ctrl CWnd Pointer for LButton Processing on ToolTip.
protected:
	CWnd *		m_pChartWnd;

public:
	short	GetDelay( void)	{	return m_sDelay;	}
	void GetMonitorRectFromRect(CRect& rcMonitor, LPRECT prcRect);

};

#endif

