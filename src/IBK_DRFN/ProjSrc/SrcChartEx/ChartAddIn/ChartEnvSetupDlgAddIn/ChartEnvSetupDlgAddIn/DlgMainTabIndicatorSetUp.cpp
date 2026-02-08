// DlgMainTabIndicatorSetUp.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "DlgMainTabIndicatorSetUp.h"

#include <MMSystem.h>						// for timeGetTime()

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTOL0()	// 2008.01.25 by LYH >>
#include "../Include_Chart/OutsideCommonInterfaceFunction.h"	// for COutsideCommonInterfaceFunction
#include "../Include_Chart/GraphData.h"

#include "BaseDialog.h"
#include "BaseData.h"
#include "DlgEnvSetUpAll.h"
#include "IndicatorNameListCtrl.h"
#include "IndicatorGroupButton.h"			// for CIndicatorGroupButton


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabIndicatorSetUp dialog

IMPLEMENT_DYNAMIC( CDlgMainTabIndicatorSetUp, CBaseTabDlg)

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void  : 
// Param  IChartOCX *p_pIChartOCX : 
// Param  CMainBlock *pMainBlock : 
// Param  CIndicatorList *pIndicatorList : 
// Param  CWnd *pParent : 
// Comments		: 
//-----------------------------------------------------------------------------
CDlgMainTabIndicatorSetUp::CDlgMainTabIndicatorSetUp( IChartOCX *p_pIChartOCX, CWnd *pParent )
	: CBaseTabDlg(CDlgMainTabIndicatorSetUp::IDD, pParent), 
	m_bIsOnlyViewData(false)
//	: CDialog(CDlgMainTabIndicatorSetUp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMainTabIndicatorSetUp)
	m_editIndicatorExp = _T("");
	m_bSaveDefault = FALSE;
	//}}AFX_DATA_INIT

	// 1. Member 변수들을 초기화 한다.
	m_pIChartOCX			= p_pIChartOCX;
	m_pIChartManager		= NULL;
	m_pIGraphInfoManager	= NULL;
	if( m_pIChartOCX)
	{
		m_pIChartOCX->AddRef();
		m_pIChartManager		= m_pIChartOCX->GetIChartManager();
		m_pIGraphInfoManager	= m_pIChartOCX->GetIGraphInfoManager();
	}

	m_pChildDlg = NULL;
	m_nInitChartIndex = -1;

	m_pftOldStaticFont = NULL;

	m_bPreview = FALSE;
	m_bSave = FALSE;

	m_bDlgMode = TRUE;

	m_pIndicatorNameList	= NULL;
	m_pOrgIndicatorNameList	= NULL;
	m_pIPacketManager = NULL;

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = 0;

// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
// 2008.01.25 by LYH <<
}
CDlgMainTabIndicatorSetUp::~CDlgMainTabIndicatorSetUp()   // standard destructor
{
	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
	}
	if( m_pIChartManager)
	{
		m_pIChartManager->Release();
		m_pIChartManager = NULL;
	}
	if( m_pIGraphInfoManager)
	{
		m_pIGraphInfoManager->Release();
		m_pIGraphInfoManager = NULL;
	}
	if( m_pIndicatorNameList)
	{
		m_pIndicatorNameList->Release();
		m_pIndicatorNameList = NULL;
	}
	if( m_pOrgIndicatorNameList)
	{
		m_pOrgIndicatorNameList->Release();
		m_pOrgIndicatorNameList = NULL;
	}
	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void : 
// Param  CDataExchange* pDX : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMainTabIndicatorSetUp)
	DDX_Control(pDX, IDC_SUBTAB_ALL, m_tabIndicatorSetUpAll);
	DDX_Check(pDX, IDC_CHK_WITH_DEFAULT, m_bSaveDefault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMainTabIndicatorSetUp, CDialog)
	//{{AFX_MSG_MAP(CDlgMainTabIndicatorSetUp)
	ON_BN_CLICKED(IDC_INITILIZE_ALLINDICATOR, OnInitilizeAllindicator)
	ON_BN_CLICKED(IDC_DEFAULT_INDICATORDATA, OnDefaultIndicatordata)
	ON_BN_CLICKED(IDC_CHK_WITH_DEFAULT, OnChkWithDefault)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnBtnRecover)
	ON_BN_CLICKED(IDC_BTN_PREVIEW, OnBtnPreview)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_ADD_ALL, OnBtnAddAll)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL, OnBtnDeleteAll)
	ON_BN_CLICKED(IDC_BTN_WITH_DEFAULT, OnBtnWithDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabIndicatorSetUp message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 9
// Modifier		: 
// Comments		: 지표 설정 TabPage를 초기화 한다.
//-----------------------------------------------------------------------------
BOOL CDlgMainTabIndicatorSetUp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 1. Dialog에 원버튼으로 포커스를 주기 위해 사용
	mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
	mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );

	if(m_pIChartOCX == NULL)
		return FALSE;

	// 2. 설정 Dialog에서 지표명에 다른 font를 사용하기 위해 폰트를 바꾸어준다.
	CFont *pFont = NULL;
	pFont = this->GetFont();
	if( pFont )
	{
		LOGFONT lfIndicatorName;
		pFont->GetLogFont( &lfIndicatorName );

		lfIndicatorName.lfWeight = FW_BOLD;
		lfIndicatorName.lfHeight = lfIndicatorName.lfHeight - 1;
		m_pftGroupNameFont.CreateFontIndirect( &lfIndicatorName );
	}

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_INDICATOR_NAME);
	m_pftOldStaticFont = pStatic->GetFont();
	pStatic->SetFont( &m_pftGroupNameFont );

	// 3. 현재 차트 화면을 저장하고 초기인덱스를 가지고 있는다.
	// 리턴된 Index는 '취소'작업을할때 필요한 Index로 지표설정창 생성시의 지표세팅값으로 돌아가기 위함.
	m_nInitChartIndex = m_pIChartOCX->SaveChartCFG_SaveNow();

	// 4. 현재 차트에 실행중인 지표들 이름 가져오기.
	if( m_pIndicatorNameList)
	{
		m_pIndicatorNameList->Release();
		m_pIndicatorNameList = NULL;
	}
	if( m_pIChartManager) m_pIndicatorNameList		= m_pIChartManager->GetAllIndicatorNameStringList();
	if( m_pOrgIndicatorNameList)
	{
		m_pOrgIndicatorNameList->Release();
		m_pOrgIndicatorNameList = NULL;
	}
	if( m_pIChartManager) m_pOrgIndicatorNameList	= m_pIChartManager->GetAllIndicatorNameStringList();

	// 5. 초기 IndicatorInfo값 저장해두기(나중에 취소시에 필요함)
	SetOrgViewData();
	
	// 6. SubTab을 생성 시킨다.
	m_tabIndicatorSetUpAll.Init( m_pIChartOCX, this);

	// 7. ListCtrl및 GroupButton생성및 위치지정.
	SetIndicatorSize();
	m_ctrlGroupButton.SetData( this );

	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) 
	m_ctrlUseGroupButton.SetData( this );

	if( m_pIPacketManager) m_pIPacketManager->Release();
	m_pIPacketManager = m_pIChartOCX->GetIDefaultPacketManager();
	
	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) - 사용지표 때문에 생성함수를 분리
	m_ctrlGroupButton.CreateGeneralCtrl( m_pIndicatorNameList, m_pIChartOCX );
	m_ctrlGroupButton.SetGroupButtonCtrlType( GENERAL_GROUP );
	m_ctrlGroupButton.SetCheck( m_pIndicatorNameList );
	m_ctrlUseGroupButton.SetGroupButtonCtrlType( USE_GROUP );
	IStringList *pIGraphNameList = m_pIChartManager->GetAllGraphNameStringList();
	if( pIGraphNameList)
	{
		m_ctrlUseGroupButton.CreateUseCtrl( pIGraphNameList, m_pIChartOCX );
		pIGraphNameList->Release();
	}
	m_ctrlUseGroupButton.SetCheck( m_pIndicatorNameList );

	// Sang-Woo, Cho		2006 / 11 / 06
	// 가격 이동평균과 거래량 이동평균의 설정 Dialog에 control들이 많아서 동적으로 생성하면 
	// 느려보여서 두 지표만 먼저 생성해둔다.
	// Begin
// 2008.01.25 by LYH >>
	//SelChangeChildDlg( "가격 이동평균", "");
	//SelChangeChildDlg( "거래량 이동평균", "");
	SelChangeChildDlg( _MTEXT(C2_PRICE_MA), "");
	SelChangeChildDlg( _MTEXT(C2_VOLUME_MA), "");
// 2008.01.25 by LYH <<
	// End

	//SelChangeChildDlg( m_ctrlGroupButton.GetFirstCheckItem(), TRUE);
	//m_ctrlGroupButton.SetFocusItem();
	CString strIndicatorName, strGraphName;
	m_ctrlUseGroupButton.GetFirstCheckItem( strIndicatorName, strGraphName);
	SelChangeChildDlg( strIndicatorName, strGraphName);
	m_ctrlUseGroupButton.SetFocusItem();
	CIndicatorGroupButton *pIndicatorGroupButton = m_ctrlUseGroupButton.GetSelectGroupButton();
	if( pIndicatorGroupButton) pIndicatorGroupButton->SetState( FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Param  const CString &strIndicatorName : 
// Comments		: GroupButton을 선택할때 호출한다.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SelChangeGroupButton( const CString &strIndicatorName, const CString &strGraphName)
{
	// 특수지표에서 일반지표로 전환할경우에 지표명 체크상태를 다시 일치시켜야한다.
	SyncItemCheck();

	SelChangeChildDlg( strIndicatorName, strGraphName);
} 


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Param  const CString &strIndicatorName : 
// Comments		: 지표를 체크하여 추가한다.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::AddGraph( const CString &strIndicatorName, CString &p_strGraphName)
{
	p_strGraphName.Empty();
	// 해당 지표의 설정Dlg를 가져온다.
	CBaseDialog *pChildDialog = m_BaseDialogContainer.GetBaseDialog( strIndicatorName);

	ILPCSTR szGraphName = m_pIChartOCX->AddChartBlock( strIndicatorName);
	p_strGraphName = szGraphName;
	
	// Info값을 저장할 UserIndicatorInfo를 가져온다.
	IGraphInfo *pUserIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
	if( pUserIndicatorInfo == NULL) return;
	
	// Dialog 오브젝트가 없다면 새로 생성하여 Dialog관리클래스에 Insert한다.
	if( pChildDialog == NULL)
	{
		ML_SET_LANGUAGE_RES();
		m_MakeChildDlg.Initial( m_pIChartOCX, pUserIndicatorInfo, this, pChildDialog, m_pParentWnd);
		//m_pChildDlg->GetTail()
		m_BaseDialogContainer.AddBaseDialog( ( LPCTSTR)strIndicatorName, pChildDialog);
	}
	pChildDialog->Initial( pUserIndicatorInfo);
	pChildDialog->RedrawWindow();

	//sy test - bTheBeginning 값을 지정하기!!
	// 값이 변경되었을 수도있다. OrgViewData에 Info를 Insert 한다. (∴취소시에 필요한 넘이다.)
	m_OrgViewData.AddTail(pUserIndicatorInfo);
	pUserIndicatorInfo->Release();

	// chart화면에 바로 적용이 되므로 미리보기를 한것과 같은 상태이다.
	m_bPreview = TRUE;

	// OCX의 AddChart호출.
	SyncItemCheck();

// (2007/1/24 - Seung-Won, Bae) Why call ? 
//	ApplyGraph( strIndicatorName);
}


//sy 2006.04.24.
void CDlgMainTabIndicatorSetUp::SetOrgViewData()
{
	m_OrgViewData.Initialize();

	IStringList *pIniNameList = m_pIChartOCX->CreateObjectIStringList();
	if(!GetGraphNameList(m_bIsOnlyViewData, pIniNameList))
	{
		pIniNameList->Release();
		return;
	}

	POSITION pos = pIniNameList->GetHeadPosition();
	while(pos)
	{
		ILPCSTR szIndicatorName = NULL;
		pIniNameList->GetNext( pos, szIndicatorName);
		IGraphInfo *pOrgIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szIndicatorName);	
		if(pOrgIndicatorInfo == NULL)
			continue;
 
		m_OrgViewData.AddTail(pOrgIndicatorInfo, true);
		pOrgIndicatorInfo->Release();
	}
	pIniNameList->Release();
}

bool CDlgMainTabIndicatorSetUp::GetGraphNameList(const bool bIsOnlyViewData, IStringList * pIndicatorNameList)
{
	if(m_pIGraphInfoManager == NULL)
		return false;

	pIndicatorNameList->RemoveAll();
	
	// 화면에 존해하는 지표명
	bool bResult = GetGraphNameList(m_pIndicatorNameList, pIndicatorNameList);
	if(bIsOnlyViewData)
		return bResult;

	// userList 에 존재하는 지표명
	IStringList *pIUserIndiNameList = m_pIGraphInfoManager->GetIndicatorNameStringList( IGraphInfoManager::IEILT_USER, IGraphInfoManager::IEIIT_GENERAL);
	if( pIUserIndiNameList) if( GetGraphNameList(pIUserIndiNameList, pIndicatorNameList)) bResult = true;
	pIUserIndiNameList->RemoveAll();
	pIUserIndiNameList->Release();
	pIUserIndiNameList = m_pIGraphInfoManager->GetIndicatorNameStringList( IGraphInfoManager::IEILT_USER, IGraphInfoManager::IEIIT_SPECIAL);
	if( pIUserIndiNameList)
	{
		if(GetGraphNameList(pIUserIndiNameList, pIndicatorNameList))
			bResult = true;
	}
	pIUserIndiNameList->Release();

	return bResult;
}

bool CDlgMainTabIndicatorSetUp::GetGraphNameList(IStringList * pISourceIndiNameList, IStringList * targetIndiNameList)
{
	if( !pISourceIndiNameList) return false;
	if( pISourceIndiNameList->GetCount() <= 0) return false;

	ILPCSTR szGraphName = NULL;
	POSITION pos = pISourceIndiNameList->GetHeadPosition();
	while(pos != NULL)
	{
		pISourceIndiNameList->GetNext( pos, szGraphName);
		if(szGraphName.IsEmpty())
			continue;
		
		if( !IsExistenceIndicatorName( targetIndiNameList, szGraphName)) targetIndiNameList->AddTail(szGraphName);
	}
	
	return (targetIndiNameList->GetCount() > 0);
}

bool CDlgMainTabIndicatorSetUp::IsExistenceIndicatorName(IStringList * pIndicatorNameList, const char *p_szGraphName) const
{
	if( !p_szGraphName) return false;
	if( !*p_szGraphName) return false;

	ILPCSTR szGraphName = NULL;
	POSITION pos = pIndicatorNameList->GetHeadPosition();
	while(pos != NULL)
	{
		pIndicatorNameList->GetNext(pos, szGraphName);
		if( !strcmp( szGraphName, p_szGraphName)) return true;
	}
	return false;
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void	: 
// Comments		: 리스트컨트롤의 위치지정
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SetIndicatorSize()
{
	// 1. IDC_BTN_TOP_INDICATOR의 위치를 얻어와서 ListCtrl의 위치를 정한다.
	CButton *pButton = (CButton *)GetDlgItem( IDC_BTN_TOP_INDICATOR );
	CButton *pUseButton = (CButton *)GetDlgItem( IDC_BTN_USE_INDICATOR );
	CButton *pAddButton = (CButton *)GetDlgItem( IDC_BTN_ADD );
	if( pButton == NULL || pUseButton == NULL || pAddButton == NULL )
		return;

	CRect rcBtnRect, rcUseBtnRect, rcAddBtn;
	pButton->GetWindowRect( rcBtnRect );  
	ScreenToClient( rcBtnRect );

	pUseButton->GetWindowRect( rcUseBtnRect );
	ScreenToClient( rcUseBtnRect );

	pAddButton->GetWindowRect( rcAddBtn );
	ScreenToClient( rcAddBtn );

	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) 영역크기 수정
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_INDICATOR_INFO);
	CRect rcStatic;
	pStatic->GetWindowRect(rcStatic);
	ScreenToClient( rcStatic );

	// 3. 지표를 표시할 영역을 결정한다. 간격 조정선의 차이를 반영한다.
	CRect rcIndicatorRect, rcUseIndicatorRect;
	rcIndicatorRect.left = rcBtnRect.left;
	rcIndicatorRect.top = rcBtnRect.bottom + 1;
	rcIndicatorRect.right = rcBtnRect.right;
	rcIndicatorRect.bottom = rcStatic.bottom;

	rcUseIndicatorRect.left = rcUseBtnRect.left;
	rcUseIndicatorRect.top = rcUseBtnRect.bottom + 1;
	rcUseIndicatorRect.right = rcUseBtnRect.right;
	rcUseIndicatorRect.bottom = rcAddBtn.top - 3;	
		
	// 4. GoupButton의 영역을 설정한다.
	m_ctrlGroupButton.SetIndicatorSize(rcIndicatorRect);
	m_ctrlUseGroupButton.SetIndicatorSize(rcUseIndicatorRect);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Param  const CString &strIndicatorName : 
// Param  const int nCheck : 
// Comments		: 지표를 선택할때 Dialog를 가져와서(or 생성해서) 보여주는 함수.
//				  지표를 선택할때 CBaseDialogContainer에 해당 Dialog오브젝트가 있는지 확인한다. 
//				  있다면 Get으로 가져오고, 없다면 새 Dialog오브젝트를 생성해서 
//				  CBaseDialogContainer클래스에 Insert한다.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SelChangeChildDlg( const CString &strIndicatorName, const CString &strGraphName)
{
	ML_SET_LANGUAGE_RES();

	// 1. 선택되는 지표가 바뀌면 SubTab에 변경된 지표명을 저장한다.
	m_tabIndicatorSetUpAll.SetSubGraphName( strIndicatorName );

	// 2. 현재 SubTab이 "설명" Tab 이면 지표 설명 파일을 읽어들인다.
	if( m_tabIndicatorSetUpAll.IsWindowVisible() && m_tabIndicatorSetUpAll.GetCurSel() == 1 )
		m_tabIndicatorSetUpAll.ReadIndicatorExpFile();

	m_strIndicatorName = strIndicatorName;

	IGraphInfo* pIGraphInfo = NULL;
	if( strGraphName.IsEmpty())	pIGraphInfo = m_pIGraphInfoManager->GetIndicatorInfo(	IGraphInfoManager::IEILT_USER_DEFAULT,	strIndicatorName, IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET_AND_SET);
	else						pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo(		IGraphInfoManager::IEILT_USER,			strGraphName);
	if( !pIGraphInfo) return;

	// "설정" Tab일때에만 ChildDlg를 Hide 한다.
	if( m_tabIndicatorSetUpAll.GetCurSel() == 0 )
		ShowChildDialog(FALSE);

	// Dialog관리클래스에 dialog가 있다면 가져온다.
 	m_pChildDlg = m_BaseDialogContainer.GetBaseDialog((LPCTSTR)m_strIndicatorName);

	// Dialog 오브젝트가 없다면 새로 생성하여 Dialog관리클래스에 Insert한다.
	if(m_pChildDlg == NULL)
	{
		m_MakeChildDlg.Initial( m_pIChartOCX, pIGraphInfo, this, m_pChildDlg, m_pParentWnd);
		//m_pChildDlg->GetTail()
		m_BaseDialogContainer.AddBaseDialog((LPCTSTR)m_strIndicatorName, m_pChildDlg);
	}
	m_pChildDlg->Initial( pIGraphInfo);
	pIGraphInfo->Release();

	if( m_pChildDlg) m_bSaveDefault = m_pChildDlg->m_bSaveDefault;
	UpdateData(FALSE);

	// ChildDlg 위치 지정
	SetChildDlgPosition();

	// "설정" Tab일때에만 현재의 ChildDlg를 Show 한다.
	if( m_tabIndicatorSetUpAll.GetCurSel() == 0 )
		ShowChildDialog(TRUE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Param  BOOL bShow : bShow - TRUE: Show, FALSE: Hide
// Comments		: ChildDialog를 Show / Hide시킴.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::ShowChildDialog(BOOL bShow)
{
	// 1. ChildDialog의 List가 없으면 빠져나간다. 
	if( !m_pChildDlg) return;
	
	// 2. m_pChildDlg를 loop를 돌면서 Child Dialog를 Show / Hide를 한다.
	m_pChildDlg->ShowWindow( bShow ? SW_SHOW : SW_HIDE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Comments		: ChildDlg의 위치를 세팅한다.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SetChildDlgPosition()
{
	CRect rcCommon;
	CButton* pCommonBtn = (CButton* )m_tabIndicatorSetUpAll.GetSubTab(0);

	if( pCommonBtn == NULL || !m_pChildDlg) return;

	pCommonBtn->GetWindowRect(rcCommon);
	ScreenToClient(rcCommon);

	pCommonBtn->ShowWindow(SW_HIDE);

	// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - "설정"과 "설명" Tab의 유지를 위해서 수정.
	if( m_tabIndicatorSetUpAll.GetCurSel() == 0 )
		m_pChildDlg->SetWindowPos( &wndTop, rcCommon.left, rcCommon.top, rcCommon.Width(), rcCommon.Height(), SWP_SHOWWINDOW);
	else
		m_pChildDlg->SetWindowPos( &wndTop, rcCommon.left, rcCommon.top, rcCommon.Width(), rcCommon.Height(), SWP_HIDEWINDOW);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  const int nCheck : 
// Param  const CString &strIndicatorName : 
// Comments		: ListCtrl에서 다른 지표등을 선택했을 경우. (선택된 지표의 ChildDlg를 생성한다.)
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SelChangeGraph( const CString &strIndicatorName, const CString &strGraphName)
{
	SelChangeChildDlg( strIndicatorName, strGraphName);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 23
// Return void : 
// Comments		: ListCtrl의 아이템체크상태를 Chart와 일치시킨다. (지표 추가할때 
//				  추가할 Block수를 넘겼을 경우를 대비함)
//				  현재 사용중인 지표명을 담은 List를 전부 지우고 MainBlock에서 다시 사용중인
//				  지표명리스트를 가져온다. ListCtrl의 모든 체크상태를 지우고, 지표명List로
//				  체크상태를 다시 세팅한다.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SyncItemCheck()
{
	if( m_pIndicatorNameList)
	{
		m_pIndicatorNameList->RemoveAll();
		m_pIndicatorNameList->Release();
		m_pIndicatorNameList = NULL;
	}
	m_pIndicatorNameList =  m_pIChartManager->GetAllIndicatorNameStringList();

	// ListCtrl에 다시 체크한다.
	m_ctrlGroupButton.SetAllUnCheck();
	m_ctrlGroupButton.SetCheck(m_pIndicatorNameList);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::InitializeSelectedIndicator( void )
{
	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, m_strIndicatorName);
	if(pIGraphInfo == NULL || !m_pChildDlg) return;
	
	m_pChildDlg->Initial(pIGraphInfo);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
// Modifier		: 
// Comments		: 전체 지표를 초기화 한다.
//-----------------------------------------------------------------------------
//sy 2006.04.24. -> 전체지표 초기화
// -> 초기화 되면 복원 될수가 없기 때문에 화면에 바로 적용하기
void CDlgMainTabIndicatorSetUp::OnInitilizeAllindicator( void ) 
{
	if(!IsActionInitilizeAllindicator())
		return;

	// 사용자 지표설정값을 모두 삭제.
	m_pIGraphInfoManager->RemoveAllIndicatorInfo( IGraphInfoManager::IEILT_USER);
	m_pIChartManager->Initialize_GraphInfo(false);

	POSITION pos = m_BaseDialogContainer.GetStartPosition();
	while(pos != NULL)
	{
		CBaseDialog*pChildDialog = m_BaseDialogContainer.GetNextAssoc(pos);
		if( pChildDialog == NULL) continue;

		Initilizeindicator( pChildDialog);
	}

	m_bPreview = TRUE;

	m_pIChartOCX->InvalidateControl();
}

bool CDlgMainTabIndicatorSetUp::IsActionInitilizeAllindicator()
{
	if(m_pIChartOCX == NULL || m_pIGraphInfoManager == NULL || m_pIChartManager == NULL)
		return false;

	ML_SET_LANGUAGE_RES();
	CString strMsgInit, strMsgInitTitle;
	strMsgInit.LoadString(IDS_MSG_INIT);
	strMsgInitTitle.LoadString(IDS_MSG_INIT_TITLE);
	CString strMessage(strMsgInit);
	int nOk = MessageBox(strMessage, strMsgInitTitle, MB_OKCANCEL | MB_ICONQUESTION);
	return (nOk == IDOK);
}

bool CDlgMainTabIndicatorSetUp::Initilizeindicator(CBaseDialog* pBaseDlg)
{
	if(pBaseDlg == NULL)
		return false;

	// "기본값으로 저장" 옵션은 system default 값으로 사용하진 않는다.
	m_bSaveDefault = pBaseDlg->m_bSaveDefault = pBaseDlg->m_bOrgSaveDefault;

	// Dlg 초기화
	CString strIndicatorName = pBaseDlg->GetIndicatorName();
	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, strIndicatorName);
	if(pIGraphInfo == NULL)
		return false;

	// subGraph title이 조건값에 해당되는 경우에 
	pIGraphInfo->ChangeAllSubGraphTitleFromCalConditionData();
	pBaseDlg->Initial(pIGraphInfo);
	pIGraphInfo->Release();

	// defaulte 값을 화면에 바로 반영시켜주기 (화면색, 조건값에 따른 계산 다시함)
	m_pIChartManager->ChangeGraphDataFromGraphInfo(strIndicatorName, true);
	return true;
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
// Return void	: 
// Comments		: 저장 유무에 상관없이 선택된 지표의 값을 디폴트 값으로 돌리기
//				  -> 복원될수가 있기 때문에 화면에 바로 적용되지는 않는다.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnDefaultIndicatordata() 
{
	//sy 2006.04.24 -> 선택지표 초기화
	if(m_pChildDlg == NULL)
		return;

	CBaseDialog* pHeadDlg = m_pChildDlg;
	if(pHeadDlg == NULL || pHeadDlg->GetIndicatorName() != m_strIndicatorName)
		return;

	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, m_strIndicatorName);
	if(pIGraphInfo == NULL)
		return;

	pHeadDlg->Initial(pIGraphInfo);
	pIGraphInfo->Release();

	// defaulte 값을 화면에 바로 반영시켜주기 (화면색, 조건값에 따른 계산 다시함)
	//ApplyGraph(m_strIndicatorName);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 5
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnChkWithDefault() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	//IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo(m_strIndicatorName);
	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, m_strIndicatorName);
	if( pIGraphInfo == NULL || !m_pChildDlg) return;
	
	m_pChildDlg->m_bSaveDefault = m_bSaveDefault;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 30
// Return void	: 
// Comments		: 전체 지표 설정 내용을 저장한다.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnSave() 
{
	m_bSave = TRUE;

	POSITION posDialog = m_BaseDialogContainer.GetStartPosition();
	CBaseDialog *pChildDialog = NULL;
	while( posDialog)
	{
		pChildDialog = m_BaseDialogContainer.GetNextAssoc( posDialog);

		// 적용할 Data는 UserIndicatorInfo에 적용해야한다.
		IGraphInfo *pIGraphInfo = pChildDialog->GetGraphInfo();
		if( !pIGraphInfo) continue; 

		pChildDialog->OnApply( pIGraphInfo);

		// 지표를 계산
		if( pChildDialog->DoCalculate())
		{
			// 그물차트는 패킷갯수도 설정할 수 있으므로 항상 패킷 초기화를 시켜준다.
			ILPCSTR szIndicatorName	= pIGraphInfo->GetIndicatorNameString();
			if( !strcmp( szIndicatorName, _MTEXT( C2_RAINBOW_CHART )))
				m_pIChartManager->Initialize_AllPackets( _MTEXT( C2_RAINBOW_CHART ), true);
		}

		if( pIGraphInfo) pIGraphInfo->Release();
	}


	// (2007/1/23 - Seung-Won, Bae) Do not update for each graph.
	//		Because, some GraphInfo does not have BaseDialog.
	m_pIChartManager->ChangeAllGraphDataFromGraphInfo( true);

	// 다시 그려줌
	m_pIChartOCX->InvalidateControl();
	m_pOrgIndicatorNameList->RemoveAll();

	m_bPreview = FALSE;
	m_bSave = FALSE;
}



// "취소" 버튼 - 모든 내용을 화면 로드 상태로 돌린 후 화면 닫기
void CDlgMainTabIndicatorSetUp::OnBtnRecover() 
{
	m_OrgViewData.ChangeAllIndicatorList( m_pIGraphInfoManager);

	// 차트화면을 초기화면으로 복원한다.
	RestoreChartCfg(m_nInitChartIndex);
}

//sy 2006.04.20.
void CDlgMainTabIndicatorSetUp::RestoreChartCfg(const int nInitChartIndex)
{
	if(m_pIChartOCX == NULL || nInitChartIndex < 0)
		return;

	// 차트화면을 초기화면으로 복원한다.
	m_pIChartOCX->SaveChartCFG_RestoreChartOfIndex(nInitChartIndex);
}

int CDlgMainTabIndicatorSetUp::GetInitChartIndex() const
{
	return m_nInitChartIndex;
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 12
// Return void : 
// Comments		: 미리보기
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnPreview() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = TRUE;

	POSITION posDialog = m_BaseDialogContainer.GetStartPosition();
	CBaseDialog *pChildDialog = NULL;
	while( posDialog)
	{
		pChildDialog = m_BaseDialogContainer.GetNextAssoc( posDialog);

		// 적용할 Data는 UserIndicatorInfo에 적용해야한다.
		IGraphInfo *pIGraphInfo = pChildDialog->GetGraphInfo();
		if( !pIGraphInfo) continue;

		pChildDialog->OnApply( pIGraphInfo);
		// 지표를 계산
		if( pChildDialog->DoCalculate())
		{
			// 그물차트는 패킷갯수도 설정할 수 있으므로 항상 패킷 초기화를 시켜준다.
			ILPCSTR szIndicatorName	= pIGraphInfo->GetIndicatorNameString();
			if( !strcmp( szIndicatorName, _MTEXT( C2_RAINBOW_CHART )))
				m_pIChartManager->Initialize_AllPackets( _MTEXT( C2_RAINBOW_CHART ), true);
		}

		if( pIGraphInfo) pIGraphInfo->Release();
	}


	// (2007/1/23 - Seung-Won, Bae) Do not update for each graph.
	//		Because, some GraphInfo does not have BaseDialog.
	m_pIChartManager->ChangeAllGraphDataFromGraphInfo( true);

	// 다시 그려줌
	m_pIChartOCX->InvalidateControl();
}


BOOL CDlgMainTabIndicatorSetUp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			//OnBtnSave();
			OK();
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			Cancel();
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMainTabIndicatorSetUp::OK()
{
	if( m_pParentWnd != NULL )
		( (CDlgEnvSetUpAll *)m_pParentWnd)->OK();
}

void CDlgMainTabIndicatorSetUp::Cancel()
{
	if( m_pParentWnd != NULL )
		( (CDlgEnvSetUpAll *)m_pParentWnd)->Cancel();
}



BOOL CDlgMainTabIndicatorSetUp::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_INDICATOR_NAME);

	if( m_pftOldStaticFont != NULL )
		pStatic->SetFont(m_pftOldStaticFont);

	return CDialog::DestroyWindow();
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 11
// Return  void : 
// Param  CBaseTabDlg *pDlgTabPage   : 
// Explain		: Tab이 바뀌었을때 수행해야 되는 동작이 있으면 호출된다.
//-----------------------------------------------------------------------------
//sy 2005.10.12
bool CDlgMainTabIndicatorSetUp::ChangeTabPage( CBaseTabDlg *pSourceDlgTabPage )
{
	int nDrawingCurPrice = 0;
	if(!GetDrawingCurPrice(pSourceDlgTabPage, nDrawingCurPrice))
		return false;

	IGraphInfo* pIGraphInfo = GetIndicatorInfoInBaseDlg(_MTEXT( C2_PRICE_CHART ));
	if(pIGraphInfo == NULL)
		return false;

	int nCurData = -1;
	BOOL bLowHigh = FALSE, bLowCur = FALSE, bHighCur = FALSE, bPrevCur = FALSE, bShowOHLC = FALSE;
	pIGraphInfo->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
	nCurData = nDrawingCurPrice;
	pIGraphInfo->SetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
	pIGraphInfo->Release();
	return true;
}

// -------------------------------------------------------------------------------------
CBaseTabDlg::DLG_TYPE CDlgMainTabIndicatorSetUp::GetDialogType() const
{
	return CBaseTabDlg::INDI_SET;
}

IGraphInfo* CDlgMainTabIndicatorSetUp::GetIndicatorInfoInBaseDlg(const CString& strIndicatorName)
{
	if(m_pChildDlg == NULL || strIndicatorName.IsEmpty())
		return NULL;

	IGraphInfo* pIGraphInfo = m_pChildDlg->GetGraphInfo();
	if( pIGraphInfo)
	{
		ILPCSTR szGraphName = pIGraphInfo->GetGraphNameString();
		if( !strcmp( szGraphName, strIndicatorName)) return pIGraphInfo;
		pIGraphInfo->Release();
	}

	return NULL;
}

bool CDlgMainTabIndicatorSetUp::GetDrawingCurPrice(CBaseTabDlg *pSourceDlgTabPage, int& nDrawingCurPrice) const
{
	if(pSourceDlgTabPage == NULL || (CBaseTabDlg*)this == pSourceDlgTabPage)
		return false;

	if(pSourceDlgTabPage->GetDialogType() != CBaseTabDlg::ENV_SET)
		return false;

	pSourceDlgTabPage->UpdateData(TRUE);
	return (pSourceDlgTabPage->GetCurrentPrice(nDrawingCurPrice));
}
//sy end


// (2006/8/15 - Seung-Won, Bae) Support MChart Interface
// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
void CDlgMainTabIndicatorSetUp::GetMChartInterface( COutsideCommonInterfaceFunction &p_outsideIF)
{
	if( !m_pIChartOCX) return;
	ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
	p_outsideIF.SetOutsideCommonInterface( m_pIChartOCX->GetMChartInterface(), CString( szDataPath));
}

CString CDlgMainTabIndicatorSetUp::GetDataFolderPath( void )
{
	if( !m_pIChartOCX) return "";

	CString strPath;
	ILPCSTR szPath = m_pIChartOCX->GetDefaultPathString();
	strPath = szPath;

	return strPath;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnAdd() 
{
	ML_SET_LANGUAGE_RES();

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLastClickedTime < 100) return;

	GetDlgItem( IDC_BTN_ADD)->EnableWindow( FALSE);

	// TODO: Add your control notification handler code here
	CIndicatorNameListCtrl *pGeneralNameListCtrl = NULL;
	pGeneralNameListCtrl = m_ctrlGroupButton.GetSelectListCtrl();
	
	if( pGeneralNameListCtrl != NULL )
	{
		int nGeneralSelectItemIndex = pGeneralNameListCtrl->GetSelectItemIndex();
		if(  nGeneralSelectItemIndex > -1 )
		{
			CIndicatorNameListCtrl *pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();

// 2008.01.25 by LYH >>
			//CString strIndicatorName = pGeneralNameListCtrl->GetItemText( pGeneralNameListCtrl->GetSelectItemIndex(), 0 );
			CString strIndicatorName = _LTOM2(pGeneralNameListCtrl->GetItemText( pGeneralNameListCtrl->GetSelectItemIndex(), 0 ));
// 2008.01.25 by LYH >>

			if( strIndicatorName != "" )
			{
				if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
					AfxMessageBox( IDS_MSG_PRICECHART_ADD );
				else
				{
					CString strGraphName;
					AddGraph( strIndicatorName, strGraphName);
					if( !strGraphName.IsEmpty()) pUseNameListCtrl->AddGraph( strIndicatorName, strGraphName);
				}
			}
		}
	}
	GetDlgItem( IDC_BTN_ADD)->EnableWindow( TRUE);

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = timeGetTime();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnAddAll() 
{
	ML_SET_LANGUAGE_RES();

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLastClickedTime < 100) return;

	GetDlgItem( IDC_BTN_ADD_ALL)->EnableWindow( FALSE);

	// TODO: Add your control notification handler code here
	CIndicatorNameListCtrl *pGeneralNameListCtrl = NULL;
	pGeneralNameListCtrl = m_ctrlGroupButton.GetSelectListCtrl();
	
	if( pGeneralNameListCtrl != NULL )
	{
		int nAllCount = pGeneralNameListCtrl->GetItemCount();
		CIndicatorNameListCtrl *pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();
		CString strIndicatorName = "";
		for( int nGeneralItemIndex = 0 ; nGeneralItemIndex < nAllCount ; nGeneralItemIndex++ )
		{
// 2008.01.25 by LYH >>
			//strIndicatorName = pGeneralNameListCtrl->GetItemText( nGeneralItemIndex, 0 );
			strIndicatorName = _LTOM2(pGeneralNameListCtrl->GetItemText( nGeneralItemIndex, 0 ));
// 2008.01.25 by LYH <<
			if( strIndicatorName != "" )
			{
				if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
				{
					AfxMessageBox( IDS_MSG_PRICECHART_ADD );
					continue;
				}

				CString strGraphName;
				AddGraph( strIndicatorName, strGraphName);
				if( !strGraphName.IsEmpty()) pUseNameListCtrl->AddGraph( strIndicatorName, strGraphName);
			}
		}
	}	
	GetDlgItem( IDC_BTN_ADD_ALL)->EnableWindow( TRUE);

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = timeGetTime();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnDelete() 
{
	ML_SET_LANGUAGE_RES();

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLastClickedTime < 100) return;

	GetDlgItem( IDC_BTN_DELETE)->EnableWindow( FALSE);

	// TODO: Add your control notification handler code here
	CIndicatorNameListCtrl *pUseNameListCtrl = NULL;
	pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();
	
	if( pUseNameListCtrl)
	{
		int nUseSelectItemIndex = pUseNameListCtrl->GetSelectItemIndex();
		if( 0 <= nUseSelectItemIndex)
		{
// 2008.01.25 by LYH >>
			//CString strIndicatorName	= pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 0);
			CString strIndicatorName	= _LTOM2(pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 0));
// 2008.01.25 by LYH <<
			CString strGraphName		= pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 1);
			if( !strGraphName.IsEmpty())
			{
				if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
					AfxMessageBox( IDS_MSG_PRICECHART_DEL );
				// OCX의 AddChart호출
				else RemoveGraph( strGraphName);
			}
		}
	}

	GetDlgItem( IDC_BTN_DELETE)->EnableWindow( TRUE);

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = timeGetTime();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnDeleteAll() 
{
	ML_SET_LANGUAGE_RES();

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLastClickedTime < 100) return;

	GetDlgItem( IDC_BTN_DELETE_ALL)->EnableWindow( FALSE);

	// TODO: Add your control notification handler code here
	CIndicatorNameListCtrl *pUseNameListCtrl = NULL;
	pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();
	
	if( pUseNameListCtrl != NULL)
	{
		int nAllCount = pUseNameListCtrl->GetItemCount();
		for( int nUseItemIndex = nAllCount - 1; 0 <= nUseItemIndex; nUseItemIndex--)
		{
// 2008.01.25 by LYH >>
			//CString strIndicatorName	= pUseNameListCtrl->GetItemText( nUseItemIndex, 0);
			CString strIndicatorName	= _LTOM2(pUseNameListCtrl->GetItemText( nUseItemIndex, 0));
// 2008.01.25 by LYH <<
			CString strGraphName		= pUseNameListCtrl->GetItemText( nUseItemIndex, 1);

			if( strGraphName != "")
			{
				if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
				{
					AfxMessageBox( IDS_MSG_PRICECHART_DEL );
					continue;
				}

				// OCX의 AddChart호출
				RemoveGraph( strGraphName);
			}
		}
	}		
	GetDlgItem( IDC_BTN_DELETE_ALL)->EnableWindow( TRUE);

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = timeGetTime();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 15
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnWithDefault() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	//IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo(m_strIndicatorName);
	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, m_strIndicatorName);
	if(pIGraphInfo == NULL)
		return;

	ILPCSTR szGraphName = NULL;
	CBaseDialog *pChildDialog = m_BaseDialogContainer.GetBaseDialog(szGraphName);
	if( pChildDialog == NULL )
		return;

	IGraphInfo *pDefaultIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, szGraphName);
	if( pChildDialog->m_bSaveDefault == TRUE )
	{
		IGraphInfo *pDefaultIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, szGraphName);
		pChildDialog->OnApply( pDefaultIndicatorInfo );
		pChildDialog->m_bOrgSaveDefault = TRUE;
	}

	pDefaultIndicatorInfo->Release();
}

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CDlgMainTabIndicatorSetUp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	CIndicatorNameListCtrl *pUseNameListCtrl = NULL;
	pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();
	if( !pUseNameListCtrl) return;

	int nCount = pUseNameListCtrl->GetItemCount();
	int nUseSelectItemIndex = 0;
	for(  nUseSelectItemIndex = 0; nUseSelectItemIndex < nCount; nUseSelectItemIndex++)
		if( pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 1) == p_szGraphName) break;
	if( nCount <= nUseSelectItemIndex) return;

// 2008.01.25 by LYH >>
	//CString strIndicatorName	= pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 0);
	CString strIndicatorName	= _LTOM2(pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 0));
// 2008.01.25 by LYH <<
	CString strGraphName		= pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 1);
	pUseNameListCtrl->RemoveGraph( nUseSelectItemIndex);

	if( strGraphName.IsEmpty()) return;

	// 해당 지표의 설정Dlg를 가져온다.
	CBaseDialog *pChildDialog = m_BaseDialogContainer.GetBaseDialog( strIndicatorName);
	if( pChildDialog) pChildDialog->Initial( NULL);

	// 지표명을 List에서 제거
	POSITION pos = m_pIndicatorNameList->Find( strIndicatorName);
	if(pos == NULL) return;
	m_pIndicatorNameList->RemoveAt(pos);

	// (2007/1/25 - Seung-Won, Bae) Show other Dialog
	NM_LISTVIEW nmListView;
	if( nUseSelectItemIndex >= pUseNameListCtrl->GetItemCount() - 1)
			nmListView.iItem =	pUseNameListCtrl->GetItemCount() - 1;
	else	nmListView.iItem =	nUseSelectItemIndex;
	long lResult = 0;
	pUseNameListCtrl->OnClick( ( NMHDR *)&nmListView, &lResult);

	// chart화면에 바로 적용이 되므로 미리보기를 한것과 같은 상태이다.
	m_bPreview = TRUE;
}

BOOL CDlgMainTabIndicatorSetUp::RemoveGraph( const char *p_szGraphName)
{
	if( !m_pIChartOCX) return FALSE;
	return m_pIChartOCX->RemoveGraph( p_szGraphName);
}
