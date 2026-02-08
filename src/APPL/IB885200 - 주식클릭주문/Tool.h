// Tool.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxtempl.h>

class CTool  
{
public:
	CTool(CWnd* pwndWizard);
	virtual ~CTool();

public:
	

protected:
	CWnd*		m_pwndWizard;

public:
	inline CWnd* GetWizard() { return m_pwndWizard; };
	char*		GetHome();
	char*		GetName();
	COLORREF	GetIndexColor(int iIndex);

	// GDI
	CFont*		RequestFont(CString strName, int iPoint, BOOL bItalic, int iBold);
	CPen*		RequestPen(COLORREF rgbLine, int iWidth, int iStyle);
	CBrush*		RequestBrush(COLORREF rgbBrush);
	CBitmap*	RequestBitmap(CString stPath);
	HBITMAP		RequestHBitmap(CString stPath);


	// TR
	LRESULT		SendTR(int iLength, char* pcData);
	LRESULT		SendTR(CString strCode, CString strData, int iKey, int iStat = NULL);
	LRESULT		SendTR(CString strCode, char* pcData, int iLength, int iKey, int iStat = NULL);

	// variant
	void		RegistAccount(CString strData);
	void		SetTitle(CString strData);
	CString		GetAccount();
	CString		GetAccountx(CString strData);
	CString		GetPassword(CString strAccount);
	void		SetHistory(CString strData);
	void		SetTrigger(CString strData);
	CString		GetHistory(CString strSymbol);
	int		GetMajorKey();
	int		GetMinorKey();

	// code
	CString		GetCodeName(CString strCode);
///	CString		GetCodeType(CString strCode);
	int			GetCodeType(CString strCode);

	// string
	CString		AddComma(CString strSrc);
	CString		AddComma(int iSrc);
	CString		RemoveComma(CString strSrc);
	CString		RemovePoint(CString strSrc);
	CString		RemoveSign(CString strSrc);
	CString		ToString(void* pvData, int iLength);
	CString		ToString(void* pvData);
	int		ToInt(void* pvData, int iLength);
	double		ToDouble(void* pvData, int iLength);
	int		AtoI(CString strData);

	// MAP Management
	CWnd*		CreateMap(CWnd* pwndParent, CRect rect, UINT nID);
	int		AttatchMap(CWnd* pWnd);
	BOOL		LoadMap(int iKey, CString strParam);
	void		DominoMap(int iKey, CString strParam);
	void		DetachMap(int iKey);
	void		DeleteMap(CWnd* pWnd);

	// parsing
	bool		Parsing(CString strData, CMap<int, int, CString, CString>& mapData);
	bool		Parsing(char* pcData, int iLength, CMap<int, int, CString, CString>& mapData);
	bool		ParsingN(CString strData, CMap<int, int, CString, CString>& mapData);
	CString		Parser(CString &strSrc, char cDel);

	// push & pop
	void		Push(CString strSymbol, CString strData);
	CString		Pop(CString strSymbol);

	// window
	void		CreateView(int iKind, CString strMapName, CString strOption);

	// excel
	void		MakeExcel(CString strData);

	// Account & Password
	// modeACN=0, modeUID=1
	CString		GetEncPswd(CString strPass, CString strKey, int mode = 0);
	

	// Trigger
	void		Trigger(CString strData);

	// Message
	void		Message(CString strMsg);

	// Menu
	void		PopupMenu(CString strMapName, int iX, int iY, bool bGrid = false);

	// Jong Mok Dlg
	char*		GetJCodeByDlg(int iKind, int iType, int iX, int iY);

	// Trace
	void		WriteFile(CString strData);

	// etc
	void		Proc(int iMajorKey, int iMinorKey, CString strData);
	double		DifToDouble(CString strData);
	int		GetOrderPopupPos();
	CString		MakeSpace(int iCount);
	
	
//	bool		RunExcel(CString strExcel);

//protected:
	CString		Variant(int iComm, CString strData = _T(""));

};

/*
	Class CTool Version History

	v1.018		2006.02.10		Add	AtoI()
	v1.017		2006.02.01		Add Parser()
	v1.016		2006.01.31		Add MakeSpace()
	v1.015		2006.01.31		Add WriteFile()
	v1.014		2006.01.26		Add GetJCodeByDlg()
	v1.013		2006.01.23		Add GetEncPswd()
	v1.012		2005.12.27		Add RemoveSign()
	v1.011		2005.12.14		Add ParsingN()
	v1.010		2005.12.06		Add ToDouble()
	v1.009		2005.11.21		Add ToString(char*)
	v1.008		2005.11.14		Add ToInt()
	v1.007		2005.11.09		GetOrderPopupPos()
	v1.006		2005.11.08		Push(), Pop(), RemovePoint(), PopupMenu(), MakeExcel() Add
	v1.005		2005.10.21		ToString() add
	v1.004		2005.10.12		Major, Minor key Add
						Proc Add
						Trigger Add
	v1.003		2005.10.10		Debug RemoveComma()
	v1.002		2005.10.05		MAP Manage Add
	v1.001		2005.09			TR Moudle Add
	v1.0		2005.08			Class Define

*/