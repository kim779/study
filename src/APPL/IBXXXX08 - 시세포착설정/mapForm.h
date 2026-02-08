#pragma once


#include "axMapForm.h"
#include "afxtempl.h"
#include "imgBtn.h"
#include "axStatic.h"
#include "exGrid.h"
#include "exListCtrl.h"
#include "exEdit.h"
#include "libDefine.h"
#include "../../h/interst.h"
#include "../../h/jmcode.h"
#include <afxmt.h>
#include <afxtempl.h>
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define ROUND_GAP 5

#define ROUND_PANEL		1
#define ROUND_CONTENTS	2

#define	GUIDE_HEIGHT	18
#define GRIDHEADER_HEIGHT 45

#define	GUIDCOLOR_INDEX		34
#define	GUIDTXTCOLOR_INDEX	97

#define	TOOLGAP			1
#define VERT_GAP		3

#define clIndexPanel	66

#define		BACKINDEXCOLOR			64
#define		TOOLINDEXCOLOR			66

struct _code
{
	CString codeS;
	CString nameS;
	char	kosd{};			// 종목구분	// 0:장내  10:장외  30:제3시장
	char	ssgb{};			// 소속구분
					// 0:default
					// 4:뮤추얼펀드
					// 5:신주인수권
					// 6:리츠
					// 7:신주인수권증서
					// 8:ETF
					// 9:하이일드펀드
};

struct _fcode
{
	CString codeS;
	CString nameS;
};

class CMapForm : public CAxMapForm
{
public:
	CMapForm();
	virtual ~CMapForm();
	DECLARE_DYNCREATE(CMapForm)

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_DATA(CMapForm)
	enum { IDD = IDD_FORMVIEW };
	CImgBtn		m_btnHistory;
	CImgBtn		m_btnCatchDel;
	CImgBtn		m_btnInter;
	CImgBtn		m_btnApply;
	CAxStatic	m_result;
	CExListCtrl	m_list;
	CExEdit		m_editSearch;
	CTreeCtrl	m_tree;
	CImgBtn		m_btnUp;
	CImgBtn		m_btnOk;
	CImgBtn		m_btnDel;
	CImgBtn		m_btnCancel;
	CImgBtn		m_btnADel;
	CImgBtn		m_btnAdd;
	CImgBtn		m_btnSise;
	CString		m_strSearch;
	//}}AFX_DATA

public:
	//{{AFX_VIRTUAL(CMapForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMapForm)
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnadd();
	afx_msg void OnBtnadel();
	afx_msg void OnBtncancel();
	afx_msg void OnBtndel();
	afx_msg void OnBtnok();
	afx_msg void OnBtnup();
	afx_msg void OnRdsave();
	afx_msg void OnRddel();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnsise();
	afx_msg void OnBtnupdate();
	afx_msg void OnSetfocusSearch();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnapply();
	afx_msg void OnBtninter();
	afx_msg void OnBtncatchdel();
	afx_msg void OnBtnhistory();
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLocal(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGrCatchLMouseup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGrCatchDblClicked(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnGrCatchRMouseDown(NMHDR *pNMHDR, LRESULT *pResult);	
	DECLARE_MESSAGE_MAP()
public:
	
	//afx_msg void OnChangeSearch();
protected:
	void	initConstruct();
	void	destroy();
	void	ChangeColors();

	// controls
	void	initControl();

	void	initGrid();
	void	initGrid(CExGrid* pGrid, UINT idc);
	int		insertGrid(CExGrid* pGrid, UINT idc, int row);
	void	updateCatch();
	void	updateCatch(int row, _codeinfo *codeinfo, bool bInit);

	void	initList();
	BOOL	addColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width);
	int		addItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem);

	void	initTree();
	void	setSubTree(HTREEITEM hItem, int gubn);
	void	updateListCode(int kosd, int ssgb = -1);
	void	updateListOther(int gubn, int sub);
	void	updateListFCode();
	void	updateListOCode(int type);
	
	void	loadEnvironment();
	void	saveEnvironment();
	bool	loadJCode();
	bool	loadFCode();
	bool	loadOCode();
	bool	loadCatch();
	void	saveCatch();
	
	void	selectList(int key = SC_SISE);
	void	querySise(CString codeS, int key = SC_SISE, bool bCheck = true);
	void	queryHistory();
	void	sendTR(CString trC, char* pBytes, int nBytes, int key, BYTE stat);
	void	sendTRx(CString trCode, char* datB, int datL, int key);
	void	oubSise(char* buf, int len);
	void	oubHistory(char* buf, int len);

	void    alertx(gsl::span<DWORD> data);
	void	alert(char* buf, int len);				// 실시간 시세
	void	alertSise(const CMapStringToString &rtmStore, CString symS, int col);
	void	trigger(CString tmpS);
	void	domino(CString tmpS);

	void	setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit);

	COLORREF GetAxColor(int iColor);
	CString	getCodeName(CString codeS, int& gubn);
	bool	isExistFile(int gno);
	CString	getString(char* datB, int datL);

	void	deleteAllMapCatch();
	void	popupSetup(bool bAdd, CString codeS, int updateRow = -1);
	void	setMessage(char* format, ...);
	void	setMessageBox(char* format, ...);	

	int		findIndex(char* str, int len);
	int		findIndex(char* str, int len, bool bnum);
	int		delSpace(char* str, int len);

	void	InitMoveCtrl();
	int		MoveCtrl(UINT nCtrlID, int nCx, int nCy, int nCtrlWidthGap);
	int		MoveCtrl(CWnd* pWnd, int nCx, int nCy, int nCtrlWidthGap);
#ifdef _DEBUG
	void	draw_groupBox(CDC* pdc, int ids);
#else
	void	draw_groupBox(xxx::CMemDC *pdc, int ids);
#endif

	CString GetPalette();
	void	DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	void	DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	CString	Variant(int comm, CString data);
	void	DrawGuideBar(CDC* pDC, CRect rc);
	void	DrawCornerRoundBitmap(CDC* pDC, CRect rc);

	int		getFKind(int siseCol);	
	
private:
	HTREEITEM _hInter{};
	CCriticalSection	m_section;
	std::unique_ptr<class	CExGrid>	m_pSise;
	std::unique_ptr<class	CExGrid>	m_pCatch;
	std::unique_ptr<class	CSetupDlg>	m_pSetupDlg;
	std::unique_ptr<class	CCatchWnd>	m_pPopup;
	
	CString	m_sRootDir;	CString	m_sTabDir;				// directory
	CString	m_sImgDir;	CString	m_sUser;
	CString m_strPal;
	bool	m_bCustomer;	CString	m_szFilename;	CString	m_envPath;
	char	m_gubn{};		CString	m_sName;				// 대투/하나구분

	COLORREF	m_clrBk{}, m_clrBlink{}, m_clrRoundColor{};	// 바탕색, blink, Round Color
	CString		m_rtsCode, m_saveCode;					// real code, save code
	CString	m_sFontName;	int	m_nFontPoint{};				// font
	CFont*	m_pFont{};	CFont*	m_pBFont{};
	LOGFONT	m_lf{};		LOGFONT	m_blf{};
	
	CArray	<_code, _code>		m_arCode;
	CArray	<_fcode, _fcode>	m_arFCode;
	CArray	<ojcode, ojcode>	m_arOCode;
	CStringArray			m_arOHead;

	std::vector<std::unique_ptr<_codeinfo>>	_vInfo;
	CMap<CString, LPCTSTR, _codeinfo *, _codeinfo *>	m_mapCatch;
	CStringArray	m_arCatch;
	bool		m_bNum{};
	tId			m_IdCurKind{};
	CBrush		m_br;
	int			m_iGuideHeight{};
	int			m_iGridHeadrHeight{};
	COLORREF	m_guidecolor{}, m_guideTxtcolor{}, m_panelcolor[2]{};
	HBITMAP		m_hGuideBitmap{}, m_hRoundBitmap[4]{};

	CRect   m_ClientRect;
	CString	m_strGuideMessage;

public:

	void Request_GroupList();
	void Request_GroupCode(int iseq);
	void receiveOub(int key, CString data);
	void receiveOubCode(CString& data, int key);
};

