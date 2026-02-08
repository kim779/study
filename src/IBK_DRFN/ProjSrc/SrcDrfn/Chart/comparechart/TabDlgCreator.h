// TabDlgCreator.h: interface for the CTabDlgCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABDLGCREATOR_H__7FC4AF02_6618_44AC_BF60_FE159D0AE7F8__INCLUDED_)
#define AFX_TABDLGCREATOR_H__7FC4AF02_6618_44AC_BF60_FE159D0AE7F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLeftBarDlg;
class CTabDlg;
class CTabCtrl;
class CTabDlgCreator  
{
public:
	CTabDlgCreator();
	virtual ~CTabDlgCreator();

	void Init(CWnd* pCompareChartWnd, CLeftBarDlg *pParentDlg,CTabCtrl *pTabCtrl,CWnd *pChartMng, void* pDBMgr);
	void GetCaptionOfTabDlg(UINT nIndex, char *pszCaption);
	CTabDlg* CreateTabDlg(UINT nIndex);
	int GetCntOfTabDlg();
public:
	CLeftBarDlg *m_pParentDlg;
	CTabCtrl *m_pTabCtrl;
	CWnd* m_pCompareChartWnd;
	CWnd* m_pChartMng;
	void* m_pDBMgr;
};

#endif // !defined(AFX_TABDLGCREATOR_H__7FC4AF02_6618_44AC_BF60_FE159D0AE7F8__INCLUDED_)
