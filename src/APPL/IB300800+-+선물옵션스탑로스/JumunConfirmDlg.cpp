// JumunConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB300800.h"
#include "JumunConfirmDlg.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString m_strLblList[] = {"계좌번호", "계  좌  명",  "종       목", "구       분", "신용구분", "수       량", "단       가"};
/////////////////////////////////////////////////////////////////////////////
// CJumunConfirmDlg dialog


CJumunConfirmDlg::CJumunConfirmDlg(int iType, CFont* pFont, COLORREF* colorlist, COLORREF txtcolor, COLORREF bgcolor, 
								   COLORREF* dangacolor, bool bcrttype, CWnd* pParent /*=NULL*/)
	: CDialog(CJumunConfirmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJumunConfirmDlg)
	//}}AFX_DATA_INIT
	m_iType = iType;
	m_pParent = pParent;
	for(int i=0; i<4; i++)
		m_colorList[i] = colorlist[i];

	m_pFont = pFont;
	m_txtcolor = txtcolor;
	m_backcolor = bgcolor;

	for(int i=0; i<2; i++)
		m_dangaColor[i] = dangacolor[i];

	LOGFONT lf = GetLogFont(12, true);
	m_pBoldFont.CreateFontIndirect(&lf);
	m_bCrtType = bcrttype;
}


void CJumunConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJumunConfirmDlg)
	DDX_Control(pDX, IDC_BUTTONOK, m_OkBtn);
	DDX_Control(pDX, IDC_BUTTONCANCEL, m_CancelBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJumunConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(CJumunConfirmDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTONOK, OnButtonok)
	ON_BN_CLICKED(IDC_BUTTONCANCEL, OnButtoncancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJumunConfirmDlg message handlers

BOOL CJumunConfirmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_pParent)
	{
		HBITMAP hbitmap[3];
		hbitmap[0] = ((CMainWnd*)m_pParent)->getBitmap("2BTN.bmp");
		hbitmap[1] = ((CMainWnd*)m_pParent)->getBitmap("2BTN_DN.bmp");
		hbitmap[2] = ((CMainWnd*)m_pParent)->getBitmap("2BTN_EN.bmp");
		m_OkBtn.SetFont(m_pFont);
		m_OkBtn.SetImgBitmap(hbitmap[0], hbitmap[1], hbitmap[2]);

		m_CancelBtn.SetFont(m_pFont);
		m_CancelBtn.SetImgBitmap(hbitmap[0], hbitmap[1], hbitmap[2]);
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJumunConfirmDlg::SetValue(CString sAccount, CString sAccnName, CString sCodeName, CString sGubn, CString sCrtText, CString sVol, CString sPrice)
{
	CString s;
	s.Format("SETVALUE : [%s]	[%s]\n",sVol,sPrice);
	OutputDebugString(s);
	m_strValue[0] = sAccount;
	m_strValue[1] = sAccnName;

	int iFind = -1;
	if( (iFind=sCodeName.Find("&")) > 0)  sCodeName.Insert(iFind, "&"); 
	m_strValue[2] = sCodeName;
	m_strValue[3] = sGubn;
	m_strValue[4] = sCrtText;
	m_strValue[5] = sVol;
	m_strValue[6] = sPrice;

	s.Format("SETVALUE 2 : [%s]		[%s]\n",m_strValue[5],m_strValue[6]);
	OutputDebugString(s);
}

void CJumunConfirmDlg::OnPaint() 
{
	if(!m_pParent) return;

	CPaintDC dc(this); // device context for painting
	CRect wndrect;
	GetClientRect(&wndrect);
	dc.FillSolidRect(wndrect, m_backcolor); 
	wndrect.DeflateRect(2,2,2,2); 
	dc.FillSolidRect(wndrect, ((CMainWnd*)m_pParent)->m_bodycolor); 

	
	
	// TODO: Add your message handler code here
	((CMainWnd*)m_pParent)->DrawCornerRoundBitmap(&dc, wndrect);

	CFont* pOldfont= dc.SelectObject(m_pFont); 
	CPen pen(PS_SOLID, 1, m_colorList[1] );
	CPen* pOldPen = dc.SelectObject(&pen); 
	CBrush brush(m_colorList[2] );
	CBrush* pOldBrush = dc.SelectObject(&brush); 
//Lable1
	int iboxw = 240;
	int ilblwidth= 98;
	int ihgap = 2;
	int ih= 19;
	int irow = 4;
	if(m_bCrtType) irow = 5;

	CRect rect(4, 30, ilblwidth, 115);
	CRect rc1 =rect;
	rc1.bottom = rect.top + ih* irow;
	dc.Rectangle(rc1); 
	rc1.top = rc1.bottom + ihgap;
	rc1.bottom = rc1.top + ih* 2;
	dc.Rectangle(rc1); 
	dc.SelectObject(pOldBrush); 
	DeleteObject(pOldBrush);

//Lable2
	CBrush brush1(m_colorList[3] );
	pOldBrush = dc.SelectObject(&brush1); 

	rc1 = rect;
	int ilblw = rc1.left = rect.right;
	rc1.bottom = rect.top + ih* irow;
	rc1.right = iboxw;
	dc.Rectangle(rc1); 
	int idivtop = rc1.top = rc1.bottom + ihgap;
	rc1.bottom = rc1.top + ih* 2;
	dc.Rectangle(rc1); 
	dc.SelectObject(pOldBrush); 
	DeleteObject(pOldBrush);
/////

	dc.SetTextColor(m_txtcolor); 
	dc.SetBkMode(TRANSPARENT); 
	rc1 = rect;
	rc1.bottom = rect.top + ih* irow;
	rc1.right = iboxw;
	int itop = rc1.top + ih;
	CRect trect = rc1, trect2=rc1;
	trect2.left = trect.right = ilblw;
	trect2.right = iboxw-4;
	trect2.left += 4;

	for(int i=0; i<irow; i++)
	{
		trect2.bottom = trect.bottom = itop;
		dc.DrawText(m_strLblList[i], trect, DT_CENTER|DT_SINGLELINE|DT_VCENTER); 
		dc.DrawText(m_strValue[i], trect2, DT_LEFT|DT_SINGLELINE|DT_VCENTER); 

		if(i >= (irow-1)) break;
		dc.MoveTo(rc1.left, itop);  
		dc.LineTo(rc1.right, itop);
		trect2.top = trect.top = itop+1;

		itop +=  ih;
	}

	int ileft = ilblw;
	dc.MoveTo(ileft, rc1.top);  
	dc.LineTo(ileft, rc1.bottom);

	itop = idivtop + ih;
	trect2.top = trect.top = idivtop;
	trect2.left += 4;

	for(int i=5; i<7; i++)
	{
		trect2.bottom = trect.bottom = itop;
		dc.DrawText(m_strLblList[i], trect, DT_CENTER|DT_SINGLELINE|DT_VCENTER); 
		dc.DrawText(m_strValue[i], trect2, DT_RIGHT|DT_SINGLELINE|DT_VCENTER); 

		if(i > 5 ) 
		{
			//단가
			trect = CRect(ilblwidth, trect.top, iboxw, trect.bottom);
			trect.DeflateRect(1,1,1,1); 
			dc.FillSolidRect(trect, m_dangaColor[0]); 
			trect2 = trect;
			trect2.DeflateRect(0,1,3,0); 
			
			dc.SelectObject( pOldfont);
			pOldfont= dc.SelectObject(&m_pBoldFont); 

			
			dc.SetTextColor( m_dangaColor[1]);
			dc.DrawText(m_strValue[6], trect2, DT_RIGHT|DT_SINGLELINE|DT_VCENTER); 
			break;
		}

		dc.MoveTo(rc1.left, itop);  
		dc.LineTo(rc1.right, itop);

		trect2.top = trect.top = itop;
		itop +=  ih;
	}


	trect = CRect(4, wndrect.bottom - 50, iboxw, wndrect.bottom-25);
	dc.SetTextColor( m_txtcolor);
	dc.DrawText("주문처리하시겠습니까?", trect, DT_LEFT|DT_SINGLELINE|DT_VCENTER); 

	CString sText = "※ ";
	if(m_bCrtType) sText += "신용";

	if(!m_iType) sText += "매도주문확인창";
	else sText += "매수주문확인창";

	trect = CRect(4, 3, iboxw, 30);
	dc.DrawText(sText, trect, DT_LEFT|DT_SINGLELINE|DT_VCENTER); 

	dc.SelectObject(pOldPen); 
	DeleteObject(pOldPen);

	dc.SelectObject( pOldfont);
	DeleteObject(pOldfont);
}

void CJumunConfirmDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CJumunConfirmDlg::OnButtonok() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK(); 
	
}

void CJumunConfirmDlg::OnButtoncancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel(); 
	
}
