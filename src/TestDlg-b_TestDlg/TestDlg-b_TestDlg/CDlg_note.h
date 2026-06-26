#pragma once


// CDlg_note 대화 상자
#define DF_NOTEWIDTH 250
#define DF_NOTEHEIGHT 130
#define DF_NOTEHMARGIN 5
#define DF_NOTEHEAD  50

bool sticker_call(int, WPARAM, LPARAM);
class CDlg_note : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_note)

public:
	CDlg_note(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_note();

	CString m_slog{};
	int getNottY();
	void DrawBack(CDC* pdc);
	BOOL	DrawRow(CDC* pDC, long nRow, CRect visibleRc, BOOL ShowLOG = FALSE);
	void AddOrderDetail(CString strData = "");
	int			m_lDataHeight = DF_NOTEHEIGHT;
	int			m_lHeadHeight = DF_NOTEHEAD;
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
	void ArrangNoteWnd(bool btop = true);


	BOOL IsPointInScrollBarArea(CPoint point);
	BOOL m_bDragging;     // 드래그 여부를 확인하는 플래그
	int m_nLastYPos;      // 마지막 마우스 Y 위치

	int m_iInsertType{};
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGNOTE };
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
public:

	std::unique_ptr <class CNotePaneWnd> m_pPaneWnd{};
	std::unique_ptr <class CFuncWnd> m_pFuncWnd{};
};

