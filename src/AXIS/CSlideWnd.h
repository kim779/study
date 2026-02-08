#pragma once


// CAutoOrderList 대화 상자
#define DF_NOTEWIDTH 250
#define DF_NOTEHEIGHT 130
#define DF_NOTEHMARGIN 5

class CSlideWnd : public CDialog
{
	DECLARE_DYNAMIC(CSlideWnd)

public:
	CSlideWnd(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSlideWnd();
	CString m_slog{};
	class CMainFrame* m_pMainFrm{};


public: //drawing
	int getNottY();
	void DrawBack(CDC* pdc);
	BOOL	DrawRow(CDC* pDC, long nRow, CRect visibleRc, BOOL ShowLOG = FALSE);

public:
	BOOL m_bIsVisible{};
	void SlideOpen(CRect baseRC);
	void SlideClose(CRect baseRC);


	void AddOrderDetail(CString strData = "");
private:
	int			m_lDataHeight = DF_NOTEHEIGHT;
	int			m_lHeadHeight = DF_NOTEHMARGIN;
	CPoint		m_StartPnt{};
	int		m_wDisableBar = SB_VERT | SB_HORZ;

	void InitScroll();
	void	adjustScroll(int wBar = SB_BOTH);
	long	GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	long	GetScrollMax32(int nBar);
	

	CScrollBar m_scrollBar; // 수직 스크롤바
	int m_totalHeight{}; // 총 컨트롤 높이
	std::vector<CWnd*> m_childWindows; // 자식 윈도우들
// 대화 상자 데이터입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SLIDEWND};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
};
