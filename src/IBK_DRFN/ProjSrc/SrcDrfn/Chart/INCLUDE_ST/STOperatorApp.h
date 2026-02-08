#if !defined(AFX_STOPERATORAPP_H__7D2EF7FF_AB35_4BCF_927A_992A57139255__INCLUDED_)
#define AFX_STOPERATORAPP_H__7D2EF7FF_AB35_4BCF_927A_992A57139255__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STOperatorApp.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CSTOperatorApp thread
class CDBMgr;
class CSTActor;
class AFX_EXT_CLASS CSTOperatorApp : public CWinApp
{
public:
	CSTOperatorApp();
	long LoadST(void (*fnSTMain)(CSTActor* pSTActor)
					, HWND hParent
					, HWND hSocket
					, CDBMgr* pChartItem
					, long lKey
					, LPCTSTR lpSTName
					, int nSTPos
					, LPCTSTR lpAdditionalOption
					, char *szInputs[]
					, LPCTSTR lpUserID
					, LPCTSTR lpPassword
					, long lExpiredDate
					, long lSourceType
					, BOOL bUseUserID	
					, BOOL bUsePassword	
					, BOOL bUseExpiredDate	);
	long UnloadST(long lSTActor);
	long ExecuteST(long lSTActor, LPCTSTR lpInputData);
	long StopST(long lSTActor);
	long PauseST(long lSTActor);
	long UpdateST(long lSTActor,long lPos, BOOL bIsUpdateData);
	long CommandST(long lSTActor, LPCTSTR lpCommand, LPCTSTR lpOption);
private:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTOperatorApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	

	//{{AFX_MSG(CSTOperatorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOPERATORAPP_H__7D2EF7FF_AB35_4BCF_927A_992A57139255__INCLUDED_)
