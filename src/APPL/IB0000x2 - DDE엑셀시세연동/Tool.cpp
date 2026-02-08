// Tool.cpp: implementation of the CTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tool.h"
#include "../../h/AxisFire.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTool::CTool(CWnd* pwndWizard)
{
	ASSERT(pwndWizard);
	m_pwndWizard	= pwndWizard;
}

CTool::~CTool()
{

}


char* CTool::GetHome()
{
	return (char*)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);
}


char* CTool::GetName()
{
	return (char*)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), NULL);
}


COLORREF CTool::GetIndexColor(int iIndex)
{
	return m_pwndWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)iIndex);
}

CString CTool::Variant(int iComm, CString strData)
{
	CString strReturn;

	char* pcData = (char*)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, iComm), (LPARAM)(LPCTSTR)strData);
	if ((long)pcData > 1)
		strReturn = pcData;
	else
		return "";

	return strReturn;
}

CFont* CTool::RequestFont(CString strName, int iPoint, BOOL bItalic, int iBold)
{
	struct	_fontR fontR;

	fontR.name	= (char*)(strName.operator LPCTSTR());
	fontR.point	= iPoint;
	fontR.italic	= (bItalic == TRUE ? true : false);
	fontR.bold	= iBold;

	return (CFont*)m_pwndWizard->SendMessage(WM_USER, (WPARAM)MAKEWORD(getFONT, NULL), (LPARAM)&fontR);
}

CPen* CTool::RequestPen(COLORREF rgbLine, int iWidth, int iStyle)
{
	struct	_penR	penR;
	penR.clr = rgbLine;
	penR.width = iWidth;
	penR.style = iStyle;

	return (CPen*)m_pwndWizard->SendMessage(WM_USER, (WPARAM)MAKEWORD(getPEN, NULL), (LPARAM)&penR);
}

CBrush* CTool::RequestBrush(COLORREF rgbBrush)
{
	return (CBrush*)m_pwndWizard->SendMessage(WM_USER, (WPARAM)MAKEWORD(getBRUSH, NULL), (LPARAM)rgbBrush);
}

CBitmap* CTool::RequestBitmap(CString stPath)
{
	return (CBitmap*)m_pwndWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)stPath.operator LPCTSTR());
}

void CTool::RegistAccount(CString strData)
{
	Variant(accountCCs, strData);
}

void CTool::SetTitle(CString strData)
{
	Variant(titleCC, strData);
}

CString CTool::GetAccount()
{
	return Variant(accountCC);
}

CString CTool::GetAccountx(CString strData)
{
	return Variant(accountCCx, strData);
}

CString CTool::GetPassword(CString strAccount)
{
	return Variant(passCC, strAccount);
}

CString CTool::GetCodeName(CString strCode)
{
	CString strInput = CString(strCode.operator LPCTSTR());	// buffer∞° ±˙¡¸¿ª πÊ¡ˆ
	CString strName;
	strName.Empty();
	if (strCode.GetLength() > 0)
	{
		strName = (char*) m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)strInput.operator LPCTSTR());
		strName.TrimLeft();
		strName.TrimRight();
	}

	return strName;
}

int CTool::GetCodeType(CString strCode)
{
	int type = -1;
	CString strInput = CString(strCode.operator LPCTSTR());	// buffer∞° ±˙¡¸¿ª πÊ¡ˆ
	if (strInput.GetLength() > 0)
	{
		if (strInput.GetAt(0) == 'J' || strInput.GetAt(0) == 'A')
			strInput = strInput.Mid(1);
		type = m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)strInput.operator LPCTSTR());
	}

	return type;
}

void CTool::SetHistory(CString strData)
{
	Variant(codeCC, strData);
}

void CTool::SetTrigger(CString strData)
{
	Variant(triggerCC, strData);
}

CString CTool::GetHistory(CString strSymbol)
{
	return Variant(historyCC, strSymbol);
}

LRESULT CTool::SendTR(int iLength, char* pcData)
{
	return m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, iLength), (LPARAM)pcData);
}

LRESULT CTool::SendTR(CString strCode, CString strData, int iKey, int iStat)
{
	std::unique_ptr<char[]> pcDataBuffer = std::make_unique<char[]>(L_userTH + strData.GetLength());


	ZeroMemory(pcDataBuffer.get(), sizeof(pcDataBuffer.get()));
	struct	_userTH* puserTH;
	puserTH = (struct _userTH *)pcDataBuffer.get();

	strcpy(puserTH->trc, strCode.operator LPCTSTR());
	puserTH->key = iKey;
	puserTH->stat = iStat;

	CopyMemory(&pcDataBuffer.get()[L_userTH], strData.operator LPCTSTR(), strData.GetLength());

	const LRESULT lResult = m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM) pcDataBuffer.get());

	return lResult;
}

LRESULT CTool::SendTR(CString strCode, char* pcData, int iLength, int iKey, int iStat)
{
	std::unique_ptr<char[]> pcDataBuffer = std::make_unique<char[]>(L_userTH + iLength);
	
	ZeroMemory(pcDataBuffer.get(), sizeof(pcDataBuffer.get()));
	struct	_userTH* puserTH;
	puserTH = (struct _userTH *)pcDataBuffer.get();

	strcpy(puserTH->trc, strCode.operator LPCTSTR());
	puserTH->key = iKey;
	puserTH->stat = iStat;

	CopyMemory(&pcDataBuffer.get()[L_userTH], pcData, iLength);

	const LRESULT lResult = m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, iLength), (LPARAM) pcDataBuffer.get());

	return lResult;
}

CString CTool::AddComma(CString strSrc)
{
	strSrc.TrimLeft();
	strSrc.TrimRight();
	strSrc.Remove(',');

	CString strFloat;
	CString strNatural;
	const int iPos = strSrc.Find('.');
	if (iPos >= 0)
	{
		strNatural = strSrc.Left(iPos);
		strFloat = strSrc.Mid(iPos);
	}
	else
	{
		strNatural = strSrc;
		strFloat.Empty();
	}
	
	CString strReturn;
	int jj = -1;
	for (int ii = strNatural.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (strNatural.GetAt(0) != '-' && strNatural.GetAt(0) != '+')
				strReturn = ',' + strReturn;
			else if (ii != 0)
				strReturn = ',' + strReturn;
			
			jj = 0;
		}
		strReturn = strNatural.GetAt(ii) + strReturn;
	}
	strReturn += strFloat;
	
	return strReturn;
}

CString CTool::AddComma(int iSrc)
{
	CString strSrc;
	strSrc.Format("%d", iSrc);
//	strSrc.TrimLeft();
//	strSrc.TrimRight();

	CString strFloat;
	CString strNatural;
	const int iPos = strSrc.Find('.');
	if (iPos >= 0)
	{
		strNatural = strSrc.Left(iPos);
		strFloat = strSrc.Mid(iPos);
	}
	else
	{
		strNatural = strSrc;
		strFloat.Empty();
	}
	
	CString strReturn;

	int jj = -1;
	for (int ii = strNatural.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (strNatural.GetAt(0) != '-' && strNatural.GetAt(0) != '+')
				strReturn = ',' + strReturn;
			else if (ii != 0)
				strReturn = ',' + strReturn;
			
			jj = 0;
		}
		strReturn = strNatural.GetAt(ii) + strReturn;
	}
	strReturn += strFloat;
	
	return strReturn;
}

CString CTool::RemoveComma(CString strSrc)
{
	strSrc.TrimLeft();
	strSrc.TrimRight();

	
	CString strReturn = strSrc;
	strReturn.Remove(',');
	
	return strReturn;
}

CString CTool::RemovePoint(CString strSrc)
{
	strSrc.TrimLeft();
	strSrc.TrimRight();

	
	CString strReturn = strSrc;
	strReturn.Remove('.');
	
	return strReturn;
}

CString CTool::RemoveSign(CString strSrc)
{
	strSrc.TrimLeft();
	strSrc.TrimRight();

	
	CString strReturn = strSrc;
	strReturn.Remove('+');
	strReturn.Remove('-');
	
	return strReturn;
}


CWnd* CTool::CreateMap(CWnd* pwndParent, CRect rect, UINT nID)
{
	std::unique_ptr<CWnd> pWnd = std::make_unique<CWnd>();
	if (!pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pwndParent, nID))
	{
		return nullptr;
	}
	return pWnd.release();
}

int CTool::AttatchMap(CWnd* pWnd)
{
	const int iKey = (int)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(attachMAP, NULL), (LPARAM)pWnd);
	return iKey;
}


BOOL CTool::LoadMap(int iKey, CString strParam)
{
	return (BOOL)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(loadMAP, iKey), (LPARAM)strParam.operator LPCTSTR());
}


void CTool::DominoMap(int iKey, CString strParam)
{
	m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(dominoMAP, iKey), (LPARAM)strParam.operator LPCTSTR());
}

void CTool::DetachMap(int iKey)
{
	m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(detachMAP, iKey), NULL);
}

void CTool::DeleteMap(CWnd* pWnd)
{
	DestroyWindow(pWnd->m_hWnd);
	delete pWnd;
	pWnd = NULL;
}

int CTool::GetMajorKey()
{
	return (int)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);
}

int CTool::GetMinorKey()
{
	return (int)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, minorCC), NULL);
}

void CTool::Proc(int iMajorKey, int iMinorKey, CString strData)
{
	m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(iMajorKey, iMinorKey)), (LPARAM)strData.operator LPCTSTR());
}

void CTool::Trigger(CString strData)
{
	m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)strData.operator LPCTSTR());
}

CString CTool::ToString(void* pvData, int iLength)
{
	CString strReturn = CString((char*)pvData, iLength);
	strReturn.TrimLeft();
	strReturn.TrimRight();

	return strReturn;
}

CString CTool::ToString(void* pvData)
{
	CString strReturn = CString((char*)pvData);
	strReturn.TrimLeft();
	strReturn.TrimRight();

	return strReturn;
}

int CTool::ToInt(void* pvData, int iLength)
{
	CString strReturn = CString((char*)pvData, iLength);
	strReturn.TrimLeft();
	strReturn.TrimRight();

	return atoi(strReturn);
}

double CTool::ToDouble(void* pvData, int iLength)
{
	CString strReturn = CString((char*)pvData, iLength);
	strReturn.TrimLeft();
	strReturn.TrimRight();

	return atof(strReturn);
}

bool CTool::Parsing(CString strData, CMap<int, int, CString, CString>& mapData)
{
	strData.TrimLeft();
	strData.TrimRight();
	mapData.RemoveAll();

	CString strSymbol;
	CString strValue;
	while (!strData.IsEmpty())
	{
		int iPos = strData.Find('\t');
		if (iPos < 0)
		{
			return false;
		}
		else
		{
			strSymbol = strData.Left(iPos++);
			strData = strData.Mid(iPos);
		}

		iPos = strData.Find('\t');
		if (iPos < 0)
		{
			strValue = strData;
			strData.Empty();
		}
		else
		{
			strValue = strData.Left(iPos++);
			strData = strData.Mid(iPos);
		}

		strValue.TrimLeft();
		strValue.TrimRight();

		mapData.SetAt(atoi(strSymbol), strValue);
	}

	return true;
}

bool CTool::Parsing(char* pcData, int iLength, CMap<int, int, CString, CString>& mapData)
{
	char* pcCurr = pcData;
	char* pcEnd = pcCurr + iLength;
	char* pcNext{};
	CString strSymbol;
	CString strValue;
	while (pcCurr && pcCurr <= pcEnd)
	{
		pcNext = strchr(pcCurr, '\t');
		if (pcNext == 0)
		{
			return false;
		}
		else
		{
			strSymbol = CString(pcCurr, pcNext - pcCurr);
			pcCurr = pcNext + 1;
		}

		pcNext = strchr(pcCurr, '\t');
		if (pcNext == 0)
		{
			strValue = CString(pcCurr, pcEnd - pcCurr);
			pcCurr = NULL;
		}
		else
		{
			strValue = CString(pcCurr, pcNext - pcCurr);
			pcCurr = pcNext + 1;
		}

		strValue.TrimLeft();
		strValue.TrimRight();

		mapData.SetAt(atoi(strSymbol), strValue);
	}

	return true;
}

bool CTool::ParsingN(CString strData, CMap<int, int, CString, CString>& mapData)
{
	strData.TrimLeft();
	strData.TrimRight();
	mapData.RemoveAll();

	CString strSymbol;
	CString strValue;
	CString	strLine;
	while (!strData.IsEmpty())
	{
		int iPos = strData.Find('\n');
		if (iPos < 0)
		{
			strLine = strData;
			strData.Empty();
		}
		else
		{
			strLine = strData.Left(iPos++);
			strData = strData.Mid(iPos);
		}

		iPos = strLine.Find('\t');
		if (iPos < 0)
		{
			return false;
		}
		else
		{
			strSymbol = strLine.Left(iPos++);
			strValue = strLine.Mid(iPos);
		}

		strSymbol.TrimLeft();
		strSymbol.TrimRight();
		strValue.TrimLeft();
		strValue.TrimRight();

		mapData.SetAt(atoi(strSymbol), strValue);
	}

	return true;
}

double CTool::DifToDouble(CString strData)
{
	double dDif  = 0;
	if (!strData.IsEmpty())
	{
		dDif = atof(strData.Mid(1));
		if (strData.GetAt(0) == '4' || strData.GetAt(0) == '5')
			dDif *= -1;
	}

	return dDif;
	
}

void CTool::PopupMenu(CString strMapName, int iX, int iY, bool bGrid)
{
	CString strLPARM;
	if (bGrid)
		strLPARM.Format("%s\t\n%d\t%d", strMapName, iX, iY);
	else
		strLPARM.Format("%s\n%d\t%d", strMapName, iX, iY);
	// mapN \n pt.x \t pt.y \n SYM \t DATA \n
	m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (LPARAM)strLPARM.operator LPCTSTR());
}


void CTool::Push(CString strSymbol, CString strData)
{
	CString strText = strSymbol + "\t" + strData;
	Variant(pushCC, strText);
}

CString CTool::Pop(CString strSymbol)
{
	return Variant(popCC, strSymbol);
}

void CTool::CreateView(int iKind, CString strMapName, CString strOption)
{
	CString strLPARM = strMapName + strOption;
	m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, iKind), (LPARAM)strLPARM.operator LPCTSTR());
}

void CTool::MakeExcel(CString strData)
{
	m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(excelDLL, true), (LPARAM)strData.operator LPCTSTR());
}

int CTool::GetOrderPopupPos()
{
	CString strFile;
	strFile.Format("%s\\user\\%s\\EnSetup.ini", GetHome(), GetName());
	return GetPrivateProfileInt("ORDER", "POS", 4, strFile);
}

void CTool::Message(CString strMsg)
{
	Variant(guideCC, strMsg);
}

CString CTool::GetEncPswd(CString strPass, CString strKey, int mode/* = modeACN*/)
{
	CString strBuffer;

	strBuffer.Format("%s\t%s", strPass, strKey);
	strPass = (char*)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, mode), (long)strBuffer.operator LPCTSTR());
	return strPass;
}

char* CTool::GetJCodeByDlg(int iKind, int iType, int iX, int iY)
{
	return (char*)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(iKind, iType)), MAKELPARAM(iX, iY));
}

void CTool::WriteFile(CString strData)
{
	CFile		fileTrace;
	DWORD		dwPos{};
	CString		strFileName, strTime;
	SYSTEMTIME	st;
	
	GetLocalTime(&st);
	strTime.Format("[%p] [%d≥‚ %dø˘ %d ¿œ% dΩ√ %d∫– %d√ ]\n" 
			, this, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	strFileName.Format("%s\\%d%02d%02d.trc", GetHome(), st.wYear, st.wMonth, st.wDay);
	const BOOL bExist = fileTrace.Open(strFileName, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);
	if (!bExist)
	{
		fileTrace.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite);
		fileTrace.Write((char *) strTime.operator LPCTSTR(), strTime.GetLength());
		fileTrace.Write(strData.operator LPCTSTR(), strData.GetLength());
	}
	else
	{
		dwPos = (int)fileTrace.SeekToEnd();
		fileTrace.Seek((long) dwPos, CFile::begin);
		fileTrace.Write((char *) strTime.operator LPCTSTR(), strTime.GetLength());
		fileTrace.Write(strData.operator LPCTSTR(), strData.GetLength());
	}

	const char newline[] = "\r\n\r\n";
	fileTrace.Write(newline, strlen(newline));
	fileTrace.Flush();
	fileTrace.Close();
}

CString CTool::MakeSpace(int iCount)
{
	CString strSpace;
	strSpace.Empty();

	for (int ii = 0; ii < iCount; ii++)
	{
		strSpace += ' ';
	}

	return strSpace;
}

CString CTool::Parser(CString &strSrc, char cDel)
{
	CString strReturn;
	strReturn.Empty();
	if (strSrc.Find(cDel) < 0)
	{
		strReturn = strSrc;
		strSrc.Empty();
	}
	else
	{
		strReturn = strSrc.Left(strSrc.Find(cDel));
		strSrc = strSrc.Mid(strSrc.Find(cDel) + 1);

	}
	return strReturn;
}

int CTool::AtoI(CString strData)
{
	strData = RemoveComma(strData);
	return atoi(strData);
}

/*
bool CTool::RunExcel(CString strExcel)
{
	if (strExcel.IsEmpty())
		return false;

	::OleInitialize(NULL);

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		TRACE("CreateDispatch error....\n");
		::OleUninitialize();
		return true;
	}

	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	Workbooks  Wbs; _Workbook  Wb;
	Worksheets Wss; _Worksheet Ws;
	Range	range;

	Wbs = excel.GetWorkbooks();
	Wb  = Wbs.Add(vOpt);

	Wss = Wb.GetWorksheets();
	Ws  = Wss.Add(vOpt, vOpt, vOpt, vOpt);

	Ws.SetName("Document");

	if (strExcel.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); m_excelH = NULL;
		return false;
	}

	const	int alphaC = 24;
	CString	string; string.Format("A1:%c%d", (char)('A' + m_nCols), m_nFixedRows + m_nRows);
	if (m_nCols >= alphaC)
		string.Format("A1:%c%c%d", (char)('A' + ((m_nCols/alphaC) -1)),
					(char)('A' + m_nCols % alphaC), m_nFixedRows + m_nRows);

	range = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
	range.Clear();

	if (strExcel.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); m_excelH = NULL;
		return false;
	}

	for (int jj = 0; m_nFixedRows && jj < m_nCols; jj++)
	{
		if (!IsCellAttribute(CIdCell(0, jj), GVAT_TEXT))
			continue;

		string.Format("%c%d:%c%d", (char)('A' + jj), m_nFixedRows, (char)('A' + jj), m_nRows);
		if (jj >= alphaC)
		{
			string.Format("%c%c%d:%c%c%d", (char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), m_nFixedRows,
							(char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), m_nRows);
		}

		Range rangex = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
		string = _T("@"); rangex.SetNumberFormat(COleVariant(string));
	}

	long row, col;
	bool last = false;
	int idx; CString stringX;
	for (int ii = 0; !strExcel.IsEmpty() && ii < m_nRows; ii++)
	{
		idx = strExcel.Find("\n");
		if (idx != -1)
		{
			stringX = strExcel.Left(idx);
			strExcel = strExcel.Mid(idx+1);
		}
		else
		{
			stringX = strExcel; strExcel = _T(""); last = true;
		}

		for (int jj = 0; !stringX.IsEmpty() && jj < m_nCols; jj++)
		{
			idx = stringX.Find("\t");
			if (idx != -1)
			{
				string  = stringX.Left(idx);
				stringX = stringX.Mid(idx+1);
			}
			else
			{
				string = stringX; stringX = _T("");
			}

			if (string.IsEmpty())
				continue;

			TRY
			{
				row = ii+1; col = jj+1;
				range.Set_Default(COleVariant(row), COleVariant(col), COleVariant((LPCTSTR)string));
			}
			CATCH(COleDispatchException, e)
			{
				TRACE("runexcel error [%s]\n", e->m_strDescription);
				break;
			}
			END_CATCH
		}

		if (!Ws.m_lpDispatch) break;
	}

	excel.SetVisible(m_excelF);
	if (!m_excelF)
	{
		Wbs.Close(); excel.Quit();
	}

	excel.ReleaseDispatch();
	::OleUninitialize();

	m_excelH = NULL;
	return true;
}
*/
