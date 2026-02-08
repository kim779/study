// SiteDefine.h: interface for the CSiteDefine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITEDEFINE_H__EAE48A29_C075_440E_AF1D_F07337CEAE16__INCLUDED_)
#define AFX_SITEDEFINE_H__EAE48A29_C075_440E_AF1D_F07337CEAE16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/6
// Modifier		: 
// Comments		: Declare Global Variable for Site Custom Environment
//-----------------------------------------------------------------------------
extern CString	   g_strChartProgID;

inline void AfxSetChartProgID()
{
	CString szCfgPath;
	{
		//CString szRootDir;
		char _szBuf_[255];		
		::GetModuleFileName(NULL, _szBuf_, 255);
		CString	_strRoot_ = _szBuf_;
		int _loc_ = _strRoot_.ReverseFind('\\');
		if( _loc_ > 0)
			_strRoot_ = _strRoot_.Left(_loc_);
		
		//szRootDir = _strRoot_;
		szCfgPath.Format("%s\\Data\\%s", _strRoot_, "ChartMetaTable.dat");
	}

	char szProgID[ 256];
	DWORD dwError = GetPrivateProfileString( "Chart Version", "ProgID", "Prog ID Error", szProgID, 255, szCfgPath);
	//long lResult = CLSIDFromProgID( szProgID, &clsid);
	//if( lResult < 0)
	if(dwError<0)
	{
		//AfxMessageBox( CString( szProgID) + "'s CLSID is not found!");
		TRACE(CString( szProgID) + "'s CLSID is not found!\n");
	}
	else
	{
		g_strChartProgID = szProgID;
	}
}

#endif // !defined(AFX_SITEDEFINE_H__EAE48A29_C075_440E_AF1D_F07337CEAE16__INCLUDED_)
