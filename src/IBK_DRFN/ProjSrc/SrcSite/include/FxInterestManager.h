// FxInterestManager.h: interface for the CFxInterestManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FXINTERESTMANAGER_H__844E2FAF_E0AF_40F3_A201_6629D826A1AD__INCLUDED_)
#define AFX_FXINTERESTMANAGER_H__844E2FAF_E0AF_40F3_A201_6629D826A1AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

struct _pfcode;
struct _pfgroup;

class AFX_EXT_CLASS CFxInterestManager
{
public:
	CFxInterestManager();
	virtual ~CFxInterestManager();

public:
	CString GetTRName();
	BOOL AddGroup(CString strKey, CString strName, int nField = 0);
	BOOL AddCode(CString strKey, CString strCode, CString strPrice = _T(""), CString strAmount = _T(""));
	BOOL CopyGroup(CString strSrcKey, CString strDescKey, CString strGroupName);
	BOOL GetCodeList(CString strKey, CArray<_pfcode*, _pfcode*>* pArr);
	BOOL GetGroupList(CArray<_pfgroup*, _pfgroup*>* pArr);
	BOOL InsertBlank(CString strKey, int nIndex);
	BOOL InsertCode(CString strKey, int nIndex, CString strCode, CString strPrice = _T(""), CString strAmount = _T(""));
	BOOL IsNewCode(CString strKey, CString strCode);
	BOOL ModifyGroup(CString strKey, CString strModifyName);
	BOOL ModifyField(CString strKey, CString strField);
	BOOL MoveBottomCode(CString strKey, int nIndex);
	BOOL MoveBottomGroup(CString strKey);
	BOOL MoveDownCode(CString strKey, int nIndex);
	BOOL MoveDownGroup(CString strKey);
	BOOL MoveTopCode(CString strKey, int nIndex);
	BOOL MoveTopGroup(CString strKey);
	BOOL MoveUpCode(CString strKey, int nIndex);
	BOOL MoveUpGroup(CString strKey);
	BOOL QueryGroup(int nTRKey, CString strKey);
	BOOL QueryGroupList(int nTRKey);
	BOOL QueryLoad(int nTRKey, struct usrinfo* pUI = NULL);
	BOOL QuerySave(int nTRKey, BOOL bAll = TRUE, CString strKey = _T(""));
	BOOL QuerySaveCode(int nTRKey, CString strKey, CString strCode);
	BOOL ReceiveGroup(char* pData, int nLen);
	BOOL ReceiveGroupList(char* pData, int nLen);
	BOOL ReceiveLoad(char* pData, int nLen);
	BOOL ReceiveSave(char* pData, int nLen);
	BOOL ReceiveSaveCode(char* pData, int nLen);
	BOOL RemoveGroup(CString strKey);
	BOOL RemoveAllCode(CString strKey);
	BOOL RemoveCode(CString strKey, int nIndex);
	BOOL RemoveCodeInGroupAll(CString strCode);
	BOOL WriteInfo(CString strKey, int nIndex, CString strPrice, CString strAmount);
	BOOL WriteMemo(CString strKey, int nIndex, CString strMemo);	
	BOOL WritePrice(CString strKey, int nIndex, CString strPrice);
	BOOL WriteAmount(CString strKey, int nIndex, CString strAmount);
	BOOL WriteCode(CString strKey, int nIndex, CString strCode);
	CString GenerateKey();
	CString Variant(int nComm, CString strData = _T(""));
	int FindGroup(CString strKey);
	int GetCodeCount(CString strKey);
	void FormatThousand(CString& str);
	void GetGroupList(CStringArray* pArr);
	void Initialize(CWnd* pWizard, CWnd* pNotifyWnd, BOOL bControl = FALSE, CString strCtrlName = _T(""), int nCtrlKey = -1, int nBaseMode = 0);
	void SendInterestTrigger(UINT nMsg);
	CString SaveFileInterestGroup(); // _kgr 10.06.15 관심그룹 파일로 저장(OPS 사용)
	CString SaveFileInterestCode(); // _kgr 10.06.15 관심종목 파일로 저장(OPS 사용)
	
private:
	BOOL _AddCode(CString strKey, CString strCode, CString strPrice = _T(""), CString strAmount = _T(""));
	BOOL SendTR(CString strTR, CString strData, int nKey, int nStat);
	BOOL SendTRForCtrl(CString strTR, CString strData, int nKey, int nStat);
	char GetCodeType(CString strCode);
	CString	Parser(CString &strSource, CString strSep = _T("\t"));
	void _AddGroup(CString strKey, CString strName, int nField = 0);
	void RemoveAllGroup();
	
private:
	CWnd* m_pWizard;
	CWnd* m_pNotifyWnd;
	CArray<_pfgroup*, _pfgroup*> m_arrGroup;
	CString m_strCtrlName;

	CString m_strRoot;
	CString m_strName;
	CString m_strUserPath;
	CString m_strFileGroup;
	CString m_strFileCode;

	CStringArray m_arrGroupList;

	BOOL m_bControl;
	BOOL m_bFullList;
	int m_nCurMyst;
	int m_nCtrlKey;
	int m_nSaveTRKey;
	int m_nLoadTRKey;
	int m_nBaseMode;
};

#endif // !defined(AFX_FXINTERESTMANAGER_H__844E2FAF_E0AF_40F3_A201_6629D826A1AD__INCLUDED_)
