#pragma once
// MainWnd.h : header file
//
#include "../../control/fx_grid/fxGrid.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxSpin.h"
#include "grid_inc.h"
#include "BtnTab.h"
#include "../../h/axisvar.h"
#include "sharemsg.h"
#include "../IhUtils.h"
#include "Dlg.h"
#include "ioformat.h"
#include <afxmt.h>
#include "excel.h"

#include "ExpandWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

#define JCOLCOUNT                       18
#define JROWCOUNT						20

#define	WM_MESSAGE	(WM_USER + 150)
#define SETGROUPLIST	1
#define DLGEXIT			2

enum {FDCD, FDNM, SPRC, DTRT, WKRT, RT01, RT03, RT06, RT12, RT36, YRAT, RATE};
enum {IDNM = 13, TERM, STDT, STRT, IVNM};
struct _List
{
	CRect	rect;
	int		icol{};
	int		itoggleIdx{};
	CString	sHeader;
	
	_List(CRect rc, int icol1, int icol2)
	{
		rect = rc;
		icol = icol1;
		itoggleIdx = icol2;
		sHeader = "";
	};
	
	_List()
	{
		itoggleIdx = -1;
	};
};
typedef CArray<_List, _List> C_ListArray;

class CMainWnd : public CWnd
{
// Construction
public:
	//CMainWnd();
	CMainWnd(CWnd *parent);

// Attributes
public:
	CWnd* m_pWizard;

 	CfxImgButton	m_btTEXT;	// ¼³Á¤/¿¹»ó/¿¢¼¿
	CfxImgButton	m_btExcel;
	CComboBox	m_cbArrange;
	CComboBox	m_cbGROUP;
	CComboBox	m_cbFIELD;



// Operations
public:
	void	InitGrid();
	int		insertGridRow(int row, bool binsert=true, bool bheader=false);
	CFont*	GetAxFont(CString fName = _T("±¼¸²Ã¼"), int point = 9, bool bBold = false, bool bItalic = false);
	CFont*	GetAxFont(struct _fontR* font);
	CString	Variant(int nComm, CString strData = _T("") );
	COLORREF GetIndexColor(UINT nIndex);
	
	void	Init();
	void	DrawGridHeader(CDC *pDC);

	LOGFONT	GetLogFont(int fsize, bool bold=false, CString sFont="±¼¸²Ã¼");
	void	MainQueryExcelSise(int gno);
	LRESULT SendTR(CString strCode, CString strData, int iKey, int iStat = NULL);
	void	GridDataParsing(char* datB, int datL);

	void	InitBtntab();
	HBITMAP	getBitmap(CString path);
	COLORREF	GetAxColor(int color);
	CString	GetGroupName(int gno);
	int		loadActiveField();
	CString	loadFieldnm(int fno = -1);
	void	loadTabItem();
	void	loadcfg();
	void	SelectMode(int nmode);
	CRect	MoveButton(CRect rcPrev, CSize size, CSize gap, CfxImgButton* pButton);
	CRect	GetNextRect(CRect rcPrev, CSize size, CSize gap);
	
	void	DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	void	DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	CString GetPalette();
	void	DrawRoundRect(CDC *pDC,int nSx,int nSy,int nEx,int nEy,long Color1,long Color2);
	void	DrawRoundRect(CDC *pDC,RECT Rect,long Color1,long Color2);

	void	GridData();
	void	DeleteGridItem();
	void	DeleteGroupList();
	

	void	QueryExcelSise();
	void	parsingData(char* datB, int datL);
	void	ParsingGroup(char* datB, int datL);

	int		GetLastPage();
	void	SetGroupWndActiveChk();
	void	clickBtn();
	CString GetGroupNo(int nIndex, CString gname);

	void	ParsingDataNGroup(char* datB, int datL);
	void	ParsingRestore(char* datB, int datL);

	int		openView(int type, CString data);

	void	SetGuideMessage(CString sMessage);
	void	ExcelList();

	void	testSaveFile3(CString strSec, CString strDat);

	std::unique_ptr<CExpandWnd>	m_pExpandWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL


protected:

	bool m_bFirst;
	CRect m_pToolLblRect[5];
	//std::unique_ptr<CfxGrid>m_pRemainGrid;
	CfxGrid* m_pRemainGrid;
	COLORREF m_bgcolor,  m_gridhdrbgcolor, m_gridLinecolor, m_gridRowColor[2];
	COLORREF m_gridhdrtxtcolor, m_txtcolor;
	_gridHdr m_GridHdr[JCOLCOUNT+1];
	CStringArray m_arGridHdrList;
	CRect m_pClientRect, m_pContentRect;
	int	m_igridRowHeight, m_iGuideHeight, m_iGridHeadrHeight, m_iToolheight;
	int m_iOldFont;
	C_ListArray	m_arGridToggleList;
	CString		m_fileIMG;
	CString		m_root, m_id, m_user;
	COLORREF	m_clrPallete;
	BOOL		m_bMoveCfg;
	int			m_nMoveSave;
	BOOL		m_bBig;	
	CString		m_fileFIELD;
	CString		m_activefield;
	CString		m_filePPOLIO;
	int			m_selectedTab;
	CString		m_fileCFG;
	CArray<CWnd*, CWnd*>	m_arCONTROL;
	
	CString		m_strPal;
	CRect	roundRect;
	int		m_LineThick;
	int		m_lastpage;
	BOOL	m_dlg;
	BOOL	m_init;
	
	int		m_iSelectRow;
	CString m_strGuideMessage;
	CRect	m_errorrect;
	CArray <item3*, item3*> m_gridiem;
	CArray <_grouplist*, _grouplist*> m_grouplist;
// Implementation
public:
	virtual ~CMainWnd();

private:
	
	CBtnTab m_btntab;
	CFont*	m_pFont;
	std::unique_ptr<CDlg> dlg;
	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeBtntab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)

		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	afx_msg LONG OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

