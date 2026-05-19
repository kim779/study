// PropertyGridButtonProperty.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PropertyGridButtonProperty.h"

#include "attrDlg.h"
#include "cellEdit.h"
#include "comboItem.h"
#include "colorDlg.h"
#include "coRGB.h"
#include "controlDlg.h"
#include "format.h"
#include "gridColumn.h"
#include "option.h"
#include "publicDlg.h"
#include "tabPage.h"

#include "../h/mainvar.h"

#include "../h/mapform.H"
#include "../awTool/awObjectLoad.h"
#include "../../dll/lib/axislib.h"
#include "../h/fmObject.h"



#define	AFX_PROP_HAS_BUTTON	0x0002

// CPropertyGridButtonProperty

IMPLEMENT_DYNAMIC(CPropertyGridButtonProperty, CMFCPropertyGridCtrl)

CPropertyGridButtonProperty::CPropertyGridButtonProperty(bool full, int type, const CString& strName, const _variant_t& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData)
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
	m_infos = _T("");
}

CPropertyGridButtonProperty::~CPropertyGridButtonProperty()
{
}

void CPropertyGridButtonProperty::SetInfo(CWnd* parent, int fN, class mapForm* mapForm, CString infos)
{
	m_parent = parent;
	m_fN = fN;
	m_mapH = mapForm;
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
		if (!m_mapH->forms.IsEmpty())
		{
			CString	pFname = NULL;
			WORD	*pFstyle = NULL;
			WORD	*pFpoint = NULL;
			CFontDialog	fontD;
			if (m_fN == -1)
			{
				pFname = m_mapH->fName;
				pFstyle = &m_mapH->fStyle;
				pFpoint = &m_mapH->fPoint;
			}
			else if (m_fN == -2)
			{
				for (int jj = 0; jj < m_mapH->forms.GetCount(); jj++)
				{
					CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(jj))->getProp();
					if (form->bCommon)
					{
						pFname = form->fontName;
						pFstyle = (WORD*)&form->fontStyle;
						pFpoint = (WORD*)&form->fontPoint;
						break;
					}
				}
			}
			else
			{
				CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(m_fN))->getProp();
				pFname = form->fontName;
				pFstyle = (WORD*)&form->fontStyle;
				pFpoint = (WORD*)&form->fontPoint;
			}

			fontD.m_cf.lStructSize = sizeof(CHOOSEFONT);
		
			ZeroMemory(fontD.m_cf.lpLogFont, sizeof(LOGFONT));
			if (pFname)
				strcpy_s(fontD.m_cf.lpLogFont->lfFaceName, LF_FACESIZE, pFname);
			else
				strcpy_s(fontD.m_cf.lpLogFont->lfFaceName, LF_FACESIZE, m_mapH->fName);
			
			if (*pFstyle == fsITALIC || *pFstyle == fsIBOLD)
				fontD.m_cf.lpLogFont->lfItalic = TRUE;
			else
				fontD.m_cf.lpLogFont->lfItalic = FALSE;
			
			if (*pFstyle == fsBOLD || *pFstyle == fsIBOLD)
				fontD.m_cf.lpLogFont->lfWeight = FW_BOLD;
			else
				fontD.m_cf.lpLogFont->lfWeight = FW_NORMAL;

			CDC*	pDC = m_pWndInPlace->GetDC();
			fontD.m_cf.lpLogFont->lfHeight = MulDiv(-(int)*pFpoint, pDC->GetDeviceCaps(LOGPIXELSY), 72);
			m_pWndInPlace->ReleaseDC(pDC);

			fontD.m_cf.lpLogFont->lfCharSet = DEFAULT_CHARSET;
			fontD.m_cf.hDC = 0;
			fontD.m_cf.Flags  = CF_ENABLEHOOK | CF_FORCEFONTEXIST | CF_SCREENFONTS;
			fontD.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT|CF_NOVERTFONTS;
			fontD.m_cf.nFontType  = SCREEN_FONTTYPE;

			if (fontD.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				int jj = 0;
				do
				{
					CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(jj))->getProp();
					CfmObject* Obj = ((CObjectLoad*)m_mapH->forms.GetAt(jj))->getObject();
					if (m_fN == -2 && !form->bCommon)
					{
						jj++;
						if (jj >= m_mapH->forms.GetCount())
							break;
						continue;
					}
					else if (m_fN == -2)
					{
						pFstyle = (WORD*)&form->fontStyle;
						pFpoint = (WORD*)&form->fontPoint;
					}

					*pFstyle = fsNORMAL;
					if (fontD.IsItalic())
						*pFstyle = fsITALIC;
					if (fontD.IsBold())
						*pFstyle += fsBOLD - fsNORMAL;
					
					CString	text = fontD.GetFaceName();
					if (m_fN == -1)
					{
						int	size = text.GetLength();
						pFname = text;
						//pFname[size] = '\0';
					}
					else if (m_fN == -2)
						form->fontName = text;
					else
					{
						form = ((CObjectLoad*)m_mapH->forms.GetAt(m_fN))->getProp();
						Obj = ((CObjectLoad*)m_mapH->forms.GetAt(m_fN))->getObject();
						form->fontName = text;
					}
					*pFpoint = fontD.GetSize()/10;
					
					if (m_pWndInPlace != NULL)
						m_pWndInPlace->SetWindowText(text);
					m_varValue = (LPCTSTR)text;		// 값 가지고 있기
					jj++;

					Obj->Reload();
					Obj->SetFont(form->fontPoint, form->fontStyle, form->fontName, 0);

					if (jj >= m_mapH->forms.GetCount())
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
				_T("Bitmap Files (*.bmp)|*.bmp|Gif Files (*.gif)|*.gif||"));
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
		if (m_mapH == NULL)
			break;
		if (m_mapH->forms.IsEmpty())
			break;

		if (m_fN >= 0 || m_fN == -2)
		{
			int	idx = m_fN;
			if (idx == -2)
			{
				for (int jj = 0; jj < m_mapH->forms.GetCount(); jj++)
				{
					CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(jj))->getProp();
					if (form->bCommon)
					{
						idx = jj;
						break;
					}
				}
			}

			if (((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->kind == fmGRID 
				|| ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->kind == fmGRIDEX
				|| ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->kind == fmTABLE)
			{

				
				CAttrDlg attrDlg(m_parent, ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->kind,
					((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->charType,
					((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->ioType,
					((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->attributes,
					((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->auxAttributes);
				if (attrDlg.DoModal() == IDOK)
				{
					CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();
					m_mapH->modified = true;
					((cellOb*)form->cellProperties->GetAt(form->CellSelected))->attributes = attrDlg.m_attributes;
					((cellOb*)form->cellProperties->GetAt(form->CellSelected))->auxAttributes = attrDlg.m_auxAttributes;
				}

				if (((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->kind == fmGRID)
					m_parent->SendMessage(WM_USER, MAKEWPARAM(1, idx));
			}
			else
			{
				CAttrDlg attrDlg(m_parent, ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->kind, 
					((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->charType,
					((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->ioType,
					((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->attributes,
					((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->auxAttributes);
				if (attrDlg.DoModal() == IDOK)
				{
					m_mapH->modified = true;
					do
					{
						CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();
						if (m_fN == -2 && !form->bCommon)
						{
							idx++;
							if (idx >= m_mapH->forms.GetCount())
								break;
							continue;
						}
						form->attributes = attrDlg.m_attributes;
						form->auxAttributes = attrDlg.m_auxAttributes;
						idx++;
						if (idx >= m_mapH->forms.GetCount())
							break;
					} while(m_fN == -2);
				}
			}
		}
		break;

	case 1005:	// combo str
		if (m_mapH && !m_mapH->forms.IsEmpty())
		{
			int	idx = m_fN;

			if (idx == -2)
			{
				for (int jj = 0; jj < m_mapH->forms.GetCount(); jj++)
				{
					CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(jj);
					CformProp*	Prop = ObjLoad->getProp();
					if (Prop->bCommon)
					{
						idx = jj;
						break;
					}
				}
			}
			CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
			CfmObject*	Obj = ObjLoad->getObject();
			CformProp*	Prop = ObjLoad->getProp();

			CWnd*		Wnd = ObjLoad->getWnd();
			CComboItem	cbItem(m_parent, LOWORD(Prop->keyValues), Prop->size);
			if (!Prop->displays.IsEmpty())
				cbItem.m_disB = Prop->displays;
			if (!Prop->strings.IsEmpty())
				cbItem.m_datB =Prop->strings;
			if (cbItem.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				do
				{
					//if (m_fN == -2 && !Prop->bCommon)
					//	continue;

					CString tmps;
					tmps.Format(_T("%d"), cbItem.m_count);
					if (m_pWndInPlace != NULL)
						m_pWndInPlace->SetWindowText(tmps);
					m_varValue = (LPCTSTR)tmps;		// 값 가지고 있기					

					Prop->keyValues = MAKELONG((WORD)cbItem.m_count, (WORD)cbItem.m_count);
					Prop->displays = cbItem.m_disB;
					Prop->strings = cbItem.m_datB;
					if (idx >= m_mapH->forms.GetCount())
						break;
				} while(m_fN == -2);
			}
		}
		break;

	case 1007:	// edit format
		if (m_mapH)
		{
			CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_fN);
			CformProp*	Prop = ObjLoad->getProp();

			if (Prop->cellProperties == NULL || Prop->cellProperties->IsEmpty()) break;

			cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);
			CCellEdit cellEdit(m_parent, fmGRID, cellProp->charType,
				cellProp->whatEdit,
				cellProp->auxValues,
				cellProp->editFormat,
				cellProp->name,
				cellProp->attributes,   
				cellProp->size);
			if (cellEdit.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				cellProp->attributes = cellEdit.m_attributes;
				cellProp->whatEdit = cellEdit.m_whatEdit;
				cellProp->auxValues = cellEdit.m_auxValues;
				cellProp->editFormat = cellEdit.m_editFormat;
			}
		}
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
			dlg.DoModal();
			AfxGetMainWnd()->SendMessage(WM_USER,MAKEWPARAM(ID_USR_MODAL, 0), (LPARAM)FALSE);
			m_mapH->modified = true;
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
		if (m_mapH)
		{
			int	idx = m_fN;

			CformProp* Prop = ((CObjectLoad*)m_mapH->forms.GetAt(m_fN))->getProp();

			if (m_fN < 0 || Prop->cellProperties == NULL || Prop->cellProperties->IsEmpty())
				break;

			cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);
			if (cellProp->kind != ckCOMBO)
				break;

			CComboItem	cbItem(m_parent, cellProp->entryCombo, cellProp->size);
			if (cellProp->displays)
				cbItem.m_disB = cellProp->displays;
			if (cellProp->strings)
				cbItem.m_datB = cellProp->strings;
			if (cbItem.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				
				CString tmps;
				tmps.Format(_T("%d"), cbItem.m_count);
				if (m_pWndInPlace != NULL)
					m_pWndInPlace->SetWindowText(tmps);
				m_varValue = (LPCTSTR)tmps;

				cellProp->entryCombo = cbItem.m_count;
				cellProp->displays = cbItem.m_disB;
				cellProp->strings = cbItem.m_datB;
			}
		}
		break;

	case 1012:	// control setup dialog
		//if (m_mapH && m_formItem)
		//{
		//	CControlDlg dlg(m_fN);
		//	
		//	m_pWndInPlace->GetWindowText(dlg.m_ctrlName);
		//	dlg.m_ctrlParam = m_formItem->GetFormStr(m_fN, _T("str2"));
		//	if (dlg.DoModal() == IDOK)
		//	{
		//		m_mapH->modified = true;
		//		m_formItem->SetFormStr(m_fN, _T("str"), dlg.m_ctrlName);
		//		m_formItem->SetFormStr(m_fN, _T("str2"), dlg.m_ctrlParam);

		//		if (m_pWndInPlace != NULL)
		//			m_pWndInPlace->SetWindowText(dlg.m_ctrlName);
		//		m_varValue = (LPCTSTR)dlg.m_ctrlName;
		//		AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, 0); // control event refresh
		//	}
		//}
		break;

	case 1013:	// utabpage
		if (m_mapH)
		{
			CTabPage	dlg(m_parent, m_mapH, m_fN);
			dlg.DoModal();
		}
		break;

	case 1015:	// edit Format
		if (m_mapH == NULL)
			break;
		if (m_mapH->forms.IsEmpty())
			break;

		if (m_fN >= 0 || m_fN == -2)
		{
			int	idx = m_fN;

			if (idx == -2)
			{
				for (int jj = 0; jj < m_mapH->forms.GetCount(); jj++)
				{
					CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(jj);
					CformProp*	Prop = (CformProp*)ObjLoad->getProp();
					if (Prop->bCommon)
					{
						idx = jj;
						break;
					}
				}
			}

			CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
			CformProp*	Prop = (CformProp*)ObjLoad->getProp();

			if (!((Prop->kind == fmEDIT || Prop->kind == fmOUTPUT || Prop->kind == fmCOMBO)
				&& (Prop->charType == ctNUMERIC || Prop->charType == ctALPHA)
				&& (Prop->whatEdit == weIT || Prop->whatEdit == weNOP)))
			{
				break;
			}
			CString name = Prop->name, editS = Prop->editFormat;
			CFormat*	format;
			format = new CFormat(m_parent, Prop->whatEdit, editS,
				Prop->attributes, name, Prop->size);
			if (format->DoModal() == IDOK)
			{
				m_mapH->modified = true;
				do
				{
					if (m_fN == -2 && !Prop->bCommon)
					{
						idx++;
						if (idx >= m_mapH->forms.GetCount())
							break;
						continue;
					}

					if (!((Prop->kind == fmEDIT || Prop->kind == fmOUTPUT || Prop->kind == fmCOMBO)
						&& (Prop->charType == ctNUMERIC || Prop->charType == ctALPHA)
						&& (Prop->whatEdit == weIT || Prop->whatEdit == weNOP)))
					{
						idx++;
						if (idx >= m_mapH->forms.GetCount())
							break;
						continue;
					}

					Prop->attributes = format->m_attributes;
					Prop->size   = format->m_size;
					Prop->whatEdit = format->m_whatEdit;
					Prop->editFormat = format->m_edit;
					idx++;
					if (m_mapH->forms.GetCount())
						break;
				} while(m_fN == -2);
			}
			delete format;
		}
		break;

	case 1016:	// CoRGB
		if (m_mapH == NULL)
			break;
		if (m_mapH->forms.IsEmpty())
			break;

		if (m_fN >= 0 || m_fN == -2)
		{
			int	idx = m_fN;
			if (idx == -2)
			{
				for (int jj = 0; jj < m_mapH->forms.GetCount(); jj++)
				{
					CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(jj);
					CformProp*	Prop = (CformProp*)ObjLoad->getProp();
					if (Prop->bCommon)
					{
						idx = jj;
						break;
					}
				}
			}

			CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
			CformProp*	Prop = (CformProp*)ObjLoad->getProp();

			if (!((Prop->kind == fmOUTPUT
				|| (Prop->kind == fmEDIT && Prop->ioType == ioBOTH))
				&& (Prop->whatEdit == weCORGB || Prop->whatEdit == weNOP)))
			{
				break;
			}
			CCoRGB*		coRGB;
			if (Prop->whatEdit == weCORGB)
			{
				coRGB = new CCoRGB(m_parent, Prop->name, Prop->whatEdit,
						HIWORD(Prop->auxValues), LOWORD(Prop->auxValues));
			}
			else
			{
				long lCorgb = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCORGB);
				coRGB = new CCoRGB(m_parent, Prop->name, 
					Prop->whatEdit, LOWORD(lCorgb), HIWORD(lCorgb));
			}
			
			if (coRGB->DoModal() == IDOK)
			{
				m_mapH->modified = true;
				do
				{
					if (m_fN == -2 && !Prop->bCommon)
					{
						idx++;
						if (idx >= m_mapH->forms.GetCount())
							break;
						continue;
					}

					if (!((Prop->kind == fmOUTPUT 
						|| (Prop->kind == fmEDIT && Prop->ioType == ioBOTH))
						&& (Prop->whatEdit == weCORGB || Prop->whatEdit == weNOP)))
					{
						idx++;
						if (idx >= m_mapH->forms.GetCount())
							break;
						continue;
					}
						
					Prop->whatEdit = coRGB->m_whatEdit;
					if (Prop->whatEdit == weCORGB)
					{
						Prop->auxValues = MAKELONG(coRGB->m_dnRGB, coRGB->m_upRGB);
						Prop->attributes   |= faCORGB;
						Prop->auxAttributes |= ccRAW;
					}
					else
					{
						Prop->auxValues = 0;
						Prop->attributes &= ~faCORGB;
						Prop->auxAttributes &= ~ccRAW;
					}
					idx++;
					if (idx >= m_mapH->forms.GetCount())
						break;
				} while(m_fN == -2);
			}
			delete coRGB;
		}
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
	case 1017:
	{
		char szFilter[] = "All Files(*.*)|*.*||";

		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

		if (IDOK == dlg.DoModal()) {
			CString strPathName = dlg.GetPathName();
			m_mapH->modified = true;

			int pos = strPathName.ReverseFind('\\');
			CString path = strPathName.Left(pos);
			CString name = strPathName.Mid(pos + 1);
			if (m_pWndInPlace != NULL)
			{
				m_pWndInPlace->SetWindowText(name);
			}
			CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_fN);
			CformProp*	Prop = (CformProp*)ObjLoad->getProp();
			Prop->keyNames = path;
			m_varValue = (LPCTSTR)name;		// 값 가지고 있기
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
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
}