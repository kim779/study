#if !defined(AFX_STACTOR_H__22B05A13_F544_4173_8BA7_42DFCC32BB15__INCLUDED_)
#define AFX_STACTOR_H__22B05A13_F544_4173_8BA7_42DFCC32BB15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STActor.h : header file
//

#include <atlbase.h>
#include "../Include_ST/ISTController.h"

/////////////////////////////////////////////////////////////////////////////
// CSTActor window

class CDBMgr;
class CSTControllerLoader;
class AFX_EXT_CLASS CSTActor : public CWnd
{
// Construction
public:
	CSTActor();

	void	SetReceiver(HWND hReceiver, long lKey);
	long	Execute(LPCTSTR lpInputData);
	long	Update(long lPos, BOOL bIsUpdateData);
	long	Pause();
	long	Stop();
	long	Command(LPCTSTR lpCommand, LPCTSTR lpOption);

	void	(*m_fnSTMain)(CSTActor* pSTActor);

// Attributes
public:
	LPCTSTR		 m_lpFilePath;
	CStringArray m_stringArrayInput;
	CString		 m_strUserID;
	CString		 m_strPassword;
	long		 m_lExpiredDate;
	long		 m_lSourdeType;
	BOOL		 m_bUseUserID;
	BOOL		 m_bUsePassword;
	BOOL		 m_bUseExpiredDate;
	CString		 m_strSTName;
	int			 m_nSTPos;
	CString		 m_strAdditionalOption;

	//KHD
	long		 m_pChartItem;

// Operations
public:
	BOOL	SetInputs(long lIndex, LPCTSTR lpName, int nType, double dInitValue);
	BOOL	SetVariables(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue);
	BOOL	SetArray(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue, long lArrayCnt);
	BOOL	SetSubInputsFrom(long lIndex, double dValue);
	BOOL	SetCurrentPos(long lPos);
	BOOL	IfCondition(BOOL bIfConditon);
	void	SubIn(long lIndex);
	void	SubOut();
	void	IfIn();
	void	IfOut();
	void	ForIn();
	void	ForOut();
	void	BracketIn();
	void	BracketOut();

	double	GetRawData(long lDataIndex, double dReverseIndex);
	CString	GetRawSData(long lDataIndex, double dReverseIndex);
	double	GetVarHolderData(long lHolderIndex, double dIndex);
	double	GetInputHolderData(long lHolderIndex, double dIndex);
	double	GetFuncData(double dIndex);
	void	SetVarHolderData(long lHolderIndex,double dIndex, double dValue);	
	void	SetInputHolderData(long lHolderIndex,double dIndex, double dValue);
	void	SetFuncData(double dIndex, double dValue);

	void	SetArrayHolderData(long lHolderIndex,double dIndex, double dValue);	
	double	GetArrayHolderData(long lHolderIndex,double dIndex);	

	BOOL	DeclareSubFunc(long lHolderIndex, LPCTSTR lpFuncName);
	double	SubFunc(long lSTActor, long lIndexFunc, long lIndex,
					BOOL bTemp0 = FALSE, 
					BOOL bTemp1 = FALSE, 
					BOOL bTemp2 = FALSE, 
					BOOL bTemp3 = FALSE, 
					BOOL bTemp4 = FALSE, 
					BOOL bTemp5 = FALSE, 
					BOOL bTemp6 = FALSE, 
					BOOL bTemp7 = FALSE, 
					BOOL bTemp8 = FALSE, 
					BOOL bTemp9 = FALSE);

	BOOL	DeclareInnerFunc(long lType, long lHolderIndex, LPCTSTR lpFuncName);
	double	InnerFunc(long lType, long lIndexFunc, long lIndex,
					CComVariant varArg1 = NULL, 
					CComVariant varArg2 = NULL, 
					CComVariant varArg3 = NULL, 
					CComVariant varArg4 = NULL, 
					CComVariant varArg5 = NULL, 
					CComVariant varArg6 = NULL, 
					CComVariant varArg7 = NULL, 
					CComVariant varArg8 = NULL, 
					CComVariant varArg9 = NULL, 
					CComVariant varAgr10 = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTActor)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTActor();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSTActor)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CISTController*			m_pSTController;
	CSTControllerLoader*	m_pSTControllerLoader;

	HWND	m_hReceiver;
	long	m_lKey;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STACTOR_H__22B05A13_F544_4173_8BA7_42DFCC32BB15__INCLUDED_)
