// tlIO.cpp: implementation of the CtlIO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlIO.h"

#include "../childview.h"
#include "../mapform.h"
#include "../resource.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtlIO::CtlIO(CChildView* view, UINT id)
	: CtlRect(view)
{
	switch (id)
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
	case ID_TOOL_MEMO:
		m_type = ioMEMO;
		m_name = _T("Memo");
		break;
	case ID_TOOL_CONTROL:
		m_type = ioCONTROL;
		m_name = _T("Control");
		break;
	default:
		m_type = ioNONE;
		break;
	}
}

CtlIO::~CtlIO()
{
}

void CtlIO::drawEnd()
{
	CtlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		switch (m_type)
		{
		case ioEDIT:		// Edit
			m_rect.right  = m_rect.left + getDefaultWidth("EDIT", 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight("EDIT", m_fheight + 7);
			break;

		case ioOUT:		// Output
			m_rect.right  = m_rect.left + getDefaultWidth("OUTPUT", m_fwidth * strlen(OUTPUTFACE));
			m_rect.bottom = m_rect.top  + getDefaultHeight("OUTPUT", m_fheight + 2);
			break;
		
		case ioCOMBO:		// ComboBox
			m_rect.right  = m_rect.left + getDefaultWidth("COMBO", 120);
			m_rect.bottom = m_rect.top  + getDefaultHeight("COMBO", m_fheight + 7);
			break;
		
		case ioMEMO:		// Memo
			m_rect.right  = m_rect.left + getDefaultWidth("MEMO", 150);
			m_rect.bottom = m_rect.top  + getDefaultHeight("MEMO", 70);
			break;
		case ioCONTROL:
			m_rect.right  = m_rect.left + getDefaultWidth("CONTROL", 150);
			m_rect.bottom = m_rect.top  + getDefaultHeight("CONTROL", 70);
			break;
		}
	}

	CRect	wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	m_pFormItem->getFormPoint(m_fN)->left   = m_rect.left;
	m_pFormItem->getFormPoint(m_fN)->top    = m_rect.top;
	m_pFormItem->getFormPoint(m_fN)->right  = m_rect.right;
	m_pFormItem->getFormPoint(m_fN)->bottom = m_rect.bottom;

	switch (m_type)
	{
	case ioEDIT:
  		m_pFormItem->getFormPoint(m_fN)->kind  = FM_EDIT;
		m_pFormItem->getFormPoint(m_fN)->type  = IO_NUM;
  		m_pFormItem->getFormPoint(m_fN)->iok   = EIO_INPUT;
		m_pFormItem->getFormPoint(m_fN)->attr |= FA_WRAP;
		m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("EDIT", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("EDIT", basINPUT);
		break;
		
	case ioOUT:
		m_pFormItem->getFormPoint(m_fN)->kind = FM_OUT;
		m_pFormItem->getFormPoint(m_fN)->type = IO_NUM;
		m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("OUTPUT", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("OUTPUT", basINPUT);
		break;
		
	case ioCOMBO:
		m_pFormItem->getFormPoint(m_fN)->kind = FM_COMBO;
		m_pFormItem->getFormPoint(m_fN)->type = IO_NUM;
		m_pFormItem->getFormPoint(m_fN)->pRGB = basINPUT;
		m_pFormItem->getFormPoint(m_fN)->tRGB = 232;
		m_pFormItem->getFormPoint(m_fN)->iok  = EIO_INPUT;
		m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("COMBO", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("COMBO", basINPUT);
		break;
		
	case ioMEMO:
		m_pFormItem->getFormPoint(m_fN)->kind = FM_MEMO;
		m_pFormItem->getFormPoint(m_fN)->iok  = EIO_INPUT;
		m_pFormItem->getFormPoint(m_fN)->keys = 0;
		m_pFormItem->getFormPoint(m_fN)->type = IO_NUM;
		m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("MEMO", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("MEMO", basINPUT);
		break;
		
	case ioCONTROL:
		m_pFormItem->getFormPoint(m_fN)->kind = FM_CONTROL;
		m_pFormItem->getFormPoint(m_fN)->attr = FA_PROTECT;
		m_pFormItem->getFormPoint(m_fN)->tRGB = getDefaultTRGB("CONTROL", 232);
		m_pFormItem->getFormPoint(m_fN)->pRGB = getDefaultPRGB("CONTROL", basINPUT);
		break;
		
	default:
		break;
	}

	m_dc->Rectangle(m_rect);
}

void CtlIO::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	tRc,  fRc;
	int	len,  ndc = dc->SaveDC();
	UINT	style;
	CString	str = _T("");

	CTOOL::Draw(dc, fN);
	fRc = getRect();
	switch (m_type)
	{
	case ioEDIT:
		setFont(dc);
		fRc = getRect();
		tRc = fRc;
		if (m_pFormItem->getFormPoint(m_fN)->vals0 > 0)
			tRc.right -= szSCROLL;
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_CODE)
			tRc.right -= (szSCROLL + 2);
		if (m_pFormItem->getFormPoint(m_fN)->attr & FA_DATE)
			tRc.right -= (szSCROLL + 2);
		setTextColor(dc);
	
		brush = setBrush(dc);
		setBkColor(dc);
		dc->FillRect(fRc, brush);

		if (m_pFormItem->getFormPoint(m_fN)->offs != 0)
			m_pFormItem->getFormPoint(m_fN)->attr |= FA_RIGHT;
		if (m_pFormItem->getFormPoint(m_fN)->attr & FA_RIGHT)
			style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;
		else
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT;

		if (m_pFormItem->getFormPoint(m_fN)->size != 0)
		{
			len = (int)(m_pFormItem->getFormPoint(m_fN)->size - m_pFormItem->getFormPoint(m_fN)->offs);
			if (m_pFormItem->getFormPoint(m_fN)->isEdit == IE_EDIT)
			{
				if (m_pFormItem->getFormPoint(m_fN)->type == IO_NUM || m_pFormItem->getFormPoint(m_fN)->type == IO_ALPHA)
					str = m_pFormItem->getFormStr(m_fN, "editS");
				else
				{
					m_pFormItem->setFormStr(m_fN, "editS", "");
					m_pFormItem->getFormPoint(m_fN)->isEdit = IE_NOP;
					setInOutFace(str, m_pFormItem->getFormPoint(m_fN)->size, m_pFormItem->getFormPoint(m_fN)->type);
				}
			}
			else
			{
				if (m_pFormItem->getFormPoint(m_fN)->attr & FA_PASSWD)
					str = CString('*', m_pFormItem->getFormPoint(m_fN)->size);
				else if (m_pFormItem->getFormPoint(m_fN)->type == IO_NUM && m_pFormItem->getFormPoint(m_fN)->attr & FA_COMMA)
				{
					editComma(str, len);
					if (len != m_pFormItem->getFormPoint(m_fN)->size)
					{
						str += '.';
						CString	tmps = _T("");
						setInOutFace(tmps, m_pFormItem->getFormPoint(m_fN)->offs, IO_NUM);
						str += tmps;
					}
				}
				else
				{
					setInOutFace(str, len, m_pFormItem->getFormPoint(m_fN)->type);
					if (len != m_pFormItem->getFormPoint(m_fN)->size)
					{
						str += '.';
						CString	tmps = _T("");
						setInOutFace(tmps, m_pFormItem->getFormPoint(m_fN)->offs, IO_NUM);
						str += tmps;
					}
				}
			}
		}
		else
			str = EDITFACE;

		tRc.DeflateRect(4, 0);
		switch (*m_pDisplayMode)
		{
		case dmDATA:
			dc->DrawText(str, tRc, style);	break;
		case dmSYMBOL:
			dc->DrawText(m_pFormItem->getFormStr(m_fN, "name"), tRc, style);	break;
		case dmNONE:
		default:
			break;
		}
		tRc.InflateRect(4, 0);
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_CODE)
			tRc.right += (szSCROLL + 2);
		if (m_pFormItem->getFormPoint(m_fN)->attr & FA_DATE)
			tRc.right += (szSCROLL + 2);
	/*	if (m_pFormItem->getFormPoint(m_fN)->borders == ST_CLASSIC)
			drawEdge(dc, tRc, EDGE_SUNKEN);
		else if (m_pFormItem->getFormPoint(m_fN)->borders == ST_FLAT ||
			m_pFormItem->getFormPoint(m_fN)->borders == ST_FLAT2)
			dc->Draw3dRect(tRc, clBLACK, clBLACK);*/
		dc->Draw3dRect(tRc, clBLACK, clBLACK);

		if (m_pFormItem->getFormPoint(m_fN)->vals0 > 0)
		{
			CRect	sRc1, sRc2;

			tRc = fRc;
			tRc.left = tRc.right - szSCROLL;
			if (tRc.Height() % 2)
				tRc.bottom++;
			sRc1 = sRc2 = tRc;
			sRc1.bottom = sRc1.top + tRc.Height()/2;
			sRc2.top = sRc1.bottom;
			dc->DrawFrameControl(sRc1, DFC_SCROLL, DFCS_SCROLLUP);
			dc->DrawFrameControl(sRc2, DFC_SCROLL, DFCS_SCROLLDOWN);
		}
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_CODE)
		{
			tRc = fRc;
			if (m_pFormItem->getFormPoint(m_fN)->vals0 > 0)
				tRc.right -= szSCROLL;
			tRc.DeflateRect(2, 2);
			tRc.left = tRc.right - szSCROLL;
			dc->DrawFrameControl(tRc, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
		}

		if (m_pFormItem->getFormPoint(m_fN)->attr & FA_DATE)
		{
			tRc = fRc;
			tRc.DeflateRect(1, 1);
			tRc.left = tRc.right - szSCROLL;
			brush = setBrush(dc, clEDGE);
			//dc->FillRect(tRc, brush);
			dc->DrawFrameControl(tRc, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
			drawEdge(dc, tRc, EDGE_RAISED);
		}

		break;

	case ioOUT:
		setFont(dc);
		setTextColor(dc);
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
			dc->SetBkMode(TRANSPARENT);
		else
		{
			brush = setBrush(dc);
			setBkColor(dc);
			dc->FillRect(fRc, brush);
		}

		tRc = fRc;
		if (m_pFormItem->getFormPoint(m_fN)->borders != BD_NONE)
			tRc.DeflateRect(4, 0);

		if (m_pFormItem->getFormPoint(m_fN)->offs != 0)
			m_pFormItem->getFormPoint(m_fN)->attr |= FA_RIGHT;
		switch (m_pFormItem->getFormPoint(m_fN)->alignment)
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

		if (m_pFormItem->getFormPoint(m_fN)->attr & FA_RIGHT)
			style = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;

		if (m_pFormItem->getFormPoint(m_fN)->size != 0)
		{
			len = (int)(m_pFormItem->getFormPoint(m_fN)->size - m_pFormItem->getFormPoint(m_fN)->offs);
			if (m_pFormItem->getFormPoint(m_fN)->isEdit == IE_EDIT)
				str = m_pFormItem->getFormStr(m_fN, "editS");
			else
			{
				if (m_pFormItem->getFormPoint(m_fN)->attr & FA_PASSWD)
					str = CString('*', m_pFormItem->getFormPoint(m_fN)->size);
				else if (m_pFormItem->getFormPoint(m_fN)->type == IO_NUM && m_pFormItem->getFormPoint(m_fN)->attr & FA_COMMA)
				{
					editComma(str, len);
					if (len != m_pFormItem->getFormPoint(m_fN)->size)
					{
						str += '.';
						CString	tmps = _T("");
						setInOutFace(tmps, m_pFormItem->getFormPoint(m_fN)->offs, IO_NUM);
						str += tmps;
					}
				}
				else
				{
					setInOutFace(str, len, m_pFormItem->getFormPoint(m_fN)->type);
					if (len != m_pFormItem->getFormPoint(m_fN)->size)
					{
						str += '.';
						CString	tmps = _T("");
						setInOutFace(tmps, m_pFormItem->getFormPoint(m_fN)->offs, IO_NUM);
						str += tmps;
					}
				}
			}
		}
		else
		{
			str = OUTPUTFACE;
		}
		tRc.DeflateRect(4, 0, 4, 0);
		switch (*m_pDisplayMode)
		{
		case dmDATA:
			dc->DrawText(str, tRc, style);	break;
		case dmSYMBOL:
			dc->DrawText(m_pFormItem->getFormStr(m_fN, "name"), tRc, style);	break;
		case dmNONE:
		default:
			break;
		}

		switch (m_pFormItem->getFormPoint(m_fN)->iok)
		{
		case PN_BORDER:
			switch (m_pFormItem->getFormPoint(m_fN)->borders)
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
			default:
				break;
			}
			break;
		case PN_LINE:
			setPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->bRGB));
			dc->SelectStockObject(NULL_BRUSH);
			dc->Rectangle(fRc);
			break;

		case PN_NONE:
		default:
			break;
		}
		break;

	case ioCOMBO:
		setFont(dc);
		fRc = getRect();
		setTextColor(dc);
		if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
			dc->SetBkMode(TRANSPARENT);
		else
		{
			brush = setBrush(dc);
			setBkColor(dc);
			dc->FillRect(fRc, brush);
		}

		tRc = fRc;
	/*	if (m_pFormItem->getFormPoint(m_fN)->borders == ST_CLASSIC)
			drawEdge(dc, fRc, EDGE_SUNKEN);
		else if (m_pFormItem->getFormPoint(m_fN)->borders == ST_FLAT)
		{
			dc->Draw3dRect(fRc, clBLACK, clBLACK);
			setBkColor(dc);
		}*/
		drawEdge(dc, fRc, EDGE_SUNKEN);

		tRc.left += 2;
		switch (*m_pDisplayMode)
		{
		case dmDATA:
			if (!m_pFormItem->getFormStr(m_fN, "editS").IsEmpty())
				dc->DrawText(m_pFormItem->getFormStr(m_fN, "editS"), tRc, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			else
				dc->DrawText(COMBOFACE, tRc, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			break;
		case dmSYMBOL:
			dc->DrawText(m_pFormItem->getFormStr(m_fN, "name"), tRc, DT_LEFT|DT_SINGLELINE|DT_VCENTER);	break;
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
		if (m_pFormItem->getFormPoint(m_fN)->keys > 0)
		{
			int	ww = m_fwidth*(m_pFormItem->getFormPoint(m_fN)->keys+1)+23;
			if (ww > fRc.Width())
				m_pFormItem->getFormPoint(m_fN)->right = m_pFormItem->getFormPoint(m_fN)->left + ww;
		}
		setTextColor(dc, clBLACK);

		brush = setBrush(dc, clWHITE);
		dc->FillRect(fRc, brush);

		tRc = fRc;
		tRc.left += 2;
		tRc.top  += 2;
		dc->SetBkMode(TRANSPARENT);

		switch (*m_pDisplayMode)
		{
		case dmDATA:
			dc->DrawText(MEMOFACE, tRc, DT_SINGLELINE|DT_TOP|DT_LEFT);	break;
		case dmSYMBOL:
			dc->DrawText(m_pFormItem->getFormStr(m_fN, "name"), tRc, DT_SINGLELINE|DT_TOP|DT_LEFT);	break;
		case dmNONE:
		default:
			break;
		}

		if (m_pFormItem->getFormPoint(m_fN)->borders)
			dc->Draw3dRect(fRc, clBLACK, clBLACK);
		break;
	case ioCONTROL:
		{
			CString str = m_pFormItem->getFormStr(m_fN, "str");
			if (str.IsEmpty())
				str = "CONTROL";

			setFont(dc);
			setTextColor(dc);
			if (!(m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT))
			{
				brush = setBrush(dc);
				dc->FillRect(fRc, brush);
			}
			tRc = fRc;
			tRc.left += 2;
			tRc.top  += 2;
			dc->SetBkMode(TRANSPARENT);
			switch (*m_pDisplayMode)
			{
			case dmDATA:
				dc->DrawText(str, tRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);	break;
			case dmSYMBOL:
				dc->DrawText(m_pFormItem->getFormStr(m_fN, "name"), tRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);	break;
			case dmNONE:
			default:
				break;
			}

			/*CString dir;
			dir.Format("%s\\dev\\%s.dll", m_view->m_root, m_pFormItem->getFormStr(m_fN, "str"));
			m_param.name = m_pFormItem->getFormStr(m_fN, "name");
			m_param.rect = CRect(m_pFormItem->getFormPoint(m_fN)->left, 
				m_pFormItem->getFormPoint(m_fN)->top, 
				m_pFormItem->getFormPoint(m_fN)->right, 
				m_pFormItem->getFormPoint(m_fN)->bottom);
			m_param.fonts = "±¼¸²";
			m_param.point = 9;
			m_param.style = 0;
			m_param.tRGB = 63;
			m_param.pRGB = 62;
			m_param.options = m_pFormItem->getFormStr(m_fN, "str2");
			
			m_hControlDll = LoadLibrary(dir);
			if (!m_hControlDll)
			{
				TRACE("Control.dll load error !!\n");
				break;
			}
			
			void	(APIENTRY *axPreview)(CDC*, CWnd *, void*);
			axPreview = (void (APIENTRY *)(CDC*, CWnd *, void*)) GetProcAddress(m_hControlDll, "axPreview");
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