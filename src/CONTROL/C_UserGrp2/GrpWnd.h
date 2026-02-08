#pragma once

#include <afxtempl.h>
#include <afxmt.h>

struct _tick	// Left/Right/Bottom Tick Info
{
	bool	bDraw=false;
	int	lcolor=0;
	int	lstyle=0;
	int	lwidth=0;
	CRect	tkRect;
};

struct _RgnInfo	// Region Info
{
	int	tkHeight=0;

	_tick	tick[3];	// left/right/bottom
	CRect	gpRect;		// graph rect
};

class CGrpWnd : public CWnd
{
public:
	CGrpWnd(CWnd *pView, CWnd *pParent, struct _param *pInfo);
	virtual ~CGrpWnd();

	//{{AFX_VIRTUAL(CGrpWnd)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGrpWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT	OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnGrp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void	DrawGraph(CDC *pDC);
	void	ClearGrp();
	void	Resize();
	struct _NMinMaxChk*	GetMinMaxChk(int rKey);
	int	GetGrpKindFromConfig(int index);
	void	DrawEmpty(CDC *pDC, bool bInit);
	void	DrawFrameRect(CDC *pDC, CRect rc, COLORREF rgb);
	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
	void	SetShowLine(int nLine,bool bflag);
	class CCrossLine* GetCrossLine() { return m_pCrossLine.get(); }
	void	MessageToParent(int message, int wParam, int lParam)	{ m_pParent->SendMessage(message, wParam, lParam); }
public:
	std::array<std::unique_ptr<struct _RgnInfo>, MAX_REGION>	m_pRgnInfo;		// 영역 정보	
	std::array <std::unique_ptr<class CGrp_Base>, MAX_GRAPH>       m_pGrpInfo;		// 그래프 정보
	std::unique_ptr<class CGrp_Data>	m_pDataInfo;			// 그래프 데이터 정보

	bool		m_bShow[MAX_GRAPH];
	CArray<_GrpLine, _GrpLine>	m_arGrpLine;		// 그래프 설정정보

	CString			m_CfgRgnInfo;		// 설정된 영역 분할 비율 정보
	int			m_CfgRgnCnt;		// 설정된 영역 개수
	int			m_RgnCnt;		// 영역 개수
	int			m_GrpCnt;		// 그래프 개수

	std::vector<std::unique_ptr<struct _NMinMaxChk>>	m_pMinMaxChk;
	int			m_MinMaxCnt;
	int			m_totalDay;
	int			m_dispPos;
	int			m_dispEnd;
	int			m_dispDay;
	int			m_ShowCnt;	// param option, 출력개수
	int			m_ReqCnt;	// param option, 요청개수

	CFont			*m_pFont;	// Font
	CFont			*m_pBoldFont;	// Bold Font
	COLORREF		m_TkTextColor;	// Tick Text color
	COLORREF		m_TkLineColor;	// Tick Internal Line line

	CRect			m_ObjRect;	// Entire Rect
	CRect			m_GrpRect;	// Graph Rect
	int			m_fPoint;	// Font point
	CString			m_fName;	// Font name
	
	int			m_dIndex;	// 일주월분틱

	CString			m_sTipName;	// Tooltip Head String
	CString			m_sExtraName;	// Extraname
	bool			m_bStringIndex;	// 하단 색인정보가 문자열데이터인지
	CString			m_sTickUnit[3];	// 좌/우/하 단위
	bool			m_bHDraw;	// 수평선 그리기
	bool			m_bVDraw;	// 수직선 그리기
	bool			m_bCtrl;	// contrl 사용여부
	bool			m_bMinMax;	// 최대최소 표시
	bool			m_bPreview;	// Preview
	bool			m_bRevDraw;	// 역순으로 그리기
	bool			m_bTickVertic;	// 하단틱 수직으로
	bool			m_bItemRts;	// 각 항목별 개별 실시간
	bool			m_b3NChart;	// IGK_2D3NBAR

	CString		m_strMapOption{}, m_strparamName{}; 
	int			m_key{};
	bool			m_bInterValSearch{};

private:
	int	DispatchData(WPARAM wParam, LPARAM lParam);
	int	DispatchOubData(WPARAM wParam, LPARAM lParam);
	void	RealTimeData(CString sRTS);
	void	RealTimeData(struct _alertR* alert);

	bool	RealTimeBasic(CString sCode, DWORD* data);
	bool	RealTime2009Basic(CString sCode, DWORD* data);
	bool	RealTimeItems(CString sCode, DWORD* data);
	bool	AssignRegion(int nRgn);
	bool	AssignGraph(int nGrp);
	
	void	ParseParam(char *param);
	int	RequestHead();
	void	RemoveComponent();
	int	GetPalette(int index);
	int	GetTickColor(int color);

	BOOL	IsPtInGrp();
	CString	GetTipStr(CPoint point);

	bool	ChangeDisplayScale(int scale);
	bool	ChangeDisplay(int dispDay);
	bool	ChangeDisplayShift(int shift);
	void	ChangePalette();
	void	ExcelProc();
	void	RecalculateMinMax();
	void	ReviseTick();
	int	CetBottomVerticHeight(CDC *pDC);
	void	SetError(CString sErr) { m_sError = sErr; }

private:
	CWnd			*m_pView;
	CWnd			*m_pParent;
	std::unique_ptr<class CToolTip>		m_pToolTip;
	std::unique_ptr<class CCrossLine>	m_pCrossLine;
	char			m_pHeader[sz_UPHEADER+1];
	
	int			m_TickView;
	CString			m_PosData;
	UINT			m_timerID;	
	COLORREF		m_tRGB;		// foreground color
	COLORREF		m_pRGB;		// background color
	int			m_tOrgRGB;	// param
	int			m_pOrgRGB;	// param
	CC_UserGrpApp*		m_pApp;	
	CString			m_sError;
	int			m_nShowLine;

	CString m_slog;
	CString m_strCode{};
	BOOL m_bDestory{};
};
