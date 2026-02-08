#pragma once
// MainWnd.h : header file
//
#include "UpGroup.h"
#include "../../axis/axMsg.hxx"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"

#define WD_UPDBCLICK	WM_USER + 0x01
#define WD_TRIGGERCC	WM_USER + 0x02

#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
struct jid2grid_mod 
{
	char	upcode[3];
	char	upname[20];
	char	xcode[12];
	char	hname[40];
	char	curr[7];
	char	diff[7];
	char	rate[7];
	char	siga[10];		//시가총액		(억원)
	char	amt[12];		//거래량
	char	mny[9];			//거래대금
	char	pers[8];		//개인 순매수량		(천주)
	char	forg[8];		//외국인 순매수량	(천주)
	char	gign[8];		//기관 순매수량		(천주)
//	char	mnpers[12];		//개인 순매수대금	(천원)
//	char	mnforg[12];		//외국인 순매수대금	(천원)
//	char	mngign[12];		//기관 순매수대금	(천원)
};

struct pidojid2_mod
{
	char	rcnt[4];
	jid2grid_mod	grid[1];
};

#define L_pidojido_mid	sizeof(pidojido_mod)


struct	pibo2133_grid_mod {
	char	upcode[3];		// 업종코드
	char	upname[20];		// 업종명
	char	jisu[9];		// 지수
	char	diff[8];		// 대비
	char	rate[7];		// 등락율
	char	amt[8];			// 거래량(천주)
	char	gmnvol[8];		// 거래대금(백만원)
	char	siga[12];		// 시가총액(백만원)
	char	pers[8];		// 개인순매수량
	char	forg[8];		// 외국인순매수량
	char	gign[8];		// 기관인순매수량
	char	mnpers[12];		// 개인순매수대금
	char	mnforg[12];		// 외국인순매수대금
	char	mngign[12];		// 기관순매수대금
};

struct  pibo2133_mod {                                                                                      
	char	nrec[4];                                                                                       
	pibo2133_grid_mod	grid[1];                                                                               
};


#define KEY_JID2		0x01
#define KEY_2133		0x02
#define KEY_LOADUPMAP		0x03

#define TR_PIBO2911		"PIBO2911"

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();
	CMainWnd(CWnd* parent,_param* param);

// Attributes
public:

private:
	CWnd*	m_pWnd;
	CString m_szName;
	int	m_key;
	CRect	m_rect;
	
	float	m_fMaxRate;
	int	m_nUpColor;
	int	m_nDownColor;
	int	m_nEqualColor;

	CString m_strUp;
	enum	_ctype	{ ctUP=0, ctSTOCK} m_nSearchType;	//0 업종지도,  1 종목지도

	COLORREF m_ggColor;
	COLORREF m_fgColor;
	COLORREF m_psColor;

	int	m_nMarketType;
	int	m_nViewType;
	int	m_nStandard;
	int	m_nScale;

	double	m_dTotalSiga;					//업종에 따른 시가총액 합계 
	double	m_dTotalAmt;					//업종에 따른 거래량  합계
	double	m_dTotalMny;					//업종에 따른 거래대금 합계 
	double	m_dTotalFore;					//업종에 따른 외국인 순매수량 합계 
	double	m_dTotalGi;					//업종에 따른 기관계 순매수량 합계 
	CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> m_arrChild;
//	CMapStringToPtr	m_mapChild;
	CMap <CString, LPCTSTR, std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> m_mapChild;

	bool	m_bSending;

// Operations
public:
	void	ParseOub(char* pData, int nData);
	void	Parse2133(char* pData, int nData);
	void	updateRect(CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo,CRect rect);
	void	updateRectByRate(CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo,CRect rect);
	CString Parser(CString &strSrc, CString strSub);

private:
	void	updateColor();
	void	updateRect();
	void	updateRectByRate();
	void	SendTR(CString name, CString data, BYTE type, int key);
	void	sendTR(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey);
	CString Variant(int comm, CString data);
	void	draw();
	void	Quick_Sort(CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo, int count, int type);
	void	Quick_Recursive(CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo, int left, int right, int type);
	bool	CheckUpcode(CString strup);
	int	openView(int type, CString param);
	double	GetUpdata(CString strUp, CString strgubn);
	CString GetForedata(CString strUp);
	CString GetGigndata(CString strUp);
	void	clear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
public:
	virtual void OnFinalRelease();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpDBClick(WPARAM wParma, LPARAM lParam);
	afx_msg LONG OnRButtonClick(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	afx_msg void MakeMap(short type);
	afx_msg void LoadMap(short market, short type, short type2, short scale);
	afx_msg void LoadUpMap(LPCTSTR sUpcode);
	afx_msg void SearchByUp(LPCTSTR sUpCode, LPCTSTR sGubn);
	afx_msg void redo();
	afx_msg BSTR GetDataArr(LPCTSTR sArr, LPCTSTR sGubn);
	afx_msg void Redraw(short type1, short type2, short type3, short type4, LPCTSTR sGubn);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};