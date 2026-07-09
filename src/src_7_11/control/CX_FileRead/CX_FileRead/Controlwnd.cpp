// Controlwnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CX_FileRead.h"
#include "Controlwnd.h"


// CControlwnd

IMPLEMENT_DYNAMIC(CControlwnd, CWnd)

CControlwnd::CControlwnd()
{

	EnableAutomation();
}

CControlwnd::~CControlwnd()
{
}

void CControlwnd::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlwnd, CWnd)
	DISP_FUNCTION_ID(CControlwnd, "GetGroupName", dispidGetGroupName, GetGroupName, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CControlwnd, "GetGroupCode", dispidGetGroupCode, GetGroupCode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "NotToday", dispidNotToday, NotToday, VT_EMPTY,VTS_BSTR)
END_DISPATCH_MAP()

// 참고: IID_IControlwnd에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {6A81C649-6153-4B35-87C7-4DD3D5889EB9}
static const IID IID_IControlwnd =
{ 0x6A81C649, 0x6153, 0x4B35, { 0x87, 0xC7, 0x4D, 0xD3, 0xD5, 0x88, 0x9E, 0xB9 } };

BEGIN_INTERFACE_MAP(CControlwnd, CWnd)
	INTERFACE_PART(CControlwnd, IID_IControlwnd, Dispatch)
END_INTERFACE_MAP()


// CControlwnd 메시지 처리기입니다.


CString CControlwnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strValue;
	char* pValue = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pValue > 1)
		strValue = pValue;
	else
		return _T("");

	return strValue;
}



BSTR CControlwnd::GetGroupName(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	CString struser, strpath;
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));

	strpath = Variant(homeCC);
	struser = Variant(nameCC);
	
	strUserPath.Format(_T("%s\\user\\%s"), strpath, struser);
	
	CFile file;
	CFileException ex;
	CStringArray* pArr = NULL;

	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("userinterest.ini"));

	int nGrpCnt = GetPrivateProfileInt(_T("GROUPINFO"), _T("COUNT"), 0, (LPCTSTR)strFilePath);

	CString sGroup(_T(""));
	CString sGrpData(_T(""));
	CString strGroup(_T(""));

	for ( int i = 0 ; i < nGrpCnt ; i ++ )
	{
		sGroup.Format(_T("%d"), i);

		GetPrivateProfileString(_T("GROUPINFO"), _T(sGroup), _T(""), sGrpData.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
		sGrpData.ReleaseBuffer();

		_TRIMDATA(sGrpData);
		if(sGrpData.IsEmpty())
			continue;

		strGroup = Parser(sGrpData, _T(";"));
		strGroup = sGrpData;

		strResult +=  strGroup;
		strResult += _T("\t");
		//AddGroup(strGroup);
		//LoadFile(i, strGroup, nSavePos);
	}
	return strResult.AllocSysString();
}


CString CControlwnd::Parser(CString &strSrc, CString strSub /* = _T("\t") */)
{
	if (strSrc.Find(strSub) == -1)
	{
		CString temp = strSrc;
		strSrc.Empty();
			return temp;
	}
	else
	{
		CString  strTemp = strSrc.Left(strSrc.Find(strSub));
		strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
		return strTemp;
	}

	return _T("");
}

BSTR CControlwnd::GetGroupCode(LPCTSTR strgroup)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	CString struser, strpath;
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));

	strpath = Variant(homeCC);
	struser = Variant(nameCC);
	// TODO: 여기에 디스패치 처리기를 추가합니다.
	strUserPath.Format("%s\\user\\%s\\%s", strpath, struser, strgroup);

	CString strData(_T(""));

	CFile file;

	CFileException ex;

	int nTry = 0;
	
	while(1)
	{
		if (file.Open(strUserPath, CFile::modeRead | CFile::shareDenyWrite, &ex))
		{
			int nFileSize = (int)file.GetLength();

			char* buffer = new char[nFileSize + 1];

			if ( nFileSize > 0 )
			{
				ZeroMemory(buffer, nFileSize + 1);

				FillMemory(buffer, nFileSize + 1, 0L);

				file.Read(buffer, nFileSize + 1);

				strData.Format("%s", buffer);
			}

			delete []buffer;

			file.Close();

			break;
		}
		else
		{
			if (nTry >= 10)
				break;

			Sleep(10);

			nTry ++;
		}
	}

	CString sTmp(_T(""));

	int ncnt = 0;

	ncnt = 0;

	CString strCode(_T(""));

	CString strCodeName(_T(""));

	CString strMangi(_T(""));

	CString strsuik(_T(""));

	CString strName(_T(""));

	CString strCodeData(_T(""));

	CString stmp(_T(""));

	CString strCount(_T(""));

	for ( int i = 0 ; i < 80 ; i ++ )
	{
		if (strData.IsEmpty())
			break;

		strCodeData = Parser(strData, _T("\n"));
		strCode = Parser(strCodeData, _T("\t"));
		strName = Parser(strCodeData, _T("\t"));
		strsuik = Parser(strCodeData, _T("\t"));
		strMangi = Parser(strCodeData, _T("\t"));
		_TRIMDATA(strCode);

		stmp.Format(_T("%s"), strCode);
		strCodeName = _T("");

		/*
		if(!strCode.IsEmpty())
		{//종목명을 플랫폼을 통해서 받기도 한다. 파일에 적혀있는것은 바뀔수 있다??
			strCodeName = (char*)pMapWnd->GetWizard()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)(LPCTSTR)strCode);
			
			_TRIMDATA(strCodeName);

			if (stmp.IsEmpty() || strCodeName.IsEmpty())
				continue;

			RemovePrevName(strCodeName);
		}
		*/
		strResult += strCode;
		strResult += _T("\t");
		ncnt++;
	}	
	return strResult.AllocSysString();
}


void CControlwnd::NotToday(LPCTSTR title)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString tmpS;
	CString strdata;
	CString strResult;
	CString struser, strpath;
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));

	strpath = Variant(homeCC);
	struser = Variant(nameCC);
	strUserPath.Format(_T("%s\\user\\%s"), strpath, struser);
	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("NOTICE.ini"));

	DWORD dwLen = 1024;
	LPTSTR pBuffer = new char[dwLen + 1];

	GetPrivateProfileString(_T("NOTICE"), _T("TODAY"), _T(""), pBuffer, dwLen, strFilePath);

	strResult.Format(_T("%s"), pBuffer);

	strdata = Parser(strResult, _T("\t"));
	int idate = atoi(strdata);

	CTime time = CTime::GetCurrentTime();
	CString sTime;
	sTime.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

	if(atoi(sTime) > idate)  //파일에 써있는 날짜가 지난 날짜인경우 오늘 날짜로 갱신
	{
		WritePrivateProfileString(_T("NOTICE"), _T("TODAY"), _T(""), strFilePath);
		tmpS.Format(_T("\t%s"), title);
		tmpS.TrimRight();
		tmpS.TrimLeft();
		sTime += _T("\t");
		sTime += tmpS;
		WritePrivateProfileString(_T("NOTICE"), _T("TODAY"), sTime, strFilePath);
	}
	else   //파일에 써있는 날짜가 오늘 날짜인 경우 그냥 뒤에 붙이기
	{
		strResult.Format(_T("%s"), pBuffer);
		tmpS.Format(_T("\t%s"), title);
		tmpS.TrimRight();
		tmpS.TrimLeft();
		strResult += _T("\t");
		strResult += tmpS;
		WritePrivateProfileString(_T("NOTICE"), _T("TODAY"), strResult, strFilePath);
	}
}
