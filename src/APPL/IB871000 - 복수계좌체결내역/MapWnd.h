#pragma once

#define IDC_CB_GROUP	(1000)
#define IDC_TREE		(1001)
#define IDC_BTN_SAVE	(1002)
#define IDC_BTN_CONF	(1003)
#define IDC_BTN_SEND	(1004)
#define IDC_GRID		(1005)
#define IDC_BTN_HIST	(1006)
#define IDC_ED_PASSWD	(1007)
#define IDC_CHK_CODE	(1008)
#define IDC_CHK_ACNO	(1009)

#include "AxisExt.h"
#include "data_io.h"
#include "HistoryDlg.h"
#include "TreeListCtrl.h"
#include "../../control/fx_grid/fxGrid.h"

#define GRID_COL_SIZE	(7)
#define WMX_INIT_FOCUS	(WM_USER+1212)
#define WMX_INIT_SIZE	(WM_USER+1213)
#define WMX_LAZY_TRIG	(WM_USER+1214)

class CAccount
{
public:
	CAccount() : load(FALSE) { }
	CAccount(LPCSTR acno) : acno(acno), load(FALSE) { }
	CAccount(const CAccount &rsh) : 
		acno(rsh.acno), acnm(rsh.acnm), pass(rsh.pass), alias(rsh.alias), load(rsh.load), wrap(rsh.wrap) { }

public:
	BOOL load=FALSE, wrap=FALSE;
	CString acno;
	CString acnm;
	CString pass;
	CString alias;
	CString GetMaskAcno() {
		LPCSTR st = (LPCSTR)acno;
		CString mask;
		mask.Format("%.3s-%.2s-%.6s", st, st+3, st+5);
		return mask;
	}
};

class CGroup
{
public:
	CGroup() { }
	CGroup(LPCSTR name) : gpnm(name) { }
	CGroup(const CGroup &rsh) { _Copy(rsh); }
	void operator=(const CGroup &rsh) { _Copy(rsh); }

private:
	void _Copy(const CGroup &rsh)
	{
		gpnm = rsh.gpnm;
		Accounts.RemoveAll();
		Accounts.Copy(rsh.Accounts);
	}

public:
	CString gpnm;
	CArray<CAccount, CAccount&> Accounts;
};

typedef struct _ChegData
{
	_ChegData() { ZeroMemory(this, sizeof(_ChegData)); }
	
	char acno[12];
	char code[12];
	char hnam[41];
	char gubn[21];
	char stat[21];
	int juno, ojno, jprc, jqty, cprc, cqty, xqty;

	CString getKey() {
		char buff[32];
		sprintf(buff, "%s%10d", acno, juno);	//2015.05.28 KSJ %05d -> %10d
		return buff;
	}

	CString toString() {
		char buff[256];
		sprintf(buff, "acno(%s) code(%s) hnam(%s) gubn(%s) stat(%s) juno(%d) ojno(%d) jprc(%d) jqty(%d) cprc(%d) cqty(%d) xqty(%d)\n",
			acno, code, hnam, gubn, stat, juno, ojno, jprc, jqty, cprc, cqty, xqty);
		return buff;
	}
} ChegData;

class CMapWnd : public CWnd, public CAxisExt
{
public:
	CMapWnd(CWnd *pParent);

protected:
	BOOL m_bRelayCode, m_bRelayAcno;
	CString m_home, m_user, m_name, m_profile;
	CArray<CGroup, CGroup&> m_groups;
	CMap<CString, LPCSTR, HTREEITEM, HTREEITEM> m_anodes;
	CMap<CString, LPCSTR, HTREEITEM, HTREEITEM> m_cnodes;
	CArray<CAccount, CAccount&> m_req_acno;
	CMap<CString, LPCSTR, ChegData, ChegData&> m_chegdata;
	CMapStringToString m_name2codes;

protected:
	int m_conf_colsize[GRID_COL_SIZE];
	int m_conf_width, m_conf_height;
	CRect m_rcCtrl, m_rcBody, m_rcGuide;
	CBitmap *m_hRndBmp[4], *m_hGuide, m_hBackground, *m_hBtn2[3], *m_hBtn4[3], *m_hSearch[3], *m_hSend[3], *m_hHist[3];
	CBrush m_brBk;

protected:
	CStatic m_stGroup, m_stPass;
	CComboBox m_cbGroup;
	std::unique_ptr<CfxEdit>m_edPass;
	std::shared_ptr<CfxImgButton>m_btnConf, m_btnSend, m_btnHist;
	std::unique_ptr<CButton> m_chkCode, m_chkAcno;
	std::unique_ptr<CfxStatic> m_stTMado[2], m_stTMasu[2], m_stTotal[2], m_stHist;
	CTreeListCtrl m_tree;
	std::unique_ptr<CHistoryDlg> m_pHistory;
	HINSTANCE	m_hiSha256;	//2013.12.23 KSJ 일방향암호화

protected:
	void req_group();
	void res_group(LPCSTR data, int dlen);
	void req_scheg(LPCSTR nkey=NULL);
	void res_scheg(LPCSTR data, int dlen);

	void proc_notice(LPCSTR data, int dlen);

protected:
	void Account2Tree(LPCSTR acno);
	void UpdateTree(LPCSTR acno, LPCSTR code, LPCSTR hnam, LPCSTR gubn);
	CString GetPassword(LPCSTR acno);
	CString GetName(LPCSTR acno);
	CString GetMaskAcno(LPCSTR acno);

protected:
	void LoadImage();
	void DrawRoundRectangle(CDC *pDC, CRect rc);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void MakeBackground();
	void Draw(CDC *pdc);
	std::shared_ptr<CfxImgButton> CreateImgButton( LPCSTR caption, int nID, CBitmap **bmp );
	void SafeDeleteWnd(CWnd *pWnd);

	void CreateStatic(CStatic *pSt, LPCTSTR lpszCaption);
	void InitTree();
	void SaveConfig();
	void LoadConfig();
	CString GetEncPassword(CString sPswd);

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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGroupChange();
	afx_msg void OnCellDblClick(NMHDR *pHDR, LRESULT *pResult);
	afx_msg void OnCellClick(NMHDR *pHDR, LRESULT *pResult);
	afx_msg void OnClickSave();
	afx_msg void OnClickConf();
	afx_msg void OnClickSend();
	afx_msg void OnClickHist();
	afx_msg void OnRelayAcno();
	afx_msg void OnRelayCode();
	afx_msg LRESULT OnInitFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInitSize(WPARAM wParam, LPARAM lParam);
};


