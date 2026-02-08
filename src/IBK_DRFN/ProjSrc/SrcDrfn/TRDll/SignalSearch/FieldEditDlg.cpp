// FieldEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FieldEditDlg.h"

#include "MainFrmWnd.h"
//#include "ConditionResultWnd.h"
#include "./AddControls/Grid_Field.h"
#include "../../../SrcSite/include/axis_msg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DLG_WIDTH				500
#define DLG_HEIGHT				350

#define GRID_SOURCE_ROW			12
#define	MAX_FIXCOL				5

#define	OFFSET_COL_INDEX		100

#define TAB_INDEX				7
/*#define INDEX_OF_FULL			65*/
#define INDEX_OF_FULL			64

#define SOURCE_GRID_HEAD_WIDTH 129

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CFieldEditDlg dialog

CFieldEditDlg::CFieldEditDlg(CWnd* pParent, int nMoveFieldCnt)
: CDialog(CFieldEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFieldEditDlg)
	//}}AFX_DATA_INIT

	m_pwndMain = pParent;
	m_nMoveFieldCnt = nMoveFieldCnt;

	m_pgridSource = NULL;
	m_pgridDest = NULL;
	m_pTab = NULL;
	m_bInsertState = FALSE;
	m_bRedraw = TRUE;
	m_nGridSourceRow = GRID_SOURCE_ROW;
}

void CFieldEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFieldEditDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFieldEditDlg, CDialog)
	//{{AFX_MSG_MAP(CFieldEditDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_UPPOS, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_TOPPOS, OnBtnTop)
	ON_BN_CLICKED(IDC_BTN_DOWNPOS, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_BOTTOMPOS, OnBtnBottom)
	ON_BN_CLICKED(IDC_BTN_INIT, OnBtnSetInit)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FIXLINE, OnDeltaposSpin)

	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)

	ON_EN_KILLFOCUS(IDC_EDIT_FIXCOL, OnKillFocusFixColEdit)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnChangeTab)

	ON_MESSAGE(WM_NOTIFY_GRIDEVENT, OnNotifyGrid)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//###########################################################################
// CFieldEditDlg message handlers
//###########################################################################
int CFieldEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	m_brushBk.CreateSolidBrush(COLOR_DLG_CANVAS);

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pwndMain;

	if (m_pgridSource == NULL)
		m_pgridSource = new CGrid_Field(this, "FieldEditSource", pMainFrmWnd->GetAxColor(74));

	m_pgridSource->CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, ID_GRID_FIELD_SOURCE);

	m_btnUp.Create(_T("위로"), CRect(0, 0, 0, 0), this, IDC_BTN_UPPOS);
	pMainFrmWnd->SetBtnImg(&m_btnUp, 4);

	m_btnTop.Create(_T("맨위로"), CRect(0, 0, 0, 0), this, IDC_BTN_TOPPOS);
	pMainFrmWnd->SetBtnImg(&m_btnTop, 4);

	m_btnDown.Create(_T("아래"), CRect(0, 0, 0, 0), this, IDC_BTN_DOWNPOS);
	pMainFrmWnd->SetBtnImg(&m_btnDown, 4);

	m_btnBottom.Create(_T("맨아래"), CRect(0, 0, 0, 0), this, IDC_BTN_BOTTOMPOS);
	pMainFrmWnd->SetBtnImg(&m_btnBottom, 4);

	if (m_pgridDest == NULL)
		m_pgridDest = new CGrid_Field(this, "FieldEditDest", pMainFrmWnd->GetAxColor(74));
	m_pgridDest->CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, ID_GRID_FIELD_DEST);

	m_ctrlTitle.Create(_T("좌우스크롤시 필드 고정수"), 
		WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlTitle.SetFont(&m_fontDefault);

	m_editFixCol.Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_FIXCOL);
	m_editFixCol.SetFont(&m_fontDefault);

	m_btnSpin.Create(WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this, IDC_SPIN_FIXLINE);

	m_btnSetInit.Create(_T("기본값 복원"), CRect(0, 0, 0, 0), this, IDC_BTN_INIT);
	pMainFrmWnd->SetBtnImg(&m_btnSetInit, 6);

	m_btnOk.Create(_T("확인"), CRect(0, 0, 0, 0), this, IDC_BTN_OK);
	pMainFrmWnd->SetBtnImg(&m_btnOk, 4);

	m_btnApply.Create(_T("적용"), CRect(0, 0, 0, 0), this, IDC_BTN_APPLY);
	pMainFrmWnd->SetBtnImg(&m_btnApply, 4);

	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDC_BTN_CANCEL);
	pMainFrmWnd->SetBtnImg(&m_btnCancel, 4);

	m_pTab = new CDrTabCtrl();
	m_pTab->Create(0, this, IDC_TAB);
	m_pTab->SetCurSel(0);

	TC_ITEM item;
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	for(int i = 0; i < TAB_INDEX; i++)
	{
		item.pszText = (LPSTR)FIELDEDIT_TAB_STR[i];
		m_pTab->InsertItem(i, &item);
	}

	((CMainFrmWnd*)m_pwndMain)->GetTabClr(&m_clrBkgnd, &m_stTabColor);
	m_pTab->SetTabColor(m_clrBkgnd, m_clrBkgnd, m_stTabColor.clrB, NULL, NULL, m_clrBkgnd);
 	m_pTab->SetTextColor(m_stTabColor.clrSF, m_stTabColor.clrSF);

	AfxSetResourceHandle(hInstSave);

	CRect reClient;
	GetWindowRect(reClient);

	reClient.right = reClient.left + (int)DLG_WIDTH;
	reClient.bottom = reClient.top + (int)DLG_HEIGHT;
	MoveWindow(reClient);

	return 0;
}	

void CFieldEditDlg::OnDestroy() 
{
	//	CDialog::OnDestroy();

	if (m_pgridSource != NULL)
	{
		m_pgridSource->DestroyWindow();
		delete m_pgridSource;
		m_pgridSource = NULL;
	}

	if (m_pgridDest != NULL)
	{
		m_pgridDest->DestroyWindow();
		delete m_pgridDest;
		m_pgridDest = NULL;
	}

	if(m_pTab)
	{
		delete m_pTab;
		m_pTab = NULL;
	}

	m_fontDefault.DeleteObject();
	m_brushBk.DeleteObject();

	delete m_plistTempHeader;
	m_plistTempHeader = NULL;

	CDialog::OnDestroy();
}


void CFieldEditDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;	
	
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, pwndMainFrm->m_crBackColor);

	dc.FillSolidRect(m_rtTab, pwndMainFrm->GetAxColor(66));

	HPEN hOldPen;
	hOldPen = (HPEN)dc.SelectObject(pwndMainFrm->GetAxPen(pwndMainFrm->GetAxColor(13), 1, PS_SOLID));
	
	MoveToEx(dc.m_hDC, m_rtTab.right + 1, m_rtTab.top, NULL);		// 오른쪽 테두리
	LineTo(dc.m_hDC, m_rtTab.right + 1,  m_rtTab.bottom);
	
	MoveToEx(dc.m_hDC, m_rtTab.right + 1, m_rtTab.bottom, NULL);	// 아래 테두리
	LineTo(dc.m_hDC, m_rtTab.left - 1, m_rtTab.bottom);
	
	MoveToEx(dc.m_hDC, m_rtTab.left - 1, m_rtTab.bottom, NULL);			// 왼쪽 테두리
	LineTo(dc.m_hDC, m_rtTab.left - 1, m_rtTab.top);
	dc.SelectObject(hOldPen);

// 	m_pTab->SetBKColor(COLOR_MAIN_CANVAS);	
// 	m_pTab->SetTabColor(COLOR_MAIN_CANVAS, stTabColor.clrTabSelGraUp, stTabColor.clrTabUnSelGraUp, stTabColor.clrTabSelGraDown, stTabColor.clrTabUnSelGraDown, stTabColor.clrTabBody);
// 	m_pTab->SetTextColor(stTabColor.clrTabSelText, stTabColor.clrTabUnSelText);
// 	Invalidate();
}

HBRUSH CFieldEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		//2009.10.08 by LYH >> window7 테마 지원(고대비검정) <<
		pDC->SetTextColor(RGB(0,0,0));
		hbr = m_brushBk;
	}

	if(m_ctrlTitle.GetDlgCtrlID() == pWnd->GetDlgCtrlID())
	{
		pDC->SetBkMode(TRANSPARENT);
		CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pwndMain;
		hbr = pMainFrmWnd->GetAxBrush(pMainFrmWnd->m_crBackColor);
	}

	return hbr;
}

void CFieldEditDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect reClient, reBottom, reTemp;
	GetClientRect(reClient);

	reBottom = reClient;

	reBottom.right -= 5;
	reBottom.left += 5;

	reBottom.bottom -= 7;
	reBottom.top = reBottom.bottom - 20;

	reTemp = reBottom;

	reTemp.top += 2;
	reTemp.bottom += 2;

	reTemp.left = reTemp.right - 60;
	m_btnCancel.MoveWindow(reTemp);

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnOk.MoveWindow(reTemp);

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnApply.MoveWindow(reTemp);

	reTemp.right = reTemp.left - 3;
	reTemp.left = reTemp.right - 84;
	m_btnSetInit.MoveWindow(reTemp);

	reTemp = reBottom;

	reTemp.top += 6;
	reTemp.bottom -= 1;
	reTemp.right = reTemp.left + 150;
	m_ctrlTitle.MoveWindow(reTemp);

	reTemp.top -= 4;
	reTemp.bottom += 3;

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 30;
	m_editFixCol.MoveWindow(reTemp);

	reTemp.left = reTemp.right;
	reTemp.right = reTemp.left + 15;
	m_btnSpin.MoveWindow(reTemp);


	reBottom.bottom = reBottom.top - 4;
	reBottom.top = reClient.top + 5;

	reBottom.left = reClient.left + 5;
	reBottom.right = reClient.right - 5;
	reTemp = reBottom;

	/*
	if (m_pgridDest->GetSafeHwnd() != NULL)
	{
	reTemp.right = reTemp.left + 150;
	m_pgridDest->MoveWindow(reTemp);
	}

	reTemp.left = reTemp.right;
	reTemp.right = reTemp.left + GetBtnWidth(3) + 8;
	OnSizePosBar(reTemp);

	if (m_pgridSource->GetSafeHwnd() != NULL)
	{
	reTemp.left = reTemp.right;
	reTemp.right = reBottom.right;
	m_pgridSource->MoveWindow(reTemp);
	}
	*/

	if(m_pTab->GetSafeHwnd() != NULL)
	{
		m_rtTab = reTemp;
		m_rtTab.top += 20;

		CRect reTap;
		reTap = reTemp;
		reTap.bottom = reTap.top + 22;
		reTap.left -= 1;
		reTap.right += 2;
	//	reTap.bottom += 1;

		m_pTab->SetWindowPos(&wndBottom, reTap.left, reTap.top, reTap.Width(), reTap.Height(), SWP_SHOWWINDOW);
	}

	reTemp.bottom -= 5;
	if (m_pgridDest->GetSafeHwnd() != NULL)
	{
		reTemp.left = reTemp.right - 150;
		reTemp.right -= 4;

		reTemp.top += 29;

		m_pgridDest->MoveWindow(reTemp);
	}

	reTemp.right = reTemp.left - 1;
	reTemp.left = reTemp.right - 60 - 8;
	OnSizePosBar(reTemp);

	if (m_pgridSource->GetSafeHwnd() != NULL)
	{
		reTemp.right = reTemp.left - 1;
		reTemp.left = reBottom.left + 4;
		m_pgridSource->MoveWindow(reTemp);
	}
}

void CFieldEditDlg::OnSizePosBar(CRect reClient)
{
	CRect reTemp;
	reTemp = reClient;

	reTemp.left += 4;
	reTemp.right -= 4;

	int nBtnHeight = 20;
	int nTopOffset;
	nTopOffset = ((reClient.bottom - reClient.top) - (nBtnHeight * 4 + 2 * 3)) / 2;

	reTemp.top += nTopOffset;
	reTemp.bottom = reTemp.top + nBtnHeight;
	m_btnTop.MoveWindow(reTemp);

	reTemp.top = reTemp.bottom + 3;
	reTemp.bottom = reTemp.top + nBtnHeight;
	m_btnUp.MoveWindow(reTemp);

	reTemp.top = reTemp.bottom + 3;
	reTemp.bottom = reTemp.top + nBtnHeight;
	m_btnDown.MoveWindow(reTemp);

	reTemp.top = reTemp.bottom + 3;
	reTemp.bottom = reTemp.top + nBtnHeight;
	m_btnBottom.MoveWindow(reTemp);
}


BOOL CFieldEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->SetWindowText((CString)PROGRAM_TITLE + " [조회항목 편집]");

	POSITION		pos, posUser;
	HeadInfoType	stHeadInfoType;
	CString			strTemp;
	CStringArray	saGridData;
	HeadFixInfo		stHeadFixInfo;	

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;

	m_plistFixHeadUser		= &pwndMainFrm->m_listFixHeadUser;
	m_plistFixHeadSys		= &pwndMainFrm->m_listFixHeadSys;	
	m_plistSelectedHeader	= &pwndMainFrm->m_listSelectedHead;
	m_plistSelectedHeader->RemoveAll();

	int i = 0;
	for( i = 0; i < INDEX_OF_FULL; i++)
	{
		pos = m_plistFixHeadSys->FindIndex(i);
		stHeadInfoType = m_plistFixHeadSys->GetAt(pos);

		for(int j = 0; j < m_plistFixHeadUser->GetCount(); j++)
		{
			posUser = m_plistFixHeadUser->FindIndex(j);
			stHeadFixInfo = m_plistFixHeadUser->GetAt(posUser);

			if(stHeadInfoType.strHeadName == stHeadFixInfo.stHeadType.strHeadName)
			{
				m_plistSelectedHeader->AddTail(stHeadInfoType);
				break;
			}
		}
	}

	m_plistTempHeader = new CList<HeadFixInfo, HeadFixInfo&>;
	m_plistTempHeader->RemoveAll();
	for(i = 0; i < m_plistFixHeadUser->GetCount(); i++)
	{
		POSITION pos = m_plistFixHeadUser->FindIndex(i);
		stHeadFixInfo = m_plistFixHeadUser->GetAt(pos);
		m_plistTempHeader->AddTail(stHeadFixInfo);
	}
	
	OnChangeTab(NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CFieldEditDlg::GetRowField(CString strField, int nFieldID)
{
	int nResult = -1;

	CString strFieldID;
	strFieldID.Format("%d", nFieldID);

	BOOL bState = FALSE;
	CStringArray saGridData;
	for (int nRow = 0; nRow < m_nGridSourceRow; nRow++)
	{
		for (int nCol = 0; nCol < 2; nCol++)
		{
			saGridData.RemoveAll();
			//# Check / Name / Id / Dest Row
			m_pgridSource->GetData_FieldEditSource(nCol, nRow, saGridData);
			if ((strField == saGridData.GetAt(1)) && (strFieldID == saGridData.GetAt(2)))
			{
				nResult = nRow;
				if (nCol > 0)
					nResult = nRow + (int)OFFSET_COL_INDEX;
				bState = TRUE;
				break;
			}
		}

		if (bState == TRUE)
			break;
	}

	return nResult;
}

void CFieldEditDlg::OnBtnCancel() 
{
	OnCancel();
}

void CFieldEditDlg::OnBtnApply()
{
	m_plistFixHeadUser->RemoveAll();

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	//CConditionResultWnd *pwndConditionResult = (CConditionResultWnd *)pwndMainFrm->GetWnd("RESULT");

	POSITION		posUser;
	HeadFixInfo		stHeadFixInfo;
	HeadInfoType	stHeadInfoType;
	int nRow;
	int nCol = -1;
	for (nRow = 0; nRow < m_pgridDest->GetNumberRows(); nRow++)
	{
		CString szHeadName = m_pgridDest->GetCellData(nRow, 0);

		for(int i = 0; i < m_plistSelectedHeader->GetCount(); i++)
		{
			posUser = m_plistSelectedHeader->FindIndex(i);
			stHeadInfoType = m_plistSelectedHeader->GetAt(posUser);

			if(stHeadInfoType.strHeadName == szHeadName)
			{
				stHeadFixInfo.bView = TRUE;
				break;
			}
		}

		if (stHeadFixInfo.bView == TRUE)
			nCol++;

		stHeadFixInfo.nColNum = nCol;
		stHeadFixInfo.stHeadType = stHeadInfoType;

		m_plistFixHeadUser->AddTail(stHeadFixInfo);
	}

	CString strTemp;
	m_editFixCol.GetWindowText(strTemp);
	pwndMainFrm->m_nFixCol = atoi(strTemp);

	pwndMainFrm->SetFieldEditApply();

	//# Not User! => Parent to Save!
	// 	CString strFilePath = pwndMainFrm->GetFilePath("FIX_USER");
	// 	BOOL bWrite = SetSaveXmlFile(strFilePath);
}

void CFieldEditDlg::OnBtnOk() 
{
	OnBtnApply();
	OnOK();
}


BOOL CFieldEditDlg::SetSaveXmlFile(CString strFile)
{
	BOOL bResult = TRUE;

	try 
	{
		MSXML::IXMLDOMDocumentPtr pDOMIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

		MSXML::IXMLDOMProcessingInstructionPtr pPI;
		MSXML::IXMLDOMNamedNodeMapPtr					pNAPI;
		MSXML::IXMLDOMNodePtr									pNEncoding;

		MSXML::IXMLDOMNodePtr									pNIndex;
		MSXML::IXMLDOMNamedNodeMapPtr					pNAIndex;

		MSXML::IXMLDOMNodePtr									pN_FItem;
		MSXML::IXMLDOMNamedNodeMapPtr					pNA_FItem;


		pPI = pDOMIndex->createProcessingInstruction("xml", "version=\"1.0\"");

		pNEncoding = pDOMIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";

		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);

		pDOMIndex->appendChild(pPI);

		pNIndex = pDOMIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;

		MSXML::IXMLDOMNodePtr pNAttributeTemp;
		pNAttributeTemp = pDOMIndex->createAttribute("VERSION");
		pNAttributeTemp->text = (LPCTSTR)m_strVersion;
		pNAIndex->setNamedItem(pNAttributeTemp);

		pDOMIndex->appendChild(pNIndex);


		POSITION				pos;
		HeadFixInfo			stHeadFixInfo;
		CString					strTemp;

		int nColNo = -1;
		for (int nIndex = 0;  nIndex < m_plistFixHeadUser->GetCount(); nIndex++)	
		{
			// 속성 설정 - start
			pos = m_plistFixHeadUser->FindIndex(nIndex);
			stHeadFixInfo = m_plistFixHeadUser->GetAt(pos);

			MSXML::IXMLDOMNodePtr pNAttributeTemp;

			// create element 
			pN_FItem = pDOMIndex->createElement("FITEM");
			pNIndex->appendChild(pN_FItem);
			pNA_FItem = pN_FItem->attributes;

			//ID
			pNAttributeTemp = pDOMIndex->createAttribute("ID");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.lHeadID);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);


			//COL_NO
			pNAttributeTemp = pDOMIndex->createAttribute("COL_NO");
			strTemp.Format("%d", stHeadFixInfo.nColNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//FIELD_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("FIELD_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//UNIT_NUM
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_NUM");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nUnitNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//UNIT_TEXT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_TEXT");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strUnitText;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//ALIGN
			pNAttributeTemp = pDOMIndex->createAttribute("ALIGN");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strAlignment;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//COL_WIDTH
			pNAttributeTemp = pDOMIndex->createAttribute("COL_WIDTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nColWidth);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//DATA_TYPE
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_TYPE");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strDataType;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//DATA_LENGTH
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_LENGTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nDataLength);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);




			//VIEW_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("VIEW_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//VISIBLE
			pNAttributeTemp = pDOMIndex->createAttribute("VISIBLE");
			pNAttributeTemp->text =	(LPCTSTR)(stHeadFixInfo.bView == TRUE)?"TRUE":"FALSE";
			pNA_FItem->setNamedItem(pNAttributeTemp);
		}

		pDOMIndex->save(_bstr_t(strFile));
	}	
	catch(_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

void CFieldEditDlg::OnBtnUp()
{
	CString strErrorMsg;
	int nRow = m_pgridDest->m_nSelectRow;
	if (nRow == 0)
	{
		strErrorMsg.Format("제일 상단의 필드명입니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	else if(!m_bInsertState && (m_pgridDest->GetCellData(nRow, 0) == "분" || m_pgridDest->GetCellData(nRow, 0) == "신"))
	{
		strErrorMsg.Format("해당필드는 이동할 수 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	CString strFirstDest = m_pgridDest->GetCellData(0, 0);
	CString strSecondDest = m_pgridDest->GetCellData(1, 0);

	if(strFirstDest == "분" && nRow == 1)
		return;
	else if(strFirstDest == "분" && strSecondDest == "신" && nRow == 2)
		return;
	else if(strFirstDest == "신" && nRow == 1)
		return;
	
	// 비정상종료 예외처리
	POSITION posTemp = m_plistTempHeader->FindIndex(nRow);
	if( posTemp == NULL ) return ;

	CStringArray saGridData;
	m_pgridDest->GetData_FieldEditDest(nRow, saGridData);
	m_pgridDest->DeleteRow(nRow);

	POSITION pos = m_plistTempHeader->FindIndex(nRow);
	HeadFixInfo stHeadFixInfo = m_plistTempHeader->GetAt(pos);
	m_plistTempHeader->RemoveAt(pos);

	m_pgridDest->InsertRow(nRow - 1);
	nRow = nRow - 1;
	m_pgridDest->SetData_FieldEditDest(nRow, &saGridData);
	m_pgridDest->SetSelRow(nRow);

	pos = m_plistTempHeader->FindIndex(nRow);
	m_plistTempHeader->InsertBefore(pos, stHeadFixInfo);

	m_bInsertState = FALSE;
}

void CFieldEditDlg::Debug_DataView(CStringArray *psaData)
{
#ifdef _DEBUG
	CString strTemp;
	for (int nIndex = 0; nIndex < psaData->GetSize(); nIndex++)
	{
		strTemp.Format("\n# Debug_DataView => CStringArray : [%s]", psaData->GetAt(nIndex));
		TRACE(strTemp);
	}
#endif
}

void CFieldEditDlg::OnBtnTop()
{
	CString strErrorMsg;
	int nRow = m_pgridDest->m_nSelectRow;
	if (nRow == 0)
	{
		strErrorMsg.Format("제일 상단의 필드명입니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	else if(!m_bInsertState && (m_pgridDest->GetCellData(nRow, 0) == "분" || m_pgridDest->GetCellData(nRow, 0) == "신"))
	{
		strErrorMsg.Format("해당필드는 이동할 수 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	CString strFirstDest = m_pgridDest->GetCellData(0, 0);
	CString strSecondDest = m_pgridDest->GetCellData(1, 0);

	if(strFirstDest == "분" && nRow == 1)
		return;
	else if(strFirstDest == "분" && strSecondDest == "신" && nRow == 2)
		return;
	else if(strFirstDest == "신" && nRow == 1)
		return;
	
	int nDestRow = 0;
	if(strFirstDest == "분")
		nDestRow = 1;
	else if(strFirstDest == "분" && strSecondDest == "신")
		nDestRow = 2;
	else if(strFirstDest == "신")
		nDestRow = 1;

	// 비정상종료 예외처리
	POSITION posTemp = m_plistTempHeader->FindIndex(nRow);
	if( posTemp == NULL ) return ;

	CStringArray saGridData;
	m_pgridDest->GetData_FieldEditDest(nRow, saGridData);
	m_pgridDest->DeleteRow(nRow);

	m_pgridDest->InsertRow(nDestRow);
	m_pgridDest->SetData_FieldEditDest(nDestRow, &saGridData);
	m_pgridDest->SetSelRow(nDestRow);

	m_pgridDest->SetTopRow(nDestRow);

	POSITION pos = m_plistTempHeader->FindIndex(nRow);
	HeadFixInfo stHeadFixInfo = m_plistTempHeader->GetAt(pos);
	m_plistTempHeader->RemoveAt(pos);

	pos = m_plistTempHeader->FindIndex(nDestRow);
	m_plistTempHeader->InsertBefore(pos, stHeadFixInfo);

	m_bInsertState = FALSE;
}

void CFieldEditDlg::OnBtnDown()
{
	CString strErrorMsg;
	int nRow = m_pgridDest->m_nSelectRow;
	if (nRow == m_pgridDest->GetNumberRows() - 1)
	{
		strErrorMsg.Format("제일 하단의 필드명입니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	else if(!m_bInsertState && (m_pgridDest->GetCellData(nRow, 0) == "분" || m_pgridDest->GetCellData(nRow, 0) == "신"))
	{
		strErrorMsg.Format("해당필드는 이동할 수 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	// 비정상종료 예외처리
	POSITION posTemp = m_plistTempHeader->FindIndex(nRow);
	if( posTemp == NULL ) return ;

	CStringArray saGridData;
	m_pgridDest->GetData_FieldEditDest(nRow, saGridData);
	m_pgridDest->DeleteRow(nRow);

	POSITION pos = m_plistTempHeader->FindIndex(nRow);
	HeadFixInfo stHeadFixInfo = m_plistTempHeader->GetAt(pos);
	m_plistTempHeader->RemoveAt(pos);

	m_pgridDest->InsertRow(nRow + 1);
	nRow = nRow + 1;
	m_pgridDest->SetData_FieldEditDest(nRow, &saGridData);
	m_pgridDest->SetSelRow(nRow);

	pos = m_plistTempHeader->FindIndex(nRow);
	m_plistTempHeader->InsertBefore(pos, stHeadFixInfo);

	m_bInsertState = FALSE;
}

void CFieldEditDlg::OnBtnBottom()
{
	CString strErrorMsg;
	int nRow = m_pgridDest->m_nSelectRow;
	if (nRow == m_pgridDest->GetNumberRows() - 1)
	{
		strErrorMsg.Format("제일 하단의 필드명입니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	else if(!m_bInsertState && (m_pgridDest->GetCellData(nRow, 0) == "분" || m_pgridDest->GetCellData(nRow, 0) == "신"))
	{
		strErrorMsg.Format("해당필드는 이동할 수 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	// 비정상종료 예외처리
	POSITION posTemp = m_plistTempHeader->FindIndex(nRow);
	if( posTemp == NULL ) return ;

	CStringArray saGridData;
	m_pgridDest->GetData_FieldEditDest(nRow, saGridData);
	m_pgridDest->DeleteRow(nRow);
	m_pgridDest->AppendRow();

	POSITION pos = m_plistTempHeader->FindIndex(nRow);
	HeadFixInfo stHeadFixInfo = m_plistTempHeader->GetAt(pos);
	m_plistTempHeader->RemoveAt(pos);

	nRow = m_pgridDest->GetNumberRows() - 1;
	m_pgridDest->SetData_FieldEditDest(nRow, &saGridData);
	m_pgridDest->SetSelRow(nRow);

	m_plistTempHeader->AddTail(stHeadFixInfo);

	int nRowLast = (int)(m_pgridDest->GetNumberRows());
	m_pgridDest->SetTopRow(nRowLast - 1);

	m_bInsertState = FALSE;
}


BOOL CFieldEditDlg::GetPosField(CString strFieldID, int &nFindRow, int &nFindCol)
{
	int bResult = FALSE;

	BOOL bState = FALSE;
	CStringArray saGridData;
	for (int nRow = 0; nRow < m_nGridSourceRow; nRow++)
	{
		for (int nCol = 0; nCol < 2; nCol++)
		{
			saGridData.RemoveAll();
			//# Check / Name / Id / Dest Row
			m_pgridSource->GetData_FieldEditSource(nCol, nRow, saGridData);
			if (strFieldID == saGridData.GetAt(2))
			{
				nFindRow = nRow;
				nFindCol = nCol;
				bResult = TRUE;

				bState = TRUE;
				break;
			}
		}

		if (bState == TRUE)
			break;
	}

	return bResult;
}

void CFieldEditDlg::OnBtnSetInit()
{
	m_pgridDest->ClearAll();

	int nRow, nCol;
	CStringArray saGridData;
	for (nRow = 0; nRow < m_plistFixHeadSys->GetCount(); nRow++)
	{
		saGridData.RemoveAll();
		if (nRow < m_nGridSourceRow)
		{
			m_pgridSource->GetData_FieldEditSource(0, nRow, saGridData);
			saGridData.SetAt(0, "0");	//# Check!
			m_pgridSource->SetData_FieldEditSource(0, nRow, &saGridData);
		}
		else if(m_pTab->GetCurSel() == 0 && nRow < 19)
		{
			m_pgridSource->GetData_FieldEditSource(1, nRow - m_nGridSourceRow, saGridData);
			saGridData.SetAt(0, "0");	//# Check!
			m_pgridSource->SetData_FieldEditSource(1, nRow - m_nGridSourceRow, &saGridData);
		}

	}
	m_plistSelectedHeader->RemoveAll();
	m_plistTempHeader->RemoveAll();

	m_pgridSource->RedrawAll();
	m_pgridDest->RedrawAll();

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	//CConditionResultWnd *pwndConditionResult = (CConditionResultWnd *)pwndMainFrm->GetWnd("RESULT");

	CStringArray saFieldId;
	
	m_bRedraw = FALSE;	

	int nSetCurSel = -1;
	if(m_pTab->GetCurSel() != 0)
	{
		::LockWindowUpdate(GetSafeHwnd());
		nSetCurSel = m_pTab->GetCurSel();
		m_pTab->SetCurSel(0);
		OnChangeTab(NULL, NULL);
	}

	if (pwndMainFrm->GetLoadFixField_Def(saFieldId) == TRUE)
	{
		for (int nIndex = 0; nIndex < saFieldId.GetSize(); nIndex++)
		{
			if (GetPosField(saFieldId.GetAt(nIndex), nRow, nCol) == TRUE)
				SetDest_Insert(nRow, nCol);
		}

		if(nSetCurSel != -1)
		{
			m_pTab->SetCurSel(nSetCurSel);
			OnChangeTab(NULL, NULL);
			UnlockWindowUpdate();
		}

		SetSpin();
		m_editFixCol.SetWindowText("2");
		m_btnSpin.SetPos(2);
	}

	m_bRedraw = TRUE;
}


void CFieldEditDlg::OnKillFocusFixColEdit()
{
	int nLower, nUpper;
	m_btnSpin.GetRange(nLower, nUpper);

	CString strData;
	m_editFixCol.GetWindowText(strData);

	if(atoi(strData) >= nUpper)
	{
		CString strTemp;
		strTemp.Format("%d", nUpper);
		m_editFixCol.SetWindowText(strTemp);
	}

	TRACE("\n" + strData);
}


void CFieldEditDlg::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	int nFixCol;
	nFixCol = pNMUpDown->iPos + pNMUpDown->iDelta;

	if (nFixCol >= 0)
	{
		int nLower, nUpper;
		m_btnSpin.GetRange(nLower, nUpper);

		if (nUpper >= nFixCol)
		{
			CString strTemp;
			strTemp.Format("%d", nFixCol);
			m_editFixCol.SetWindowText(strTemp);
		}
	}

	*pResult = 0;
}

LRESULT CFieldEditDlg::OnNotifyGrid(WPARAM wParam, LPARAM lParam)
{
	GridEventInfo		stGridEventInfo;
	int nRow, nCol;
	CStringArray saSourceData, saDestData;

	stGridEventInfo = *(GridEventInfo*)lParam;
	if (wParam == NM_CLICK_CELLTYPE)
	{
		if (stGridEventInfo.strType == "BUTTON")
		{

		}
		else if (stGridEventInfo.strType == "CHECK")
		{
			if (stGridEventInfo.strGridType == "FieldEditSource")
			{
				if (atoi(stGridEventInfo.strData) == 1)
				{
					SetDest_Insert(stGridEventInfo.nRow, stGridEventInfo.nCol);
				}

				else
				{
					CStringArray saSourceData, saDestData;
					//# Check / Name / Id / Dest Row
					m_pgridSource->GetData_FieldEditSource(stGridEventInfo.nCol, stGridEventInfo.nRow, saSourceData);

					if(saSourceData.GetAt(1) == "유사도")
					{
						m_pgridSource->QuickSetText(stGridEventInfo.nCol, stGridEventInfo.nRow, "1");
						SetMessageBox("유사도 필드는 제거할 수 없습니다", (CString)PROGRAM_TITLE, MB_OK);
						return 1L;
					}

					CString szHeadName;
					for (nRow = 0; nRow < m_pgridDest->GetNumberRows(); nRow++)
					{
						saDestData.RemoveAll();
						//# Name / Id / Index
						m_pgridDest->GetData_FieldEditDest(nRow, saDestData);

						szHeadName = m_pgridDest->GetCellData(nRow, 0);

						if (saSourceData.GetAt(2) == saDestData.GetAt(1))
						{
							m_pgridDest->DeleteRow(nRow);
							break;
						}
					}

					POSITION		pos;
					HeadInfoType	stHeadInfoType;
					int i = 0;
					for( i = 0; i < m_plistSelectedHeader->GetCount(); i++)
					{
						pos = m_plistSelectedHeader->FindIndex(i);
						stHeadInfoType = m_plistSelectedHeader->GetAt(pos);

						if(stHeadInfoType.strHeadName == szHeadName)
						{
							m_plistSelectedHeader->RemoveAt(pos);
							break;
						}					
					}

					HeadFixInfo stHeadFixInfo;
					for(i = 0; i < m_plistTempHeader->GetCount(); i++)
					{
						pos = m_plistTempHeader->FindIndex(i);
						stHeadFixInfo = m_plistTempHeader->GetAt(pos);

						if(stHeadFixInfo.stHeadType.strHeadName == szHeadName)
						{
							m_plistTempHeader->RemoveAt(pos);
							break;
						}
					}

				}
				SetSpin();
			}
		}
		else if (stGridEventInfo.strType == "COMBO")
		{

		}
	}
	else if (wParam == NM_DBCLICK_L)
	{
		if (stGridEventInfo.strGridType == "FieldEditDest")
		{
			//# Name / Id / Index
			m_pgridDest->GetData_FieldEditDest(stGridEventInfo.nRow, saDestData);

			//			if (atoi(saDestData.GetAt(2)) == (int)GRID_MOVEFIELD_POS_INDEX)
			//			{
			//				CString strMsg;
			//				strMsg.Format("%s는 삭제 할수 없습니다.", (CString)GRID_MOVEFIELD_POS_NAME);
			//				SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);
			//			}
			//			else
			{
				CString szHeadName;
				szHeadName = m_pgridDest->GetCellData(stGridEventInfo.nRow, 0);

				POSITION		pos;
				HeadInfoType	stHeadInfoType;
				int i = 0;
				for( i = 0; i < m_plistSelectedHeader->GetCount(); i++)
				{
					pos = m_plistSelectedHeader->FindIndex(i);
					stHeadInfoType = m_plistSelectedHeader->GetAt(pos);

					if(stHeadInfoType.strHeadName == szHeadName)
					{
						m_plistSelectedHeader->RemoveAt(pos);
						break;
					}					
				}

				HeadFixInfo stHeadFixInfo;
				for(i = 0; i < m_plistTempHeader->GetCount(); i++)
				{
					pos = m_plistTempHeader->FindIndex(i);
					stHeadFixInfo = m_plistTempHeader->GetAt(pos);
					
					if(stHeadFixInfo.stHeadType.strHeadName == szHeadName)
					{
						m_plistTempHeader->RemoveAt(pos);
						break;
					}
				}
				
				m_pgridDest->DeleteRow(stGridEventInfo.nRow);

// 				if (atoi(saDestData.GetAt(2)) + 1 > MAX_FIELD_CNT)
// 				{
// 					nCol = 1;
// 					nRow = atoi(saDestData.GetAt(2)) - (int)OFFSET_COL_INDEX;
// 				}
// 				else
// 				{
// 					nCol = 0;
// 					nRow = atoi(saDestData.GetAt(2));
// 				}

				int nDeleteSource = GetRowField(stHeadInfoType.strHeadName, 
					stHeadInfoType.lHeadID);
				
				if(nDeleteSource > -1)
				{
					CStringArray saGetData;
					if (nDeleteSource < (int)OFFSET_COL_INDEX && nDeleteSource != -1)
					{
						m_pgridSource->GetData_FieldEditSource(0, nDeleteSource, saGetData);
						saGetData.SetAt(0, "0");
						m_pgridSource->SetData_FieldEditSource(0, nDeleteSource, &saGetData);
					}
					else
					{
						m_pgridSource->GetData_FieldEditSource(1, nDeleteSource - (int)OFFSET_COL_INDEX, saGetData);
						saGetData.SetAt(0, "0");
						m_pgridSource->SetData_FieldEditSource(1, nDeleteSource - (int)OFFSET_COL_INDEX, &saGetData);
					}
				}
				
				for (int nRow = 0; nRow < m_plistTempHeader->GetCount(); nRow++)
				{
					pos = m_plistTempHeader->FindIndex(nRow);
					if( pos == NULL ) break;
					stHeadFixInfo = m_plistTempHeader->GetAt(pos);
					
					int nSourceRow = GetRowField(stHeadFixInfo.stHeadType.strHeadName, 
						stHeadFixInfo.stHeadType.lHeadID);
					
					CStringArray saGetData;
					if (nSourceRow < (int)OFFSET_COL_INDEX && nSourceRow != -1)
					{
						m_pgridSource->GetData_FieldEditSource(0, nSourceRow, saGetData);
						saGetData.SetAt(0, "1");	//# Check!	
						m_pgridSource->SetData_FieldEditSource(0, nSourceRow, &saGetData);
					}
					else
					{
						m_pgridSource->GetData_FieldEditSource(1, nSourceRow - (int)OFFSET_COL_INDEX, saGetData);
						saGetData.SetAt(0, "1");	//# Check!
						m_pgridSource->SetData_FieldEditSource(1, nSourceRow - (int)OFFSET_COL_INDEX, &saGetData);
					}
				}
				
				m_pgridDest->RedrawAll();
				m_pgridSource->RedrawAll();

				SetSpin();
			}
		}
		else if (stGridEventInfo.strGridType == "FieldEditSource")
		{
			if (atoi(stGridEventInfo.strData) == 1)
			{
				SetDest_Insert(stGridEventInfo.nRow, stGridEventInfo.nCol);
			}
			
			else
			{
				CStringArray saSourceData, saDestData;
				//# Check / Name / Id / Dest Row
				m_pgridSource->GetData_FieldEditSource(stGridEventInfo.nCol, stGridEventInfo.nRow, saSourceData);
				
				if(saSourceData.GetAt(1) == "유사도")
				{
					m_pgridSource->QuickSetText(stGridEventInfo.nCol, stGridEventInfo.nRow, "1");	
					SetMessageBox("유사도 필드는 제거할 수 없습니다", (CString)PROGRAM_TITLE, MB_OK);
					
					return 1L;
				}
				
				CString szHeadName;
				for (nRow = 0; nRow < m_pgridDest->GetNumberRows(); nRow++)
				{
					saDestData.RemoveAll();
					//# Name / Id / Index
					m_pgridDest->GetData_FieldEditDest(nRow, saDestData);
					
					szHeadName = m_pgridDest->GetCellData(nRow, 0);
					
					if (saSourceData.GetAt(2) == saDestData.GetAt(1))
					{
						m_pgridDest->DeleteRow(nRow);
						break;
					}
				}
				
				POSITION		pos;
				HeadInfoType	stHeadInfoType;
				int i = 0;
				for( i = 0; i < m_plistSelectedHeader->GetCount(); i++)
				{
					pos = m_plistSelectedHeader->FindIndex(i);
					stHeadInfoType = m_plistSelectedHeader->GetAt(pos);
					
					if(stHeadInfoType.strHeadName == szHeadName)
					{
						m_plistSelectedHeader->RemoveAt(pos);
						break;
					}					
				}
				
				HeadFixInfo stHeadFixInfo;
				for(i = 0; i < m_plistTempHeader->GetCount(); i++)
				{
					pos = m_plistTempHeader->FindIndex(i);
					stHeadFixInfo = m_plistTempHeader->GetAt(pos);
					
					if(stHeadFixInfo.stHeadType.strHeadName == szHeadName)
					{
						m_plistTempHeader->RemoveAt(pos);
						break;
					}
				}
				
			}
			SetSpin();
		}
	}
	return 1L;
}

void CFieldEditDlg::SetSpin()
{
	CString strTemp;
	if (m_pgridDest->GetNumberRows() > 0)
	{
		m_btnSpin.SetRange((short)0, short(m_pgridDest->GetNumberRows() - 1));

//		m_editFixCol.m_nMaxVal = m_pgridDest->GetNumberRows() - 1;
		if (m_pgridDest->GetNumberRows() - 1 > 10)
			m_editFixCol.SetLimitText(2);
		else
			m_editFixCol.SetLimitText(1);

		m_editFixCol.GetWindowText(strTemp);
		if (m_pgridDest->GetNumberRows() - 1 < atoi(strTemp))
		{
			strTemp.Format("%d", m_pgridDest->GetNumberRows() - 1);
			m_editFixCol.SetWindowText(strTemp);

			m_btnSpin.SetPos(m_pgridDest->GetNumberRows() - 1);
		}
	}
	else
	{
		m_btnSpin.SetRange(0, 0);

		m_editFixCol.SetWindowText("0");
		m_editFixCol.SetLimitText(1);
//		m_editFixCol.m_nMaxVal = 0;

		m_btnSpin.SetPos(0);
	}
}

void CFieldEditDlg::SetDest_Insert(int nRow, int nCol)
{
	//# Check / Name / Id / Dest Row
	CStringArray saGridData;

	m_pgridSource->GetData_FieldEditSource(nCol, nRow, saGridData);

	int nFieldCnt = m_pgridDest->GetNumberRows()/* + m_nMoveFieldCnt*/;
	if (nFieldCnt >= (int)MAX_FIELD_CNT)
	{
		CString strMsg;
		strMsg.Format("값보기 항목을 더 이상 추가 할 수 없습니다.\n선택 가능한 항목수는 최대 %d개 입니다.",
			(int)MAX_FIELD_CNT);
		/*SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);*/
		char* szOriginTitle = (char*)AfxGetApp()->m_pszAppName;
		AfxGetApp()->m_pszAppName = (LPCTSTR)PROGRAM_TITLE;
		AfxMessageBox(strMsg);
		AfxGetApp()->m_pszAppName = szOriginTitle;

		saGridData.SetAt(0, "0");
		m_pgridSource->SetData_FieldEditSource(nCol, nRow, &saGridData, m_bRedraw);
	}
	else 
	{
		int nTabIndex = m_pTab->GetCurSel();

		int nStartIndex = 0;
		int nEndIndex	= 0;

		switch (nTabIndex)
		{
			case 0 : nStartIndex = 0;	nEndIndex = 20; break;
			case 1 : nStartIndex = 20;	nEndIndex = 12; break;
			case 2 : nStartIndex = 32;	nEndIndex = 6;	break;
			case 3 : nStartIndex = 38;	nEndIndex = 10;	break;
			case 4 : nStartIndex = 48;	nEndIndex = 6;	break;
			case 5 : nStartIndex = 54;	nEndIndex = 4;	break;
			/*case 6 : nStartIndex = 57;	nEndIndex = 8;	break;*/
			case 6 : nStartIndex = 58;	nEndIndex = 6;	break;
		}
		
		//int nPos = nStartIndex + nRow + 12 * nCol;
		int nPos = nStartIndex + nRow + m_nGridSourceRow * nCol;
		
		POSITION pos = m_plistFixHeadSys->FindIndex(nPos);
		HeadInfoType stHeadInfoType = m_plistFixHeadSys->GetAt(pos);

		if(stHeadInfoType.strHeadName == "분")
			m_plistSelectedHeader->AddHead(stHeadInfoType);
		else if(stHeadInfoType.strHeadName == "신")
		{
			CString strMsg;
			strMsg.Format("해당화면은 신호 필드를 사용하실 수 없습니다.");
			SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);

			saGridData.SetAt(0, "0");
			m_pgridSource->SetData_FieldEditSource(nCol, nRow, &saGridData, m_bRedraw);
			return;
// 			POSITION _pos = m_plistSelectedHeader->FindIndex(0);
// 			HeadInfoType _stHeadInfoType = m_plistSelectedHeader->GetAt(_pos);
// 			if(_stHeadInfoType.strHeadName == "분")
// 			{
// 				_pos = m_plistSelectedHeader->FindIndex(0);
// 				m_plistSelectedHeader->InsertAfter(_pos, stHeadInfoType);
// 			}
// 			else
// 				m_plistSelectedHeader->AddHead(stHeadInfoType);
		}
		else
		{
			for(int nIndex = 0; nIndex < m_plistSelectedHeader->GetCount(); nIndex++)
			{
				POSITION pos = m_plistSelectedHeader->FindIndex(nIndex);
				HeadInfoType stHeadInfoTypeTemp = m_plistSelectedHeader->GetAt(pos);
				
				if(stHeadInfoTypeTemp.strHeadName == stHeadInfoType.strHeadName)
				{
					return;
				}
			}
			m_plistSelectedHeader->AddTail(stHeadInfoType);
		}

		saGridData.SetAt(0, "1");
		m_pgridSource->SetData_FieldEditSource(nCol, nRow, &saGridData, TRUE);

		HeadFixInfo stHeadFixInfo;
		stHeadFixInfo.nColNum = nCol;
		stHeadFixInfo.stHeadType = stHeadInfoType;

		/*m_plistFixHeadUser->AddTail(stHeadFixInfo);*/
		m_plistTempHeader->AddTail(stHeadFixInfo);
		
		CString strTemp;
		//# Name / Id / Index
		CStringArray saSetData;
		saSetData.Add(saGridData.GetAt(1));		//# Name
		saSetData.Add(saGridData.GetAt(2));		//# Id
		strTemp.Format("%d", nCol * (int)OFFSET_COL_INDEX  + nRow);
		saSetData.Add(strTemp);
		
		m_pgridDest->AppendRow();
		nRow = m_pgridDest->GetNumberRows() - 1;
		m_pgridDest->SetData_FieldEditDest(nRow, &saSetData, TRUE);	
		
		if(stHeadInfoType.strHeadName == "분")
		{
			int nSelected = m_pgridDest->m_nSelectRow;
			m_pgridDest->m_nSelectRow = nRow;
			m_bInsertState = TRUE;
			OnBtnTop();
			m_pgridDest->m_nSelectRow = nSelected;
		}
		else if(stHeadInfoType.strHeadName == "신")
		{
			int nSelected = m_pgridDest->m_nSelectRow;
			m_pgridDest->m_nSelectRow = nRow;
			m_bInsertState = TRUE;
			OnBtnTop();
			if(m_pgridDest->GetCellData(1, 0) == "분")
			{
				m_bInsertState = TRUE;
				OnBtnDown();
			}
			m_pgridDest->m_nSelectRow = nSelected;
		}
	}
}

int CFieldEditDlg::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
// 	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
// 	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);

	char* szOriginTitle = (char*)AfxGetApp()->m_pszAppName;
	AfxGetApp()->m_pszAppName = (LPCTSTR)lpszCaption;
	AfxMessageBox(lpszText);
	AfxGetApp()->m_pszAppName = szOriginTitle;
	
	return TRUE;
}

BOOL CFieldEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(m_editFixCol.GetSafeHwnd() == pMsg->hwnd)
		{
			OnKillFocusFixColEdit();
			return TRUE;
		}

		return FALSE;
	}
	else if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		OnCancel();

	return CDialog::PreTranslateMessage(pMsg);
}

void CFieldEditDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CFieldEditDlg::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTabIndex = m_pTab->GetCurSel();

	int nStartIndex = 0;
	int nEndIndex	= 0;

	switch (nTabIndex)
	{
		case 0 : nStartIndex = 0;	nEndIndex = 20; break;
		case 1 : nStartIndex = 20;	nEndIndex = 12; break;
		case 2 : nStartIndex = 32;	nEndIndex = 6;	break;
		case 3 : nStartIndex = 38;	nEndIndex = 10;	break;
		case 4 : nStartIndex = 48;	nEndIndex = 6;	break;
		case 5 : nStartIndex = 54;	nEndIndex = 4;	break;
		/*case 6 : nStartIndex = 57;	nEndIndex = 8;	break;*/
		case 6 : nStartIndex = 58;	nEndIndex = 6;	break;
	}

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;

	m_plistFixHeadUser = &pwndMainFrm->m_listFixHeadUser;
	m_plistFixHeadSys = &pwndMainFrm->m_listFixHeadSys;

	int				nRow;
	POSITION		pos;
	HeadInfoType	stHeadInfoType;
	CString			strTemp;
	CStringArray	saGridData;
	HeadFixInfo		stHeadFixInfo;

	m_pgridSource->OnHeaderSetup();

	if(nTabIndex == 0)
	{
		m_pgridSource->SetColWidth(0, SOURCE_GRID_HEAD_WIDTH);
		m_pgridSource->SetColWidth(1, SOURCE_GRID_HEAD_WIDTH);
	}
	else
	{
		m_pgridSource->SetColWidth(0, SOURCE_GRID_HEAD_WIDTH * 2);
		m_pgridSource->SetColWidth(1, 0);
	}
	
	if(nEndIndex > GRID_SOURCE_ROW)
	{
		m_nGridSourceRow = nEndIndex / 2;

		if(nEndIndex % 2 > 0)
			m_nGridSourceRow++;
	}
	else
		m_nGridSourceRow = GRID_SOURCE_ROW;

	for (nRow = 0; nRow < nEndIndex; nRow++)
	{
		pos = m_plistFixHeadSys->FindIndex(nStartIndex);
		if( pos == NULL ) break;
		stHeadInfoType = m_plistFixHeadSys->GetAt(pos);

		saGridData.RemoveAll();

		saGridData.Add("0");			//# Check
		saGridData.Add(stHeadInfoType.strHeadName);	//# Name
		strTemp.Format("%d", stHeadInfoType.lHeadID);
		saGridData.Add(strTemp);	//# ID
		saGridData.Add("-1");			//# Dest Row

		if (nRow < m_nGridSourceRow)
		{
			m_pgridSource->AppendRow();
			m_pgridSource->SetData_FieldEditSource(0, nRow, &saGridData);
		}
		else
			m_pgridSource->SetData_FieldEditSource(1, nRow - m_nGridSourceRow, &saGridData);

		nStartIndex++;
	}
	m_pgridSource->SetColor_EvenOdd();
	/*m_pgridSource->RedrawAll();*/

	//# Name / Id / Index
	m_pgridDest->ClearAll();
	/*HeadFixInfo stHeadFixInfo;*/
	for (nRow = 0; nRow < m_plistTempHeader->GetCount(); nRow++)
	{
		pos = m_plistTempHeader->FindIndex(nRow);
		if( pos == NULL ) break;
		stHeadFixInfo = m_plistTempHeader->GetAt(pos);

		int nSourceRow = GetRowField(stHeadFixInfo.stHeadType.strHeadName, 
			stHeadFixInfo.stHeadType.lHeadID);

		saGridData.RemoveAll();
		{
			saGridData.Add(stHeadFixInfo.stHeadType.strHeadName);		//# Name

			strTemp.Format("%d", stHeadFixInfo.stHeadType.lHeadID);
			saGridData.Add(strTemp);	//ID

			CStringArray saGetData;
			if (nSourceRow < (int)OFFSET_COL_INDEX && nSourceRow != -1)
			{
				m_pgridSource->GetData_FieldEditSource(0, nSourceRow, saGetData);
				saGetData.SetAt(0, "1");	//# Check!	
				m_pgridSource->SetData_FieldEditSource(0, nSourceRow, &saGetData);
			}
			else
			{
				m_pgridSource->GetData_FieldEditSource(1, nSourceRow - (int)OFFSET_COL_INDEX, saGetData);
				saGetData.SetAt(0, "1");	//# Check!
				m_pgridSource->SetData_FieldEditSource(1, nSourceRow - (int)OFFSET_COL_INDEX, &saGetData);
			}
			strTemp.Format("%d", nSourceRow);
			saGridData.Add(strTemp);		//# Index
			/*m_pgridSource->RedrawAll();*/
		}
		m_pgridDest->AppendRow();
		m_pgridDest->SetData_FieldEditDest(m_pgridDest->GetNumberRows() - 1, &saGridData);
		/*m_pgridDest->RedrawAll();*/
	}
	
	if(m_bRedraw)
	{
		m_pgridDest->RedrawAll();
		m_pgridSource->RedrawAll();
	}
	

	//# Set Spin Default!
	strTemp.Format("%d", pwndMainFrm->m_nFixCol);
	m_editFixCol.SetWindowText(strTemp);

//	m_editFixCol.m_nMaxVal = m_plistTempHeader->GetCount() - 1; 
	if (m_plistTempHeader->GetCount() - 1 > 10)
		m_editFixCol.SetLimitText(2);
	else
		m_editFixCol.SetLimitText(1);

//	m_editFixCol.m_pbtnSpin = &m_btnSpin;

	m_btnSpin.SetRange(0, m_plistTempHeader->GetCount() - 1);
	m_btnSpin.SetPos(pwndMainFrm->m_nFixCol);
	m_btnSpin.SetBase(pwndMainFrm->m_nFixCol);
}

void CFieldEditDlg::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;

	m_pTab->SetTabColor(clrBkgnd, clrBkgnd, stTabColor.clrB, NULL, NULL, clrBkgnd);
 	m_pTab->SetTextColor(stTabColor.clrSF, stTabColor.clrSF);

	Invalidate();
}
BOOL CFieldEditDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect reClient;
	GetClientRect(reClient);

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pwndMain;

	pDC->FillSolidRect(reClient, pMainFrmWnd->GetAxColor(66));
	return FALSE;
}

int CFieldEditDlg::GetBtnWidth(int nTextLen)
{
	switch(nTextLen)
	{
	case 2 :
		return 36;
	case 4 :
		return 60;
	case 6 :
		return 88;
	}
	return 0;
}