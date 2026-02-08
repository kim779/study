#include "stdafx.h"
#include "axisgdlg.h"
#include "ObjConfig.h"
#include "IndiObj.h"
#include "IndiObjMA.h"
#include "MainPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CObjConfig::CObjConfig(CWnd* pMain, COLORREF tkCOLOR, char* envinfo, char* graphtotal, char* graphinfo, CWnd* pParent)
	: CDialog(CObjConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjConfig)
	//}}AFX_DATA_INIT

	m_pMain = pMain;
	m_pGraph = (struct _graph*)graphinfo;
	memset(&m_tmpGraph, 0x00, sizeof(m_tmpGraph));
	memcpy(&m_tmpGraph, m_pGraph, SZ_GRAPH);
	m_pChild = NULL;
	m_pEnv = envinfo;
	m_tkCOLOR = tkCOLOR;

	int	nMaxCount = ((struct _cntInfo*)graphtotal)->wMainCnt + ((struct _cntInfo*)graphtotal)->wAidCnt;
	graphtotal += SZ_CNTINFO;	
	struct	_graph* pGraph = (struct _graph*)graphtotal;
	m_pBong = NULL;

	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();

	for ( int ii = 0 ; ii < nMaxCount ; ii++ )
	{
		m_mapInfo.SetAt(pGraph[ii].wGKind, &pGraph[ii]);
		if (pApp->IsExist(pGraph[ii].wGKind) && pApp->IsMain(pGraph[ii].wGKind) && pGraph[ii].wGKind == GK_JPN)
		{
			m_pBong = pGraph;
		}
	}
}

void CObjConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjConfig)
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CObjConfig, CDialog)
	//{{AFX_MSG_MAP(CObjConfig)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BT_DEFAULT, OnBtDefault)
	ON_BN_CLICKED(IDC_BT_CUSTOM_SAVE, OnBtCustomSave)
	ON_BN_CLICKED(IDC_BT_CUSTOM_LOAD, OnBtCustomLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CObjConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect	rect;
	GetDlgItem(IDC_ST_FRAME)->GetWindowRect(rect);
	ScreenToClient(rect);	
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	
	if (m_pChild == NULL)
	{
		CHART_INFO* pObject = pApp->GetChartObj(m_tmpGraph.wGKind);

		if (pObject == NULL)
			return FALSE;

		if (pObject->m_nGroup < 0)
		{			
			UINT	ctrlID[] = {IDC_ST_DEFAULT, IDC_BT_CUSTOM_SAVE, IDC_BT_CUSTOM_LOAD};
			int	ncnt = sizeof(ctrlID) / sizeof(UINT);
			CWnd*	pWnd = NULL;
			for ( int ii = 0 ; ii < ncnt ; ii++)
			{
				pWnd = GetDlgItem(ctrlID[ii]);
				pWnd->ShowWindow(SW_HIDE);
			}
			m_pChild = new CMainPreview(m_tkCOLOR, m_pEnv);
			((CMainPreview*)m_pChild)->Create(CMainPreview::IDD, this);
			m_pChild->ShowWindow(SW_HIDE);
			CRect	rc;
			m_pChild->GetWindowRect(&rc);
			if (rc.Height() > rect.Height())
				rect.bottom += rc.Height() - rect.Height();
			m_pChild->MoveWindow(rect);
			m_pChild->ShowWindow(SW_SHOW);
			((CMainPreview*)m_pChild)->SetScreen(m_pBong, &m_tmpGraph, true);
			
		}
		else
		{
			if (m_tmpGraph.wGKind == GK_PMA || m_tmpGraph.wGKind == GK_VMA || m_tmpGraph.wGKind == GK_AMA)
			{				
				m_pChild = new CIndiObjMA();
				((CIndiObjMA*)m_pChild)->Create(CIndiObjMA::IDD, this);
				m_pChild->ShowWindow(SW_HIDE);
				m_pChild->MoveWindow(rect);
				m_pChild->ShowWindow(SW_SHOW);
				((CIndiObjMA*)m_pChild)->SetScreen(&m_tmpGraph, true);
			}
			else
			{
				m_pChild = new CIndiObj();
				((CIndiObj*)m_pChild)->Create(CIndiObj::IDD, this);
				m_pChild->ShowWindow(SW_HIDE);
				m_pChild->MoveWindow(rect);
				m_pChild->ShowWindow(SW_SHOW);
				((CIndiObj*)m_pChild)->SetScreen(&m_tmpGraph, true);
			}
		}
	}

	return TRUE;
}

void CObjConfig::OnDestroy() 
{	
	CDialog::OnDestroy();	
	
	if (LIB_IsWndAlive(m_pChild))
		LIB_DeleteWnd(m_pChild);

	m_mapInfo.RemoveAll();
}

void CObjConfig::OnOk() 
{
	memcpy(m_pGraph, &m_tmpGraph, sizeof(m_tmpGraph));

	struct _envInfo* penvinfo = (struct _envInfo*)m_pEnv;
	int iIndex = penvinfo->datainfo.btIndex;
	int iUnit = 0;
	switch (penvinfo->datainfo.btUnit)
	{
	case GU_FUTURE:
	case GU_OPTION:
	case GU_COFUT:
	case GU_COOPT:
		iUnit = GU_FUTURE-GU_CODE;
		break;
	case GU_CODE:
	case GU_INDEX:	
		iUnit = penvinfo->datainfo.btUnit - GU_CODE;
		break;
	case GU_FOREIGN:
		iUnit = 3;
		break;
	default:
		break;
	}

	penvinfo->datainfo.aaDayInfo[iUnit][iIndex - GI_DAY].wUnionGKind = ((CMainPreview*)m_pChild)->GetUnion();

	if (LIB_IsWndAlive(m_pChild))
		LIB_DeleteWnd(m_pChild);


	CDialog::OnOK();	
}

void CObjConfig::OnBtDefault() 
{
	//m_pChild->SendMessage(WM_MANAGE, MNG_DEFAULT, 1);
	// 디폴트일시 캡션창을 다시 수정할 필요가 없음
	m_pChild->SendMessage(WM_MANAGE, MNG_DEFAULT, 0);
//	OnBtCustomSave();
}

void CObjConfig::OnBtCustomSave() 
{
	struct _graph*	pGraph = NULL;
	m_mapInfo.Lookup(m_tmpGraph.wGKind, pGraph);
	pGraph->wCalOption = m_tmpGraph.wCalOption;
	pGraph->dwDraOption = m_tmpGraph.dwDraOption;
	memcpy(&pGraph->cInfo, &m_tmpGraph.cInfo, SZ_CHARTINFO);
	memcpy(&pGraph->tInfo, &m_tmpGraph.tInfo, SZ_TICKINFO);
}

void CObjConfig::OnBtCustomLoad() 
{
	struct _graph*	pGraph = NULL;
	
	m_mapInfo.Lookup(m_tmpGraph.wGKind, pGraph);
	m_tmpGraph.wCalOption = pGraph->wCalOption;
	m_tmpGraph.dwDraOption = pGraph->dwDraOption;
	memcpy(&m_tmpGraph.cInfo, &pGraph->cInfo, SZ_CHARTINFO);
	memcpy(&m_tmpGraph.tInfo, &pGraph->tInfo, SZ_TICKINFO);
	
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	CHART_INFO*	pObject = pApp->GetChartObj(m_tmpGraph.wGKind);

	if (pObject == NULL)
		return;

	if (pObject->m_nGroup < 0)
		((CMainPreview*)m_pChild)->SetScreen(m_pBong, &m_tmpGraph, true, false);
	else
	{
		if (m_tmpGraph.wGKind == GK_PMA || m_tmpGraph.wGKind == GK_VMA || m_tmpGraph.wGKind == GK_AMA)
			((CIndiObjMA*)m_pChild)->SetScreen(&m_tmpGraph, true, false);
		else
		{
			struct _tickinfo	tickinfo;
			CopyMemory(&tickinfo, &m_tmpGraph.tInfo, SZ_TICKINFO);
			((CIndiObj*)m_pChild)->SetScreen(&m_tmpGraph, true, false);
			CopyMemory(&m_tmpGraph.tInfo, &tickinfo, SZ_TICKINFO);
		}
	}
}
