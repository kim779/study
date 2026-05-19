#pragma once

// CShowObject 명령 대상입니다.

class AFX_EXT_CLASS CShowObject : public CObject
{
public:
	CShowObject(CWnd* pWnd, CRect rect, CString root, CString fileN = _T(""));
	virtual ~CShowObject();

public:
	bool	m_bfirst;

	bool	m_load;
	CRect	m_rect;

protected:
	CString	m_root;
	CString	m_fileN;
	CWnd*	m_view;

	class	CAmForm*	m_form;
	CMap	<int, int, CShowObject*, CShowObject*>	m_forms;
	CMap	<CString, LPCTSTR, CAmForm*, CAmForm*>	m_mapForm;

// Operations
public:
	bool	Load(CString fileN, int flag = -1);
	void	Draw(CDC* dc);
	void	Resize(CRect rect);
	void	Resize(CRect rect, float hRatio, float vRatio);
	CSize	GetSize();
	CString	GetTitle();
	

protected:
	void	removeForms();
	CString	getPath(CString mapN);

};


