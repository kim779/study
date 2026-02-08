// AcntDialogBar.h: interface for the CAcntDialogBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACNTDIALOGBAR_H__C54D93D3_1D1E_4D88_8F8B_150C722C2A66__INCLUDED_)
#define AFX_ACNTDIALOGBAR_H__C54D93D3_1D1E_4D88_8F8B_150C722C2A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class AFX_EXT_CLASS CAcntDialogBar : public CControlBar
{
	const COLORREF	m_clrBtnHilight;
	const COLORREF	m_clrBtnShadow;
// Construction
public:
	CAcntDialogBar(CWnd* parent, bool (*callback)(int, WPARAM, LPARAM), CWnd* wizard = NULL);  // standard constructor
// Attributes
public:

	BOOL IsHorz() const;
// Operations
public:

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcntDialogBar)
	public:
	virtual BOOL Create(CWnd* pParentWnd/*, CDialog *pDialog, CString &pTitle*/, UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_RIGHT);
	virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
    virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAcntDialogBar();
	void StartTracking();
    void StopTracking(BOOL bAccept);
    void OnInvertTracker(const CRect& rect);
	CPoint& ClientToWnd(CPoint& point);
	void ProcessAccount(char* data, int len);
	void ProcessGroup(char* data, int len);
	void Refresh813(int tp);
	void SetAuthInfo(LPCSTR usid, LPCSTR uspw) { m_usid=usid; m_uspw=uspw;}
	CString Variant(int nComm, CString strData = "");
protected:

	CWnd* m_parent{};
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
//////////////
    CSize       m_sizeMin;
    CSize       m_sizeHorz;
    CSize       m_sizeVert;
    CSize       m_sizeFloat;
    CRect       m_rectBorder;
    CRect       m_rectTracker;
    UINT        m_nDockBarID;
    CPoint      m_ptOld;
    BOOL        m_bTracking;
    BOOL        m_bInRecalcNC;
    int         m_cxEdge;
	CRect		m_rectUndock;
	CRect		m_rectClose;
	CRect		m_rectGripper;
	int			m_cxGripper;
	int			m_cxBorder;
	class CfxIGrid2* m_grid{};
	class CComboBox* m_grp{};
	class CNButton* m_search{};  //조회
	

    CBrush		m_brushBkgd;

	// Rob Wolpov 10/15/98 Added support for diagonal resizing
	int  m_cyBorder;
	int  m_cMinWidth;
	int  m_cMinHeight;
	int  m_cCaptionSize;
	void		DrawGripper(CDC &dc);
protected:

	// Generated message map functions
	//{{AFX_MSG(CAcntDialogBar)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);   //vc2019 UINT->LRESULT
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnGroupSelEndOk();
	afx_msg LRESULT OnGrpEdit(WPARAM wparam, LPARAM lparam); //vc2019 void->LRESULT   , WPARAM wparam, LPARAM lparam 추가
	afx_msg LRESULT OnGridDblClk(WPARAM wparam, LPARAM lparam);  //vc2019 void->LRESULT
	DECLARE_MESSAGE_MAP()
private:
	void MenuCommand(int tp);
	void InitGridData();
	CString parseX(CString &srcstr, CString substr);
	int _httoi(const TCHAR *value);
	DWORD GetAttrByStr(CString sAttr);
	CString m_data;
	CWnd* m_wizard{};
	CString m_dept, m_usid, m_uspw;
	bool sendTR(CString trN, char* datB, int datL, BYTE stat, int key);
	bool sendTR(char* datB, int datL);
	void QueryAccntList(CString strAccount);
	void QueryGroupList(CString strGrp);
	void ReadAccount(CString grp);
	void ReadGroup();
	bool TotalRefresh;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLDIALOGBAR_H__6EB5FA61_FFAD_11D1_98D4_444553540000__INCLUDED_)
