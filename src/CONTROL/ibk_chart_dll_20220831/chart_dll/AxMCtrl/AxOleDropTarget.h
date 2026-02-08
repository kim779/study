// AxOleDropTarget.h: interface for the CAxOleDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXOLEDROPTARGET_H__227B591E_FFF6_4C09_A3A0_F7D8CB0AAB76__INCLUDED_)
#define AFX_AXOLEDROPTARGET_H__227B591E_FFF6_4C09_A3A0_F7D8CB0AAB76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxole.h"
class CAxOleDropTarget : public COleDropTarget
{
public:
	CAxOleDropTarget();
	virtual ~CAxOleDropTarget();

	CString Parser(CString &srcstr, CString substr);

   DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                                dwKeyState, CPoint point );
   DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                               dwKeyState, CPoint point );
   void OnDragLeave(CWnd* pWnd);               
   
   BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT 
                                          dropEffect, CPoint point );
};

#endif // !defined(AFX_AXOLEDROPTARGET_H__227B591E_FFF6_4C09_A3A0_F7D8CB0AAB76__INCLUDED_)
