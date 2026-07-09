// show.cpp : implementation file
//

#include "stdafx.h"
#include "show.h"
#include "../../dll/form/axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShow

IMPLEMENT_DYNCREATE(CShow, CObject)

CShow::CShow(CWnd* pWnd, CAxisPalette* palette, CAxisDraw* draw, CRect rect, CString root, CString fileN)
{
	m_view    = pWnd;
	m_palette = palette;
	m_draw    = draw;
	m_rect    = rect;
	m_rect.SetRectEmpty();
	m_root    = root;
	m_load    = false;
	m_fileN   = _T("");

	m_form    = (CAxisForm *) NULL;
	m_forms.RemoveAll();

	Load(fileN);
}

CShow::~CShow()
{
	if (m_form)	delete m_form;
	removeForms();
}

bool CShow::Load(CString fileN, int flag)
{
	if (fileN.IsEmpty())
		return false;

	if (flag == 1)
		fileN.Format("%s\\exe\\tmp\\%s.ax", m_root, fileN);
	else
		fileN = getPath(fileN);

	if (m_form)
	{
		delete m_form;
		removeForms();
		m_load = false;
	}

	m_form = new CAxisForm();
	m_form->m_root    = m_root;
	m_form->m_palette = m_palette;
	m_form->m_draw    = m_draw;
	m_form->m_view    = m_view;

	if (!m_form->LoadMAP(fileN))
		return false;

	m_rect.SetRect(0, 0, m_form->m_mapH->width, m_form->m_mapH->height);
	m_fileN = fileN;
	m_form->LoadForm(m_rect, true);
	m_load = true;

	CRect	rect;
	CShow*	form;
	for (int ii = 0; ii < m_form->m_mapH->formN; ii++)
	{
		if (m_form->m_formR[ii].kind != FM_OBJECT)
			continue;

		rect = m_form->GetAtForm(ii)->GetRect();
		form = new CShow(m_view, m_palette, m_draw, rect, m_root);
		m_forms.SetAt(ii, form);
		m_form->GetAtForm(ii)->ReadData(fileN);
		form->Load(fileN);
	}

	return true;
}

void CShow::Draw(CDC* dc)
{
	if (!m_load || !m_form)
		return;

	m_form->DrawForm(dc);

	int	key;
	CShow*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		form->Draw(dc);
	}
}

void CShow::Resize(CRect rect)
{
	if (!m_load || !m_form || m_rect.EqualRect(&rect))
		return;

	float	hR = (float)rect.Width()  / (float)m_form->m_mapH->width;
	float	vR = (float)rect.Height() / (float)m_form->m_mapH->height;
	m_form->ResizeForm(hR, vR);
	m_form->OffsetForm(CSize(rect.left-m_rect.left, rect.top-m_rect.top));
	m_rect = rect;

	int	key;
	CShow*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		rect = m_form->GetAtForm(key)->GetRect();
		form->Resize(rect, hR, vR);
	}
}

void CShow::Resize(CRect rect, float hRatio, float vRatio)
{
	if (!m_load || !m_form || m_rect.EqualRect(rect))
		return;

	m_form->ResizeForm(hRatio, vRatio);
	m_form->OffsetForm(CSize(rect.left-m_rect.left, rect.top-m_rect.top));
	m_rect = rect;

	int	key;
	CShow*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )

	{
		m_forms.GetNextAssoc(pos, key, form);
		rect = m_form->GetAtForm(key)->GetRect();
		form->Resize(rect, hRatio, vRatio);
	}
}

CSize CShow::GetSize()
{
	CSize	size = CSize(0, 0);

	if (m_load && m_form)
	{
		size.cx = m_form->m_mapH->width;
		size.cy = m_form->m_mapH->height;
	}

	return size;
}

CString CShow::GetTitle()
{
	if (!m_load)
		return _T("");

	return (char *)m_form->m_mapH->caption;
}

void CShow::removeForms()
{
	int	key;
	CShow*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		delete form;
	}
	m_forms.RemoveAll();
}

CString CShow::getPath(CString mapN)
{
	CString path;

	path.Format("%s\\%s\\%s\\%s\\%s", m_root, MAPDIR, mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);
	return path;
}
