#pragma once

#include "resource.h"
#include "TrListctrl.h"
#include "afxwin.h"


#define IDC_LIST_RESOURCE		1000

#define GAP		10
#define DIST_GAP	5
#define SEQN_MAX 8


//struct g_loadhistrsc
//{
//	char	idno[8];		// ID no.
//	char	fname[64];		// File Name
//	char	type;			// Type
//					// 'B' : Form Bin
//					// 'S' : Form Src
//					// 'G' : Image
//					// 'T' : TR Layout
//					// 'I' : Information
//	char ver1[4];
//	char ver2[4];
//	char ver3[4];
//	char usid[16];			// update ID
//	char udat[14];			// Updata Date Time
//	char rchk;			// check release
//					// 'Y' : Release wait ok
//					// 'N' : Release wait No
//	char rlid[16];			// release request usid
//	char rdat[14];			// release date time
//	char mdsc[1024];		// modify description
//};
//
//struct o_loadhistrsc
//{
//	char fname[64];
//	char nrec[4];
//	struct g_loadhistrsc rec[50];
//};

struct g_rschist {
	char	seqn[SEQN_MAX];		/* idno     */
	char	ver1[4]; 		/* Version 1                */
	char	ver2[4]; 		/* Version 2                */
	char	ver3[4]; 		/* Version 3                */
	char	usid[32];		/* update Id            */
	char	udat[14];		/* Update datetime           */
	char	ruid[32];		/* release request usid   */
	char	rdat[14];		/* release date time    */
	char	mdsc[1024];		/* modify description         */
};

struct o_loadhistrsc {
	char	fnam[64];		/* File Name(FORMName)     */
	char	type[1];
	char	seqn[SEQN_MAX];
	char	mesg[80];
	char	nrec[4];
	struct	g_rschist	rhist[1];
};

struct inin
{
	char name[4];
	char age[3];
	char cnt[2];
};

// CResourceHistDlg 대화 상자입니다.

class CResourceHistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResourceHistDlg)

public:
	CResourceHistDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CResourceHistDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RESOURCE_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetRoot(CString sRoot) { m_sRoot = sRoot; }
	void SetName(CString sMap)  { m_sMap = sMap;   }
	void SetID(CString sID)	    { m_sID = sID;     }
	void setPath(CString Path)  { m_sPath = Path;  }


private:

	CTrListctrl	m_ResourceList;
	int			m_row, m_col;
	CFont			m_font;
	class CFormLoadDlg*	m_formDlg;
	class CDownload*	m_pDown;
	CString			m_sRoot;
	CString			m_sMap;
	CString			m_sID;
	CString			m_sPath;
	char*			m_seqn;

public:
	void	SetGridData(char* pchData);
	void	SearchHistory(char Type = 'S', char dirt = ' ', char* seqn = NULL);
	virtual BOOL OnInitDialog();

private:	
	void	setFont();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg LRESULT OnReceiveClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnChagnes();
};
