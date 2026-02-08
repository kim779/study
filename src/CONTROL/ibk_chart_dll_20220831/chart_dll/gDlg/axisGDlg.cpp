#include "stdafx.h"
#include "axisGDlg.h"
#include "../../h/axisgbinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAxisGDlgApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisGDlgApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAxisGDlgApp::CAxisGDlgApp()
{

}

CAxisGDlgApp::~CAxisGDlgApp()
{

}


CAxisGDlgApp theApp;

CString	CAxisGDlgApp::GetTitle(int nKind)
{
	CHART_INFO* pObject = GetChartObj(nKind);	

	if (pObject == NULL) 
	{
		if (nKind == GK_CDVV || nKind == GK_EQVV)
			return _T("거래량");
		return _T("");
	}

	return pObject->m_sztitle;
}

class CHART_INFO* CAxisGDlgApp::GetChartObj(int nKind)
{
	CHART_INFO*	pObject = NULL;
	
	m_mapChartInfo.Lookup(nKind, pObject);
	return pObject;
}

bool CAxisGDlgApp::IsMain(int nKind)
{
	CHART_INFO*	pObject = GetChartObj(nKind);	

	if (pObject == NULL)
		return false;	

	return ((pObject->m_nGroup < 0) ? true : false);
}

bool CAxisGDlgApp::IsExist(int nKind)
{
	CHART_INFO*	pObject = GetChartObj(nKind);	

	if (pObject == NULL)
		return false;

	return true;
}

int CAxisGDlgApp::GetGroup(int nKind)
{
	CHART_INFO*	pObject = GetChartObj(nKind);

	if (pObject == NULL)
		return -1;
	
	return pObject->m_nGroup;
}

CString CAxisGDlgApp::GetGroupName(int nKind)
{
	CHART_INFO* pObject = GetChartObj(nKind);

	if (pObject == NULL)
		return _T("");
	
	return pObject->m_szGroup;
}

int CAxisGDlgApp::GetKind(CArray<int, int>& arKind, bool bMain)
{
	int		nCount = m_mapChartInfo.GetCount();
	CHART_INFO*	pObject = NULL;

	arKind.RemoveAll();

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		pObject = m_mapChartInfo.GetAt(ii);

		if (pObject == NULL)
			continue;

		if (bMain)
		{
			if (!pObject->m_nGroup)		
				arKind.Add(pObject->m_nKind);
		}
		else
		{
			if (!pObject->m_nGroup)		
				arKind.Add(pObject->m_nKind);
		}			
	}

	return arKind.GetSize();
}

struct _indiInfo* CAxisGDlgApp::GetIndiObj(int nKind)
{
	struct _indiInfo* pObject = NULL;

	m_mapScreenInfo.Lookup(nKind, (_indiInfo * &) pObject);
	return pObject;
}

struct _graph* CAxisGDlgApp::GetDefault(int nKind)
{
	struct _graph*	pGraph = NULL;
	m_mapDefault.Lookup(nKind, pGraph);
	return pGraph;
}

bool CAxisGDlgApp::IsPrice(int nKind)
{
	bool	bRet = false;

	switch (nKind)
	{
	case GK_BOLB:
	case GK_ENVL:
	case GK_PIVOT:
	case GK_DEMARK:
	case GK_PMA:
	case GK_PARB:
	case GK_NET:
		bRet = true;
		break;
	}

	return bRet;
}

BOOL CAxisGDlgApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	AfxEnableControlContainer();
	
	return CWinApp::InitInstance();
}

void CAxisGDlgApp::ReleaseChart() 
{		
	m_mapChartInfo.RemoveAll(); 
	m_arGroup.RemoveAll();
	m_mapScreenInfo.RemoveAll(); 
}

/*
class CMyTool* CAxisGDlgApp::GetMyTool(CWnd* pwndView)
{
	if (!m_pMyTool)
		m_pMyTool = new CMyTool(pwndView);

	return m_pMyTool;
}
*/

#include "../../h/axisfire.h"
CFont* CAxisGDlgApp::GetFont(CWnd *pView, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

void CAxisGDlgApp::InitChart()
{	
	int		nCount = 0;
	int		nSize = 0;
	int		ii = 0;
	CString		tmpstr;
	CHART_INFO*	pObject = NULL;
	char*	group[] = {"추세지표", "모멘텀지표", "거래량지표", "시장강도지표", "채널지표", "기타지표"};
	
	nCount = sizeof(group) / sizeof(char*);

	m_arGroup.RemoveAll();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{	
		tmpstr.Format("%s", group[ii]);
		m_arGroup.Add(tmpstr);
	}

	nCount = sizeof(match) / sizeof(struct _kindmatch);

	m_mapChartInfo.RemoveAll();
	nSize = m_arGroup.GetSize();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		tmpstr.Format("%s", match[ii].str);		
		pObject = new CHART_INFO;
		pObject->m_nKind = match[ii].kind;
		pObject->m_sztitle = tmpstr;
		pObject->m_nGroup = match[ii].group;		

		if (pObject->m_nGroup <= 0 || pObject->m_nGroup > nSize)
		{
			pObject->m_szGroup = _T("");
			pObject->m_nGroup = -1;
		}
		else
			pObject->m_szGroup = m_arGroup.GetAt(pObject->m_nGroup - 1);

		m_mapChartInfo.SetAt(match[ii].kind, pObject);		
	}

	nCount = sizeof(__indiAll) / sizeof(struct _indiInfo);
	m_mapScreenInfo.RemoveAll();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		m_mapScreenInfo.SetAt(__indiAll[ii].kind, &__indiAll[ii]);		
	}
//////////////////////////////////////////////////////////////////////////////////////
// setting default value
//////////////////////////////////////////////////////////////////////////////////////
	struct _graph*	pGraph = NULL;
	nCount = sizeof(basicInfo) / sizeof(struct _basicInfo);

	m_mapDefault.RemoveAll();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		pGraph = new struct _graph;
		memset(pGraph, 0x00, SZ_GRAPH);
		pGraph->wCalOption = basicInfo[ii].wCOption;
		pGraph->dwDraOption = basicInfo[ii].dwDOption;
		pGraph->tInfo.rgbOverBuy = basicInfo[ii].tickInfo.rgbOverBuy;
		pGraph->tInfo.rgbOverSell = basicInfo[ii].tickInfo.rgbOverSell;
		memcpy(&pGraph->cInfo, &basicInfo[ii].chartInfo, SZ_CHARTINFO);
		memcpy(&pGraph->tInfo, &basicInfo[ii].tickInfo, SZ_TICKINFO);
		m_mapDefault.SetAt(basicInfo[ii].wGrpKind, pGraph);
	}

	memset(&m_envDefault, 0x00, SZ_ENVINFO);

	m_envDefault.display.wLMargin = 2;
	m_envDefault.display.wRMargin = 20;
	m_envDefault.display.dwDspOption = DO_SCREEN | DO_DISPRT | DO_TKVGRID | DO_TKHGRID | DO_MOD | DO_LOCK | DO_CURRENT | DO_MINMAX | DO_USEGRID | DO_VIEWGRID | DO_NOVIEWTIP;

	m_envDefault.display.rgbBG = RGB(243,249,254);
	m_envDefault.display.rgbEdge = RGB(192,192,192);
	m_envDefault.display.rgbChartBG = RGB(255,255,255); 
	m_envDefault.display.rgbChartEdge = RGB(180,192,203);

	//m_envDefault.display.toolinfo.lcolor = DEFCR4;
	m_envDefault.display.toolinfo.rgbLine = DEFCR4;
	m_envDefault.display.hline.rgbLColor = RGB(180,192,203);
	m_envDefault.display.vline.rgbLColor = RGB(180,192,203);
	m_envDefault.display.dline.rgbLColor = RGB(255, 0, 0);	
	m_envDefault.display.cline.rgbLColor = RGB(0, 128, 0);	

	//m_envDefault.display.toolinfo.btLStyle = PS_SOLID;
	m_envDefault.display.toolinfo.btLineStyle = PS_SOLID;
	m_envDefault.display.hline.btLStyle = PS_SOLID;
	m_envDefault.display.vline.btLStyle = PS_SOLID;
	m_envDefault.display.dline.btLStyle = PS_DOT;
	m_envDefault.display.cline.btLStyle = PS_SOLID;	

	//m_envDefault.display.toolinfo.btLWidth = 1;
	m_envDefault.display.toolinfo.btLineWidth = 1;
	m_envDefault.display.hline.btLWidth = 1;
	m_envDefault.display.vline.btLWidth = 1;
	m_envDefault.display.dline.btLWidth = 1;
	m_envDefault.display.cline.btLWidth = 1;	

	CString	fname = _T("굴림");
	memset(m_envDefault.display.font.acFName, 0x00, sizeof(m_envDefault.display.font.acFName));
	memcpy(m_envDefault.display.font.acFName, fname, fname.GetLength());
	
	m_envDefault.display.font.btFPoint = 9;
	m_envDefault.display.font.btFWeight = 0;
}
