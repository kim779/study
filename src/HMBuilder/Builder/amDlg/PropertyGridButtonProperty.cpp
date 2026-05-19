// PropertyGridButtonProperty.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PropertyGridButtonProperty.h"

#include "attrDlg.h"
#include "comboItem.h"
#include "colorDlg.h"
#include "gridColumn.h"
#include "option.h"
#include "publicDlg.h"
#include "tabPage.h"

#include "../h/mainvar.h"
#include "../amCom/formItem.h"


#define	AFX_PROP_HAS_BUTTON	0x0002

// CPropertyGridButtonProperty

IMPLEMENT_DYNAMIC(CPropertyGridButtonProperty, CMFCPropertyGridCtrl)

CPropertyGridButtonProperty::CPropertyGridButtonProperty(bool full, int type, const CString& strName, const _variant_t& varValue, LPCTSTR lpszDescr, DWORD dwData)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{

	m_full  = full;
	m_type  = type;
	m_dwFlags = AFX_PROP_HAS_BUTTON;

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetWindowText((LPCTSTR)varValue.bstrVal);	// 값 보여주기
	}
	m_parent = NULL;
	m_mapH = NULL;
	m_formItem = NULL;
	m_infos = _T("");
	m_fontD = NULL;
}

CPropertyGridButtonProperty::~CPropertyGridButtonProperty()
{
	if (m_fontD != NULL) 
		delete m_fontD;
}

void CPropertyGridButtonProperty::SetInfo(CWnd* parent, int fN, struct _mapH* mapH, CFormItem* formItem, CString infos)
{
	m_parent = parent;
	m_fN = fN;
	m_mapH = mapH;
	m_formItem = formItem;
	m_infos = infos;
}

void CPropertyGridButtonProperty::AdjustButtonRect()
{
	CMFCPropertyGridProperty::AdjustButtonRect();

	if (m_full && m_parent)
	{
		int	leftW = ((CMFCPropertyGridCtrl *)m_parent)->GetLeftColumnWidth();
		m_rectButton.left = GetRect().left + leftW + 1;
	}
}

void CPropertyGridButtonProperty::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	if (!m_pWndInPlace->m_hWnd)
	{
		m_bButtonIsDown = FALSE;
		return;
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	switch (m_type)
	{
	case 1001:	// font dialog
		if (m_mapH && m_formItem)
		{
			char	*pFname = NULL;
			WORD	*pFstyle = NULL;
			WORD	*pFpoint = NULL;

			if (m_fontD == NULL)
			{
				m_fontD = new CFontDialog();
				m_fontD->m_cf.lStructSize = sizeof(CHOOSEFONT);
			}

			if (m_fN == -1)
			{
				pFname = m_mapH->fName;
				pFstyle = &m_mapH->fStyle;
				pFpoint = &m_mapH->fPoint;
			}
			else if (m_fN == -2)
			{
				for (int jj = 0; jj < m_formItem->GetCount(); jj++)
				{
					if (m_formItem->GetForm(jj)->bCommon)
					{
						pFname = m_formItem->GetForm(jj)->fName;
						pFstyle = &m_formItem->GetForm(jj)->fStyle;
						pFpoint = &m_formItem->GetForm(jj)->fPoint;
						break;
					}
				}
			}
			else
			{
				pFname = m_formItem->GetForm(m_fN)->fName;
				pFstyle = &m_formItem->GetForm(m_fN)->fStyle;
				pFpoint = &m_formItem->GetForm(m_fN)->fPoint;
			}

			ZeroMemory(m_fontD->m_cf.lpLogFont, sizeof(LOGFONT));

			if (pFname)
				strcpy_s(m_fontD->m_cf.lpLogFont->lfFaceName, LF_FACESIZE, pFname);
			else
				strcpy_s(m_fontD->m_cf.lpLogFont->lfFaceName, LF_FACESIZE, m_mapH->fName);

			if (*pFstyle == FS_ITALIC || *pFstyle == FS_IBOLD)
				m_fontD->m_cf.lpLogFont->lfItalic = TRUE;
			else
				m_fontD->m_cf.lpLogFont->lfItalic = FALSE;

			if (*pFstyle == FS_BOLD || *pFstyle == FS_IBOLD)
				m_fontD->m_cf.lpLogFont->lfWeight = FW_BOLD;
			else
				m_fontD->m_cf.lpLogFont->lfWeight = FW_NORMAL;

			CDC*	pDC = m_pWndInPlace->GetDC();
			m_fontD->m_cf.lpLogFont->lfHeight = MulDiv(-(int)*pFpoint, pDC->GetDeviceCaps(LOGPIXELSY), 72);
			m_pWndInPlace->ReleaseDC(pDC);

			m_fontD->m_cf.lpLogFont->lfCharSet = DEFAULT_CHARSET;
			m_fontD->m_cf.hDC = 0;
			m_fontD->m_cf.Flags  = CF_ENABLEHOOK | CF_FORCEFONTEXIST | CF_SCREENFONTS;
			m_fontD->m_cf.Flags |= CF_INITTOLOGFONTSTRUCT|CF_NOVERTFONTS;
			m_fontD->m_cf.nFontType  = SCREEN_FONTTYPE;

			(int)CommDlgExtendedError();
			if (m_fontD->DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				int jj = 0;
				do
				{
					if (m_fN == -2 && !m_formItem->GetForm(jj)->bCommon)
					{
						jj++;
						if (jj >= m_formItem->GetCount())
							break;
						continue;
					}
					else if (m_fN == -2)
					{
						pFstyle = &m_formItem->GetForm(jj)->fStyle;
						pFpoint = &m_formItem->GetForm(jj)->fPoint;
					}

					*pFstyle = FS_NORM;
					if (m_fontD->IsItalic())
						*pFstyle = FS_ITALIC;
					if (m_fontD->IsBold())
						*pFstyle += FS_BOLD - FS_NORM;

					CString	text = m_fontD->GetFaceName();
					if (m_fN == -1)
					{
						int size = text.GetLength();
						CopyMemory(pFname, (char *)text.operator LPCTSTR(), size);
						pFname[size] = '\0';
					}
					else if (m_fN == -2)
						m_formItem->SetFormStr(jj, _T("fName"), text);
					else
						m_formItem->SetFormStr(m_fN, _T("fName"), text);
					*pFpoint = m_fontD->GetSize()/10;

					if (m_pWndInPlace != NULL)
						m_pWndInPlace->SetWindowText(text);
					m_varValue = (LPCTSTR)text;		// 값 가지고 있기
					jj++;
					if (jj >= m_formItem->GetCount())
						break;
				} while (m_fN == -2);
			}
		}
		break;

	case 1002:	// color Dialog
		{
			DWORD	rgb = atol((LPCTSTR)m_varValue.bstrVal);
			CRect	rc;

			m_pWndInPlace->GetClientRect(&rc);
			m_pWndInPlace->ClientToScreen(&rc);
			CColorDlg* dlg = new CColorDlg(m_parent, rgb, true, rc);
			if (dlg->DoModal() == IDOK)
			{
				CString	tmps;

				m_mapH->modified = true;
				tmps.Format(_T("%d"), dlg->m_color);
				if (m_pWndInPlace != NULL)
					m_pWndInPlace->SetWindowText(tmps);
				m_varValue = (LPCTSTR)tmps;		// 값 가지고 있기					
			}
			dlg->DestroyWindow();
			delete dlg;
		}
		break;

	case 1003:	// File Dialog
		{
			CFileDialog	dlg(TRUE, NULL, NULL,
				OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
				_T("Image Files (*.png)|*.png||"));
			if (dlg.DoModal() != IDOK)
				return;

			CString	tmps = dlg.GetFileName();
			m_mapH->modified = true;
			if (m_pWndInPlace != NULL)
				m_pWndInPlace->SetWindowText(tmps);
			m_varValue = (LPCTSTR)tmps;		// 값 가지고 있기					
		}
		break;

	case 1004:	// attributes
		if (m_mapH == NULL || m_formItem == NULL)
			break;

		int uprgb, dnrgb;

		if (m_fN >= 0 || m_fN == -2)
		{
			int	idx = m_fN;
			if (idx == -2)
			{
				for (int jj = 0; jj < m_formItem->GetCount(); jj++)
					if (m_formItem->GetForm(jj)->bCommon)
					{
						idx = jj;
						break;
					}
			}

			if (m_formItem->GetForm(idx)->kind == FM_GRID || m_formItem->GetForm(idx)->kind == FM_GRIDEX || m_formItem->GetForm(idx)->kind == FM_TABLE)
			{
				struct _repR *repR   = (struct _repR *)m_formItem->GetForm(idx)->auxR;
				CCellItem cellItem(repR);
				
				if (cellItem.GetCell(repR->selected)->attr & FA_CORGB)
				{
					uprgb = HIWORD(cellItem.GetCell(repR->selected)->onEdit);
					dnrgb = LOWORD(cellItem.GetCell(repR->selected)->onEdit);
				}
				else
				{
					long lCorgb = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCORGB);
					uprgb = LOWORD(lCorgb);
					dnrgb = HIWORD(lCorgb);
				}

				CAttrDlg attrDlg(m_parent, m_formItem->GetForm(idx)->kind, cellItem.GetCell(repR->selected)->type, cellItem.GetCell(repR->selected)->iok,
					cellItem.GetCell(repR->selected)->attr, cellItem.GetCell(repR->selected)->attr2, 
					uprgb, dnrgb);	
				if (attrDlg.DoModal() == IDOK)
				{
					m_mapH->modified = true;
					cellItem.GetCell(repR->selected)->attr = attrDlg.m_attr;
					cellItem.GetCell(repR->selected)->attr2 = attrDlg.m_attr2;
					cellItem.GetCell(repR->selected)->onEdit = MAKELONG(attrDlg.m_dnRGB, attrDlg.m_upRGB);
				}

				if (m_formItem->GetForm(idx)->kind == FM_GRID ||m_formItem->GetForm(idx)->kind == FM_GRIDEX )
					m_parent->SendMessage(WM_USER, MAKEWPARAM(1, idx));
			}
			else
			{
				if (m_formItem->GetForm(idx)->attr & FA_CORGB)
				{
					uprgb = HIWORD(m_formItem->GetForm(idx)->onEdit);
					dnrgb = LOWORD(m_formItem->GetForm(idx)->onEdit);
				}
				else
				{
					long lCorgb = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCORGB);
					uprgb = LOWORD(lCorgb);
					dnrgb = HIWORD(lCorgb);
				}

				CAttrDlg attrDlg(m_parent, m_formItem->GetForm(idx)->kind, m_formItem->GetForm(idx)->type, m_formItem->GetForm(idx)->iok, 
						m_formItem->GetForm(idx)->attr, m_formItem->GetForm(idx)->attr2, 
						uprgb, dnrgb);
				if (attrDlg.DoModal() == IDOK)
				{
					m_mapH->modified = true;
					do
					{
						if (m_fN == -2 && !m_formItem->GetForm(idx)->bCommon)
						{
							idx++;
							if (idx >= m_formItem->GetCount())
								break;
							continue;
						}
						m_formItem->GetForm(idx)->attr  = attrDlg.m_attr;
						m_formItem->GetForm(idx)->attr2 = attrDlg.m_attr2;
						m_formItem->GetForm(idx)->onEdit = MAKELONG(attrDlg.m_dnRGB, attrDlg.m_upRGB);
						idx++;
						if (idx >= m_formItem->GetCount())
							break;
					} while(m_fN == -2);
				}
			}
		}
		break;

	case 1005:	// combo str
		if (m_mapH && m_formItem)
		{
			int	idx = m_fN;

			if (idx == -2)
			{
				for (int jj = 0; jj < m_formItem->GetCount(); jj++)
				{
					if (m_formItem->GetForm(jj)->bCommon)
					{
						idx = jj;
						break;
					}
				}
			}
			CComboItem	cbItem(m_parent, LOWORD(m_formItem->GetForm(idx)->keys), m_formItem->GetForm(idx)->size);
			if (m_formItem->GetForm(idx)->dat)
				cbItem.m_disB = m_formItem->GetForm(idx)->dat;
			if (m_formItem->GetForm(idx)->str)
				cbItem.m_datB =m_formItem->GetForm(idx)->str;
			if (cbItem.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				do
				{
					if (m_fN == -2 && !m_formItem->GetForm(idx)->bCommon)
						continue;

					CString tmps;
					tmps.Format(_T("%d"), cbItem.m_count);
					if (m_pWndInPlace != NULL)
						m_pWndInPlace->SetWindowText(tmps);
					m_varValue = (LPCTSTR)tmps;		// 값 가지고 있기					

					m_formItem->GetForm(idx)->keys = MAKELONG((WORD)cbItem.m_count, (WORD)cbItem.m_count);
					m_formItem->SetFormStr(idx, _T("dat"), cbItem.m_disB);
					m_formItem->SetFormStr(idx, _T("str"), cbItem.m_datB);
					if (idx >= m_formItem->GetCount())
						break;
				} while(m_fN == -2);
			}
		}
		break;

	case 1007:	// edit format
		break;

	case 1008:	// map option dialog
		if (m_mapH)
		{
			COption dlg(m_mapH);
			if (dlg.DoModal() == IDOK)
				m_mapH->modified = true;
		}
		break;

	case 1009:	// column edit Dialog
		if (m_mapH && m_fN >= 0)
		{
			AfxGetMainWnd()->SendMessage(WM_USER,MAKEWPARAM(ID_USR_MODAL, 0), (LPARAM)TRUE);
			CGridColumn	dlg(m_parent, m_mapH, m_fN);
			
			if (dlg.DoModal() == IDOK)
				m_mapH->modified = true;
			AfxGetMainWnd()->SendMessage(WM_USER,MAKEWPARAM(ID_USR_MODAL, 0), (LPARAM)FALSE);
		}
		break;

	case 1010:	// publicDlg
		if (m_mapH)
		{
			CPublicDlg dlg;
			
			m_pWndInPlace->GetWindowText(dlg.m_publicR);
			//Dlg.m_publicR = m_mapH->publicR;
			if (dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
			//	strcpy_s(m_mapH->publicR, sizeof(m_mapH->publicR), dlg.m_publicR);
				if (m_pWndInPlace != NULL)
					m_pWndInPlace->SetWindowText(dlg.m_publicR);
				m_varValue = (LPCTSTR)dlg.m_publicR;		// 값 가지고 있기					
			}
		}
		break;

	case 1011:	// cell combo str
		if (m_mapH && m_formItem)
		{
			int	idx = m_fN;

			if (m_fN < 0 || !m_formItem->GetForm(m_fN)->auxR)
				break;
			
			struct _repR *repR = (struct _repR *)m_formItem->GetForm(m_fN)->auxR;
			CCellItem cellItem(repR);
		//	if (repR->selected < 0)
		//		repR->selected = m_InputPnt.x;

			_cellR	*pCell = cellItem.GetCell(repR->selected);
			if (pCell->kind != CK_COMBO)
				break;

			CComboItem	cbItem(m_parent, pCell->keys, pCell->size);
			if (pCell->dat)
				cbItem.m_disB = pCell->dat;
			if (pCell->str)
				cbItem.m_datB =pCell->str;
			if (cbItem.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				
				CString tmps;
				tmps.Format(_T("%d"), cbItem.m_count);
				if (m_pWndInPlace != NULL)
					m_pWndInPlace->SetWindowText(tmps);
				m_varValue = (LPCTSTR)tmps;

				pCell->keys = cbItem.m_count;
				strcpy_s(pCell->dat, sizeof(pCell->dat), cbItem.m_disB);
				strcpy_s(pCell->str, sizeof(pCell->str), cbItem.m_datB);
			}
		}
		break;

	case 1012:	// control setup dialog
		break;

	case 1013:	// utabpage
		if (m_mapH)
		{
			CTabPage	dlg(m_parent, m_mapH, m_fN);
			dlg.DoModal();
		}
		break;

	case 1015:	// edit Format
		break;

	case 1016:	// CoRGB
		break;

	case 1101:	//	control-defined property Dialog
		if (!m_infos.IsEmpty())
		{
			HINSTANCE handle;
			CString	tmps, mtbl, path = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);

			if (!path.IsEmpty() && path.GetAt(path.GetLength() - 1) != _T('\\'))
				path += _T("\\");

			mtbl = path;
			mtbl += _T("mtbl\\");

			path += _T("dev\\");
			SetCurrentDirectory(path);
			path += m_infos;
			path += _T(".dll");

			m_pWndInPlace->GetWindowText(tmps);
			handle = LoadLibrary(path);
			if (handle)
			{
				char*	(APIENTRY *axPropDlg)(const char*, const char*);
				axPropDlg = (char* (APIENTRY*)(const char*, const char*))GetProcAddress(handle, _T("axPropDlg"));
				if (axPropDlg != NULL)
				{
					CString	data = CString(axPropDlg(tmps.operator LPCTSTR(), mtbl.operator LPCTSTR()));
					if (m_pWndInPlace != NULL)
						m_pWndInPlace->SetWindowText(data);
					m_varValue = (LPCTSTR)data;
				}
				
				FreeLibrary(handle);
			}
		}
		break;
	}

/*	{
		CString strValue;

		// Show Dlg : 직접 만든 Dlg를 띄워서 정보값을 얻어온다.
		CDlgCostumeSelection dlgCostumeSel;
		if (dlgCostumeSel.DoModal() == IDOK)
		{
			strValue = dlgCostumeSel.GetSelValue();		// 값 얻기

			if (m_pWndInPlace != NULL)
				m_pWndInPlace->SetWindowText(strValue);	// 값 보여주기

			m_varValue = (LPCTSTR)strValue;		// 값 가지고 있기
		}
	}
*/
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_fN);

	m_bButtonIsDown = FALSE;
	Redraw();
	if (m_type == 1001)	// font
		m_parent->PostMessage(WM_USER, MAKEWPARAM(2, 0));
}

void CPropertyGridButtonProperty::OnDrawValue(CDC* pDC, CRect rect)
{
	if (m_type == 1002)	// color
	{
		COLORREF tClr, bClr;
		DWORD	clr = atoi(CString(m_varValue.bstrVal));
		COLORREF rgb = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, clr);
		pDC->FillSolidRect(&rect, rgb);

		if ((GetRValue(rgb) + GetGValue(rgb) + GetBValue(rgb)) / 3 >= 192)
			tClr = pDC->SetTextColor(RGB(0, 0, 0));
		else
			tClr = pDC->SetTextColor(RGB(255, 255, 255));
		bClr = pDC->SetBkColor(rgb);
		CMFCPropertyGridProperty::OnDrawValue(pDC, rect);

		pDC->SetTextColor(tClr);
		pDC->SetBkColor(bClr);
		return;
	}

	if (m_full)
	{
		AdjustButtonRect();
	}

	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
}