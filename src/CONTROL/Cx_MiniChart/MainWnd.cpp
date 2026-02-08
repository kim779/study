// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Cx_MiniChart.h"
#include "MainWnd.h"
#include "GrpWnd.h"
#include <math.h>
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#define	sz_hCTRL	16

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd *pParent, _param* pInfo)
{
	EnableAutomation();

	CCx_MiniChartApp *pApp = (CCx_MiniChartApp *)AfxGetApp();
	pApp->SetView(pParent);
	
	m_bCtrl = false;
	m_pParent = pParent;
	m_pGrpWnd = nullptr;

	SetParam(pInfo);

}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	DISP_PROPERTY_EX(CMainWnd, "visible", GetVisible, SetVisible, VT_BOOL)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "Send", Send, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_NOTIFY(CMainWnd, "sMarket", m_strMarket, OnsMarketChanged, VT_BSTR)
	//}}AFX_DISPATCH_MAP
	//DISP_PROPERTY_EX_ID(CMainWnd, "sMarket", dispidsMarket, GetsMarket, SetsMarket, VT_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "SetMarket", dispidSetMarket, SetMarket, VT_EMPTY, VTS_I2)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {C353FAF5-4263-4055-8B55-C86B2A7D56EC}
static const IID IID_IMainWnd =
{ 0xc353faf5, 0x4263, 0x4055, { 0x8b, 0x55, 0xc8, 0x6b, 0x2a, 0x7d, 0x56, 0xec } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers
#define	MJ_FILE_NAME	"GXMiniChart.ghi"


int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString	root = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	CString name = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0);

	m_infoPath.Format("%s\\%s\\%s\\%s", root, GEXDIR, name, MJ_FILE_NAME);
	m_imgPath.Format("%s\\%s\\", root, IMAGEDIR);

	struct _minijisu	mj;
	ReadCondition(&mj);

	m_pGrpWnd = std::make_unique<CGrpWnd>(m_pParent, this, (char *)&mj, &m_param);
	m_pGrpWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE , CRect(0,0,0,0), this, 0);
//	m_pGrpWnd->init();
	return 0;	

}

void CMainWnd::Send() 
{
	// TODO: Add your dispatch handler code here
	m_pGrpWnd->Send();
}

void CMainWnd::SetParam(_param *pParam)
{
	m_param.key = pParam->key;
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
// 	m_clrForeColor = m_Param.tRGB = pParam->tRGB;
// 	m_clrBackColor = m_Param.pRGB = pParam->pRGB;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();	
}

BOOL CMainWnd::GetVisible() 
{
	// TODO: Add your property handler here
	
	return IsWindowVisible();//TRUE;
}

void CMainWnd::SetVisible(BOOL bNewValue) 
{
	// TODO: Add your property handler here
	if (bNewValue)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	
	if (cx <= 0 && cy <= 0)
		return;

	Resize();	
}

LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrpWnd)
		return m_pGrpWnd->OnUser(wParam, lParam);
	
	return 0;
}

void CMainWnd::Resize()
{
	CRect	rc;
	GetClientRect(&rc);
	
	if (rc.Width() == 0 || rc.Height() == 0)
		return;
	
	if (!m_pGrpWnd)	
		return;

	m_pGrpWnd->MoveWindow(0, 0, rc.Width(), rc.Height());

}

///////////////// Automation

BSTR CMainWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	
	return strResult.AllocSysString();
}

void CMainWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
	
}

/*
data		- _cdTick
graph		- line chart(1분봉, 틱)
parameter	- /d/r/e/u
d : display count - 0:all
r : request count
e : data expand(true-분, false-틱)
u : data unit - 1:종목, 2:업종, 3:선물, 4:업종, 5:ECN
t : draw tick - 0: no 1: left, 2: right
i : data index - 1:틱, 2:분
*/

void CMainWnd::ReadCondition(struct _minijisu *mj)
{
	if (readFile(m_infoPath, (char*)mj, sz_MINIJISU))
	{
		if (mj->version != MJ_VERSION)
		{
			DeleteFile(m_infoPath);
			MakeBasicInfo(mj);
		}
	}
	else
		MakeBasicInfo(mj);
}

void CMainWnd::SaveCondition(struct _minijisu *mj)
{
	writeFile(m_infoPath, (char*)mj, sz_MINIJISU);
}

#include "../../H/jmcode.h"
void CMainWnd::MakeBasicInfo(struct _minijisu *mj)
{
	constexpr _jisu	ijisu[MAX_JISU] = {	// kospi, kospi200, kosadq, kosdaq50, kospi선물
		{"0001", "코스피종합", 1, RGB(255,  0,  0), PS_SOLID, 1},
		{"0101", "코스피200", 0, RGB(23,134,216), PS_SOLID, 1},
		{"0201", "코스닥종합", 1, RGB(0,  0,255), PS_SOLID, 1},
		/*{"0303", "(코)KOSTAR", 0, RGB(126, 26, 77), PS_SOLID, 1},*/
		{"1"  , "(거)선물", 1, RGB(  0,128,  0), PS_SOLID, 1}	 // 30303, 최근월물
		//{"0"  , "(코)선물", 0, RGB(165,111, 227), PS_SOLID, 1},	 // 30303, 최근월물
	};

	int	codeN{};
	CString 	path = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	path += "\\tab\\fjcode.dat";

	CFile	file;
	struct  fjcode  FJCode {};
	CString strFName;
	if (!file.Open(path, CFile::modeRead | CFile::typeBinary))
	{
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}
	// 데이터의 갯수...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct fjcode));
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&FJCode, sizeof(struct fjcode));
		if (ii == 0)
		{
			strFName = CString(FJCode.hnam, FNameLen);
			strFName.TrimRight();
			strFName = strFName.Right(6);
			strFName = "선물" + strFName;
		
			break;
		}
	}

	file.Close();

	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		if (ii == 3)
		{
			CopyMemory(&mj->jisu[ii], &ijisu[ii], sz_JISU);
			CopyMemory(&mj->jisu[ii].name, strFName.GetBuffer(0), strFName.GetLength());
		}
		else
			CopyMemory(&mj->jisu[ii], &ijisu[ii], sz_JISU);
	}

	mj->version = MJ_VERSION;
	mj->compare = CMP_CSIGA;
	mj->tick = TK_LEFT;
	mj->fixmode = 0;

	writeFile(m_infoPath, (char*)mj, sz_MINIJISU);
}

bool CMainWnd::readFile(CString path, char *pBytes, int nBytes)
{
	CFile f;
	CFileException e;

	if( !f.Open( path, CFile::modeRead, &e ) )
	{
		TRACE("[READ] %s file open error(%d)\n", path, e.m_cause);
		return false;
	}
	if (int(f.GetLength()) != nBytes)
	{
		f.Close();
		return false;
	}

	const UINT nByteRead = f.Read(pBytes, nBytes);

	if (nByteRead != UINT(nBytes))
	{
		f.Close();
		return false;
	}
	f.Close();

	return true;
}

bool CMainWnd::writeFile(CString path, char *pBytes, int nBytes)
{
	CFile f;
	CFileException e;

	if( !f.Open( path, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		TRACE("[WRITE] %s file open error(%d)\n", path, e.m_cause);
		return false;
	}

	f.Write(pBytes, nBytes);
	f.Close();

	return true;
}

void CMainWnd::SendTR(CString name, CString data, BYTE type, int key, CString keyName)
{ 
	std::string trData = ""; 	
	struct _userTH udat{};

	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;
	
	trData = keyName.GetString();
	trData += '\t';
	trData += std::string((char *)&udat, L_userTH);
	trData += data.GetString();
	
	const BOOL bflag = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, trData.size() - L_userTH - m_param.name.GetLength() - 1), (LPARAM)trData.c_str());
}	

CFont* CMainWnd::GetFont(int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) m_pParent->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CMainWnd::GetPen(int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*) m_pParent->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CMainWnd::GetBrush(COLORREF rColor)
{
	return (CBrush*) m_pParent->SendMessage(WM_USER, getBRUSH, rColor);
}

CBitmap* CMainWnd::GetBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

void CMainWnd::OnsMarketChanged()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_pGrpWnd->m_strMarket = m_strMarket;
	m_pGrpWnd->SetTrKey(0);
	//CString stmp;
	//stmp.Format("%s", newVal);
	//stmp.TrimRight();
	//m_pGrpWnd->m_strMarket = stmp;
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}


void CMainWnd::SetMarket(SHORT sMarket)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString stmp;
	stmp.Format("%d", sMarket);
	stmp.TrimRight();
	m_pGrpWnd->m_strMarket = stmp;
	
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}
