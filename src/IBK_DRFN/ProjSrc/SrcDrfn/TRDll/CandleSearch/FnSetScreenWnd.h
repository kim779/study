// FnSetScreenWnd.h: interface for the FnSetScreenWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FNSETSCREENWND_H__1C270657_69B3_4275_8713_C60D8FAA627D__INCLUDED_)
#define AFX_FNSETSCREENWND_H__1C270657_69B3_4275_8713_C60D8FAA627D__INCLUDED_

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/include/jmcode.h"

#define FONT_TYPE		"±¼¸²Ã¼"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFnSetScreenWnd : public CWnd
{
public:
	CFnSetScreenWnd();
	virtual ~CFnSetScreenWnd();

public:
	CWnd*		m_pWndModuleMain;
	CWnd*		m_pWndScreen;

	CFont*		m_pFontDef;
	CFont*		m_pBoldFont;

	COLORREF	m_clrPalete;

private:
	CMapStringToString	m_mapMarketMaster;
	CMapStringToPtr		m_mapKospiStockMarketGruop;
	CMapStringToPtr		m_mapKosdaqStockMarketGroup;
	CList <hjcodex, hjcodex&>* m_plistKOSPI200;
	CList <hjcodex, hjcodex&>* m_plistKRX;

	HINSTANCE	m_hCheckVersion;
	typedef void(*_DLL_Open)(CString strFormCode, CWnd* pTargetWnd, CFont* pFontDef);
	typedef void(*_DLL_Close)(CWnd* pTargetWnd);
	
	_DLL_Open	Lib_Open;
	_DLL_Close	Lib_Close;

	CString m_strUserName;

public:
	BOOL		SetInit(CWnd* pWndModuleMain, CWnd* pWndScreenWnd);
	void		SetPalete();
	CString		Variant(int nComm, CString strData = "");
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold);
	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	CBrush*		GetAxBrush(COLORREF clr);
	COLORREF	GetAxColor(int nColor);

	void		SendTR(CString strTRCode, char* szSendData, int nDataLen, int nKey, char cStat);
	LONG		ReceiveTR(WPARAM wParam, LPARAM lParam);
	LONG		ReceiveTRx(int nKey, char* pBytes, int nLen);

	BOOL		GetKSPUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption);
	BOOL		GetKSQUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption);

	CFont*		GetDefFont()							{ return m_pFontDef;					};
	CFont*		GetBoldFont()							{ return m_pBoldFont;					};
	COLORREF	GetPalete()								{ return m_clrPalete;					};

	void		OpenScreen(CString strScreenNo, CStringArray* pSaCode, CStringArray* pSaName, CString strOtherData);

	void		GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName);
	BOOL		GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey);	

	void		RegPortItem(CStringArray* pSaItemCode);

	void		ShareCode(CString strCode);
	void		ReceiveReal(WPARAM wParam, LPARAM lParam);

	CString		GetFormCode();

	BOOL		GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub);
	BOOL		GetUpMasterData(CStringArray *&strCode, CStringArray *&strName, int jgub);
	
	CString		GetMarketNameToCode(CString strMarketCode);

	int			SetUserMaster(LPCSTR szKey, int nCount, CStringArray *pArrCode, CStringArray *pArrName);
	int			GetUserMaster(LPCSTR szKey, CStringArray &arrCode, CStringArray &arrName);

	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle);
	
private:
	CFont*		GetAxFont(CString fName = FONT_TYPE, int point = 9, bool bBold = false, bool bItalic = false);
	HBITMAP		GetBitmap(CString strPath);
	HCURSOR		GetCursor(int nKind);

	void		DoClearDataFiles();
	void		DoPreReceiveData(CString strTRCode, LPVOID pData, int nBodyLen);
	BOOL		DoFileRemove(LPCSTR pszSrc);
	int			DoPreCheck(CString strTRCode);
	int			DoPreCheck_MSTCode();
	int			DoPreCheck_WorkDate();

	void		DoParsingCode(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName, bool bInsert=false, int nStartIdx=0);

	CString		Parser(CString &sSrc, CString sDelimiter);
	CString		PeekCode(CString strData);

	void		SetStockMasterData();
	void		SetUpMasterData();
	int			CheckKospiStock(UINT Upgub, hjcodex JCode);
	int			CheckKosdaqStock(UINT Upgub, hjcodex JCode);
	void		SetMakrketGroup(CList <hjcodex, hjcodex&>* plistKospiStock,
								CList <hjcodex, hjcodex&>* plistKosdaqStock);
	
	void		LoadVersionManager();
	void		OnLook(CStringArray* psaItemCode, CStringArray* psaItemName);
protected:
	//{{AFX_MSG(CFnSetScreenWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_FNSETSCREENWND_H__1C270657_69B3_4275_8713_C60D8FAA627D__INCLUDED_)
