#pragma once
#include "dataio.h"
#include "AxisExt.h"
#include "OpSensCal.h"
#include "Cfg.h"
#include "ScreenSetDlg.h"
#include "grid/GridCtrl.h"
#include "OptionWnd.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SCR_TYPES	5
#define SCR_TYPE1	0
#define SCR_TYPE2	1
#define SCR_TYPE3	2
#define SCR_TYPE4	3
#define SCR_TYPE5	4

#define IDC_CBMARKET	(999)
#define IDC_CBMONTH		(1000)
#define IDC_BTN_CONF	(1001)
#define IDC_BTN_GREEK	(1002)
#define IDC_GRID_FUTURE	(1003)
#define IDC_GRID_OPTION	(1004)
#define IDC_CHECK_PREDICT (1005)
#define IDC_SCRTYPE		(2000)

class CMapWnd : public CWnd, public CAxisExt
{
public:
	CMapWnd(CWnd *pParent);

protected: // Data
	int m_style, m_OptionCurRow, m_FutureCurRow, m_AtmRow, m_RAtmRow;
	CString m_home, m_user, m_name, m_profile;
	CString m_cEvent, m_cOScreen, m_cFScreen;
	int m_cCX, m_cCY, m_cStyle, m_cKind;
	CString strPrevK200Time, strPrevHour,strPrevMin,strPrevSec;
	BOOL m_bClose;
	int m_nIndex;

protected:
	int m_opt_tval, m_opt_bptype, m_opt_rdtype;
	double m_opt_k200rate, m_opt_stim, m_opt_etim;
	double m_cdkr, m_bjis, m_clday, m_bzday, m_clday2, m_bzday2, m_basic;
	CStringArray m_fcodes;
	CMap<CString,LPCSTR, JISU_DATA*,   JISU_DATA*  > m_jdat;
	CMap<CString,LPCSTR, OPTION_DATA*, OPTION_DATA*> m_odat;
	CMap<CString,LPCSTR, FUTURE_DATA*, FUTURE_DATA*> m_fdat;
	CArray<double,double> m_hsga;

protected: // Resource
	CRect m_rcCtrl, m_rcBody;
	COLORREF m_crEven, m_crOdd, m_crAtm;
	CBrush *m_pBrBk;
	CBitmap *m_hRndBmp[4], *m_hGuide, m_hBackground, *m_hBtn2[3], *m_hBtn4[3], *m_hSearch[3], *m_hSend[3], *m_hHist[3];

protected: // Control
	COpSensCal m_opcal;
	CCfg m_cfg;
	CfxStatic *m_stMonth, *m_stCall, *m_stPut;
	std::unique_ptr<CComboBox> m_cbMarket;
	std::unique_ptr<CComboBox> m_cbMonth;
	std::unique_ptr<CButton>m_chkPrd;

	CButton* m_rdType[SCR_TYPES];
	CfxImgButton *m_btnConf, *m_btnGreek;
	CGridCtrl *m_gdFuture, *m_gdOption;

protected:
	void OnOptionGridNotify(GV_DISPINFO *pDI);
	void OnFutureGridNotify(GV_DISPINFO *pDI);

protected:
	void ChangeTheme();
	void DrawOptionData(OPTION_DATA *pod = NULL, CDC *pDC = NULL);
	void DrawFutureData(FUTURE_DATA *pfd = NULL, CDC *pDC = NULL);

protected:
	void Send();
	void ClearData();
	void ParsePIBO4013(LPCSTR dat, int len);
	void ParseJISU(LPCSTR dat, int len);
	void ParseOption(LPCSTR dat, int len);
	void ParseFUTURE(LPCSTR dat, int len);
	void ParseAlert(LPCSTR dat, int len);
	void ParseAlert( struct _alertR* alertR);
	time_t ConvertSecondTime(int parm_year, int parm_month, int parm_day,int parm_hour, int parm_minute, int parm_second);

protected: // Function
	void CalcRAtm();
	void GenFutureData();
	void GenOptionData(OPTION_DATA *pod = NULL);
	double GetOptionStrike(LPCSTR code);
	int  GetMinTime(LPCSTR val);
	void LoadCfg();
	void LoadScreenConfig();
	void SaveScreenConfig();
	void LoadOptionData();
	void LoadFutureData();
	void InitFutureGrid();
	void InitOptionGrid();
	void SelectOptionGrid(int type);
	void LoadImage();
	void DrawRoundRectangle(CDC *pDC, CRect rc, COLORREF colBk);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void MakeBackground();
	void SelectFutureGrid(int nIndex);
	void Draw(CDC *pdc);
	CfxImgButton* CreateImgButton( LPCSTR caption, int nID, CBitmap **bmp );
	CfxStatic* CreateStatic(LPCTSTR lpszCaption, COLORREF colBk, COLORREF colFg, COLORREF colBorder=0);

	void MakeOptionWnd();
	//COptionWnd* m_pOptionwnd[4];
	std::array<std::shared_ptr<COptionWnd>, 4> m_pOptionwnd ;
	int m_key[4];
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

public:
	virtual ~CMapWnd();

protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnScreenType1();
	afx_msg void OnScreenType2();
	afx_msg void OnScreenType3();
	afx_msg void OnScreenType4();
	afx_msg void OnScreenType5();
	afx_msg void OnMonthChange();
	afx_msg void OnMarketChange();
	afx_msg void OnGreek();
	afx_msg void OnConf();
	afx_msg void OnCheckPredict();
};

