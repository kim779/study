// BaseInfo.h: interface for the CBaseInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEINFO_H__F5407BF9_8F70_4DA8_811D_A44969AC812D__INCLUDED_)
#define AFX_BASEINFO_H__F5407BF9_8F70_4DA8_811D_A44969AC812D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include/CommonDef.h"
#include "../../../CommonLib/ServerMsg.h"
#include "../../Include_BASE/IncMng.h"
#include "../../../Include/SharedBase.h"
#include "../../../CtlCore/ColorMap.h"
// {{ Include Library }}
//../../../../Lib/DataMgr.lib 
//../../../../Lib/CommonLib.lib 
//../../../../Lib/MapDB.lib 
//../../../../Lib/CtlCore.lib 
//../../../../Lib/CtlStd.lib 

class CDBMgr;
class CMapDB;
class CBaseObject;
class CBaseInfo  
{
public:
	CBaseInfo();
	virtual ~CBaseInfo();

public:
	CDBMgr*		m_pDBMgr;
	CMapDB*		m_pMapDB;
	CString		m_sBasePath;
	CString		m_sUserID;
	HWND		m_hParent;


public:
	void		InitInfo(void* pDBMgr);
	void		CloseInfo();
	HWND		GetSocketHWND();
	CString		GetUserPath();
	CString		GetMainPath();

	// Code Get from HanWha Lib
public:
	BOOL		GetCodeInfo(DWORD nCode, CStringArray& arCode, CStringArray& arName);
	DWORD		GetCodeType(LPCSTR szCode, int nECN=0);
	BOOL		GetCodeName(DWORD wType, LPCTSTR szCode, CString &strName, BOOL bFullName = TRUE, BOOL bAlert = FALSE);
	void		GetCodeGroup(CString sGroup, CStringArray& arCode, CStringArray& arName);

	static BOOL GetCodeIntType(DWORD nType); // 관심AP와 한화 마스터간의 인덱스 Convert Han->Inc(관심)
	static int GetCodeTRType(DWORD nType);
//Real Functions
public: 
	void AddRealUpdate(CMapStringToPtr *pKeyMap, HWND hWnd);
	void AddRealUpdate(LPCTSTR szKey, HWND hWnd);
	
	void ResetRealUpdate(CMapStringToPtr *pKeyMap, HWND hWnd);
	void ResetRealUpdate(LPCTSTR szKey, HWND hWnd);

	WORD GetRealType(long lParam);

public:
	void		SetKeyCodeToMain_AllControl();

	// codeinput ----------------------------------------------------------------------------------------->>
	CBaseObject	* m_pSelCode;
	void		CreateControl_CodeInput(HWND hWnd, CRect rc, WORD nID = 0,
										long lcodetype = CODE_JONGMOK|CODE_KJONGMOK,
										long lcodeMaxLength = 6);
	
	void		SetTextToCodeInput(CString strCode);
	CString		GetTextToCodeInput();
	//void		SetFocusToCodeInput();	-> 작동안함
	void		DestoryControl_CodeInput();
	// ---------------------------------------------------------------------------------------------------<<

	// AcctInput ----------------------------------------------------------------------------------------->>
	CBaseObject	* m_pAcctInput;
	void		CreateControl_AcctInput(HWND hWnd, CRect rc, WORD nID = 0,
										long lAcctTypeCnt = 2,
										CString strAcctType = "0102");

	CString		GetTextToAcctInput();
	CString		GetAccountToGNameForDR(CString strAcc);
	void		SetEnableToAcctInput(BOOL bEnable=TRUE);
	// ---------------------------------------------------------------------------------------------------<<



	// CodeCombo ( 업종 코드 )---------------------------------------------------------------------------->>	
	void		CreateControl_UpjongCodeCombo(HWND hWnd, CRect rc, WORD nID = 0, CString strcodetype="CODE_ALLUPCODE");
	CString		GetUpjongNameToCodeForDR(CString strcode , long lcodetype = CODE_ALLUPCODE);
		
	// ---------------------------------------------------------------------------------------------------<<

	// CodeCombo ( 해외지수  코드 )---------------------------------------------------------------------------->>
	//sy, nam 05.06.21	
	void		CreateControl_ForeignCodeCombo(HWND hWnd, CRect rc, WORD nID = 0, CString strcodetype="CODE_EXCH_ALL");
	CString		GetForeignNameToCodeForDR(CString strcode , long lcodetype = CODE_EXCH_ALL);
		
	// ---------------------------------------------------------------------------------------------------<<


	// CodeCombo ( 선물옵션  코드 )---------------------------------------------------------------------------->>
	//sy, nam 05.06.22	
	void		CreateControl_FutOptCodeCombo(HWND hWnd, CRect rc, WORD nID = 0, CString strcodetype="CODE_FUTOPT", long lcodeMaxLength = 8);
	CString		GetFutOptNameToCodeForDR(CString strcode , long lcodetype = CODE_FUTOPT);
	void		DestoryControl_FutOptCodeCombo();
	// ---------------------------------------------------------------------------------------------------<<

	//HANWHA TR이용시 사용 
public:
	CServerMsg  m_Msg;

	void* GetCurTrHWS09501(LPCSTR szMarket, LPCSTR szCode);
	void* GetCurTrHWS09501(int nType, int nGroup);
	
	//공유의 등록 및 제거
public:
	BOOL AdviseSharedInfo(CString sShareName, CSharedBase* pSharedBase, int nSharedType, int nSharedArea, int nGroup=0);
	BOOL UnAdviseSharedInfo(CString sShareName, CSharedBase* pSharedBase);
	BOOL SetSharedData(HWND hCtrl,CString sShareName,BYTE* pData, int nLen, int nStock, BOOL bOnlySetData);
	BOOL SetSharedData2(HWND hArea,int nArea, CString sShareName,BYTE* pData, int nLen, int nStock, BOOL bOnlySetData=FALSE);
	BYTE* GetSharedData(CString sShareName, int& nLen, int& nStock);	
public:
	void ShowCodeCombo(int nShow);
	void SetWindowRectCodeCombo(CRect rect);
	void EnabelCodeCombo(BOOL bEnable);

public:
	CMapColor m_MapColor;
	COLORREF GetColor(int nID);
	

};

#endif // !defined(AFX_BASEINFO_H__F5407BF9_8F70_4DA8_811D_A44969AC812D__INCLUDED_)
