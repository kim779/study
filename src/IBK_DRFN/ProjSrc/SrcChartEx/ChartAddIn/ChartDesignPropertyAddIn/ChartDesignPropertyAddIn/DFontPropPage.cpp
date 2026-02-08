// DFontPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DFontPropPage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _CRTM1()
#include "../Include_Chart/xUtil_Chart.h"				// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"				// for IPropertyMap
#include "PropertiesData.h"								// for g_anPropertyShortKey[]

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDFontPropPage, CDialog)
	//{{AFX_MSG_MAP(CDFontPropPage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_FONT_BOLD, OnFontBold)
	ON_BN_CLICKED(IDC_FONT_CANCELLINE, OnFontCancelline)
	ON_BN_CLICKED(IDC_FONT_ITALY, OnFontItaly)
	ON_BN_CLICKED(IDC_FONT_UNDERLINE, OnFontUnderline)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTLIST, OnSelchangeComboFontlist)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTSIZE, OnSelchangeComboFontsize)
	ON_CBN_EDITCHANGE(IDC_COMBO_FONTSIZE, OnEditchangeComboFontsize)
	ON_CBN_EDITCHANGE(IDC_COMBO_FONTLIST, OnEditchangeComboFontlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDFontPropPage::CDFontPropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CDFontPropPage::CDFontPropPage(CWnd* pOcxWnd, CWnd* pParent /*=NULL*/) 
	: CDialog(CDFontPropPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDFontPropPage)
	m_FontProp = _T("");
	//}}AFX_DATA_INIT
	mt_bBold = FALSE;
	mt_bCancelLine = FALSE;
	mt_bItaly = FALSE;
	mt_bUnderLine = FALSE;
	mt_nlfHeight = 12;

	m_pOcxWnd = pOcxWnd;

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = m_pOcxWnd->GetSafeHwnd();
}

/////////////////////////////////////////////////////////////////////////////
// CDFontPropPage::DoDataExchange - Moves data between page and properties

void CDFontPropPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CDFontPropPage)
	DDX_Control(pDX, IDC_FONT_UNDERLINE, m_checkUnderLine);
	DDX_Control(pDX, IDC_FONT_ITALY, m_checkItaly);
	DDX_Control(pDX, IDC_FONT_CANCELLINE, m_checkCancelLine);
	DDX_Control(pDX, IDC_FONT_BOLD, m_checkBold);
	DDX_Control(pDX, IDC_COMBO_FONTLIST, m_cbFontList);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_cbFontSize);
	DDX_Control(pDX, IDC_FONT_PREVIEW, m_FontPreview);
	DDX_Text(pDX, IDC_EDIT_FONT, m_FontProp);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CDFontPropPage message handlers
#include <Wingdi.h>
#include <Windows.h>

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시44분	
	리턴 타입 : int
	함 수 명  : EnumFontProc
	함수 설명 : 시스템에 있는 폰트정보를 콤보에 입력한다.
	인     자 : 	
*********************************************************************/
int CALLBACK EnumFontProc(ENUMLOGFONT* lpelf, NEWTEXTMETRIC* lpntm,
						  unsigned long nFontType, LPARAM lParam)
{    
    CComboBox *pCombo = (CComboBox *)lParam;
    if (nFontType & TRUETYPE_FONTTYPE) {
        pCombo->AddString(lpelf->elfLogFont.lfFaceName);
    }
    return(TRUE);
}

BOOL CDFontPropPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_FONTLIST);
	CDC *pDC = GetDC();
	::EnumFontFamilies(pDC->m_hDC, NULL, (FONTENUMPROC)EnumFontProc, (LPARAM)pCombo);
	ReleaseDC(pDC);	

	FT_vInitFont(m_FontProp);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDFontPropPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawPreview();
	// Do not call COlePropertyPage::OnPaint() for painting messages
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시43분	
	리턴 타입 : void
	함 수 명  : OnFontBold
	함수 설명 : 굵 체크버튼을 체크했을때 
	인     자 : 	
*********************************************************************/
void CDFontPropPage::OnFontBold() 
{	
	mt_bBold = m_checkBold.GetCheck();
	DrawPreview();		//미리보기 변경을위한 ReDraw
	FT_vSaveFont();		//폰트정보스트링을 다시 셋팅하기위한 함수
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시43분	
	리턴 타입 : void
	함 수 명  : OnFontCancelline
	함수 설명 : 취소선 체크버튼을 체크했을때 
	인     자 : 	
*********************************************************************/
void CDFontPropPage::OnFontCancelline() 
{
	mt_bCancelLine = m_checkCancelLine.GetCheck();
	DrawPreview();		//미리보기 변경을위한 ReDraw
	FT_vSaveFont();		//폰트정보스트링을 다시 셋팅하기위한 함수
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시43분	
	리턴 타입 : void
	함 수 명  : OnFontItaly
	함수 설명 : 기울임꼴 체크버튼을 체크했을때 
	인     자 : 	
*********************************************************************/
void CDFontPropPage::OnFontItaly() 
{	
	mt_bItaly = m_checkItaly.GetCheck();
	DrawPreview();		//미리보기 변경을위한 ReDraw
	FT_vSaveFont();		//폰트정보스트링을 다시 셋팅하기위한 함수
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시43분	
	리턴 타입 : void
	함 수 명  : OnFontUnderline
	함수 설명 : 언더라인 체크버튼을 체크했을때 
	인     자 : 	
*********************************************************************/
void CDFontPropPage::OnFontUnderline() 
{
	mt_bUnderLine = m_checkUnderLine.GetCheck();
	DrawPreview();		//미리보기 변경을위한 ReDraw
	FT_vSaveFont();		//폰트정보스트링을 다시 셋팅하기위한 함수
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시42분	
	리턴 타입 : void
	함 수 명  : OnSelchangeComboFontlist
	함수 설명 : 폰트명을 콤보에서 선택했을때
	인     자 : 	
*********************************************************************/
void CDFontPropPage::OnSelchangeComboFontlist() 
{	
	int index = m_cbFontList.GetCurSel();
	mt_strlfFaceName = "";
	if(index != -1)
		m_cbFontList.GetLBText(index,mt_strlfFaceName);
	DrawPreview();		//미리보기 변경을위한 ReDraw
	FT_vSaveFont();		//폰트정보스트링을 다시 셋팅하기위한 함수
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시14분	
	리턴 타입 : void
	함 수 명  : OnSelchangeComboFontsize
	함수 설명 : 폰트사이즈가 바뀔때 갓을 다시셋팅해준다.
	인     자 : 	
*********************************************************************/
void CDFontPropPage::OnSelchangeComboFontsize() 
{
	int index = m_cbFontSize.GetCurSel();
	CString tempSize = "";
	if(index != -1)
		m_cbFontSize.GetLBText(index,tempSize);
	mt_nlfHeight = -MulDiv(atoi(tempSize), GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY), 72); 
	DrawPreview();		//미리보기 변경을위한 ReDraw
	FT_vSaveFont();		//폰트정보스트링을 다시 셋팅하기위한 함수
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시11분	
	리턴 타입 : void
	함 수 명  : FT_vInitFont
	함수 설명 : 스트링으로 되어있는 폰트정보를 읽어서 각컨트롤에 초기값을 주고
				상태를 표시한다.
	인     자 : 	
*********************************************************************/
void CDFontPropPage::FT_vInitFont(const CString& initStr)
{
//	CString initStr="";
//	GetDlgItem(IDC_EDIT_FONT)->GetWindowText(initStr);
	CString strFont = initStr;
	//폰트이름
	int namePos = strFont.Find(";");
	CString lfFaceName = strFont.Mid(0,namePos);
	m_cbFontList.SetWindowText(lfFaceName);	//콤보에 셀렉트시킨다.
	mt_strlfFaceName = lfFaceName;			//변수에 저장
	//폰트크기
	int HeightPos = strFont.Find(";",namePos+1);
	CString lfHeight = strFont.Mid(namePos+1,HeightPos - (namePos+1));
	int nlfHeight = atoi(lfHeight);
	mt_nlfHeight = nlfHeight;				//변수에 저장
	//폰트단위 변화
	int lfFontSize = (nlfHeight*72)/(GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY));	
		
	CString fontSizeTemp="";
	fontSizeTemp.Format("%d",abs(lfFontSize));
	m_cbFontSize.SetWindowText(fontSizeTemp);	//콤보에 셀렉트시킨다.

	//굵은폰트
	int BoldPos = strFont.Find(";",HeightPos+1);
	CString strBold = strFont.Mid(HeightPos+1,BoldPos - (HeightPos+1));
	BOOL	blfWeight = atoi(strBold);
	m_checkBold.SetCheck(blfWeight);		//체크박스를 체크한다.
	mt_bBold = blfWeight;					//변수에 저장
	//취소선
	int CCLinePos = strFont.Find(";",BoldPos+1);
	CString strCCLine = strFont.Mid(BoldPos+1,CCLinePos-(BoldPos+1));
	BOOL	blfStrikeOut = atoi(strCCLine);
	m_checkCancelLine.SetCheck(blfStrikeOut);//체크박스를 체크한다.
	mt_bCancelLine = blfStrikeOut;			//변수에 저장
	//기울임꼴
	int italyPos = strFont.Find(";",CCLinePos+1);
	CString strItaly = strFont.Mid(CCLinePos+1,italyPos - (CCLinePos+1));
	BOOL	blfItalic = atoi(strItaly);
	m_checkItaly.SetCheck(blfItalic);		//체크박스를 체크한다.
	mt_bItaly = blfItalic;					//변수에 저장
	//언더라인
	int UnderLinePos = strFont.Find(";",italyPos+1);
	CString strUnderLine = strFont.Mid(italyPos+1,UnderLinePos-(italyPos+1));
	BOOL	blfUnderline = atoi(strUnderLine);
	m_checkUnderLine.SetCheck(blfUnderline);//체크박스를 체크한다.
	mt_bUnderLine = blfUnderline;			//변수에 저장
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월17일  16시10분	
	리턴 타입 : void
	함 수 명  : FT_vSaveFont
	함수 설명 : 각컨트롤의 폰트정보를 스트링으로 표현해서 에디트컨트롤에 입력한다.
				에디트컨트롤로서 프로퍼티를 전송한다.
	인     자 : 	
*********************************************************************/
void CDFontPropPage::FT_vSaveFont()
{
	CString strFont = "";
	strFont.Format(";%d;%d;%d;%d;%d;",mt_nlfHeight,mt_bBold,mt_bCancelLine,mt_bItaly,mt_bUnderLine);
	m_FontProp= "";
	m_FontProp = mt_strlfFaceName + strFont;
	GetDlgItem(IDC_EDIT_FONT)->SetWindowText(m_FontProp);
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월18일  9시07분	
	리턴 타입 : void
	함 수 명  : DrawPreview
	함수 설명 : 글꼴의 미리보기부분을 업데이트하는 함수
	인     자 : 	
*********************************************************************/
void CDFontPropPage::DrawPreview()
{
	CDC* pDC = m_FontPreview.GetDC();
	CRect PreViewRect;
	m_FontPreview.GetClientRect(&PreViewRect);
	PreViewRect.DeflateRect(1,1,1,1);
	pDC->FillSolidRect(PreViewRect,RGB(255,255,255));
	
	
	
	CFont* OldFont=pDC->GetCurrentFont();
	CFont m_Font;
	LOGFONT m_logFont;
	
	m_logFont.lfHeight = mt_nlfHeight;
	m_logFont.lfWidth = 0;
	m_logFont.lfEscapement = 0;
	m_logFont.lfOrientation = 0;
	if(mt_bBold)
		m_logFont.lfWeight = FW_BOLD;
	else
		m_logFont.lfWeight = FW_THIN;
	m_logFont.lfItalic = mt_bItaly;
	m_logFont.lfUnderline = mt_bUnderLine;
	m_logFont.lfStrikeOut = mt_bCancelLine;
	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_logFont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
	strcpy(m_logFont.lfFaceName, mt_strlfFaceName);
	
	m_Font.CreateFontIndirect(&m_logFont);	
	
	pDC->SelectObject(&m_Font);
	pDC->SetBkMode(TRANSPARENT);
	
	pDC->DrawText(mt_strlfFaceName,PreViewRect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	
	m_Font.DeleteObject();
	pDC->SelectObject(OldFont);
	
	OldFont=NULL;	delete OldFont;	
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월18일  9시32분	
	리턴 타입 : void
	함 수 명  : OnEditchangeComboFontsize
	함수 설명 : 사이즈를 설정하는 콤보박스에서 에디트창에 직접 숫자를 입력했을경우
	인     자 : 	
*********************************************************************/
void CDFontPropPage::OnEditchangeComboFontsize() 
{
	CString EditStr = "";
	m_cbFontSize.GetWindowText(EditStr);
	mt_nlfHeight = -MulDiv(atoi(EditStr), GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY), 72);
	DrawPreview();
	FT_vSaveFont();		//폰트정보스트링을 다시 셋팅하기위한 함수
}

/********************************************************************
	만든이    : 도성진(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	만든시간  : 2001년9월18일  9시33분	
	리턴 타입 : void
	함 수 명  : OnEditchangeComboFontlist
	함수 설명 : 폰트를 구하는 콤보에서 직접 폰트이름을 입력했을 경우
	인     자 : 	
*********************************************************************/
void CDFontPropPage::OnEditchangeComboFontlist() 
{
	CString EditStr = "";
	m_cbFontList.GetWindowText(EditStr);
	if(m_cbFontList.FindStringExact(-1,EditStr) == CB_ERR)
		mt_strlfFaceName = "";
	else
		mt_strlfFaceName = EditStr;

	DrawPreview();
	FT_vSaveFont();		//폰트정보스트링을 다시 셋팅하기위한 함수
}

// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
void CDFontPropPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CUSTOMFONT], m_FontProp,	_DPV_CUSTOMFONT);	
	CHART_CONVERT_END()

	int nFNIndex = m_FontProp.Find( ';');
	CString strFontName = m_FontProp.Left( nFNIndex);
	strFontName = _MTOL0( strFontName);
	m_FontProp = strFontName + m_FontProp.Mid( nFNIndex);

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

void CDFontPropPage::OnOK() 
{
	int nFNIndex = m_FontProp.Find( ';');
	CString strFontName = m_FontProp.Left( nFNIndex);
	_SETM( "$FontName$", strFontName);
	m_FontProp = "$FontName$" + m_FontProp.Mid( nFNIndex);

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CUSTOMFONT], m_FontProp,	_DPV_CUSTOMFONT);
	CHART_ADDTOSTRLIST_END

	CDialog::OnOK();
}
