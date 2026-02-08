// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__44001BAF_D22B_4C18_B8A6_23B4EF3444AF__INCLUDED_)
#define AFX_STDAFX_H__44001BAF_D22B_4C18_B8A6_23B4EF3444AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <AfxTempl.h>

//# Xml User!
#import "msxml.dll"
using namespace MSXML;

typedef MSXML::IXMLDOMElementPtr				XMLELEMENT;
typedef MSXML::IXMLDOMDocumentPtr				DOCUMENT;
typedef MSXML::IXMLDOMNodeListPtr				NODELIST;
typedef MSXML::IXMLDOMNodePtr					NODE;
typedef MSXML::IXMLDOMNamedNodeMapPtr			ATTRIBUTE;
typedef MSXML::IXMLDOMProcessingInstructionPtr	PROCESSINGINSTRCT;
typedef MSXML::IXMLDOMParseErrorPtr				PARSEERROR;

#include "./definefiles/color_TypeDef.h"
#include "./definefiles/client_TypeDef.h"
#include "./definefiles/server_TypeDef.h"
#include "./definefiles/message_TypeDef.h"

#include <afxtempl.h>
#include "../../Chart/COMMONTR/TRDefine.h"
#include "../../ForSite/FS_Env.h"
#include "../../DRCommon/Utils.h" //20120112 ÀÌ¹®¼ö >>

#define MakeNull(xx)	xx=NULL;
#define SafeDelete(xx)	if(xx)delete xx; MakeNull(xx);

#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#define SAFE_DELETEW(p) if(p) { p->DestroyWindow(); delete p; p = NULL; }
void Msg(char* fmt, ...);

#include "../../Chart/COMMONTR/KB_p0602.h"
#include "../../inc/PcTrMngHelper.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__44001BAF_D22B_4C18_B8A6_23B4EF3444AF__INCLUDED_)
