// tlInOut.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlInOut.h"

#include "../resource.h"
#include "../amCom/formItem.h"

// CTlInOut

CTlInOut::CTlInOut(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type)
	: CTlRect(view, draw, formItem)
{
	switch (type)
	{
	case ID_TOOL_EDIT:
		m_type = ioEDIT;
		m_name = _T("Edit");
		break;
	case ID_TOOL_OUT:
		m_type = ioOUT;
		m_name = _T("Output");
		break;
	case ID_TOOL_COMBO:
		m_type = ioCOMBO;
		m_name = _T("ComboBox");
		break;
	/*case ID_TOOL_MEMO:
		m_type = ioMEMO;
		m_name = _T("Memo");
		break;*/
	case ID_TOOL_CONTROL:
		m_type = ioCONTROL;
		m_name = _T("Control");
		break;
	default:
		m_type = ioNONE;
		break;
	}
}

CTlInOut::~CTlInOut()
{
}

void CTlInOut::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	tRc,  fRc;
	int	len,  ndc = dc->SaveDC();
	UINT	style;
	CString	str = _T("");
	CString	str2 = _T("");
	struct	_formR*	formR = m_formItem->GetForm(fN);

	m_display = m_view->m_display;

	CTlBase::Draw(dc, fN);
	fRc = getRect();
	switch (m_type)
	{
	case ioEDIT:
		setFont(dc);
		fRc = getRect();
		tRc = fRc;

		setTextColor(dc);
		brush = setBrush(dc);
		setBkColor(dc);
		dc->FillRect(fRc, brush);

		switch (formR->alignment)
		{
		case AL_RIGHT:
			style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;
			break;
		case AL_CENTER:
			style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;
			break;
		case AL_LEFT:
		default:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
			break;
		}

		if (formR->size != 0)
		{
			len = (int)(formR->size - formR->offs);
			//if (formR->isEdit == IE_EDIT)
			if (!m_formItem->GetFormStr(fN, _T("editS")).IsEmpty())
			{
				if (formR->type == IO_NUM || formR->type == IO_ALPHA)
					str = m_formItem->GetFormStr(fN, _T("editS"));
				else
				{
					m_formItem->SetFormStr(fN, _T("editS"), _T(""));
					//formR->isEdit = IE_NOP;
					setInOutFace(str, formR->size, formR->type);
				}
			}
			else
			{
				if (formR->attr & FA_PASSWD)
					str = CString(_T('*'), formR->size);
				else if (formR->type == IO_NUM && formR->attr & FA_COMMA)
				{
					editComma(str, len);
					if (len != formR->size)
					{
						str += _T('.');
						CString	tmps = _T("");
						setInOutFace(tmps, formR->offs, IO_NUM);
						str += tmps;
					}
				}
				else
				{
					setInOutFace(str, len, formR->type);
					if (len != formR->size)
					{
						str += _T('.');
						CString	tmps = _T("");
						setInOutFace(tmps, formR->offs, IO_NUM);
						str += tmps;
					}
				}
			}
		}
		else
			str = EDITFACE;

		switch (formR->borders)
		{
		case BD_RAISED:
			drawEdge(dc, fRc, EDGE_RAISED);
			break;
		case BD_SUNKEN:
			drawEdge(dc, fRc, EDGE_SUNKEN);
			break;
		case BD_BUMP:
			drawEdge(dc, fRc, EDGE_BUMP);
			break;
		case BD_ETCHED:
			drawEdge(dc, fRc, EDGE_ETCHED);
			break;
		case BD_LINE:
			setPen(dc, getRGB(formR->bRGB));
			dc->SelectStockObject(NULL_BRUSH);
			dc->Rectangle(fRc);
			break;
		default:
			break;
		}
		tRc.DeflateRect(4, 0);

		str2 = m_formItem->GetFormStr(fN, _T("str2"));
		if (!str2.IsEmpty())
		{
			drawPNG(dc, tRc, str2);
			dc->SetBkMode(TRANSPARENT);
		}

		
		switch (m_display)
		{
		case dmDATA:
			dc->DrawText(str, tRc, style);
			break;
		case dmSYMBOL:
			dc->DrawText(m_formItem->GetFormStr(fN, _T("name")), tRc, style);
			break;
		case dmNONE:
		default:
			break;
		}
		tRc.InflateRect(4, 0);
		break;

	case ioOUT:
		setFont(dc);
		setTextColor(dc);
		brush = setBrush(dc);
		setBkColor(dc);
		dc->FillRect(fRc, brush);

		tRc = fRc;
		if (formR->borders != BD_NONE)
			tRc.DeflateRect(4, 0);

		switch (formR->alignment)
		{
		case AL_RIGHT:
			style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;
			break;
		case AL_CENTER:
			style = DT_SINGLELINE|DT_VCENTER|DT_CENTER;
			break;
		case AL_LEFT:
		default:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
			break;
		}

		if (formR->size != 0)
		{
			len = (int)(formR->size - formR->offs);
			//if (formR->isEdit == IE_EDIT)
			if (!m_formItem->GetFormStr(fN, _T("editS")).IsEmpty())
				str = m_formItem->GetFormStr(fN, _T("editS"));
			else
			{
				if (formR->attr & FA_PASSWD)
					str = CString(_T('*'), formR->size);
				else if (formR->type == IO_NUM && formR->attr & FA_COMMA)
				{
					editComma(str, len);
					if (len != formR->size)
					{
						str += _T('.');
						CString	tmps = _T("");
						setInOutFace(tmps, formR->offs, IO_NUM);
						str += tmps;
					}
				}
				else
				{
					setInOutFace(str, len, formR->type);
					if (len != formR->size)
					{
						str += _T('.');
						CString	tmps = _T("");
						setInOutFace(tmps, formR->offs, IO_NUM);
						str += tmps;
					}
				}
			}
		}
		else
			str = OUTPUTFACE;

		tRc.DeflateRect(4, 0, 4, 0);

		str2 = m_formItem->GetFormStr(fN, _T("str2"));
		if (!str2.IsEmpty())
		{
			drawPNG(dc, tRc, str2);
			dc->SetBkMode(TRANSPARENT);
		}

		switch (m_display)
		{
		case dmDATA:
			dc->DrawText(str, tRc, style);
			break;
		case dmSYMBOL:
			dc->DrawText(m_formItem->GetFormStr(fN, _T("name")), tRc, style);
			break;
		case dmNONE:
		default:
			break;
		}

		switch (formR->borders)
		{
		case BD_RAISED:
			drawEdge(dc, fRc, EDGE_RAISED);
			break;
		case BD_SUNKEN:
			drawEdge(dc, fRc, EDGE_SUNKEN);
			break;
		case BD_BUMP:
			drawEdge(dc, fRc, EDGE_BUMP);
			break;
		case BD_ETCHED:
			drawEdge(dc, fRc, EDGE_ETCHED);
			break;
		case BD_LINE:
			setPen(dc, getRGB(formR->bRGB));
			dc->SelectStockObject(NULL_BRUSH);
			dc->Rectangle(fRc);
			break;
		default:
			break;
		}
		break;

	case ioCOMBO:
		setFont(dc);
		fRc = getRect();
		setTextColor(dc);
		brush = setBrush(dc);
		setBkColor(dc);
		dc->FillRect(fRc, brush);

		tRc = fRc;
		drawEdge(dc, fRc, EDGE_SUNKEN);

		tRc.left += 2;

		str2 = m_formItem->GetFormStr(fN, _T("str2"));
		if (!str2.IsEmpty())
		{
			drawPNG(dc, tRc, str2);
			dc->SetBkMode(TRANSPARENT);
		}

		switch (m_display)
		{
		case dmDATA:
			str = m_formItem->GetFormStr(fN, _T("editS"));
			if (!str.IsEmpty())
				dc->DrawText(str, tRc, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			else
				dc->DrawText(COMBOFACE, tRc, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			break;
		case dmSYMBOL:
			dc->DrawText(m_formItem->GetFormStr(fN, _T("name")), tRc, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			break;
		case dmNONE:
		default:
			break;
		}
		tRc.DeflateRect(2, 2);
		tRc.left   = tRc.right - szSCROLL;
		dc->DrawFrameControl(tRc, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
		break;

	case ioMEMO:
		setFont(dc);
		if (formR->keys > 0)
		{
			int	ww = m_fwidth*(formR->keys+1)+23;
			if (ww > fRc.Width())
				formR->right = formR->left + ww;
		}
		setTextColor(dc, clBLACK);

		brush = setBrush(dc, clWHITE);
		dc->FillRect(fRc, brush);

		tRc = fRc;
		tRc.left += 2;
		tRc.top  += 2;
		dc->SetBkMode(TRANSPARENT);

		switch (m_display)
		{
		case dmDATA:
			dc->DrawText(MEMOFACE, tRc, DT_SINGLELINE|DT_TOP|DT_LEFT);
			break;
		case dmSYMBOL:
			dc->DrawText(m_formItem->GetFormStr(fN, _T("name")), tRc, DT_SINGLELINE|DT_TOP|DT_LEFT);
			break;
		case dmNONE:
		default:
			break;
		}

		if (formR->borders)
			dc->Draw3dRect(fRc, clBLACK, clBLACK);
		break;

	case ioCONTROL:
		{
			CString tmp;
			CString str = m_formItem->GetFormStr(fN, _T("str"));
			if (str.IsEmpty())
				str = _T("Custom");
			else
			{
				tmp.Format(_T("Custom[%s]"), str);
				str = tmp;
			}
			setFont(dc);
			setTextColor(dc);
			brush = setBrush(dc);
			dc->FillRect(fRc, brush);

			tRc = fRc;
			tRc.left += 2;
			tRc.top  += 2;
			dc->SetBkMode(TRANSPARENT);
			switch (m_display)
			{
			case dmDATA:
				dc->DrawText(str, tRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				break;
			case dmSYMBOL:
				dc->DrawText(m_formItem->GetFormStr(fN, _T("name")), tRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				break;
			case dmNONE:
			default:
				break;
			}

			/*CString dir;
			dir.Format("%s\\dev\\%s.dll", m_view->m_root, m_formItem->getFormStr(m_fN, _T("str")));
			m_param.name = m_formItem->getFormStr(m_fN, _T("name"));
			m_param.rect = CRect(formR->left, 
				formR->top, 
				formR->right, 
				formR->bottom);
			m_param.fonts = _T("굴림");
			m_param.point = 9;
			m_param.style = 0;
			m_param.tRGB = 63;
			m_param.pRGB = 62;
			m_param.options = m_formItem->getFormStr(m_fN, _T("str2"));
			
			m_hControlDll = LoadLibrary(dir);
			if (!m_hControlDll)
			{
				TRACE(_T("Control.dll load error !!\n"));
				break;
			}
			
			void	(APIENTRY *axPreview)(CDC*, CWnd *, void*);
			axPreview = (void (APIENTRY *)(CDC*, CWnd *, void*)) GetProcAddress(m_hControlDll, _T("axPreview"));
			if (axPreview == NULL)
			{
				TRACE("Control.dll axCtrl preview error !!\n");
				break;
			}
			axPreview(dc, m_view, (void*)&m_param);
			FreeLibrary(m_hControlDll);*/
		}
		break;
	default:
		break;
	}

	dc->RestoreDC(ndc);
}

void CTlInOut::drawEnd()
{
	CTlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		switch (m_type)
		{
		case ioEDIT:		// Edit
			m_rect.right  = m_rect.left + getDefaultWidth(_T("EDIT"), 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("EDIT"), m_fheight + 7);
			break;

		case ioOUT:		// Output
			m_rect.right  = m_rect.left + getDefaultWidth(_T("OUTPUT"), m_fwidth * (int)strlen((LPCSTR)OUTPUTFACE));
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("OUTPUT"), m_fheight + 2);
			break;
		
		case ioCOMBO:		// ComboBox
			m_rect.right  = m_rect.left + getDefaultWidth(_T("COMBO"), 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("COMBO"), m_fheight + 7);
			break;
		/*
		case ioMEMO:		// Memo
			m_rect.right  = m_rect.left + getDefaultWidth(_T("MEMO"), 150);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("MEMO"), 70);
			break;
			*/
		case ioCONTROL:
			m_rect.right  = m_rect.left + getDefaultWidth(_T("CONTROL"), 150);
			m_rect.bottom = m_rect.top  + getDefaultHeight(_T("CONTROL"), 70);
			break;
		}
	}

	CRect	wRc;
	struct	_formR*	formR = m_formItem->GetForm(m_fN);

	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	formR->left   = m_rect.left;
	formR->top    = m_rect.top;
	formR->right  = m_rect.right;
	formR->bottom = m_rect.bottom;
	formR->alpha  = 100;			// alpha default setting

	switch (m_type)
	{
	case ioEDIT:
  		formR->kind  = FM_EDIT;
		formR->type  = IO_NUM;
  		formR->iok   = EIO_INPUT;
		formR->attr |= FA_WRAP;
		formR->tRGB = getDefaultTRGB(_T("EDIT"), 232);
		formR->pRGB = getDefaultPRGB(_T("EDIT"), basINPUT);
		formR->borders = BD_LINE;
		formR->bRGB = 244;
		break;
		
	case ioOUT:
		formR->kind = FM_OUT;
		formR->type = IO_NUM;
		formR->tRGB = getDefaultTRGB(_T("OUTPUT"), 232);
		formR->pRGB = getDefaultPRGB(_T("OUTPUT"), basINPUT);
		formR->borders = BD_LINE;
		formR->bRGB = 244;
		break;
		
	case ioCOMBO:
		formR->kind = FM_COMBO;
		formR->type = IO_NUM;
		formR->pRGB = basINPUT;
		formR->tRGB = 232;
		formR->iok  = EIO_INPUT;
		formR->tRGB = getDefaultTRGB(_T("COMBO"), 232);
		formR->pRGB = getDefaultPRGB(_T("COMBO"), basINPUT);
		formR->spRGB = getDefaultPRGB(_T("COMBO"), basINPUT);
		break;
	/*	
	case ioMEMO:
		formR->kind = FM_MEMO;
		formR->iok  = EIO_INPUT;
		formR->keys = 0;
		formR->type = IO_NUM;
		formR->tRGB = getDefaultTRGB(_T("MEMO"), 232);
		formR->pRGB = getDefaultPRGB(_T("MEMO"), basINPUT);
		break;
	*/	
	case ioCONTROL:
		formR->kind = FM_CONTROL;
		formR->attr = FA_PROTECT;
		formR->tRGB = getDefaultTRGB(_T("CONTROL"), 232);
		formR->pRGB = getDefaultPRGB(_T("CONTROL"), basINPUT);
		formR->alpha = 100;			// alpha default setting
		break;
		
	default:
		break;
	}

	m_dc->Rectangle(m_rect);
}