#if !defined(AFX_APQURIER_H__3C18B495_753F_4FEB_8AE0_2EB286B43071__INCLUDED_)
#define AFX_APQURIER_H__3C18B495_753F_4FEB_8AE0_2EB286B43071__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApQurier.h : header file
//
// {{ Common Header File Include }}
//#include "../../COMMON_BASE/TrComm/TrComm.h"
//#include "../../COMMON_BASE/Drds/DataUtil.h"
//#include "../../INCLUDE_BASE/DllLoadDef.h"
#include <afxtempl.h>
//#include "../../../drMlib/DrMiddleLib.h"

/////////////////////////////////////////////////////////////////////////////
// CApQurier window
#define CODE_LENGTH		6

typedef struct {
	CString DataName;
	CString FieldName;
	CString KeyName;
	UINT	nID;
	BOOL	bAdvise;
} STDRDSINFO;

#define MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

class CSTController;
class CAPDataItem;
class CCommentData;
class CApQurier : public CWnd
{
// Construction
public:
	CApQurier();

// 20060705 窃荐牢磊 力芭 : HWND hSocket, long pDBMgr
//	BOOL Init(HWND hSocket, long pDBMgr);
	BOOL Init();

//	void OnCodeUnAdvise(CAPDataItem *pAPDataItem);
//	void OnCodeAdvise(CAPDataItem *pAPDataItem);
	CString GetDateOfRealTimeData() { return m_strDateOfRealTimeData;	}

	BOOL RequestDataToServer(CCommentData* pBasicalInputData, CSTController *pSTController,CAPDataItem **ppAPDataItem);
	BOOL DeleteDataItem(CAPDataItem *pQuoteDataItem,CSTController *pSTController);
private:
	void InitDrds();
	BOOL MakeDrdsConnectAdviseFromCode(int nI);
	BOOL MakeDrdsUnConnectUnAdviseFromCode(int nI);
	
//	void			*m_pDrds;			
//	STDRDSINFO		m_DrdsInfo[2];
	int				m_nDrdsInfoCnt;
	CString m_strDateOfRealTimeData;
	BOOL	m_bRequestingNow;
//	CDrdsLoadLib	m_DrdsDLL;
	
	CMap<CString,LPCTSTR,CAPDataItem*,CAPDataItem*> m_mapAPDataItemByInput;	// Primary Index
	CList<CAPDataItem*,CAPDataItem*> m_listAPDataItem;							// Secondery Index For real time data	
	CMap<CSTController *,CSTController *,CString,LPCTSTR> m_mapInputBySTC;		// Secondery Index
	CMap<CString,LPCTSTR,int,int> m_mapRefCntCodeAdvice;	// 
	CList<void*,void*>m_listRequested;
// Implementation
protected:

	// * 家南 烹脚 包访 Nested Class
//	class CMyCallback : public ICallbackStatus	
//	{
//		STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj) {return S_OK;}
//		STDMETHOD_(ULONG,AddRef) (THIS)  {return S_OK;}
//		STDMETHOD_(ULONG,Release) (THIS) {return S_OK;}
//		
//		STDMETHOD(ReceiveData) (HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
//		{
//			MYMETHOD_PROLOGUE(CApQurier, MyCallback)
//			return pThis->ReceiveData(hTrComm, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
//		}
//		STDMETHOD(StatusChange) (int nState)
//		{
//			MYMETHOD_PROLOGUE(CApQurier, MyCallback)
//			return S_OK;			
//		}
//	} m_xMyCallback;
//	friend class CMyCallback;


	HRESULT ReceiveData(HWND hTrComm, 
						LPCSTR szTR, 
						LPCSTR szMsgCode,
						LPCSTR szMessage, 
						LPVOID aTRData, 
						DWORD dwTRDateLen);	
	HRESULT RecieveError(int nError, LPCSTR szMessage);
//	CDrMLib_CDBMgr*  m_pIDrMLib_CDBMgr;
//	CTRComm		m_TRComm;			// 家南 贸府 葛碘
	
// 20060705 家南力芭
//	HWND m_hSocket;

	BOOL Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize);
// Attributes
public:
	//KHD ADD : 
	BOOL QuireData();
	void TestKHD(CAPDataItem* pQuoteDataIem =NULL);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApQurier)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CApQurier();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CApQurier)
	afx_msg void OnDestroy();
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
//	LONG OnDrdsData(UINT wParam, LPARAM lParam);	
	afx_msg	LRESULT OnRealData(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APQURIER_H__3C18B495_753F_4FEB_8AE0_2EB286B43071__INCLUDED_)
