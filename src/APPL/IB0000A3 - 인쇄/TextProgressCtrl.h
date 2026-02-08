#pragma once



//
// TextProgressCtrl.h : header file for CTextProgressCtrl class

////////////////////////////////////////////////////////////////////////////////
#define SetBarColour		SetBarColor
#define GetBarColour		GetBarColor
#define SetBgColour			SetBarBkColor
#define GetBgColour			GetBarBkColor
#define SetTextColour		SetTextColor
#define GetTextColour		GetTextColor

#ifndef PBS_SMOOTH
	#define PBS_SMOOTH			0x01
#endif
#ifndef PBS_VERTICAL
	#define PBS_VERTICAL		0x04
#endif
#ifndef PBS_MARQUEE
	#define PBS_MARQUEE			0x08
#endif
#ifndef PBM_SETRANGE32
	#define PBM_SETRANGE32		(WM_USER+6)
	typedef struct
	{
		int iLow;
		int iHigh;
	} PBRANGE, *PPBRANGE;
#endif
#ifndef PBM_GETRANGE
	#define PBM_GETRANGE		(WM_USER+7)
#endif
#ifndef PBM_GETPOS
	#define PBM_GETPOS			(WM_USER+8)
#endif
#ifndef PBM_SETBARCOLOR
	#define PBM_SETBARCOLOR		(WM_USER+9)
#endif
#ifndef PBM_SETBKCOLOR
	#define PBM_SETBKCOLOR		CCM_SETBKCOLOR
#endif
#ifndef PBM_SETMARQUEE
	#define PBM_SETMARQUEE		(WM_USER+10)
#endif

#ifndef PBM_GETBARCOLOR 
	#define PBM_GETBARCOLOR			(WM_USER+100)
#endif 

#ifndef PBM_GETBKCOLOR
	#define PBM_GETBKCOLOR			(WM_USER+101)
#endif

#define PBM_SETTEXTCOLOR		(WM_USER+102)
#define PBM_GETTEXTCOLOR		(WM_USER+103)
#define PBM_SETTEXTBKCOLOR		(WM_USER+104)
#define PBM_GETTEXTBKCOLOR		(WM_USER+105)
#define PBM_SETSHOWPERCENT		(WM_USER+106)
#define PBM_ALIGNTEXT			(WM_USER+107)
#define PBM_SETMARQUEEOPTIONS	(WM_USER+108)


////////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl class

class CTextProgressCtrl : public CProgressCtrl
{
	public:
		CTextProgressCtrl();
		virtual ~CTextProgressCtrl();

	// Operations
	public:
		inline COLORREF SetBarColor(COLORREF crBarClr = CLR_DEFAULT)
			{ ASSERT(::IsWindow(m_hWnd)); return ((COLORREF) ::SendMessage(m_hWnd, PBM_SETBARCOLOR, 0, (LPARAM)crBarClr)); }
		inline COLORREF GetBarColor() const
			{ ASSERT(::IsWindow(m_hWnd)); return ((COLORREF) ::SendMessage(m_hWnd, PBM_GETBARCOLOR, 0, 0)); }
		inline COLORREF SetBarBkColor(COLORREF crBarBkClr = CLR_DEFAULT)
			{ ASSERT(::IsWindow(m_hWnd)); return ((COLORREF) ::SendMessage(m_hWnd, PBM_SETBKCOLOR, 0, (LPARAM)crBarBkClr)); }
		inline COLORREF GetBarBkColor() const
			{ ASSERT(::IsWindow(m_hWnd)); return ((COLORREF) ::SendMessage(m_hWnd, PBM_GETBKCOLOR, 0, 0)); }
		inline COLORREF SetTextColor(COLORREF crTextClr = CLR_DEFAULT)
			{ ASSERT(::IsWindow(m_hWnd)); return ((COLORREF) ::SendMessage(m_hWnd, PBM_SETTEXTCOLOR, 0, (LPARAM)crTextClr)); }
		inline COLORREF GetTextColor() const
			{ ASSERT(::IsWindow(m_hWnd)); return ((COLORREF) ::SendMessage(m_hWnd, PBM_GETTEXTCOLOR, 0, 0)); }
		inline COLORREF SetTextBkColor(COLORREF crTextBkClr = CLR_DEFAULT)
			{ ASSERT(::IsWindow(m_hWnd)); return ((COLORREF) ::SendMessage(m_hWnd, PBM_SETTEXTBKCOLOR, 0, (LPARAM)crTextBkClr)); }
		inline COLORREF GetTextBkColor() const
			{ ASSERT(::IsWindow(m_hWnd)); return ((COLORREF) ::SendMessage(m_hWnd, PBM_GETTEXTBKCOLOR, 0, 0)); }
		inline BOOL SetShowPercent(BOOL bShow)
			{ ASSERT(::IsWindow(m_hWnd)); return ((BOOL) ::SendMessage(m_hWnd, PBM_SETSHOWPERCENT, (WPARAM)bShow, 0)); }
		inline DWORD AlignText(DWORD dwAlignment = DT_CENTER)
			{ ASSERT(::IsWindow(m_hWnd)); return ((DWORD) ::SendMessage(m_hWnd, PBM_ALIGNTEXT, (WPARAM)dwAlignment, 0)); }
		inline BOOL SetMarquee(BOOL bOn, UINT uMsecBetweenUpdate)
			{ ASSERT(::IsWindow(m_hWnd)); return ((BOOL) ::SendMessage(m_hWnd, PBM_SETMARQUEE, (WPARAM)bOn, (LPARAM)uMsecBetweenUpdate)); }
		inline int SetMarqueeOptions(int nBarSize)
			{ ASSERT(::IsWindow(m_hWnd)); return ((BOOL) ::SendMessage(m_hWnd, PBM_SETMARQUEEOPTIONS, (WPARAM)nBarSize, 0)); }

	// Generated message map functions
	protected:
		//{{AFX_MSG(CTextProgressCtrl)
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		//}}AFX_MSG

		// handlers for shell progress control standard messages
		afx_msg LRESULT OnSetRange(WPARAM, LPARAM lparamRange);
		afx_msg LRESULT OnSetPos(WPARAM nNewPos, LPARAM);
		afx_msg LRESULT OnOffsetPos(WPARAM nIncrement, LPARAM);
		afx_msg LRESULT OnSetStep(WPARAM nStepInc, LPARAM);
		afx_msg LRESULT OnStepIt(WPARAM, LPARAM);
		afx_msg LRESULT OnSetRange32(WPARAM nLowLim, LPARAM nHighLim);
		afx_msg LRESULT OnGetRange(WPARAM bWhichLimit, LPARAM pPBRange);
		afx_msg LRESULT OnGetPos(WPARAM, LPARAM);
		afx_msg LRESULT OnSetBarColor(WPARAM, LPARAM crBar);
		afx_msg LRESULT OnSetBarBkColor(WPARAM, LPARAM crBarBk);

		// new handlers added by this class
		afx_msg LRESULT OnGetBarColor(WPARAM, LPARAM);
		afx_msg LRESULT OnGetBarBkColor(WPARAM, LPARAM);
		afx_msg LRESULT OnSetTextColor(WPARAM, LPARAM crText);
		afx_msg LRESULT OnGetTextColor(WPARAM, LPARAM);
		afx_msg LRESULT OnSetTextBkColor(WPARAM, LPARAM crTextBk);
		afx_msg LRESULT OnGetTextBkColor(WPARAM, LPARAM);
		afx_msg LRESULT OnSetShowPercent(WPARAM bShow, LPARAM);
		afx_msg	LRESULT OnAlignText(WPARAM dwAlignment, LPARAM);
		afx_msg LRESULT OnSetMarquee(WPARAM bOn, LPARAM nMsecBetweenUpdate);
		afx_msg LRESULT OnSetMarqueeOptions(WPARAM nBarSize, LPARAM);

		// helper functions
		void CreateVerticalFont();
		void CommonPaint();

		DECLARE_MESSAGE_MAP()

		// variables for class
		BOOL		m_bShowPercent;				
		CFont		m_VerticalFont;				
		COLORREF	m_crBarClr, m_crBarBkClr;	
		COLORREF	m_crTextClr, m_crTextBkClr;	
		DWORD		m_dwTextStyle;				
		int			m_nPos;						
		int			m_nStepSize;				
		int			m_nMin, m_nMax;				
		int			m_nMarqueeSize;				
};

