#pragma once
// MapWnd.h : header file
//

#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_grid/fxGrid.h"
#include "../../h/axisfire.h"
#include <afxext.h>
/////////////////////////////////////////////////////////////////////////////
// CMapWnd window
// grid 
class 	_acSave	
{
public:
	CString sPriAcc;	//대표계좌구분
	CString sSortNum;	//계좌일련번호
	CString sAccntNum;	//계좌번호
	CString sAccntName;	//계좌이름
	CString	sPassword;	//계좌비번
	CString sAllocRate;	//배정비율
	CString	sAllocMulti;	//승수
	CString sAccntNick;	//계좌별칭
public:
	_acSave()
	{
		sPriAcc = _T("");
		sSortNum = _T("");
		sAccntNum = _T("");
		sAccntName = _T("");
		sPassword = _T("");
		sAllocRate = _T("");
		sAllocMulti = _T("");
		sAccntNick = _T("");
	};
	
	_acSave(_acSave const &other)
	{
		Copy(other);
	};
	
	~_acSave() {}
	
	inline void Copy(_acSave const &other)
	{
		sPriAcc = other.sPriAcc;
		sSortNum = other.sSortNum;
		sAccntNum = other.sAccntNum;
		sAccntName = other.sAccntName;
		sPassword = other.sPassword;
		sAllocRate = other.sAllocRate;
		sAllocMulti = other.sAllocMulti;
		sAccntNick = other.sAccntNick;
	};
	
};

struct _grInfo {
	char*	heads;
	int	width;
	char*	symbols;
	UINT	format;
	DWORD	attr;
	char*	defaultVal;
	int	param;
};

class	_gpSave	
{
public:
	CString sGrSortNum;		// 그룹일련번호
	CString sGrName;		// 그룹명
	CString sAccntCnt;		// 계좌갯수
	CString	sGroupID;		// 그룹아이디
	CArray <_acSave *, _acSave *>	arAcEdit;
public:
	_gpSave()
	{
		sGrSortNum = _T("");
		sGrName = _T("");
		sAccntCnt = _T("");
		sGroupID = _T("");
		arAcEdit.RemoveAll();
	};
	
	~_gpSave()
	{
		_RemoveAr();
	};
	
	_gpSave(_gpSave const &other)
	{
		Copy(other);
	};
	
	inline	void _RemoveAr()
	{
		for (int ii = 0; ii < arAcEdit.GetSize(); ii++)
		{
			_acSave* acSave = arAcEdit.GetAt(ii);
			if (acSave)
			{
				delete acSave;
				acSave = NULL;
			}
		}
		arAcEdit.RemoveAll();
	};
	
	inline void Copy(_gpSave const &other)
	{
		sGrSortNum = other.sGrSortNum;
		sGrName = other.sGrName;
		sAccntCnt = other.sAccntCnt;
		sGroupID = other.sGroupID;
		
		_acSave* acSave = NULL;
		_acSave* newSave = NULL;
		for (int ii = 0; ii < other.arAcEdit.GetSize(); ii++)
		{
			acSave = other.arAcEdit.GetAt(ii);
			if (acSave)
			{
				newSave = new _acSave();
				newSave->Copy(*acSave);
				arAcEdit.Add(newSave);
			}
		}
	};
};

#define GRIDROW_HEIGHT  18

// grInfoAcc : grid column define
static struct _grInfo grInfoAcc[] = {
	{ "계좌번호",	90,	"A001",	GVFM_CENTER,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "고객명",	120,	"A002",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "Idx",	0,	"A003",	GVFM_CENTER,	GVAT_ONLYROW|GVAT_HIDDEN,	"",	0},	
};
enum { IACOL_ACC = 0, IACOL_NAME, IACOL_IDX };

class CMapWnd : public CWnd
{
private:
	CString GetAcctNameFromAllAccount(const char* acct, const char* name);
	void setMapPass(CString acc, CString pass);
	CString GetString(char *pChar, int nMinLen);
	void oubGroup(char* buf);
	void queryGroup();
	void deleteAllGpSave();
	void UpdateGpSave();
	void nextSend(char next, int sendKey);
	void sendTR(CString trC, char* pBytes, int nBytes, char key);
	void SetString(char *pChar, int nMinLen, CString sStr);
	void queryGroupSave(int sendKey, int selType);
	void UpdateGrid2(int tp);
	void AddAcnt(CString acnt, CString acnm, int idx = -1);
	void UpdateAllAcc(CfxGrid* s_grid = NULL);
	void deleteAllAcSave();
	void LoadAllAcnt();
	void initGrid(CfxGrid* &pGrid, UINT idc);
	CfxStatic*	m_panel;
	CWnd*		m_pWizard;
	CString		m_root;
	CString		m_user;
	CfxStatic*	m_static1;
	CfxStatic*	m_static2;
	CfxStatic*	m_static3;
	CfxStatic*	m_static4;	//2013.05.16 KSJ 계좌목록 정렬순서
	CFont*		m_font;
	CfxGrid*	m_grid1;
	CfxGrid*	m_grid2;
		
	CBitmapButton*	m_btnAdd;
	CBitmapButton*	m_btnInsert;
	CBitmapButton*	m_btnDelete;
	CBitmapButton*	m_btnTop;
	CBitmapButton*	m_btnUp;
	CBitmapButton*	m_btnDown;
	CBitmapButton*	m_btnBottom;
	CBitmapButton*	m_btnAllAdd;
	CBitmapButton*	m_btnAllDelete;
	CBitmapButton*	m_btnOK;
	CBitmapButton*	m_btnCancel;
	CButton*	m_btnRDAccn;	//2013.05.16 KSJ 계좌번호 라디오버튼
	CButton*	m_btnRDName;	//2013.05.16 KSJ 계좌명 라디오 버튼
	
	
	CArray <_acSave *, _acSave *>	m_arAcSave;
	CArray <_gpSave *, _gpSave *>	m_arGpSave;
	CString		m_rtnStr;
	char*		m_sendB;
	char*		m_sendB2;
	int		m_nextTRPos;
	int		m_totTRCnt;
	int		m_subTRCnt;
	int		m_calBufL;
	
	BOOL	m_useAlias;
	CString	m_sStaffNum;
	BOOL	m_bCustomer;
	CMapStringToString		m_mapPass;
	
	
// Construction
public:
	CMapWnd();

// Attributes
public:

// Operations
public:
	//{{AFX_DATA(CMapWnd)
	
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	
	//}}AFX_VIRTUAL

// Implementation
public:
	CString parser(CString &srcstr, CString substr = _T("\t"));
	
	CString getMapPass(CString acc);
	void AddAcctSave(const char* no, const char* nm, const char *al);
	void setImgBtn(CBitmapButton* &pButton, int nLength, BOOL bImageFit, CRect drc, UINT idc, CString title, UINT ibc);
	CFont* getAxFont(CString fName, int point, bool bItalic, int nBold);
	CString Variant(int cmd, CString data = _T(""));
	HBITMAP getBitmap2(CString bmps);
	COLORREF GetIndexColor(int index);
	void init(CWnd* pParent);
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnInsert();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnTop();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnBottom();
	afx_msg void OnBtnAllAdd();
	afx_msg void OnBtnAllDelete();
	afx_msg	void OnBtnOK();
	afx_msg	void OnBtnCancel();

	//}}AFX_MSG
	afx_msg void OnGrid1Click(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGrid1DblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGrid2DblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

