// DlgStrategyList.cpp : implementation file
//

#include "stdafx.h"
#include "DlgStrategyList.h"
#include "DlgStrategyConfigure.h"
#include "DlgSave.h"
#include "StrategyConfigureWizardDef.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STPathDefine.h"
#include "../Common_ST/STConfigdef.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용
#include "../../inc/IKSLibMng.h"

#include <io.h>				// for _chmod() function supports
#include <sys/stat.h>		// for _chmod() function supports
#include ".\dlgstrategylist.h"
#include "../../inc/IMainInfoMng.h"//KHD

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStrategyList dialog
#define	HEIGHT_BUTTON			(19)
#define HEIGHT_CONTROLAREA 		(65)//(100)
#define	HEIGHT_GAP				(10)
#define	HEIGHT_INTERGAP			(10)
#define HEIGHT_UPPERAREA		(25)
#define HEIGHT_SEDONDGRID		(25)
#define HEIGHT_GAPINTERGRID		(3)

#define WIDTH_BTN_AND			(34)
#define WIDTH_BTN_OR			(34)
#define WIDTH_BTN_NEXT			(34)
#define WIDTH_BTN_NOT			(34)
#define WIDTH_BTN_TRUE			(34)
#define WIDTH_BTN_PARENTHESIS	(34)
#define WIDTH_BTN_X				(34)
#define	WIDTH_BT4CHAR			(58)
#define	WIDTH_BT3CHAR			(46)
#define WIDTH_EDIT_FORMULA		(360)
#define	WIDTH_GAP				(10)
#define WIDTH_INTERGAP_BUTLINE	(20)
#define	WIDTH_INTERGAP			(1)
#define WIDTH_STATIC_FORMULA	(60)
#define WIDTH_OPTION			(81)

#define MAX_COL			(5)

#define COL_NO			(0)
#define COL_TITLE		(1)
#define COL_TIME		(2)
#define COL_EDIT		(3)
#define COL_DELETE		(4)
#define COL_INFOPOINTER	(1)

#define TID_KILL_DIALOG	(10)		// Timer를 통해서 Dialog 닫기처리

#define CRBACKGROUND	RGB(177,199,234)
#define DEFAULT_BKGRND_COLOR		RGB(247,248,252)


// when user clicks OK or Cancel, send it...
const UINT RMSG_STCONFIGURENAME = ::RegisterWindowMessage("RMSG_STCONFIGURENAME");
const UINT RMSG_STCONTROLLER_POINTER = ::RegisterWindowMessage("RMSG_STCONTROLLER_POINTER");
const UINT RMSG_STSAVETOUSERST = ::RegisterWindowMessage("RMSG_STSAVETOUSERST");
extern CString Parser(LPCTSTR lpInputData, int *pNShifter );

#define		ST_MAX_ROW		10

CDlgStrategyList::CDlgStrategyList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStrategyList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStrategyList)
	//}}AFX_DATA_INIT
	m_lRowEditing = -1;
	m_lPosOfGirdBottom = 0;
	m_chLastIndex = 'A';
	m_crBk = CRBACKGROUND;
	m_nFlagCnt = 0;
	m_hViewHandle = NULL;
	//m_pIAUDataMonitorManager = NULL;
	//m_dwIAUDataMonitorManager = NULL;
}


void CDlgStrategyList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStrategyList)
	DDX_Control(pDX, IDC_BTN_STDELETE, m_btnSTDel);
	DDX_Control(pDX, IDC_BT_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_EDIT_ST_TITLE, m_editSTTitle);
	DDX_Control(pDX, IDC_CTRLLOGIC, m_ctrlLogic);
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_AND, m_btnAnd);
	DDX_Control(pDX, IDC_BTN_OR, m_btnOr);
	DDX_Control(pDX, IDC_BTN_NOT, m_btnNot);
	DDX_Control(pDX, IDC_BTN_TRUE, m_btnTrue);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_PARENTHESIS, m_btnParenthesis);
	DDX_Control(pDX, IDC_BTN_X, m_btnX);
	DDX_Control(pDX, IDC_BTN_INIT, m_btnInit);
	DDX_Control(pDX, IDC_BTN_STCLOSE, m_btnSTClose);
	DDX_Control(pDX, IDC_BTN_STCONFIGDLG, m_btnSTConfigDlg);
	DDX_Control(pDX, IDC_BTN_STRUN, m_btnSTRun);
	DDX_Control(pDX, IDC_BTN_SYSTEMTRADING, m_btnSystemTrading);
	DDX_Control(pDX, IDC_BUTTON_SAVECONFIG, m_btnSaveConfig);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_STVARIABLES, m_InputList);
}


BEGIN_MESSAGE_MAP(CDlgStrategyList, CDialog)
	//{{AFX_MSG_MAP(CDlgStrategyList)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_INIT, OnBtnInit)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_AND, OnBtnAnd)
	ON_BN_CLICKED(IDC_BTN_OR, OnBtnOr)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
	ON_BN_CLICKED(IDC_BTN_NOT, OnBtnNot)
	ON_BN_CLICKED(IDC_BTN_TRUE, OnBtnTrue)
	ON_BN_CLICKED(IDC_BTN_PARENTHESIS, OnBtnParenthesis)
	ON_BN_CLICKED(IDC_BTN_X, OnBtnX)
	ON_BN_CLICKED(IDC_BT_SAVE, OnBtSave)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_STDELETE, OnBtnSTDelete)
	ON_BN_CLICKED(IDC_BTN_STCONFIGDLG, OnBnClickedSTConfigDlg)
	ON_BN_CLICKED(IDC_BTN_STCLOSE, OnBnClickedSTClose)
	ON_BN_CLICKED(IDC_BTN_STRUN, OnBnClickedSTRun)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_STCONTROLLER_POINTER, OnSTController)
	ON_REGISTERED_MESSAGE(RMSG_STCONFIGURENAME, OnUpdateSTSfg)
	ON_REGISTERED_MESSAGE(RMSG_STSAVETOUSERST, OnSaveToUserST)
	ON_BN_CLICKED(IDC_BTN_SYSTEMTRADING, OnBnClickedBtnSystemtrading)
	ON_BN_CLICKED(IDC_BUTTON_SAVECONFIG, OnBnClickedButtonSaveconfig)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStrategyList message handlers
void CDlgStrategyList::InitSTOption()
{
	//GetDlgItem(IDC_STCONFIGURE)->GetWindowRect(m_rcSTOption);
	GetDlgItem(IDC_LIST_STVARIABLES)->GetWindowRect(m_rcSTOption);
	m_rcSTOption.top -= 32;
	ScreenToClient(m_rcSTOption);
}

void CDlgStrategyList::InitGrid()
{
	if(!m_GridSTList.GetSafeHwnd())
	{
		if(!m_GridSTList.Create(CRect(0,30,580,240),this,IDC_GRID,WS_CHILD | WS_TABSTOP | WS_VISIBLE))
			return;

		LOGFONT lf;
		::memset(&lf, 0x00, sizeof(LOGFONT));
		lf.lfWidth = 0; 
		lf.lfEscapement = 0; 
		lf.lfOrientation = 0; 
		lf.lfWeight = FW_NORMAL; 
		lf.lfItalic = 0; 
		lf.lfUnderline = 0; 
		lf.lfStrikeOut = 0; 
		lf.lfCharSet = DEFAULT_CHARSET; 
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS; 
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = FF_ROMAN; 
		strcpy(lf.lfFaceName, "굴림체");
		lf.lfHeight = -12;
		if(!m_Font.CreateFontIndirect(&lf))
			return;

		m_GridSTList.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xff, 0xff, 0xff));
		m_GridSTList.SetColumnCount(MAX_COL);
		m_GridSTList.SetFixedRowCount(1);
		m_GridSTList.SetColumnResize(FALSE);
		m_GridSTList.SetRowCount(1);
		m_GridSTList.SetListMode();
		m_GridSTList.SetRowHeight(0,20);
		m_GridSTList.SetEditable(FALSE);
		m_GridSTList.EnableTitleTips(FALSE);
		m_GridSTList.SetTrackFocusCell(FALSE);
		m_GridSTList.SetSingleColSelection(FALSE);
		m_GridSTList.SetSingleRowSelection();
		m_GridSTList.SetFixedRowSelection(FALSE);
		m_GridSTList.SetFixedColumnSelection(FALSE);
		m_GridSTList.SetBkColor(RGB(255,255,255));		
		m_GridSTList.SetGridLineColor(RGB(204,210,224));
		m_GridSTList.SetFont(&m_Font);

		char *psztitle[MAX_COL] = {"이름", "조건", "시간", "수정", "삭제"};
		GV_ITEM Item;
		for (int col = 0; col < m_GridSTList.GetColumnCount(); col++)
		{
			Item.row = 0;
			Item.col = col;
			Item.strText = psztitle[col];
        
			Item.crBkClr = RGB(242,248,254);	// or - m_GridSTList.SetItemBkColour(row, col, clr);
			Item.crFgClr = RGB(0,0,0);			// or - m_GridSTList.SetItemFgColour(row, col, RGB(255,0,0));				    
			Item.nFormat = DT_CENTER;
			Item.nState =  GVIS_STATIC;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE;
			
			m_GridSTList.SetItem(&Item);
		}
		
  		m_GridSTList.ExpandColumnsToFit();
	}	
}

BOOL CDlgStrategyList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitColumn();

	m_strRootPath = ((CDlgStrategyConfigure*)GetParent())->m_strRootPath;
	m_szDataDir = ((CDlgStrategyConfigure*)GetParent())->m_strDataDir;
	m_szUserDir = ((CDlgStrategyConfigure*)GetParent())->m_strUserDir;

	if(!m_dlgTimeSelector.GetSafeHwnd())
	{
		m_dlgTimeSelector.Create(CDlgTimeSelector::IDD,this);			
		m_dlgTimeSelector.SetDlgStrategyList(this);
	}

//@신영삭제
//	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//	if(pKSLibMng)
//		m_crBk = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
//	else
		m_crBk = DEFAULT_BKGRND_COLOR;

		m_brBkColor.CreateSolidBrush(m_crBk);
	InitSTOption();
	InitGrid();

	m_btnAnd.EnableWindow(FALSE);
	m_btnOr.EnableWindow(FALSE);
	m_btnNext.EnableWindow(FALSE);
	m_btnNot.EnableWindow(FALSE);
	m_btnTrue.EnableWindow(FALSE);
	m_btnParenthesis.EnableWindow(FALSE);
	m_btnX.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStrategyList::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
}

CRect CDlgStrategyList::GetRect(int nId)
{
	CRect rect;
	switch(nId) {
	case IDC_GRID:
		{
			GetClientRect(rect);
			rect.left = 11;
			rect.top = rect.top + m_rcSTOption.top;
			//rect.right = rect.right - 252;
			rect.right = rect.right - 242;
			rect.bottom = rect.top + m_rcSTOption.Height();
		}
		break;		
	case IDC_STATIC_FORMULA:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP;
			rect.right = rect.left + WIDTH_STATIC_FORMULA;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + 5;
			rect.bottom = rect.top + HEIGHT_BUTTON;
			
		}
		break;
	case IDC_EDIT_FOMULA:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_STATIC_FORMULA + WIDTH_INTERGAP;
			rect.right = rect.left + WIDTH_EDIT_FORMULA;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_UP:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP;
			rect.right = rect.left + WIDTH_BT3CHAR;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_DOWN:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR + WIDTH_INTERGAP;
			rect.right = rect.left + WIDTH_BT3CHAR;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_EDIT:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_INTERGAP*2;
			rect.right = rect.left + WIDTH_BT4CHAR;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_DELETE:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_BT4CHAR + WIDTH_INTERGAP*3;
			rect.right = rect.left + WIDTH_BT4CHAR;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_AND:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_BT4CHAR*2 + WIDTH_INTERGAP*4 + WIDTH_INTERGAP_BUTLINE;
			rect.right = rect.left + WIDTH_BTN_AND;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_OR:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_BT4CHAR*2 + WIDTH_INTERGAP*5 + WIDTH_INTERGAP_BUTLINE + WIDTH_BTN_AND;
			rect.right = rect.left + WIDTH_BTN_OR;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_NEXT:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_BT4CHAR*2 + WIDTH_INTERGAP*6  + WIDTH_INTERGAP_BUTLINE+ WIDTH_BTN_AND + WIDTH_BTN_OR;
			rect.right = rect.left + WIDTH_BTN_NEXT;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_NOT:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_BT4CHAR*2 + WIDTH_INTERGAP*7  + WIDTH_INTERGAP_BUTLINE+ WIDTH_BTN_AND + WIDTH_BTN_OR + WIDTH_BTN_NEXT;
			rect.right = rect.left + WIDTH_BTN_NOT;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_TRUE:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_BT4CHAR*2 + WIDTH_INTERGAP*8 + WIDTH_INTERGAP_BUTLINE + WIDTH_BTN_AND + WIDTH_BTN_OR + WIDTH_BTN_NEXT + WIDTH_BTN_NOT;
			rect.right = rect.left + WIDTH_BTN_TRUE;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_PARENTHESIS:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_BT4CHAR*2 + WIDTH_INTERGAP*9  + WIDTH_INTERGAP_BUTLINE+ WIDTH_BTN_AND + WIDTH_BTN_OR + WIDTH_BTN_NEXT + WIDTH_BTN_NOT + WIDTH_BTN_TRUE;
			rect.right = rect.left + WIDTH_BTN_PARENTHESIS;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	case IDC_BTN_X:
		{
			GetClientRect(rect);
			rect.left = WIDTH_GAP + WIDTH_BT3CHAR*2 + WIDTH_BT4CHAR*2 + WIDTH_INTERGAP*10 + WIDTH_INTERGAP_BUTLINE + WIDTH_BTN_AND + WIDTH_BTN_OR + WIDTH_BTN_NEXT + WIDTH_BTN_NOT + WIDTH_BTN_TRUE + WIDTH_BTN_PARENTHESIS;
			rect.right = rect.left + WIDTH_BTN_X;
			rect.top = rect.bottom - HEIGHT_CONTROLAREA + HEIGHT_GAP + HEIGHT_BUTTON + HEIGHT_INTERGAP;
			rect.bottom = rect.top + HEIGHT_BUTTON;
		}
		break;
	default:
		break;
	}
	return rect;
}

void CDlgStrategyList::ResizeAll()
{
	if(m_GridSTList.GetSafeHwnd()) m_GridSTList.MoveWindow(GetRect(IDC_GRID));

	CRect rect;
	GetClientRect(rect);
	long lWidth[MAX_COL] = {5, 215, 0, 10, 10};

	if(m_GridSTList.GetSafeHwnd())
	{
		for (int col = 0; col < m_GridSTList.GetColumnCount(); col++)
		{
			if(col==1)
				m_GridSTList.SetColumnWidth(col,lWidth[col]);
			else
				m_GridSTList.SetColumnWidth(col,lWidth[col]);
		}
		m_GridSTList.ExpandColumnsToFit();
	}
}

LRESULT CDlgStrategyList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case UWM_ADDSTRATEGY:
		//AddRowData((LPCTSTR)wParam,lParam);
		break;
	case UWM_ADDCOMPOUND:
		if(!m_GridSTList.GetSafeHwnd()) return 0L;
		LoadST((LONG)wParam);
		break;
	case UWM_SETSEL:
		{
			CString strSetSel = (LPCTSTR)wParam;
			long lType = lParam;
			// Single word Selected..
			if(lType&MULTISELECTED)
			{
				m_btnAnd.EnableWindow(FALSE);
				m_btnOr.EnableWindow(FALSE);
				m_btnNext.EnableWindow(FALSE);
				m_btnNot.EnableWindow(FALSE);
				m_btnTrue.EnableWindow(FALSE);
				m_btnParenthesis.EnableWindow();
				if(lType&INCLUDEPARENTHESIS)
					m_btnX.EnableWindow();
				else
					m_btnX.EnableWindow(FALSE);
			}
			else
			{
				if(strSetSel==STRANDINLOGIC)
				{
					m_btnAnd.EnableWindow(FALSE);
					m_btnOr.EnableWindow();					
					m_btnNot.EnableWindow(FALSE);
					m_btnTrue.EnableWindow(FALSE);
					if(lType&NEXTISPOSSIBLE)
						m_btnNext.EnableWindow();
					else
						m_btnNext.EnableWindow(FALSE);
				}			
				else if(strSetSel==STRORINLOGIC)
				{
					m_btnAnd.EnableWindow();
					m_btnOr.EnableWindow(FALSE);
					m_btnNot.EnableWindow(FALSE);
					m_btnTrue.EnableWindow(FALSE);
					if(lType&NEXTISPOSSIBLE)
						m_btnNext.EnableWindow();
					else
						m_btnNext.EnableWindow(FALSE);
				}
				else if(strSetSel==STRNEXTINLOGIC)
				{
					m_btnAnd.EnableWindow();
					m_btnOr.EnableWindow();
					m_btnNext.EnableWindow(FALSE);
					m_btnNot.EnableWindow(FALSE);
					m_btnTrue.EnableWindow(FALSE);
				}
				else if(strSetSel.GetLength()>0)	// element
				{
					m_btnAnd.EnableWindow(FALSE);
					m_btnOr.EnableWindow(FALSE);
					m_btnNext.EnableWindow(FALSE);
					if(strSetSel[0]=='!')
					{
						m_btnNot.EnableWindow(FALSE);
						m_btnTrue.EnableWindow();
					}
					else
					{
						m_btnNot.EnableWindow();
						m_btnTrue.EnableWindow(FALSE);
					}
				}
				else if(strSetSel.GetLength()==0)	// nothing selected..
				{
					m_btnAnd.EnableWindow(FALSE);
					m_btnOr.EnableWindow(FALSE);
					m_btnNext.EnableWindow(FALSE);
					m_btnNot.EnableWindow(FALSE);
					m_btnTrue.EnableWindow(FALSE);
				}
				m_btnParenthesis.EnableWindow(FALSE);
				m_btnX.EnableWindow(FALSE);
			}
		}
		break;
	case UWM_SETTIME:
		{
			CString strTimeFromGrid;
			long lRowCount = m_GridSTList.GetRowCount();
			if(lRowCount>m_dlgTimeSelector.m_lRow)
			{
				switch(m_dlgTimeSelector.m_nOption)
				{
				case 1:
					strTimeFromGrid.Format("%d분",m_dlgTimeSelector.m_nMinute);
					break;
				case 2:
					strTimeFromGrid.Format("%d틱",m_dlgTimeSelector.m_nTick);
					break;				
				case 0:
				default:
					strTimeFromGrid = "일";
					break;
				}
				//CSDItemInfo * pSDItemInfo = (CSDItemInfo *)m_GridSTList.GetItemData(m_dlgTimeSelector.m_lRow,COL_INFOPOINTER);
				//pSDItemInfo->m_strTimeInfo = strTimeFromGrid;
				//m_GridSTList.SetItemText(m_dlgTimeSelector.m_lRow,COL_TIME,pSDItemInfo->m_strTimeInfo);
				m_GridSTList.SetItemText(m_dlgTimeSelector.m_lRow,COL_TIME,strTimeFromGrid);
				ResizeAll();
			}			
		}
		break;
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgStrategyList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
	if(wParam==IDC_GRID)
		if(pNmgv->hdr.code==NM_CLICK)
			OnClickGridSTList(pNmgv->iRow,pNmgv->iColumn);

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CDlgStrategyList::AddRowData(CString szFullSTPath, LPCTSTR lpSfgName, int nType, int nSTPos /*=-1*/)
{
	CSDItemInfo *pSDItemInfo = NULL;
	CString strPath;
	CString strMessage;
//	CString strSTFullPath;

	if(nType==SYSTEM_ST)
	{
		strPath = m_szDataDir + STDATA_SD_RELEASE;
	}
	else if(nType==USER_ST)
	{
		strPath = m_szUserDir + STDATA_SD_RELEASE;
	}

//	strSTFullPath = strPath + "\\" + lpSfgName + ".ST";
	
	// After SetNewSDItemInfo was Executed, OnSTController Called
	if(nSTPos==-1)
		nSTPos = GetNewSTPosNo();

	pSDItemInfo = new CSDItemInfo;
	//BOOL bRetValue = pSDItemInfo->SetNewSDItemInfo(GetSafeHwnd()
	//												,NULL
	//												,NULL
	//												,lpSfgName, nType,strPath, strSTFullPath, nSTPos);
	BOOL bRetValue = pSDItemInfo->SetNewSDItemInfo(GetSafeHwnd()
		,NULL
		,NULL
		,lpSfgName, nType,strPath, szFullSTPath, nSTPos);

	if(!bRetValue)
	{
		delete pSDItemInfo;
		pSDItemInfo = NULL;
		return FALSE;
	}
	return TRUE;
}

// when insert or update, Called here..
// ST 파일에 정보가 있으면 ST에서 읽고, 그렇지 않으면 기존 루틴그대로.
LRESULT CDlgStrategyList::OnSTController( WPARAM wParam, LPARAM lParam) 
{
	// 포인터로 구분..
	CSDItemInfo* pSDItemInfo = (CSDItemInfo*)wParam;
	pSDItemInfo->SetpISTControllerD(lParam);

	// to update...
	CSDItemInfo* pSDItemInfoInRow = NULL;
	int nPosOfpSDItemInfo = -1;
	if(m_lRowEditing>=1)
	{
		pSDItemInfoInRow = (CSDItemInfo*)m_GridSTList.GetItemData(m_lRowEditing,COL_INFOPOINTER);
		if(pSDItemInfoInRow==pSDItemInfo)
		{
			nPosOfpSDItemInfo = m_lRowEditing;
		}
		m_lRowEditing = -1;
	}

	if(nPosOfpSDItemInfo==-1)
	{
		nPosOfpSDItemInfo = m_GridSTList.GetRowCount();
		m_GridSTList.SetRowCount(nPosOfpSDItemInfo+1);

		CString strIndex;
		strIndex.Format("%c",m_chLastIndex);
		m_GridSTList.SetItemText(nPosOfpSDItemInfo,COL_NO,strIndex);
		m_GridSTList.SetItemFormat(nPosOfpSDItemInfo,COL_NO,DT_CENTER);	
		m_ctrlLogic.AddElement(m_chLastIndex);
		m_chLastIndex++;
	}

	CISTControllerD *pISTControllerD = pSDItemInfo->GetpISTControllerD();

	DWORD dSizeRead = 0;

	CString strPostTitle;
	CString strInput;
	for(int nPos=0;nPos<MAXCNTOFINPUTHOLDER;nPos++)
	{
		strInput = pISTControllerD->GetInputs(nPos);
		if(strInput.GetLength())
		{
			if(strPostTitle.GetLength())
			{
				strPostTitle += ",";
			}
			else
			{
				strPostTitle += "(";
			}
			strPostTitle += pISTControllerD->GetInputs(nPos);
		}
		else
		{
			break;
		}
	}
	if(strPostTitle.GetLength())
	{
		strPostTitle += ")";
	}
	m_GridSTList.SetItemText(nPosOfpSDItemInfo,COL_TITLE,pSDItemInfo->m_strTitle+strPostTitle);
	m_GridSTList.SetItemText(nPosOfpSDItemInfo,COL_TIME,pSDItemInfo->m_strTimeInfo);
	m_GridSTList.SetItemFormat(nPosOfpSDItemInfo,COL_TIME,DT_CENTER);	
	m_GridSTList.SetItemText(nPosOfpSDItemInfo,COL_EDIT,"편집");
	m_GridSTList.SetItemFormat(nPosOfpSDItemInfo,COL_EDIT,DT_CENTER);
	m_GridSTList.SetItemText(nPosOfpSDItemInfo,COL_DELETE,"X");
	m_GridSTList.SetItemFormat(nPosOfpSDItemInfo,COL_DELETE,DT_CENTER);
	m_GridSTList.SetItemData(nPosOfpSDItemInfo,COL_INFOPOINTER,(long)pSDItemInfo);

	ResizeAll();
	return 0;
}


// after Item was changed, called here..
LRESULT CDlgStrategyList::OnUpdateSTSfg( WPARAM wParam, LPARAM lParam)
{
	// 
	LPCTSTR  lpSfgName = (LPCTSTR)wParam;
	CString strSfgNameInList;
	CSDItemInfo * pSDItemInfo = NULL;
	int nRowCount = m_GridSTList.GetRowCount();	
	CCellRange cellRange = m_GridSTList.GetSelectedCellRange();
	m_lRowEditing = cellRange.GetMaxRow();

	if(m_lRowEditing>=1)
	{
		pSDItemInfo = (CSDItemInfo *)m_GridSTList.GetItemData(m_lRowEditing,COL_INFOPOINTER);
		if(pSDItemInfo)
		{
			strSfgNameInList = pSDItemInfo->m_strPath + ".SFG";
			if(strSfgNameInList==lpSfgName)
			{
				pSDItemInfo->UpdateData();	// OnSTController will be Called..
			}
		}
	}
	return 0;
}

LRESULT CDlgStrategyList::OnSaveToUserST(WPARAM wParam, LPARAM lParam)
{
	//if(m_nFlagCnt != 1) {
		if(AfxMessageBox("사용자전략만이 설정변경이 가능합니다.\n선택한전략들을 사용자전략으로 저장하시겠습니까?", MB_YESNO) == IDYES) {
			OnBtSave();

			CString strText;
			m_editSTTitle.GetWindowText(strText);

			CString szSTFilePath;
			szSTFilePath.Format("%s%s\\%s%s", m_szUserDir, STDATA_ST_USERCOMBINE, strText, ".ST");

			CFileFind fileFind;
			BOOL bFound = fileFind.FindFile(szSTFilePath);

			if(bFound) {
				CString* pszSTPath = (CString*)lParam;
				*pszSTPath = szSTFilePath;

				return 1L;
			}
			else
				return 0L;
		}
	//}

	return 0L;
}

void CDlgStrategyList::OnClickGridSTList(int nRow, int nCol)
{
	if(nRow<=0) return;
	if(nCol==COL_DELETE)
	{
		m_GridSTList.SetSelectedRange(nRow,0,nRow,MAX_COL-1);
		//CSDItemInfo * pSDItemInfo = (CSDItemInfo *)m_GridSTList.GetItemData(nRow,COL_INFOPOINTER);
		//if(!pSDItemInfo) return;
		//
		//pSDItemInfo->GetpISTControllerD()->DeleteConfig();
		//delete pSDItemInfo;

		CString strIndex = m_GridSTList.GetItemText(nRow,COL_NO);
		m_GridSTList.DeleteRow(nRow);

		m_ctrlLogic.DeleteElement(strIndex[0]);

		// 변수 설정부분도 초기화
		SetDlgItemText(IDC_EDIT_STNAME, "");
		m_InputList.DeleteAllItems();

		ResizeAll();
	}
	else if(nCol==COL_TIME)
	{
		CSDItemInfo * pSDItemInfo = (CSDItemInfo *)m_GridSTList.GetItemData(nRow,COL_INFOPOINTER);
		CString strTimeFromGrid = m_GridSTList.GetItemText(nRow,COL_TIME);
		CString strTimeType = strTimeFromGrid.Right(2);
		CString strTimeNum;
		int nLength = strTimeFromGrid.GetLength();
		if(nLength>2)
			strTimeNum = strTimeFromGrid.Left(nLength-2);

		m_dlgTimeSelector.m_lRow = nRow;
		if(strTimeType=="일")
			m_dlgTimeSelector.m_nOption = 0;
		else if(strTimeType=="분")
		{
			m_dlgTimeSelector.m_nOption = 1;
			m_dlgTimeSelector.m_nMinute = atoi(strTimeNum);
		}
		else if(strTimeType=="틱")
		{
			m_dlgTimeSelector.m_nOption = 2;
			m_dlgTimeSelector.m_nTick = atoi(strTimeNum);
		}

		CRect rect;
		m_GridSTList.GetCellRect(nRow,nCol,rect);
		ClientToScreen(rect);
		m_dlgTimeSelector.UpdateData(FALSE);
		m_dlgTimeSelector.SetWindowPos(NULL,rect.right,rect.bottom + rect.Height()*2,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
	}
	else if(nCol==COL_EDIT)
	{
		//CSDItemInfo * pSDItemInfo = (CSDItemInfo *)m_GridSTList.GetItemData(nRow,COL_INFOPOINTER);
		//CString szScriptName = pSDItemInfo->m_strName + ".sts";
	
		CString szTitle = m_GridSTList.GetItemText(nRow,COL_TITLE);
		int nEndPos = szTitle.Find('(');
		szTitle = szTitle.Left(nEndPos);
		CString szScriptName = szTitle + ".sts";
		((CDlgStrategyConfigure*)GetParent())->LoadEditor(szScriptName);
	}
	else
	{
		// CSDItemInfo * pSDItemInfo = (CSDItemInfo *)m_GridSTList.GetItemData(nRow,COL_INFOPOINTER);
		// after Changing Happened In SetConfig , called OnUpdateSTSfg..
		// m_lRowEditing = nRow;
		// pSDItemInfo->GetpISTControllerD()->SetConfigOnWizard(&m_rcSTOption);
		// 여기에서 해당하는 설정값을 로딩한다.
		// @2006.10.10
		CString szTitle = m_GridSTList.GetItemText(nRow,COL_TITLE);
		InputModify(nRow, szTitle);
	}
}

void CDlgStrategyList::OnDestroy() 
{
	CDialog::OnDestroy();
	
	//if(m_dwIAUDataMonitorManager)
	//	m_pIAUDataMonitorManager->UnAdvise(m_dwIAUDataMonitorManager);

	if(m_Font.GetSafeHandle())
		m_Font.DeleteObject();
	
	m_brBkColor.DeleteObject();
	m_dlgTimeSelector.DestroyWindow();
	DeleteAllItem();
}

BOOL CDlgStrategyList::DeleteAllItem()
{
	CSDItemInfo * pSDItemInfo = NULL;
	int nRowCount = m_GridSTList.GetRowCount();
	for(int nPos=nRowCount-1;nPos>0;nPos--)
	{
		m_GridSTList.DeleteRow(nPos);
	}
	m_ctrlLogic.DeleteAllElement();
	m_chLastIndex = 'A';
	return TRUE;
}

int CDlgStrategyList::GetNewSTPosNo()
{
	int nRowCount = m_GridSTList.GetRowCount();

	if(nRowCount<=1)
		return 0;

	CSDItemInfo* pSDItemInfoInRow = NULL;
	BOOL bExistIndex = FALSE;
	int nIndexFinder = 0;
	for(nIndexFinder = 0;nIndexFinder<256;nIndexFinder++)
	{
		bExistIndex = FALSE;
		for(int nRow=1;nRow<nRowCount;nRow++)
		{
			pSDItemInfoInRow = (CSDItemInfo*)m_GridSTList.GetItemData(nRow,COL_INFOPOINTER);
			if(pSDItemInfoInRow->m_nIndexInST==nIndexFinder) 
			{
				bExistIndex = TRUE;
				break;
			}
		}
		if(bExistIndex)
			continue;
		else
			break;
	}
	return nIndexFinder;
}

void CDlgStrategyList::OnBtnInit() 
{
	m_nFlagCnt = 0;

	GetDlgItem(IDC_BTN_STCONFIGDLG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STRUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STDELETE)->EnableWindow(FALSE);

	DeleteAllItem();
	if(m_GridSTList.GetSafeHwnd()) m_GridSTList.Invalidate();
	m_editSTTitle.SetWindowText("");
	m_szSTFilePath.Empty();

	// 변수 설정부분도 초기화
	SetDlgItemText(IDC_EDIT_STNAME, "");
	m_InputList.DeleteAllItems();
}

void CDlgStrategyList::OnBtnDown() 
{
	CCellRange CellRangeOfSTList = m_GridSTList.GetSelectedCellRange();
	int nRowCount = m_GridSTList.GetRowCount();
	int nRow = CellRangeOfSTList.GetMaxRow();
	if(nRow>0&&nRow<(nRowCount-1)&&nRowCount>2)
	{
		CSDItemInfo* pSDItemInfoInRow = NULL;
		CSDItemInfo* pSDItemInfoInRowBackup = NULL;
		CString strText;
		CString strTextBackup;
		COLORREF clrFgText;
		COLORREF clrFgTextBackup;
		UINT nFormat;
		UINT nFormatBackup;

		for(int nCol=0;nCol<MAX_COL;nCol++)
		{
			strText = m_GridSTList.GetItemText(nRow,nCol);
			strTextBackup = m_GridSTList.GetItemText(nRow+1,nCol);
			pSDItemInfoInRow = (CSDItemInfo*)m_GridSTList.GetItemData(nRow,nCol);
			pSDItemInfoInRowBackup = (CSDItemInfo*)m_GridSTList.GetItemData(nRow+1,nCol);
			clrFgText = m_GridSTList.GetItemFgColour(nRow,nCol);
			clrFgTextBackup = m_GridSTList.GetItemFgColour(nRow+1,nCol);
			nFormat = m_GridSTList.GetItemFormat(nRow,nCol);
			nFormatBackup = m_GridSTList.GetItemFormat(nRow+1,nCol);			

			m_GridSTList.SetItemText(nRow,nCol,strTextBackup);
			m_GridSTList.SetItemText(nRow+1,nCol,strText);
			m_GridSTList.SetItemData(nRow,nCol,(long)pSDItemInfoInRowBackup);
			m_GridSTList.SetItemData(nRow+1,nCol,(long)pSDItemInfoInRow);			
			m_GridSTList.SetItemFgColour(nRow,nCol,clrFgTextBackup);
			m_GridSTList.SetItemFgColour(nRow+1,nCol,clrFgText);
			m_GridSTList.SetItemFormat(nRow,nCol,nFormatBackup);
			m_GridSTList.SetItemFormat(nRow+1,nCol,nFormat);
			if(nCol==COL_NO)
				m_ctrlLogic.SwapElement(strText[0],strTextBackup[0]);
		}
		m_GridSTList.SetSelectedRange(nRow+1,0,nRow+1,MAX_COL-1);
		m_GridSTList.Invalidate();
	}
}

void CDlgStrategyList::OnBtnUp() 
{
	CCellRange CellRangeOfSTList = m_GridSTList.GetSelectedCellRange();
	int nRowCount = m_GridSTList.GetRowCount();
	int nRow = CellRangeOfSTList.GetMaxRow();
	if(nRow>1&&nRowCount>2)
	{
		CSDItemInfo* pSDItemInfoInRow = NULL;
		CSDItemInfo* pSDItemInfoInRowBackup = NULL;
		CString strText;
		CString strTextBackup;
		COLORREF clrFgText;
		COLORREF clrFgTextBackup;
		UINT nFormat;
		UINT nFormatBackup;
		for(int nCol=0;nCol<MAX_COL;nCol++)
		{
			strText = m_GridSTList.GetItemText(nRow,nCol);
			strTextBackup = m_GridSTList.GetItemText(nRow-1,nCol);
			pSDItemInfoInRow = (CSDItemInfo*)m_GridSTList.GetItemData(nRow,nCol);
			pSDItemInfoInRowBackup = (CSDItemInfo*)m_GridSTList.GetItemData(nRow-1,nCol);
			clrFgText = m_GridSTList.GetItemFgColour(nRow,nCol);
			clrFgTextBackup = m_GridSTList.GetItemFgColour(nRow-1,nCol);
			nFormat = m_GridSTList.GetItemFormat(nRow,nCol);
			nFormatBackup = m_GridSTList.GetItemFormat(nRow-1,nCol);

			m_GridSTList.SetItemText(nRow,nCol,strTextBackup);
			m_GridSTList.SetItemText(nRow-1,nCol,strText);
			m_GridSTList.SetItemData(nRow,nCol,(long)pSDItemInfoInRowBackup);
			m_GridSTList.SetItemData(nRow-1,nCol,(long)pSDItemInfoInRow);
			m_GridSTList.SetItemFgColour(nRow,nCol,clrFgTextBackup);
			m_GridSTList.SetItemFgColour(nRow-1,nCol,clrFgText);
			m_GridSTList.SetItemFormat(nRow,nCol,nFormatBackup);
			m_GridSTList.SetItemFormat(nRow-1,nCol,nFormat);
			if(nCol==COL_NO)
				m_ctrlLogic.SwapElement(strText[0],strTextBackup[0]);
		}
		m_GridSTList.SetSelectedRange(nRow-1,0,nRow-1,MAX_COL-1);
		m_GridSTList.Invalidate();
	}
}

void CDlgStrategyList::OnBtnAnd() 
{
	m_ctrlLogic.ChangeOperation(STRANDINLOGIC);
	m_ctrlLogic.SetFocus();
}

void CDlgStrategyList::OnBtnOr() 
{
	m_ctrlLogic.ChangeOperation(STRORINLOGIC);	
	m_ctrlLogic.SetFocus();
}

void CDlgStrategyList::OnBtnNext() 
{
	m_ctrlLogic.ChangeOperation(STRNEXTINLOGIC);
	m_ctrlLogic.SetFocus();
}

void CDlgStrategyList::OnBtnNot() 
{
	m_ctrlLogic.ChangeElement(FALSE);	
	m_ctrlLogic.SetFocus();
}

void CDlgStrategyList::OnBtnTrue() 
{
	m_ctrlLogic.ChangeElement(TRUE);	
	m_ctrlLogic.SetFocus();
}

void CDlgStrategyList::OnBtnParenthesis() 
{
	m_ctrlLogic.Parenthesize();	
	m_ctrlLogic.SetFocus();	
}

void CDlgStrategyList::OnBtnX() 
{
	m_ctrlLogic.Unparenthesize();	
	m_ctrlLogic.SetFocus();		
}

void CDlgStrategyList::SaveST(LPCTSTR lpSTName, BOOL bSet)
{
	CString strSTName;
	CString strSTPath;
	CString strSTDir;
	strSTName = lpSTName;

	if(bSet)
		strSTPath = m_szSTFilePath;
	else
		strSTPath = m_szUserDir + STDATA_ST_USERCOMBINE + "\\" + strSTName + ".ST";//STUSER_FOLDER + "\\" + strSTName + ".ST";

	::chmod(strSTPath, _S_IWRITE);

	CSDItemInfo* pSDItemInfoInRow = NULL;
	CString strKeyName;
	CString strData;
	int nRowCount = m_GridSTList.GetRowCount();
	strData.Format("%d",nRowCount-1);
	::WritePrivateProfileString(STRATEGYLIST,STRATEGYCNT,strData,strSTPath);
	
	int nIndex = 0;
	CString strGrid;
	CString szTitle;
	CString szTime;
	int nEndPos = 0;
	int nType = 0;
	for(int nRow=1;nRow<nRowCount;nRow++)
	{
		nIndex = nRow - 1;
		//pSDItemInfoInRow = (CSDItemInfo*)m_GridSTList.GetItemData(nRow,COL_INFOPOINTER);
		strGrid = m_GridSTList.GetItemText(nRow,COL_INFOPOINTER);
		nEndPos = strGrid.Find('(');
		if(nEndPos<0) szTitle = strGrid;
		else szTitle = strGrid.Left(nEndPos);
		szTime = m_GridSTList.GetItemText(nRow,COL_TIME);
		//if(pSDItemInfoInRow)
		//{
			strKeyName.Format("%s%d",STRATEGYPOS,nIndex);
			strData.Format("%d",nIndex);
			::WritePrivateProfileString(STRATEGYLIST,strKeyName,strData,strSTPath);

			strKeyName.Format("%s%d",STRATEGYPOSNAME,nIndex);
			strData= szTitle;
			::WritePrivateProfileString(STRATEGYLIST,strKeyName,strData,strSTPath);

			strKeyName.Format("%s%d",STRATEGYPOSTYPE,nIndex);
			nType = (int)m_GridSTList.GetItemData(nRow, COL_INFOPOINTER);
			strData.Format("%d",nType);
			::WritePrivateProfileString(STRATEGYLIST,strKeyName,strData,strSTPath);

			strKeyName.Format("%s%d",STRATEGYPOSTIME,nIndex);
			strData.Format("%s",szTime);
			::WritePrivateProfileString(STRATEGYLIST,strKeyName,strData,strSTPath);

			strKeyName.Format("%s%d",STRATEGYPOSTEXT,nIndex);
			strData = strGrid;
			::WritePrivateProfileString(STRATEGYLIST,strKeyName,strData,strSTPath);

			SaveVariable(strGrid, nIndex, strSTPath);
		//}
	}

	CString strLogic = m_ctrlLogic.GetLogic();
	::WritePrivateProfileString(STRATEGYLIST,STRATEGYLOGIC,strLogic,strSTPath);
}

void CDlgStrategyList::LoadST(LONG lpData)
{
	m_nFlagCnt++;
	if(m_nFlagCnt==1) {
		GetDlgItem(IDC_BTN_STCONFIGDLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STRUN)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BTN_STCONFIGDLG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STRUN)->EnableWindow(FALSE);
	}

	STLISTITEMDATA* pDataItem = (STLISTITEMDATA*)lpData;

	// 현재 조합그리드에 초기화 상태인가(처음 or 추가)
	int nCurSTListCount = m_GridSTList.GetRowCount() - 1;
	if(nCurSTListCount+1>ST_MAX_ROW)
	{
		CString szMsg;
		szMsg.Format("최대 %d개까지 가능합니다.(1)", ST_MAX_ROW);
		AfxMessageBox(szMsg);
		return;
	}

	// ST파일 전체경로 만듦
	CString strMessage;
	CString strSTPath = pDataItem->szSTPath;
	strSTPath += ".ST";

	// ST파일 Read
	char szBuffer[256];
	::GetPrivateProfileString(STRATEGYLIST, STRATEGYCNT, "0", szBuffer, sizeof(szBuffer), strSTPath);
	int nIndexCount = atol(szBuffer);

	if(nCurSTListCount+nIndexCount>ST_MAX_ROW)
	{
		CString szMsg;
		szMsg.Format("최대 %d개까지 가능합니다.(2)", ST_MAX_ROW);
		AfxMessageBox(szMsg);
		return;
	}

	CString strKeyName;
	CString strKeyData;
	int nSTPos = 0;
	int nType = 0;
	CString strGrid;	// 그리드(리스트)에 보여지는 타이틀 그대로
	CString strSfgName;
	CSDItemInfo * pSDItemInfo = NULL;
	int nGridRow;
	BOOL bResultOfAddRowData = FALSE;
	for(int nIndex=0;nIndex<nIndexCount;nIndex++)
	{
		strKeyName.Format("%s%d",STRATEGYPOS,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		nSTPos = atol(szBuffer) + nCurSTListCount;

		strKeyName.Format("%s%d",STRATEGYPOSNAME,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		strSfgName = szBuffer;

		strKeyName.Format("%s%d",STRATEGYPOSTYPE,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		nType = atol(szBuffer);

		strKeyName.Format("%s%d",STRATEGYPOSTEXT,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		strGrid = szBuffer;

		strKeyName.Format("%s%d",STRATEGYPOSTIME,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		strKeyData = szBuffer;
		nGridRow = m_GridSTList.InsertRow("", -1);

		CString strIndex;
		strIndex.Format("%c",m_chLastIndex);
		m_GridSTList.SetItemText(nGridRow,COL_NO,strIndex);
		m_GridSTList.SetItemFormat(nGridRow,COL_NO,DT_CENTER);	
		m_ctrlLogic.AddElement(m_chLastIndex);
		m_chLastIndex++;

		m_GridSTList.SetItemText(nGridRow,COL_TITLE, strGrid);
	
		m_GridSTList.SetItemText(nGridRow,COL_TIME,	strKeyData);
		m_GridSTList.SetItemFormat(nGridRow,COL_TIME,DT_CENTER);

		m_GridSTList.SetItemText(nGridRow,COL_EDIT,"편집");
		m_GridSTList.SetItemFormat(nGridRow,COL_EDIT,DT_CENTER);

		m_GridSTList.SetItemText(nGridRow,COL_DELETE,"X");
		m_GridSTList.SetItemFormat(nGridRow,COL_DELETE,DT_CENTER);

		m_GridSTList.SetItemData(nGridRow,COL_INFOPOINTER,(long)nType);
	}

	::GetPrivateProfileString(STRATEGYLIST,STRATEGYLOGIC,"",szBuffer,sizeof(szBuffer), strSTPath);
	m_ctrlLogic.SetLogic(szBuffer);

	CString szTitle = pDataItem->szSTPath;
	szTitle.Delete(0, szTitle.ReverseFind('\\') + 1);
	int nSTType = -1;
	//@Solomon @->^ 
	if((nSTType = szTitle.Find(gSTGubunKey)) != -1)
		szTitle = szTitle.Left(nSTType);

	// 디폴트 전략명을 선택한 전략명을 가지고 만든다.
	CString szText;
	m_editSTTitle.GetWindowText(szText);
	if(szText.IsEmpty())	szText = szTitle;
	else							szText += "," + szTitle;

	m_editSTTitle.SetWindowText(szText);
	if(nCurSTListCount==0)
	{
		m_szSTFilePath = strSTPath;		// 사용자폴더이면서 처음으로 폴더선택일 경우는 조합이름 세팅
		GetDlgItem(IDC_BTN_STRUN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STDELETE)->EnableWindow(TRUE);
	}
	GetDlgItem(IDC_BTN_STDELETE)->EnableWindow(nCurSTListCount!=1);

	// 추가된 전략 Selecting
	OnClickGridSTList(nGridRow, 1);
	m_GridSTList.SetSelectedRange(nGridRow,0,nGridRow,4);


	ResizeAll();
}

// 전략을 처음 로딩하거나, 사용자 전략을 로딩할 경우에는 전략이 로딩된 처리를 한다.
// 이미 전략이 로딩된 상태에서 추가적인 전략의 로딩은 add 기능으로 한다.
// 전략을 조합하면 저장해야 전략설정을 할 수 있다.
/*
void CDlgStrategyList::LoadST(LONG lpData)
{
	m_nFlagCnt++;
	if(m_nFlagCnt==1) {
		GetDlgItem(IDC_BTN_STCONFIGDLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STRUN)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BTN_STCONFIGDLG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STRUN)->EnableWindow(FALSE);
	}

	STLISTITEMDATA* pDataItem = (STLISTITEMDATA*)lpData;

	// 현재 조합그리드에 초기화 상태인가(처음 or 추가)
	int nCurSTListCount = m_GridSTList.GetRowCount() - 1;

	// ST파일 전체경로 만듦
	CString strMessage;
	CString strSTPath = pDataItem->szSTPath;
	strSTPath += ".ST";

	// ST파일 Read
	char szBuffer[256];
	::GetPrivateProfileString(STRATEGYLIST, STRATEGYCNT, "0", szBuffer, sizeof(szBuffer), strSTPath);
	int nIndexCount = atol(szBuffer);

	CString strKeyName;
	CString strKeyData;
	int nSTPos = 0;
	int nType = 0;
	CString strGrid;	// 그리드(리스트)에 보여지는 타이틀 그대로
	CString strSfgName;
	CSDItemInfo * pSDItemInfo = NULL;
	BOOL bResultOfAddRowData = FALSE;
	for(int nIndex=0;nIndex<nIndexCount;nIndex++)
	{
		strKeyName.Format("%s%d",STRATEGYPOS,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		nSTPos = atol(szBuffer) + nCurSTListCount;

		strKeyName.Format("%s%d",STRATEGYPOSNAME,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		strSfgName = szBuffer;

		strKeyName.Format("%s%d",STRATEGYPOSTYPE,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		nType = atol(szBuffer);

		strKeyName.Format("%s%d",STRATEGYPOSTEXT,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		strGrid = szBuffer;

		bResultOfAddRowData = AddRowData(strSTPath, strSfgName, nType, nSTPos);
		if(!bResultOfAddRowData)
		{
			OnBtnInit();
			return;
		}

		strKeyName.Format("%s%d",STRATEGYPOSTIME,nIndex);
		::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
		strKeyData = szBuffer;
		if(strKeyData.GetLength())
		{
			pSDItemInfo = (CSDItemInfo *)m_GridSTList.GetItemData(nIndex+1,COL_INFOPOINTER);
			pSDItemInfo->m_strTimeInfo = szBuffer;
			m_GridSTList.SetItemText(nIndex+1,COL_TIME,pSDItemInfo->m_strTimeInfo);
		}		
		//m_GridSTList.SetItemText(nIndex+1,COL_TITLE, strGrid);
	}

	::GetPrivateProfileString(STRATEGYLIST,STRATEGYLOGIC,"",szBuffer,sizeof(szBuffer), strSTPath);
	m_ctrlLogic.SetLogic(szBuffer);

	CString szTitle = pDataItem->szSTPath;;
	szTitle.Delete(0, szTitle.ReverseFind('\\') + 1);
	int nSTType = -1;
	if((nSTType = szTitle.Find(gSTGubunKey)) != -1)
		szTitle.Delete(nSTType, szTitle.GetLength());

	// 디폴트 전략명을 선택한 전략명을 가지고 만든다.
	CString szText;
	m_editSTTitle.GetWindowText(szText);
	if(szText.IsEmpty())	szText = szTitle;
	else					szText += "," + szTitle;

	m_editSTTitle.SetWindowText(szText);
	if(nCurSTListCount==0)
	{
		m_szSTFilePath = strSTPath;		// 사용자폴더이면서 처음으로 폴더선택일 경우는 조합이름 세팅
		GetDlgItem(IDC_BTN_STRUN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STDELETE)->EnableWindow(TRUE);
	}
}
*/

/* ------------------------------------------------------------------------------------
// 전략을 ST파일에 저장한다. 저장시 현재 파일을 똑같이 임시파일에 저장한다.
// 저장시 ST파일에 임
// 내용이 다 저장되지 않은 상태에서 "전략적용"을 누를 경우에는 임시파일의 내용을 가지고 전략을 실행한다.
------------------------------------------------------------------------------------ */
void CDlgStrategyList::OnBtSave() 
{
	CString strText;
	CString szSTFilePath;
	CString szSelSTPath;
	CString	szRunImsiFile;

	m_editSTTitle.GetWindowText(strText);
	if(strText.IsEmpty()) {
		AfxMessageBox("선택된 전략이름이 없습니다. 확인바랍니다");
		return;
	}

	//@Solomon	if(strText.Find("@")>=0)
	if(strText.Find("^")>=0 || strText.Find("@")>=0)
	{
		//@Solomon AfxMessageBox("전략명에는 ^\'@\' 문자는 들어갈 수 없습니다.");
		AfxMessageBox("전략명에는 \'@\' 문자는 들어갈 수 없습니다.");
		return;
	}

	szSelSTPath.Format("%s%s", m_szUserDir, STDATA_ST_USERCOMBINE);
	szSTFilePath.Format("%s%s\\%s%s", m_szUserDir, STDATA_ST_USERCOMBINE, strText, ".ST");

	int nRet = 0;
	if(nRet = CheckFileSave(TRUE, szSTFilePath))
	{
		m_szSTFilePath = szSTFilePath;
		SaveST(strText, FALSE);

		BOOL bUnit = m_szSTFilePath.Find("\\사용자단위전략\\") >= 0	? TRUE : FALSE;
		BOOL bComb = m_szSTFilePath.Find("\\사용자조합전략\\") >= 0 ? TRUE : FALSE;

		GetDlgItem(IDC_BTN_STCONFIGDLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STRUN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STDELETE)->EnableWindow(TRUE);

		if(bUnit)
		{
			if(nRet != 3)
			{
				//((CDlgStrategyConfigure*)GetParent())->UpdateSTList(Ext_STChange_SDAdd, strText);
				NotifySTChangeState(Ext_STChange_SDAdd, strText);
			}
		}
		else if(bComb)
		{

			if(nRet != 3)
			{
				//((CDlgStrategyConfigure*)GetParent())->UpdateSTList(Ext_STChange_Add, strText);
				//((CDlgStrategyConfigure*)GetParent())->SelectSTItem(strText, szSelSTPath);
				NotifySTChangeState(Ext_STChange_Add, strText);//하위에 알려준다.
			}
		}

		m_nFlagCnt = 1;
	}
}
/*
void CDlgStrategyList::InitLoad()
{
	int nID = UUID_IAUDataMonitorManager; 
	m_pIAUDataMonitorManager = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
	CString szKey = "STLIST_MODIFIED"; 
	m_dwIAUDataMonitorManager = m_pIAUDataMonitorManager->Advise(szKey, (long)this, (long)NULL, DataMonitorCallbackProc);

}
void CDlgStrategyList::DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData)
{
		CDlgStrategyList* pThis = (CDlgStrategyList*)pThisPoint;
		CString strKey = szKey;

		if(strKey == "STLIST_MODIFIED") 
		{
			LPCSTR szSTName = (LPCSTR)dwNotifyData;
			//pThis->UpdateSTFileName(pThis->GetParent(), szSTName);
			pThis->UpdateSTFileName(NULL, szSTName);
		}
} 

void CDlgStrategyList::UpdateSTFileName(CWnd* pParent, LPCSTR szSTName)
{
	//((CDlgStrategyConfigure*)pParent)->UpdateSTList(Ext_STChange_SDAdd, szSTName);//죽는현상..:KHD 승룡씨..
	((CDlgStrategyConfigure*)GetParent())->UpdateSTList(Ext_STChange_SDAdd, szSTName);//죽는현상..:KHD 승룡씨..
	//((CDlgStrategyConfigure*)GetParent())->SelectSTItem(strText, szSelSTPath);
	//NotifySTChangeState(Ext_STChange_SDAdd, szSTName);//하위에 알려준다.
}
*/

//기존에 같은 전략명이 있는지 확인한다. 있을 경우 사용자의 확인을 받는다.
int CDlgStrategyList::CheckFileSave(BOOL bCheck, CString& szSTFilePath)
{
	if(szSTFilePath.GetLength()>MAX_PATH) 
	{
		AfxMessageBox("파일명의 길이가 너무 깁니다. 전략명을 줄여주세요.");
		return 0;
	}

	if(!bCheck) return 1;

	CFileFind fileFind;
	BOOL bFound = fileFind.FindFile(szSTFilePath);

	if(!bFound) return 2;

	int nRet = AfxMessageBox("같은 전략명이 이미 존재합니다. \n 기존 파일을 덮어쓰시겠습니까?", MB_YESNO);
	if(IDYES==nRet)	return 3;

	return 0;
}


void CDlgStrategyList::OnBtnDelete() 
{
	m_nFlagCnt = 0;
	CCellRange cellRange = m_GridSTList.GetSelectedCellRange();
	OnClickGridSTList(cellRange.GetMaxRow(),COL_DELETE);

	GetDlgItem(IDC_BTN_STCONFIGDLG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STRUN)->EnableWindow(FALSE);
}

void CDlgStrategyList::OnBtnEdit() 
{
	CCellRange cellRange = m_GridSTList.GetSelectedCellRange();
	OnClickGridSTList(cellRange.GetMaxRow(),COL_EDIT);	
}

HBRUSH CDlgStrategyList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
	}
	
	return hbr;
}

int CDlgStrategyList::GetGridSTRowCount()
{
	if(m_GridSTList.GetSafeHwnd())
		return m_GridSTList.GetRowCount();

	return 0;
}

BOOL CDlgStrategyList::PreTranslateMessage(MSG* pMsg) 
{
	// 엔터가 쳤을때 죽는 현상 방지		JSJ_Modify_040421
	if(pMsg->message == WM_KEYDOWN)   
	{
		if(pMsg->wParam == VK_F4 && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			pMsg->wParam = VK_ESCAPE;
			return GetParent()->PreTranslateMessage(pMsg);
		}
		else
			return m_InputList.PreTranslateMessage(pMsg);
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}
	else if(pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgStrategyList::OnBtnSTDelete() 
{
	BOOL bUnit = m_szSTFilePath.Find("\\사용자단위전략\\") >= 0	? TRUE : FALSE;
	BOOL bComb = m_szSTFilePath.Find("\\사용자조합전략\\") >= 0 ? TRUE : FALSE;
	BOOL bDeletable = bUnit || bComb;

	CString szTitle;
	m_editSTTitle.GetWindowText(szTitle);

	// 사용자 단일/조합 전략 Path 필요
	CString szSTPath(m_szSTFilePath);
	if(bDeletable && !szSTPath.IsEmpty()) 
	{
		if(AfxMessageBox("전략을 삭제하시겠습니까?", MB_YESNO)==IDNO)
			return;

		m_szSTFilePath.Empty();
		::DeleteFile(szSTPath);
		OnBtnInit();

		if(bUnit)
		{
			szTitle = szSTPath;
			int nPosExt = szSTPath.ReverseFind('.');
			int nPosFileTitle = szSTPath.ReverseFind(('\\'));
			szSTPath = szSTPath.Left(nPosExt);
			szTitle = szSTPath.Mid(nPosFileTitle+1);
			
			//((CDlgStrategyConfigure*)GetParent())->UpdateSTList(Ext_STChange_SDDelete, szTitle);
			NotifySTChangeState(Ext_STChange_SDDelete, szTitle);
		}
		else if(bComb)
		{
			//((CDlgStrategyConfigure*)GetParent())->UpdateSTList(Ext_STChange_Delete, szTitle);
			NotifySTChangeState(Ext_STChange_Delete, szTitle);
		}
	}
}

void CDlgStrategyList::OnBnClickedSTConfigDlg()
{
	CString szTitle;
	m_editSTTitle.GetWindowText(szTitle);

	m_hViewHandle = (HWND)((CDlgStrategyConfigure*)GetParent())->m_pSTRunMng->GetViewHandle();

//	typedef	int (FnShowConfigureDlgRunMain)(LPCSTR szSTFile, LPCSTR szSTName);
	typedef	int (FnShowConfigureDlgRunMainEx)(HWND hViewHandle, LPCSTR szSTFile, LPCSTR szSTName);
	CString szDllName;
	//@Solomon szDllName.Format("%s%s", m_strRootPath, "\\Chart\\STConfigureDlg.dll");	// DLL명만 넣는다.
	szDllName.Format("%s%s", m_strRootPath, "\\dev\\STConfigureDlg.dll");	// DLL명만 넣는다.
	HINSTANCE hTradeInst = LoadLibrary(szDllName);
	if(hTradeInst == NULL) return;

//	FnShowConfigureDlgRunMain* pFunc = (FnShowConfigureDlgRunMain*)GetProcAddress(hTradeInst,_T("fnShowConfigureDlgRunMain"));
	FnShowConfigureDlgRunMainEx* pFunc = (FnShowConfigureDlgRunMainEx*)GetProcAddress(hTradeInst,_T("fnShowConfigureDlgRunMainEx"));
	if(pFunc == NULL) return ;

	//CString szSTPath = m_szUserDir + STDATA_ST_USERCOMBINE + "\\" + szTitle + ".ST";
	//m_szSTFilePath;

	//CString szUserPath = szSTPath;//m_szSTFilePath;
	CString szUserPath = m_szSTFilePath;
	CString szSTName = szTitle;
	int nRet = pFunc(m_hViewHandle, szUserPath, szSTName);
	FreeLibrary(hTradeInst);
}


void CDlgStrategyList::OnBnClickedSTClose()
{
	// 
	GetParent()->GetParent()->SendMessage(WM_COMMAND, IDCANCEL, 0);
}

void CDlgStrategyList::OnBnClickedSTRun()
{
//	//주문정보 가져오기 : KHD 
//	IMainInfoManager* pMainMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//	short     nHTSKey = atoi(pMainMng->GetMainData("ISJUMUN",NULL));
//	CString szPW        = pMainMng->GetMainData("PW",NULL);
//	//END
//	if(nHTSKey==1 || nHTSKey ==2)
//	{
//		if(szPW.GetLength()==0)
//		{
//			AfxMessageBox("해당 전략의 계좌 비밀번호가 입력되지 않았습니다. \n전략 설정창에서 비밀번호를 다시 입력해주세요");
//			return;
//		}
//	}
	if(SaveRunTempFile()==FALSE) return;

	//((CDlgStrategyConfigure*)GetParent())->RunST(TRUE, szRunImsiFile);
	((CDlgStrategyConfigure*)GetParent())->RunST(TRUE, m_szSTFilePath);
	SetTimer(TID_KILL_DIALOG, 300, NULL);
}

void CDlgStrategyList::NotifySTChangeState(int nState, LPCSTR szSTName)
{
	// 전략리스트가 변경되었다는 내용을 알린다.
	int nID = UUID_IAUDataMonitorManager;
	IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
	//pMng->NotifyData("STLIST_MODIFIED", NULL, NULL);
	pMng->NotifyData("STLIST_MODIFIED", nState, (long)szSTName);
}

void CDlgStrategyList::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==TID_KILL_DIALOG)
	{
		KillTimer(nIDEvent);
		OnBnClickedSTClose();
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgStrategyList::OnBnClickedBtnSystemtrading()
{
	((CDlgStrategyConfigure*)GetParent())->LoadEditor("");
}

// 실제 전략에다가 저장한다.
// 실제 파일은 "저장"시에만 저장되며, "전략적용"시에 저장된 ST파일은 전략실행이 끝난후에 ChartSTAddinIndex에의해 다시 Backup처리된다.
BOOL CDlgStrategyList::SaveRunTempFile()
{
	CString strText;

	m_editSTTitle.GetWindowText(strText);
	if(strText.IsEmpty()) {
		AfxMessageBox("선택된 전략이름이 없습니다. 확인바랍니다");
		return FALSE;
	}

	CString szRunImsiFile;
	szRunImsiFile.Format("%s%s%s%s", m_szUserDir, "\\STData\\Temp\\", strText, ".ST");
	::WritePrivateProfileString(STRATEGYLIST, "IMSIST", szRunImsiFile, m_szSTFilePath);
	::CopyFile(m_szSTFilePath, szRunImsiFile, FALSE);

	SaveST(strText, TRUE);

	// 제공된 전략은 Temp폴더의 ST파일 복사본에서 설정관련 정보는 삭제시킴
	if(m_szSTFilePath.Find("\\사용자단위전략\\") < 0 && m_szSTFilePath.Find("\\사용자조합전략\\") < 0)
	{
		::WritePrivateProfileString(STCONFIG_SIGNAL, NULL, NULL, szRunImsiFile);
		::WritePrivateProfileString(STCONFIG_ORDER, NULL, NULL, szRunImsiFile);
		::WritePrivateProfileString(STCONFIG_CLEAR, NULL, NULL, szRunImsiFile);
		::WritePrivateProfileString(STCONFIG_SETUP, NULL, NULL, szRunImsiFile);
	}

	return TRUE;
}

// ST파일에 전략의 변수들을 저장한다.
void CDlgStrategyList::SaveVariable(CString& strGrid, int nIndex, LPCSTR strSTPath)
{
	CString szTmp;
	int nFind = CHelperSTTitle::GetSTName(strGrid, szTmp);
	if(nFind<0) return;

	CString szInputStr = strGrid.Mid(nFind+1);
	szInputStr.Remove(')');

	CString szInput, strKeyName, strSection;
	strSection.Format("%s%d","Variables",nIndex);

	int i=0;
	nFind = szInputStr.Find(',');
	while(nFind>0)
	{
		szInput = szInputStr.Left(nFind);
		szInputStr = szInputStr.Mid(nFind+1);

		strKeyName.Format("%s%d", "Input", i);
		::WritePrivateProfileString(strSection,strKeyName,szInput,strSTPath);
		i++;

		nFind = szInputStr.Find(',');
	}

	strKeyName.Format("%s%d", "Input", i);
	::WritePrivateProfileString(strSection,strKeyName,szInputStr,strSTPath);

	szTmp.Format("%d", i+1);
	::WritePrivateProfileString(strSection,"InputCnt",szTmp,strSTPath);
}

/* -------------------------------------------------------------------------------
// 현재 선택된 정보를 가지고 입력정보를 수정한다.
//
------------------------------------------------------------------------------- */
void CDlgStrategyList::InputModify(int nRow, CString& szTitle)
{
	SetDlgItemText(IDC_EDIT_STNAME, "");
	m_InputList.DeleteAllItems();

	CString szSTName;
	CStringArray inArr;

	int nFind = CHelperSTTitle::GetSTName(szTitle, szSTName);
	if(nFind<0)
		return;

	szTitle = szTitle.Mid(nFind+1);
	szTitle.Remove(')');

	SetDlgItemText(IDC_EDIT_STNAME, szSTName);
	
	int nCnt = CHelperSTTitle::GetInputData(szTitle, inArr);

	// 오른쪽 리스트에 변수 변경 적용.
	int nType = (int)m_GridSTList.GetItemData(nRow, COL_INFOPOINTER);
	CString strPath;
	if(nType==SYSTEM_ST)
	{
		strPath = m_szDataDir + STDATA_SD_RELEASE;
	}
	else if(nType==USER_ST)
	{
		strPath = m_szUserDir + STDATA_SD_RELEASE;
	}
	CString szSFG;
	szSFG.Format("%s\\%s%s", strPath, szSTName, ".SFG");

	LPCSTR szSection = "Variables";
	CString szKey;
	char aTmp[MAX_PATH]={0,};
//	int nCnt = ::GetPrivateProfileInt(szSection, "InputCnt", 0, szSFG);
	int nAddRow;
	for(int i=0; i<nCnt; i++)
	{
		szKey.Format("InputName%d", i);
		::GetPrivateProfileString(szSection, szKey, "", aTmp, MAX_PATH, szSFG);

		// 리스트의 0 Column에 값을 세팅한다.
		 nAddRow = m_InputList.InsertItem(m_InputList.GetItemCount(), "");
		 m_InputList.SetItemText(nAddRow, 0, aTmp);
		 m_InputList.SetItemText(nAddRow, 1, inArr.GetAt(i));

		 m_InputList.SetEditBox(nAddRow,1);
	}
}



void CDlgStrategyList::OnBnClickedButtonSaveconfig()
{
	CString szSTName;
	CStringArray inArr;
	int nCount = m_InputList.GetItemCount();

	for(int i=0; i<nCount;i++)
	{
		inArr.Add(m_InputList.GetItemText(i, 1));
	}

	GetDlgItemText(IDC_EDIT_STNAME, szSTName);
	CString szStr = CHelperSTTitle::MakeTitle(szSTName, inArr);

	// 왼쪽의 그리드를 갱신한다.
	m_InputList.UnpressEditBox();
	CCellRange cellRange = m_GridSTList.GetSelectedCellRange();
	int nModifyRow = cellRange.GetMaxRow();

	m_GridSTList.SetItemText(nModifyRow,COL_TITLE, szStr);
	ResizeAll();	
}

void CDlgStrategyList::InitColumn()
{
	int colwidthsType0[6] = { 90, 127};	// sixty-fourths

	TCHAR *	lpszHeadersType0[] = {
		_T("변수명"),
			_T("변수값"),
			NULL };
		m_InputList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

		int i;
		int total_cx = 0;
		LV_COLUMN lvcolumn;
		memset(&lvcolumn, 0, sizeof(lvcolumn));
		// add columns
		for (i = 0; ; i++)
		{
			if (lpszHeadersType0[i] == NULL)
				break;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			lvcolumn.pszText = lpszHeadersType0[i];
			lvcolumn.iSubItem = i;
			lvcolumn.cx = colwidthsType0[i];
			total_cx += lvcolumn.cx;
			m_InputList.InsertColumn(i, &lvcolumn);
		}
		// iterate through header items and attach the image list
		memset(&lvcolumn, 0, sizeof(lvcolumn));
		// set the format again - must do this twice or first column does not get set
		for (i = 0; ; i++)
		{
			if (lpszHeadersType0[i] == NULL)
				break;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
			lvcolumn.fmt = LVCFMT_CENTER;
			lvcolumn.iSubItem = i;
			m_InputList.SetColumn(i, &lvcolumn);
		}
		m_InputList.SetHilightColor(RGB(0,0,0),RGB(208,221,240));
}
