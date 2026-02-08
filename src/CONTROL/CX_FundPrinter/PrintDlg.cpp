// PrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FundPrinter.h"
#include "PrintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg dialog


CPrintDlg::CPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrintDlg::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CPrintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_tokenSize = 0;
	m_printFlag = FALSE;
}


void CPrintDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnPrintButton)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE,OnManage)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPrintDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CPrintDlg)
	DISP_FUNCTION(CPrintDlg, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CPrintDlg, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IPrintDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {31F284BA-8B08-4B11-BBB3-8AE86F6A0672}
static const IID IID_IPrintDlg =
{ 0x31f284ba, 0x8b08, 0x4b11, { 0xbb, 0xb3, 0x8a, 0xe8, 0x6f, 0x6a, 0x6, 0x72 } };

BEGIN_INTERFACE_MAP(CPrintDlg, CDialog)
	INTERFACE_PART(CPrintDlg, IID_IPrintDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg message handlers

void CPrintDlg::OnPrintButton() 
{
	StartPrint();
	
}

int CPrintDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	this->SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1)),NULL);
	
	return 0;
}

HBRUSH CPrintDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return m_brush;
}

BOOL CPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_iXP = 1;
	m_iYP = 1;

	m_iXD = 1;
	m_iYD = 1;

	m_dpiRatio = 1;

	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	MoveWindow(0,0,700,900);
	
	CButton* btnPrint = (CButton*)GetDlgItem(IDC_BUTTON1);

	btnPrint->MoveWindow(550,20,80,30);
// 	CRect rect(23,7,76,18);
// 
// 	m_bitmap_nm = HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), imgPath + "4BTN.bmp", IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
// 	m_bitmap_dn = getAxBitmap(imgPath + "4BTN_dn.bmp")->operator HBITMAP();
// 	m_bitmap_hv = getAxBitmap(imgPath + "4BTN_en.bmp")->operator HBITMAP();
// 
// 	m_btnPrint = new CfxImgButton;
// 	m_btnPrint->Create("출력하기", rc, this, IDC_PRINT_BTN);
// 	m_btnPrint->SetFont(m_font, false);  
// 	m_btnPrint->SetImgBitmap(m_map2_nm, m_map2_dn, m_map2_hv);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrintDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//Graphics graphics(pDC->m_hDC);
	
	DrawStuff(&dc,1);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CPrintDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( !CDialog::PreCreateWindow(cs) )
        return FALSE;

	cs.style &= ~WS_THICKFRAME;
	
	return TRUE;
}

// LONG CPrintDlg::OnManage(WPARAM wParam,LPARAM lParam)
// {
// 	switch(LOWORD(wParam))
// 	{
// 		case MK_SETDATA:
// 			CString str = (char*)lParam;
// 
// 			char tab = P_TAB;
// 			char lf = P_NEW;
// 			
// 			m_tokenSize = 0;
// 
// 			vector<CString> vData = GetSplit(str,_T(' '));
// 			
// 			//메시지 형식으로 데이타를 전달 받을 경우 문자배열 방식으로 변경 필요
// 
// 			break;
// 	}
// 
// 	return 0;
// }

void CPrintDlg::GetTokenSplit(CString sText, TCHAR sSplit) 
{
	 //m_printData.clear();
	 CString sResult;

	 if(sText != "")
	 {
		 // afxwin.h을 이용하여 AfxExtractSubString함수를 이용한다.
		 while(AfxExtractSubString(sResult, sText, m_tokenSize, sSplit)) {
			if(sResult.GetLength() > 0)
			{
				sResult.TrimLeft();
				sResult.TrimRight();
				
				if(sResult == "")
				{
					sResult = " ";
				}
				
				m_printData[m_tokenSize] = sResult;
			}

			m_tokenSize += 1;
		 }
	 }
}


CString CPrintDlg::parseX(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CPrintDlg::DrawStuff(CDC* pDC, double dpiRatio)
{
	CFont* pOldFont;
	// setting stuff to get nicer results...

	//////////////////////////////////////////////////////////////////////////
	// drawing text
	try{
		DrawTitle(pDC,dpiRatio);

		CFont font;
		VERIFY(font.CreateFont(
		   MulDiv(16,m_iXP,m_iXD),                        // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_BOLD,                 // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   "굴림체"));                 // lpszFacename

		pOldFont = (CFont*)pDC->SelectObject(&font);

		CRect r(MulDiv(60,m_iXP,m_iXD),MulDiv(164,m_iXP,m_iXD),MulDiv(160,m_iXP,m_iXD),MulDiv(184,m_iXP,m_iXD));
		
		pDC->DrawText(_T("1.[이동정보]"), -1, r, DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);

		DeleteObject(&font);

		DrawBox(pDC,60,186,200,206,_T("이동예정판매사"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,186,340,206,m_printData[0],FALSE,FW_NORMAL);

		CString date = m_printData[1];

		date.Insert(4,_T("년"));
		date.Insert(8,_T("월"));
		date.Insert(12,_T("일"));

		DrawBox(pDC,340,186,420,206,_T("이동 신청일"),FALSE,FW_NORMAL);

		DrawBox(pDC,420,186,640,206,date,FALSE,FW_NORMAL);

		DrawBox(pDC,60,206,200,226,_T("펀드종목코드"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,206,340,226,m_printData[2],FALSE,FW_NORMAL);

		DrawBox(pDC,340,206,420,226,_T("펀드명"),FALSE,FW_NORMAL);

		DrawBox(pDC,420,206,640,226,m_printData[3],FALSE,FW_NORMAL);

		//////////////2.///////////////////////////////

		pOldFont = (CFont*)pDC->SelectObject(&font);

		CRect r2(MulDiv(60,m_iXP,m_iXD),MulDiv(246,m_iXP,m_iXD),MulDiv(160,m_iXP,m_iXD),MulDiv(266,m_iXP,m_iXD));
		
		pDC->DrawText(_T("2.[계약정보]"), -1, r2, DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);

		DeleteObject(&font);

		DrawBox(pDC,60,268,200,288,_T("계좌번호"),FALSE,FW_NORMAL);

		CString account = m_printData[4];

		account.Insert(3,"-");
		account.Insert(6,"-");
		account.Insert(13,"-");

		DrawBox(pDC,200,268,340,288,account,FALSE,FW_NORMAL);

		DrawBox(pDC,340,268,480,288,_T("고객명(계좌명)"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,268,640,288,m_printData[5],FALSE,FW_NORMAL);

		CString societynum = m_printData[6];

		societynum.Insert(7,"-");

		DrawBox(pDC,60,288,200,308,_T("주민번호(사업자번호)"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,288,340,308,societynum,FALSE,FW_NORMAL);

		CString much = AddComma(m_printData[7],',');

		//NUMBERFMT nFmt = { 0, 0, 3, ".", ",", 0 };

		//char ctmp[100];
		
		//::GetNumberFormat(NULL, NULL, m_printData[7], &nFmt, ctmp, strlen(ctmp)); 
		//much.Format("%s",ctmp);
	
		DrawBox(pDC,340,288,480,308,_T("수량(좌)"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,288,640,308,much + _T(" 좌"),FALSE,FW_NORMAL);

		
		CString opendate = m_printData[8];

		opendate.Insert(4,_T("년"));
		opendate.Insert(8,_T("월"));
		opendate.Insert(12,_T("일"));

		DrawBox(pDC,60,308,200,328,_T("개설일(최초만기일)"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,308,340,328,opendate,FALSE,FW_NORMAL);

		DrawBox(pDC,340,308,480,328,_T("세부식별코드"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,308,640,328,m_printData[9],FALSE,FW_NORMAL);

		CString closedate = m_printData[10];

		closedate.TrimLeft();

		if(closedate != "")
		{
			closedate.Insert(4,_T("년"));
			closedate.Insert(8,_T("월"));
			closedate.Insert(12,_T("일"));
		}

		DrawBox(pDC,60,328,200,348,_T("계약만기일"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,328,340,348,closedate,FALSE,FW_NORMAL);

		DrawBox(pDC,340,328,480,348,_T("납입방법"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,328,640,348,m_printData[11],FALSE,FW_NORMAL);


		DrawBox(pDC,60,348,200,368,_T("세금유형"),FALSE,FW_NORMAL);
		DrawBox(pDC,200,348,640,368,m_printData[12],FALSE,FW_NORMAL);
	
		CString term = m_printData[13];

		if(term != "0")
		{
			term = term + _T("개월");
		}
		else
		{
			term = "";
		}

		DrawBox(pDC,60,368,200,388,_T("계약기간"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,368,340,388,term,FALSE,FW_NORMAL);

		CString taxlimit = m_printData[14];

		if(taxlimit != "0")
		{
			taxlimit = taxlimit + _T(" 원");
		}
		else
		{
			taxlimit = "";
		}

		DrawBox(pDC,340,368,480,388,_T("세금우대한도"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,368,640,388,taxlimit,FALSE,FW_NORMAL);
		
		CString payonce = m_printData[15];

		if(payonce != "0")
		{
			payonce = payonce + _T(" 원");
		}
		else
		{
			payonce = "";
		}
		
		DrawBox(pDC,60,388,200,438,_T("정액월납입액\n/\n자유적립계약금액\n"),TRUE,FW_NORMAL);

		DrawBox(pDC,200,388,340,438,payonce,FALSE,FW_NORMAL);

		CString constract = m_printData[16];

		if(constract != "0")
		{
			constract = constract + _T(" 원");
		}
		else
		{
			constract = "";
		}

		DrawBox(pDC,340,388,480,438,_T("분기계약금액(분기한도)"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,388,640,438,constract,FALSE,FW_NORMAL);

		CString payday = m_printData[17];

		if(payday != "0")
		{
			payday = payday + _T(" 원");
		}
		else
		{
			payday = "";
		}

		DrawBox(pDC,60,438,200,458,_T("납입일"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,438,340,458,payday,FALSE,FW_NORMAL);

		DrawBox(pDC,340,438,480,458,_T("납입금액"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,438,640,458,m_printData[18]+_T(" 원"),FALSE,FW_NORMAL);

		CRect r3(MulDiv(60,m_iXP,m_iXD),MulDiv(478,m_iXP,m_iXD),MulDiv(160,m_iXP,m_iXD),MulDiv(498,m_iXP,m_iXD));

		pOldFont = (CFont*)pDC->SelectObject(&font);

		pDC->DrawText(_T("3.[문의처]"), -1, r3, DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);

		DeleteObject(&font);

		DrawBox(pDC,60,500,200,520,_T("현재 판매회사명"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,500,340,520,m_printData[19],FALSE,FW_NORMAL);

		DrawBox(pDC,340,500,480,520,_T("지점"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,500,640,520,m_printData[20],FALSE,FW_NORMAL);

		DrawBox(pDC,60,520,200,540,_T("담당자명"),FALSE,FW_NORMAL);

		DrawBox(pDC,200,520,340,540,m_printData[21],FALSE,FW_NORMAL);

		CString phonenum = m_printData[22];

		CString local = phonenum.Mid(0,4);
		CString first = phonenum.Mid(4,4);
		CString second = phonenum.Mid(8,4);

		local.TrimRight();

		phonenum = "(" + local + ") " + first + " - " + second; 

		DrawBox(pDC,340,520,480,540,_T("전화번호"),FALSE,FW_NORMAL);

		DrawBox(pDC,480,520,640,540,phonenum,FALSE,FW_NORMAL);

		CRect r4(MulDiv(60,m_iXP,m_iXD),MulDiv(560,m_iXP,m_iXD),MulDiv(160,m_iXP,m_iXD),MulDiv(580,m_iXP,m_iXD));

		pOldFont = (CFont*)pDC->SelectObject(&font);

		pDC->DrawText(_T("4.[유의사항]"), -1, r4, DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);

		DeleteObject(&font);

		CString warnMsg = "\n1.판매사 이동에 따라 제공되는 서비스의 범위,수수료등이 상이할 수 있음.\n\n2.판매사 이동이 완료되는 시점 이전에 압류 등의 권리제약 조치가 있는 경우 판매사 이동이 \n취소될 수 있음.\n\n3.이동신청 이후 이동예정 판매회사에 계좌개설 전까지는 본 계좌에 대한 환매,추가납입 등 \n각종 거래가 제한됩니다.\n\n4.이동신청일을 포함하여 5영업일 이내 이동예정 판매회사에 계좌개설을 하지 않을 경우 이\n동 신청이 자동으로 취소 될 수 있음.";
		
		DrawBox(pDC,60,582,640,732,warnMsg,TRUE,FW_BOLD);
	}
	catch(CException* e)
	{
		
	}

	//DrawBox(pDC,)
}

CString CPrintDlg::AddComma(CString strNum,TCHAR ch)
{
	int nCount = strNum.GetLength();

	for(int i=nCount - 1 ;i >= 0;--i)
	{
		if(i != 0 && (nCount - i) % 3 == 0)
			strNum.Insert(i,ch);
	}

	return strNum;
}

// float CMainDlg::CreateFontSize(HDC hDC, int points, double dpiRatio)
// {
// 	// This will calculate the font size for the printer that is specified
// 	// by a point size.
// 	//
// 	// if points is:
// 	//  (-) negative uses height as value for Net Font Height (ie. point size)
// 	//	(+) positive height is Total Height plus Leading Height!
// 	ASSERT(hDC);
// 	
// 	POINT size;
// 	int logPixelsY=::GetDeviceCaps(hDC, LOGPIXELSY);
// 	size.x = size.y = MulDiv(points, logPixelsY, 72);
// 
// 	// here we scale the font...
// 	return (float)floor(size.y*dpiRatio);
// }

void CPrintDlg::DrawTitle(CDC* pDC,double dpiRatio)
{
	//CPen pen(PS_DOT,4,RGB(255, 0, 0, 255));
	CFont* pOldFont;

	CPen pen(PS_SOLID, MulDiv(4,m_iXP,m_iXD), RGB(0,0,0));

	CPen* pOldPen = pDC->SelectObject(&pen);
	
	POINT startP,endP;

	startP.x = MulDiv(50,m_iXP,m_iXD);
	startP.y = MulDiv(80,m_iYP,m_iYD);

	endP.x = MulDiv(650,m_iXP,m_iXD);
	endP.y = MulDiv(80,m_iYP,m_iYD);

	DrawLine(pDC,startP,endP);
	
	startP.x = MulDiv(50,m_iXP,m_iXD);
	startP.y = MulDiv(130,m_iYP,m_iYD);

	endP.x = MulDiv(650,m_iXP,m_iXD);
	endP.y = MulDiv(130,m_iYP,m_iYD);

	DrawLine(pDC,startP,endP);

	pDC->SelectObject(pOldPen);

	DeleteObject(&pen);
    
	CFont font;
	VERIFY(font.CreateFont(
		   MulDiv(20,m_iXP,m_iXD),                        // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_BOLD,                 // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   "굴림체"));                 // lpszFacename

	pOldFont = (CFont*)pDC->SelectObject(&font);

	CRect rect;
	rect.top = MulDiv(84,m_iXP,m_iXD);
	rect.left = MulDiv(100,m_iXP,m_iXD);
	rect.bottom = MulDiv(130,m_iXP,m_iXD);;
	rect.right = MulDiv(600,m_iXP,m_iXD);;

	pDC->DrawText(_T("펀드 판매사 이동 계좌확인서"), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);

	DeleteObject(&font);

	CFont datefont;
	VERIFY(datefont.CreateFont(
		   MulDiv(10,m_iXP,m_iXD),                        // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_NORMAL,                 // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   "굴림체"));                 // lpszFacename

	CTime   CurTime;

	CurTime = CTime::GetCurrentTime();  
	CString nowDate;
	nowDate.Format("출력일 : %d년 %d월 %d일",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay());

	CRect rectDate(MulDiv(500,m_iXP,m_iXD),MulDiv(138,m_iXP,m_iXD),MulDiv(650,m_iXP,m_iXD),MulDiv(148,m_iXP,m_iXD));

	pOldFont = (CFont*)pDC->SelectObject(&datefont);

	pDC->DrawText(nowDate, -1, rectDate, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);

	DeleteObject(&datefont);
}

void CPrintDlg::DrawBox(CDC* pDC,int x1,int y1,int x2,int y2,LPCTSTR text,BOOL MULTI,int nBold)
{
	CFont* pOldFont;
	
	int nFontSize = 12;

	if(m_printFlag && nBold == FW_BOLD)
	{
		nFontSize = 13;
	}

	CFont font;
	VERIFY(font.CreateFont(
		   MulDiv(nFontSize,m_iXP,m_iXD),                        // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   nBold,                 // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   "굴림체"));                 // lpszFacename

	pDC->Rectangle(MulDiv(x1,m_iXP,m_iXD),MulDiv(y1,m_iXP,m_iXD),MulDiv(x2,m_iXP,m_iXD),MulDiv(y2,m_iXP,m_iXD));

	CRect rect(MulDiv(x1+4,m_iXP,m_iXD),MulDiv(y1+1,m_iXP,m_iXD),MulDiv(x2-4,m_iXP,m_iXD),MulDiv(y2-1,m_iXP,m_iXD));

	pOldFont = (CFont*)pDC->SelectObject(&font);

	if(MULTI == TRUE)
	{
		CString tmp(text);

		int nTmp = tmp.GetLength();

		rect.top = rect.top + MulDiv(2,m_iXP,m_iXD);
		
		if(nTmp > 40)
		{
			pDC->DrawText(text,-1,rect,DT_VCENTER);
		}
		else
		{
			pDC->DrawText(text,-1,rect,DT_CENTER | DT_VCENTER);
		}
	}
	else
	{
		pDC->DrawText(text,-1,rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	pDC->SelectObject(pOldFont);

	DeleteObject(&font);
}

void CPrintDlg::DrawLine(CDC* pDC,POINT sPoint,POINT ePoint)
{
	pDC->MoveTo(sPoint);
	pDC->LineTo(ePoint);
}

void CPrintDlg::StartPrint()
{
	double dpiRatio;

	CPrintDialog* pDlg = new CPrintDialog(FALSE);

	if(pDlg->DoModal() == IDCANCEL)
	{
		delete pDlg;

		return;
	}

	DEVMODE* pDev=pDlg->GetDevMode();
	// getting dpi ratio between 300dpi and current printer dpi
	m_dpiRatio=300.0f/pDev->dmPrintQuality;
	//dpiRatio = 100;
	// deallocating memory for pDev
	VERIFY(GlobalUnlock(pDev));

	DOCINFO docinfo;
	
	memset(&docinfo, 0, sizeof(docinfo));

	docinfo.cbSize= sizeof(docinfo);
	docinfo.lpszDocName = _T("펀드판매사 이동 출력");

	CPrintInfo pInfo;

    pInfo.SetMaxPage(1);
	//pInfo.m_rectDraw.SetRect(0, 0, 500, 800); 

    CDC* pDC =CDC::FromHandle(pDlg->GetPrinterDC());
	
	m_iXP = pDC->GetDeviceCaps(LOGPIXELSX);
    m_iYP = pDC->GetDeviceCaps(LOGPIXELSY);
    
    // get the current window's resolution
    CDC* pddc = GetDC();

    m_iXD = pddc->GetDeviceCaps(LOGPIXELSX);
    m_iYD = pddc->GetDeviceCaps(LOGPIXELSY);

	SetPrintAlign(pDC,pDlg->GetPrinterDC());
    
	pDC->StartDoc(&docinfo);
	pDC->StartPage();
	
	m_printFlag = TRUE;

	DrawStuff(pDC,dpiRatio);

	m_printFlag = FALSE;

	pDC->EndPage();
	pDC->EndDoc();

	delete pDlg;
    //SetTextAlign(hdc, TA_UPDATECP);

	pDC->DeleteDC();
}

void CPrintDlg::SetPrintAlign(CDC *pDC, HDC hdcPrn)
{
	short cxPage, cyPage;

	cxPage = ::GetDeviceCaps (hdcPrn, HORZRES) ;
	cyPage = ::GetDeviceCaps (hdcPrn, VERTRES) ;
	//pDC->SetMapMode (MM_ANISOTROPIC) ;
	SetMapMode(hdcPrn,MM_TEXT);
	//pDC->SetWindowExt ( 2100, 2970) ;
	SetWindowExtEx(hdcPrn,2100,2970,NULL);
	//pDC->SetViewportExt (cxPage, cyPage) ;
	SetViewportExtEx(hdcPrn,cxPage,cyPage,NULL);

	int nMarginX = (int)(100.0f / 2100.0f * (float)cxPage);
	int nMarginY = (int)(100.0f / 2970.0f * (float)cyPage);

	//pDC->SetViewportOrg (nMarginX,  nMarginY) ;
	SetViewportOrgEx(hdcPrn,nMarginX,nMarginY,NULL);
	//pDC->SetTextAlign (TA_BASELINE | TA_CENTER) ;
}

// float CPrintDlg::CreateFontSize(HDC hDC, int points, double dpiRatio)
// {
// 	// This will calculate the font size for the printer that is specified
// 	// by a point size.
// 	//
// 	// if points is:
// 	//  (-) negative uses height as value for Net Font Height (ie. point size)
// 	//	(+) positive height is Total Height plus Leading Height!
// 	ASSERT(hDC);
// 	
// 	POINT size;
// 	int logPixelsY=::GetDeviceCaps(hDC, LOGPIXELSY);
// 	size.x = size.y = MulDiv(points, logPixelsY, 72);
// 
// 	// here we scale the font...
// 	return (float)floor(size.y*dpiRatio);
// }

void CPrintDlg::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here

}

BSTR CPrintDlg::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

void CPrintDlg::SetData(LPCTSTR data) 
{
	//MessageBox(_T("정상진입"), "IBK투자증권", MB_OK|MB_ICONSTOP);
	// TODO: Add your dispatch handler code here
	//m_printData = data;
	GetTokenSplit(data,_T('|'));

// 	if(m_tokenSize < 23)
// 	{
// 		MessageBox(_T("데이타가 정상적으로 수신되지 않았읍니다."), "IBK투자증권", MB_OK|MB_ICONSTOP);
// 
// 		DestroyWindow();
// 		return;
// 	}
// 
// 	this->ShowWindow(SW_SHOW);
}

LONG CPrintDlg::OnManage(WPARAM wParam,LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
		case MK_SETDATA:
			CString str = (char*)lParam;

			char tab = P_TAB;
			char lf = P_NEW;

			SetData(str);
			
			//메시지 형식으로 데이타를 전달 받을 경우 문자배열 방식으로 변경 필요

			break;
	}

	return 0;
}
