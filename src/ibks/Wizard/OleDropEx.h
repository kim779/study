// OleDropEx.h: interface for the COleDropEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLEDROPEX_H__77B37B6F_D422_4065_957E_BEED29DB8025__INCLUDED_)
#define AFX_OLEDROPEX_H__77B37B6F_D422_4065_957E_BEED29DB8025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COleDropEx : public IDropTarget  
{
public:
	COleDropEx();
	virtual ~COleDropEx();

protected:
	ULONG		m_AddRefCount;
	CMap	<HWND, HWND, class CWorks*, CWorks*>	m_works;

public:
	virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG _stdcall AddRef(void);
	virtual ULONG _stdcall Release(void);

	virtual HRESULT _stdcall DragEnter(struct IDataObject *pObject, unsigned long state, struct _POINTL mouse, unsigned long *pDrop);
	virtual HRESULT _stdcall DragOver(unsigned long state, struct _POINTL mouse, unsigned long *pDrop);
	virtual HRESULT _stdcall DragLeave(void);
	virtual HRESULT _stdcall Drop(struct IDataObject *pObject, unsigned long state, struct _POINTL mouse, unsigned long *pDrop);

	BOOL Register(class CWorks* works, HWND hWnd);
	void Revoke(HWND hWnd);

};

#endif // !defined(AFX_OLEDROPEX_H__77B37B6F_D422_4065_957E_BEED29DB8025__INCLUDED_)
