/*
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/

// ShowObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShowObject.h"
#include "../form/amform.h"
#include "../amLib/AmPalette.h"
#include "../amLib/AmDraw.h"
// CShowObject

/*
CShowObject::CShowObject()
{
}
*/

CShowObject::CShowObject(CWnd* pWnd, CRect rect, CString root, CString fileN)
{
	m_mapForm.RemoveAll();
	m_view    = pWnd;
	m_rect.SetRectEmpty();

	if (strcmp(root.Right(1), "\\") != 0)
		root += "\\";

	m_root    = root;
	m_load    = false;
	m_fileN   = _T("");

	m_form    = NULL;
	m_forms.RemoveAll();
	Load(fileN);
}

CShowObject::~CShowObject()
{
	if (m_form)
	{
		delete m_form;
		removeForms();
		m_load = false;
	}
	removeForms();
}


// CShowObject 멤버 함수

bool CShowObject::Load(CString fileN, int flag)
{
	if (fileN.IsEmpty())
		return false;

//	if (flag == 1)
//		fileN.Format(_T("%s\\exe\\tmp\\%s.ax"), m_root, fileN);
	//else
	//	fileN = getPath(fileN);

	//if (m_form)
	//{
	//	delete m_form;
	//	removeForms();
	//	m_load = false;
	//}

	CString	tmps;
	tmps.Format("%s%s\\%s", m_root, TABDIR, PALETTE);
	if (!m_mapForm.Lookup(fileN, m_form))
	{
		m_form = new CAmForm();
		m_form->m_pPalette	= new CAmPalette((char *)tmps.operator LPCTSTR(),"");		
		m_form->m_pAmDraw	= new CAmDraw();
		m_form->m_view		= m_view;
		m_form->SetRoot(CString(m_root));
		if (!m_form->LoadMAP(fileN))
		{
			delete m_form;
			m_form = NULL;
			m_load = false;
			return false;
		}

		m_mapForm.SetAt(fileN, m_form);
	}
		
	m_rect.SetRect(0, 0, m_form->m_mapH->width, m_form->m_mapH->height);
	m_fileN = fileN;
	m_form->LoadForm(m_rect, true);
	m_load = true;

	
	CRect	rect;
	CShowObject*	form;
	for (int ii = 0; ii < m_form->m_mapH->formN; ii++)
	{
		if (m_form->m_vFORM[ii]->kind != FM_OBJECT)
			continue;

		rect = m_form->GetAtForm(ii)->GetRect();
		form = new CShowObject(m_view, /*m_palette, m_draw,*/ rect, m_root);
		m_forms.SetAt(ii, form);
		m_form->GetAtForm(ii)->ReadData(fileN);
		form->Load(fileN);
	}	
	return true;
}

void CShowObject::Draw(CDC* dc)
{	
	if (!m_load || !m_form)
		return;

	m_form->DrawForm(dc);

	int	key;
	CShowObject*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		form->Draw(dc);
	}
	
}

void CShowObject::Resize(CRect rect)
{	
	if (!m_load || !m_form || m_rect.EqualRect(&rect))
		return;

	// 에뮬의 역 비율 적용
	float	ratio = (float)(100.0 / m_form->m_mapH->resolution);
	float	hR = ((float)rect.Width()  / (float)m_form->m_mapH->width) * ratio;
	float	vR = ((float)rect.Height() / (float)m_form->m_mapH->height) * ratio;
	
	m_form->ResizeForm(hR, vR);
	m_form->OffsetForm(CSize(rect.left-m_rect.left, rect.top-m_rect.top));
	m_rect = rect;

	int	key;
	CShowObject*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		rect = m_form->GetAtForm(key)->GetRect();
		form->Resize(rect, hR, vR);		
	}
	
}

void CShowObject::Resize(CRect rect, float hRatio, float vRatio)
{	
	if (!m_load || !m_form || m_rect.EqualRect(rect))
		return;

	m_form->ResizeForm(hRatio, vRatio);
	m_form->OffsetForm(CSize(rect.left-m_rect.left, rect.top-m_rect.top));
	m_rect = rect;

	int	key;
	CShowObject*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		rect = m_form->GetAtForm(key)->GetRect();
		form->Resize(rect, hRatio, vRatio);
	}
	
}

CSize CShowObject::GetSize()
{	
	CSize	size = CSize(0, 0);

	if (m_load && m_form)
	{
		size.cx = m_form->m_mapH->width;
		size.cy = m_form->m_mapH->height;
	}
	return size;	
}

CString CShowObject::GetTitle()
{
	/*
	if (!m_load)
		return _T("");

	return (CString)(char *)m_form->m_mapH->caption;
	*/

	return NULL;
}

void CShowObject::removeForms()
{	
	int	key;
	CShowObject*	form;

	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		delete form;
	}
	m_forms.RemoveAll();	
}

CString CShowObject::getPath(CString mapN)
{	
	CString path;

	//path.Format(_T("%s\\%s\\%s\\%s\\%s"), m_root, MAPDIR, mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);
	return path;
}
