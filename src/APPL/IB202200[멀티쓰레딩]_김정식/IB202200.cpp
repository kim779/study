// IB202200.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IB202200.h"
#include "Memo.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CIB202200App

BEGIN_MESSAGE_MAP(CIB202200App, CWinApp)
	//{{AFX_MSG_MAP(CIB202200App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIB202200App construction

CIB202200App::CIB202200App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIB202200App object
CIB202200App theApp;

#include "MainWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	auto pMainWnd = std::make_unique<CMainWnd>(parent);

	theApp.setMainWnd(pMainWnd.get());
	if (!pMainWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CRect(0, 0, 0, 0), parent, 100, nullptr))
	{
		return nullptr;
	}		
	return pMainWnd.release();
}



const CString& CIB202200App::getMemo(CString sCode)
{
	if (sCode.IsEmpty())
		return std::move(CString());
	const auto it = _memoMap.find(sCode.GetString());
	if (it != _memoMap.end())
		return it->second;

	const size_t size = _memoMap.size();
	if (_pMwnd && _pMwnd->GetSafeHwnd())
		_pMwnd->sendMemo(sCode);
	return std::move(CString(""));
}


int CIB202200App::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	    {{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *ss = mstr;
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X')
		ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime)
					result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found)
			break;
		ss++;
		firsttime = false;
	}
	[[gsl::suppress(26408)]]
	free(mstr);
	return result;
}