// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "C_UserGrpDlg.h"
#include "SetupDlg.h"
#include "RegionDlg.h"
#include "ItemRtsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct	_listDef
{
	char	name[32];
	int	kind;
};
_listDef	listGrpKind[] = 
{
	{"선 차트",			IGK_LINE},
	{"점 차트",			IGK_DOT},
	{"2D 수직막대",			IGK_2DVBAR},
	{"3D 수직막대",			IGK_3DVBAR},
	{"2D 상하 수직막대",		IGK_2DUDBAR},
	{"3D 상하 수직막대",		IGK_3DUDBAR},
	{"2D 0대칭 상하 수직막대",	IGK_2D0UDBAR},
	{"3D 0대칭 상하 수직막대",	IGK_3D0UDBAR},
	{"Y기준 대비 선차트",		IGK_YBASELINE},
	{"X기준 대비 수직막대",		IGK_XBASEVBAR},
	{"2D 3쌍 수평막대",		IGK_2D3NBAR},
	{"2D 1쌍 수평막대",		IGK_2D1NBAR}
};
#define	cnt_GRPKIND	sizeof(listGrpKind) / sizeof(_listDef)

_listDef	listGrpTick[] = 
{
	{"표시하지 않음",	ICF_NONE},
	{"좌측만 표시",		ICF_LEFT},
	{"우측만 표시",		ICF_RIGHT},
	{"좌우측 표시",		ICF_LEFT|ICF_RIGHT}
};
#define	cnt_GRPTICK	sizeof(listGrpTick) / sizeof(_listDef)

_listDef	listGrpIndex[] = 
{
	{"일",	CDI_DAY},
	{"주",	CDI_WEEK},
	{"월",	CDI_MON},
	{"분",	CDI_MIN},
	{"틱",	CDI_TICK}
};
#define	cnt_GRPINDEX	sizeof(listGrpIndex) / sizeof(_listDef)

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog

CSetupDlg::CSetupDlg(CString sConfig, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupDlg)
	//}}AFX_DATA_INIT

	m_sConfig = sConfig;
}


void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDlg)
	DDX_Control(pDX, IDC_DATA_ITEMRTS, m_EnvDataItemRts);
	DDX_Control(pDX, IDC_TICK_VERTICAL, m_EnvTickVertical);
	DDX_Control(pDX, IDC_DATA_PREVIEW, m_EnvDataPreview);
	DDX_Control(pDX, IDC_DATA_REVERSE, m_EnvDataReverse);
	DDX_Control(pDX, IDC_GRP_REGION, m_GrpOptRegion);
	DDX_Control(pDX, IDC_DATA_INDEX, m_EnvDataIndex);
	DDX_Control(pDX, IDC_DATA_SHOW, m_EnvDataShow);
	DDX_Control(pDX, IDC_DATA_REQ, m_EnvDataReq);
	DDX_Control(pDX, IDC_GRP_OPTNR, m_GrpOptNameR);
	DDX_Control(pDX, IDC_GRP_OPTMM, m_GrpOptMinMax);
	DDX_Control(pDX, IDC_TICK_MDRAW, m_EnvTickMinMax);
	DDX_Control(pDX, IDC_TICK_VDRAW, m_EnvTickVDraw);
	DDX_Control(pDX, IDC_TICK_HDRAW, m_EnvTickHDraw);
	DDX_Control(pDX, IDC_TICK_CTRL, m_EnvTickCtrl);
	DDX_Control(pDX, IDC_MOD, m_BtnModify);
	DDX_Control(pDX, IDC_TICK_BOTTOM, m_EnvTickBottom);
	DDX_Control(pDX, IDC_TICK_RIGHT, m_EnvTickRight);
	DDX_Control(pDX, IDC_TICK_LEFT, m_EnvTickLeft);
	DDX_Control(pDX, IDC_GRP_KIND, m_GrpKind);
	DDX_Control(pDX, IDC_GRP_TICK, m_GrpTick);
	DDX_Control(pDX, IDC_GRP_RGN, m_GrpRegion);
	DDX_Control(pDX, IDC_GRP_LIST, m_GrpList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MOD, OnMod)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_GRP_LIST, OnDblclkGrpList)
	ON_BN_CLICKED(IDC_ASSIGN, OnAssign)
	ON_BN_CLICKED(IDC_DATA_ITEMRTS, OnDataItemrts)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers

BOOL CSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitCtrl();
	LoadGrpInfo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupDlg::OnOK() 
{
	if (!SaveGrpInfo())
		return;
	
	CDialog::OnOK();
}

void CSetupDlg::InitCtrl()
{
	CString	sTmp;
	for (int ii = 0; ii < cnt_GRPKIND; ii++)
		m_GrpKind.AddString(listGrpKind[ii].name);

	for (int ii = 0; ii < cnt_GRPTICK; ii++)
		m_GrpTick.AddString(listGrpTick[ii].name);

	for (int ii = 0; ii < cnt_GRPINDEX; ii++)
		m_EnvDataIndex.AddString(listGrpIndex[ii].name);

	for (int ii = 0; ii < MAX_REGION; ii++)
	{
		sTmp.Format("%d", ii+1);
		m_GrpRegion.AddString(sTmp);
	}

	m_GrpKind.SetCurSel(0);
	m_GrpTick.SetCurSel(3);
	m_GrpRegion.SetCurSel(0);

	m_GrpColor.SubclassDlgItem(IDC_GRP_COLOR, this);
	m_GrpColor.SetColor(PALETTERGB(128,128,128));

	m_GrpWidth.SubclassDlgItem(IDC_GRP_WIDTH, this);
	m_GrpWidth.SetLineWidth(1);

	m_GrpStyle.SubclassDlgItem(IDC_GRP_STYLE, this);
	m_GrpStyle.SetLine(PS_SOLID);

	DWORD	dwStyle = m_GrpList.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_GrpList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	m_EnvDataIndex.SetCurSel(0);
	m_EnvDataReq.SetWindowText("100");
	m_EnvDataShow.SetWindowText("30");
	m_GrpOptRegion.SetWindowText("");

	int	nItem = 0;
	AddListHeader(m_GrpList, "그래프",	nItem++, 105);
	AddListHeader(m_GrpList, "영역",	nItem++, 12);
	AddListHeader(m_GrpList, "틱표시",	nItem++, 43);
	AddListHeader(m_GrpList, "색상",	nItem++, 36);
	AddListHeader(m_GrpList, "두께",	nItem++, 12);
	AddListHeader(m_GrpList, "형태",	nItem++, 12);
	AddListHeader(m_GrpList, "옵션",	nItem++, 12);
}


BOOL CSetupDlg::AddListHeader(CListCtrl& listCtrl, LPCTSTR strItem,
		 int nItem, int nWidth, int nSubItem, int nMask, int nFmt)
{
	LV_COLUMN lvc;

	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = listCtrl.GetStringWidth(lvc.pszText) + nWidth;
	if(nMask & LVCF_SUBITEM)
	{
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	return listCtrl.InsertColumn(nItem, &lvc);
}

BOOL CSetupDlg::AddListItem(CListCtrl& listCtrl, int nItem, int nSubItem, LPCTSTR strItem)
{
	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT;
	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;
	lvi.pszText = (LPTSTR) strItem;

	if(nSubItem == 0)
		return listCtrl.InsertItem(&lvi);
	return listCtrl.SetItem(&lvi);
}

void CSetupDlg::LoadGrpInfo()
{
	if (m_sConfig.IsEmpty() || m_sConfig.GetLength() < sz_GRPINFO + sz_GRPLINE)	// 그래프가 하나도 없다면
		return;

	CString	sTmp;
	const	char*	pBuf = (char *)m_sConfig.GetString();
	const	_GrpInfo *pGrpInfo = (_GrpInfo *)pBuf;

	if ((pGrpInfo->tick & ICF_LEFT) == ICF_LEFT)		m_EnvTickLeft.SetCheck(1);
	if ((pGrpInfo->tick & ICF_RIGHT) == ICF_RIGHT)		m_EnvTickRight.SetCheck(1);
	if ((pGrpInfo->tick & ICF_BOTTOM) == ICF_BOTTOM)	m_EnvTickBottom.SetCheck(1);
	if (pGrpInfo->ctrl > ICF_CHAR)				m_EnvTickCtrl.SetCheck(1);
	if ((pGrpInfo->option1 & ICF_HDRAW) == ICF_HDRAW)	m_EnvTickHDraw.SetCheck(1);
	if ((pGrpInfo->option1 & ICF_VDRAW) == ICF_VDRAW)	m_EnvTickVDraw.SetCheck(1);
	if ((pGrpInfo->option1 & ICF_MINMAX) == ICF_MINMAX)	m_EnvTickMinMax.SetCheck(1);

	if ((pGrpInfo->option2 & ICF_PREVIEW) == ICF_PREVIEW)	m_EnvDataPreview.SetCheck(1);
	if ((pGrpInfo->option2 & ICF_REVDRAW) == ICF_REVDRAW)	m_EnvDataReverse.SetCheck(1);
	if ((pGrpInfo->option2 & ICF_TICKVERTIC) == ICF_TICKVERTIC)	m_EnvTickVertical.SetCheck(1);

	if ((pGrpInfo->option3 & ICF_ITEMRTS) == ICF_ITEMRTS)	m_EnvDataItemRts.SetCheck(1);

	sTmp.Format("%d", atoi(CString(pGrpInfo->req, sizeof(pGrpInfo->req))));	
	m_EnvDataReq.SetWindowText(sTmp);
	sTmp.Format("%d", atoi(CString(pGrpInfo->show, sizeof(pGrpInfo->show))));
	m_EnvDataShow.SetWindowText(sTmp);
	m_EnvDataIndex.SetCurSel(GetIndexIndex(atoi(CString(pGrpInfo->index, sizeof(pGrpInfo->index)))));
	
	int	rCnt = atoi(CString(pGrpInfo->rcnt, sizeof(pGrpInfo->rcnt)));
	if (rCnt > 0)
	{
		sTmp.Format("%.*s", rCnt*2, CString(pGrpInfo->rinfo, sizeof(pGrpInfo->rinfo)));
		sTmp.TrimLeft("0");
		m_GrpOptRegion.SetWindowText(sTmp);
	}

	int	iCnt = atoi(CString(pGrpInfo->gcnt, sizeof(pGrpInfo->gcnt)));

	if (m_sConfig.GetLength() < (int)(sz_GRPINFO + sz_GRPLINE * iCnt))
		return;

	for (int ii = 0; ii < iCnt; ii++)
	{
		const	_GrpLine *pGrpLine = (_GrpLine*)&pBuf[sz_GRPINFO + sz_GRPLINE*ii];

		AddListItem(m_GrpList, ii, 0, GetGrpName(atoi(CString(pGrpLine->kind,sizeof(pGrpLine->kind)))));
		sTmp.Format("%d", pGrpLine->region - ICF_CHAR);				// -ICF_CHAR
		AddListItem(m_GrpList, ii, 1, sTmp);
		AddListItem(m_GrpList, ii, 2, GetTickName(pGrpLine->tick - ICF_CHAR));	// -ICF_CHAR
		sTmp.Format("%d", atoi(CString(pGrpLine->color, sizeof(pGrpLine->color))));
		AddListItem(m_GrpList, ii, 3, sTmp);
		sTmp.Format("%d", pGrpLine->width - ICF_CHAR);				// -ICF_CHAR
		AddListItem(m_GrpList, ii, 4, sTmp);
		sTmp.Format("%d", pGrpLine->style - ICF_CHAR);				// -ICF_CHAR
		AddListItem(m_GrpList, ii, 5, sTmp);
		sTmp.Format("%d", pGrpLine->option1 - ICF_CHAR);			// -ICF_CHAR
		AddListItem(m_GrpList, ii, 6, sTmp);
	}
}

bool CSetupDlg::SaveGrpInfo()
{
	const	int	iCnt = m_GrpList.GetItemCount();

	if (iCnt <= 0)
	{
		AfxMessageBox("설정된 그래프 정보가 없습니다.");
		return false;
	}

	CString	sTmp;
	const	int	iSize = sz_GRPINFO + sz_GRPLINE * iCnt + 1;
	std::unique_ptr<char[]> pBuf = std::make_unique<char[]>(iSize);

	_GrpInfo	*pGrpInfo = (_GrpInfo *)&pBuf[0];
	pGrpInfo->tick  = (m_EnvTickLeft.GetCheck() == BST_CHECKED)? ICF_LEFT:ICF_NONE;
	pGrpInfo->tick |= (m_EnvTickRight.GetCheck() == BST_CHECKED)? ICF_RIGHT:ICF_NONE;
	pGrpInfo->tick |= (m_EnvTickBottom.GetCheck() == BST_CHECKED)? ICF_BOTTOM:ICF_NONE;
	pGrpInfo->tick |= ICF_CHAR;		// +ICF_CHAR

	pGrpInfo->ctrl |= (m_EnvTickCtrl.GetCheck() == BST_CHECKED)? 1:0;
	pGrpInfo->ctrl |= ICF_CHAR;		// +ICF_CHAR

	pGrpInfo->option1 |= (m_EnvTickHDraw.GetCheck() == BST_CHECKED)? ICF_HDRAW:0;
	pGrpInfo->option1 |= (m_EnvTickVDraw.GetCheck() == BST_CHECKED)? ICF_VDRAW:0;
	pGrpInfo->option1 |= (m_EnvTickMinMax.GetCheck() == BST_CHECKED)? ICF_MINMAX:0;
	pGrpInfo->option1 |= ICF_CHAR;		// +ICF_CHAR

	pGrpInfo->option2 |= (m_EnvDataPreview.GetCheck() == BST_CHECKED)? ICF_PREVIEW:0;
	pGrpInfo->option2 |= (m_EnvDataReverse.GetCheck() == BST_CHECKED)? ICF_REVDRAW:0;
	pGrpInfo->option2 |= (m_EnvTickVertical.GetCheck() == BST_CHECKED)? ICF_TICKVERTIC:0;
	pGrpInfo->option2 |= ICF_CHAR;		// +ICF_CHAR

	m_EnvDataReq.GetWindowText(sTmp);
	sprintf(pGrpInfo->req, "%05d", atoi(sTmp));
	m_EnvDataShow.GetWindowText(sTmp);
	sprintf(pGrpInfo->show, "%05d", atoi(sTmp));
	sprintf(pGrpInfo->gcnt, "%02d", iCnt);

	if (pGrpInfo->req <= 0)
	{
		AfxMessageBox("자료 요청개수가 0입니다.");
		return false;
	}
	if (pGrpInfo->show <= 0)
	{
		AfxMessageBox("자료 출력개수가 0입니다.");
		return false;
	}

	int	gkind = 0;
	char	pVal[64]{};
	int	i2D3NBAR = 0;
	_GrpLine *pGrpLine = nullptr;
	CArray<_GrpLine *, _GrpLine*> arGrpLine;

	for (int ii = 0; ii < iCnt; ii++)
	{
		pGrpLine = (_GrpLine*)&pBuf[sz_GRPINFO + sz_GRPLINE*ii];
		FillMemory(pGrpLine, sz_GRPLINE, ICF_CHAR);

		m_GrpList.GetItemText(ii, 0, pVal, 64);
		gkind = GetGrpKind(pVal);
		sprintf(pGrpLine->kind, "%02d", gkind);
		m_GrpList.GetItemText(ii, 1, pVal, 64);
		pGrpLine->region = atoi(pVal) | ICF_CHAR;	// +ICF_CHAR
		m_GrpList.GetItemText(ii, 2, pVal, 64);
		pGrpLine->tick = GetTickKind(pVal) | ICF_CHAR;	// +ICF_CHAR

		m_GrpList.GetItemText(ii, 3, pVal, 64);
		sprintf(pGrpLine->color, "%010d", atoi(pVal));
		m_GrpList.GetItemText(ii, 4, pVal, 64);
		pGrpLine->width = atoi(pVal) | ICF_CHAR;	// +ICF_CHAR
		m_GrpList.GetItemText(ii, 5, pVal, 64);
		pGrpLine->style = atoi(pVal) | ICF_CHAR;	// +ICF_CHAR
		m_GrpList.GetItemText(ii, 6, pVal, 64);
		pGrpLine->option1 = atoi(pVal) | ICF_CHAR;	// +ICF_CHAR
		arGrpLine.Add(pGrpLine);

		if (gkind == IGK_2D3NBAR)
			i2D3NBAR++;
	}

	if (iCnt > 1 && i2D3NBAR > 1)
	{
		AfxMessageBox("2D 3N 수평막대는 단독으로만 사용할 수 있습니다.");
		arGrpLine.RemoveAll();
		return false;
	}

// 영역 보정(1부터 채워나가는 작업)
	for (int ii = 1; ii <= MAX_REGION; ii++)
	{
		bool	bExist = false;
		int	MinRegion = MAX_REGION;

		for (int jj = 0; jj < arGrpLine.GetSize(); jj++)
		{
			pGrpLine = arGrpLine.GetAt(jj);
			if (pGrpLine->region - ICF_CHAR < ii)
				continue;
			if (pGrpLine->region - ICF_CHAR == ii)
			{
				bExist = true;
				break;
			}

			if (pGrpLine->region - ICF_CHAR < MinRegion)
				MinRegion = pGrpLine->region - ICF_CHAR;
		}

		if (!bExist)
		{
			for (int jj = 0; jj < arGrpLine.GetSize(); jj++)
			{
				pGrpLine = arGrpLine.GetAt(jj);
				if (pGrpLine->region - ICF_CHAR < ii)
					continue;
				pGrpLine->region = pGrpLine->region - (MinRegion - ii);
			}
		}
	}

	int	rCnt = 1;
	for (int ii = 0; ii < arGrpLine.GetSize(); ii++)
	{
		pGrpLine = arGrpLine.GetAt(ii);
		if (pGrpLine->region - ICF_CHAR > rCnt)
			rCnt = pGrpLine->region - ICF_CHAR;
	}
		
	if (iCnt > MAX_GRAPH)
	{
		sTmp.Format("사용가능한 최대 그래프의 개수는 %d개입니다", MAX_GRAPH);
		AfxMessageBox(sTmp);
		return false;
	}

	// 영역개수
	sTmp.Format("%02d", rCnt);
	CopyMemory(pGrpInfo->rcnt, sTmp, sTmp.GetLength()); 

	// 영역분할정보
	m_GrpOptRegion.GetWindowText(sTmp);
	if (sTmp.IsEmpty() || sTmp.GetLength() != rCnt*2)
	{
		FillMemory(pGrpInfo->rinfo, sizeof(pGrpInfo->rinfo), '0');
	}
	else
	{
		if (sTmp.GetLength() != sizeof(pGrpInfo->rinfo))
			sprintf(pGrpInfo->rinfo, "%s%0*d", (char *)sTmp.GetString(), sizeof(pGrpInfo->rinfo) - sTmp.GetLength(), 0);
		else
			CopyMemory(pGrpInfo->rinfo, sTmp, sizeof(pGrpInfo->rinfo));
	}

	m_EnvDataIndex.GetWindowText(sTmp);
	sprintf(pGrpInfo->index, "%d", GetIndexValue(sTmp));

	pGrpInfo->option3 |= (m_EnvDataItemRts.GetCheck() == BST_CHECKED)? ICF_ITEMRTS:0;
	pGrpInfo->option3 |= ICF_CHAR;		// +ICF_CHAR

	// reserved field
	FillMemory(pGrpInfo->res, sizeof(pGrpInfo->res), ICF_CHAR);

	m_sConfig.Format("%s", pBuf.get());

	arGrpLine.RemoveAll();

	if (m_EnvDataPreview.GetCheck() == BST_CHECKED)
	{
		AfxMessageBox("미리보기가 선택된 상태입니다.\n배포시에는 반드시 해제 하십시요.");
	}

	return true;
}

int CSetupDlg::GetSelectedGrpItem()
{
	POSITION pos = m_GrpList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("선택된 그래프가 없습니다.");
		return -1;
	}

	return m_GrpList.GetNextSelectedItem(pos);
}

void CSetupDlg::AddGraph(int nItem)
{
	int	pVal[4]{};
	CString	sTmp;
	CString	gName, tName;
	const	int	region = m_GrpRegion.GetCurSel();
	const	int	tick = m_GrpTick.GetCurSel();

	m_GrpKind.GetLBText(m_GrpKind.GetCurSel(), gName);
	m_GrpTick.GetLBText(m_GrpTick.GetCurSel(), tName);

	AddListItem(m_GrpList, nItem, 0, gName);	// 그래프 종류(0)
	sTmp.Format("%d", region+1);
	AddListItem(m_GrpList, nItem, 1, sTmp);		// 그래프 영역(1)
	AddListItem(m_GrpList, nItem, 2, tName);	// 그래프 틱표시(2)

	pVal[0] = m_GrpColor.GetColor();		// 그래프 색상(3)
	pVal[1] = m_GrpWidth.GetLineWidth();		// 그래프 두께(4)
	pVal[2] = m_GrpStyle.GetLine();			// 그래프 형태(5)
	for (int ii = 0; ii < 3; ii++)
	{
		sTmp.Format("%d", pVal[ii]);
		AddListItem(m_GrpList, nItem, 3+ii, sTmp);
	}

	int	opt = 0;
	if (m_GrpOptNameR.GetCheck() == BST_CHECKED)	opt |= IGO_NAMER;
	if (m_GrpOptMinMax.GetCheck() == BST_CHECKED)	opt |= IGO_MINMAX;
	sTmp.Format("%d", opt);
	AddListItem(m_GrpList, nItem, 6, sTmp);		// 그래프 추가옵션(6)
}

void CSetupDlg::SelectGraph()
{
	const	int	nItem = GetSelectedGrpItem();

	if (nItem < 0)
		return;

	char	pBuf[64]{};

	m_GrpList.GetItemText(nItem, 0, pBuf, 64);
	for (int ii = 0; ii < cnt_GRPKIND; ii++)
	{
		if (strcmp(pBuf, listGrpKind[ii].name) == 0)
		{
			m_GrpKind.SetCurSel(ii);
			break;
		}
	}

	m_GrpList.GetItemText(nItem, 1, pBuf, 64);
	m_GrpRegion.SetCurSel(atoi(pBuf) - 1);

	m_GrpList.GetItemText(nItem, 2, pBuf, 64);
	for (int ii = 0; ii < cnt_GRPTICK; ii++)
	{
		if (strcmp(pBuf, listGrpTick[ii].name) == 0)
		{
			m_GrpTick.SetCurSel(ii);
			break;
		}
	}

	m_GrpList.GetItemText(nItem, 3, pBuf, 64);
	m_GrpColor.SetColor(atoi(pBuf));
	m_GrpList.GetItemText(nItem, 4, pBuf, 64);
	m_GrpWidth.SetLineWidth(atoi(pBuf));
	m_GrpList.GetItemText(nItem, 5, pBuf, 64);
	m_GrpStyle.SetLine(atoi(pBuf));

	m_GrpList.GetItemText(nItem, 6, pBuf, 64);
	const	int	opt = atoi(pBuf);
	if (opt & IGO_NAMER)	
		m_GrpOptNameR.SetCheck(1);
	else
		m_GrpOptNameR.SetCheck(0);
	if (opt & IGO_MINMAX)	
		m_GrpOptMinMax.SetCheck(1);
	else
		m_GrpOptMinMax.SetCheck(0);

	m_BtnModify.EnableWindow(TRUE);
}

int CSetupDlg::GetGrpKind(CString sGrp)
{
	for (int ii = 0; ii < cnt_GRPKIND; ii++)
	{
		if (listGrpKind[ii].name == sGrp)
			return listGrpKind[ii].kind;
	}
	return 0;
}

CString CSetupDlg::GetGrpName(int kind)
{
	for (int ii = 0; ii < cnt_GRPKIND; ii++)
	{
		if (listGrpKind[ii].kind == kind)
			return listGrpKind[ii].name;
	}
	return "";
}

int CSetupDlg::GetTickKind(CString sTick)
{
	for (int ii = 0; ii < cnt_GRPTICK; ii++)
	{
		if (listGrpTick[ii].name == sTick)
			return listGrpTick[ii].kind;
	}
	return 0;
}

CString CSetupDlg::GetTickName(int kind)
{
	for (int ii = 0; ii < cnt_GRPTICK; ii++)
	{
		if (listGrpTick[ii].kind == kind)
			return listGrpTick[ii].name;
	}
	return "";
}

int CSetupDlg::GetIndexValue(CString sIndex)
{
	for (int ii = 0; ii < cnt_GRPINDEX; ii++)
	{
		if (listGrpIndex[ii].name == sIndex)
			return listGrpIndex[ii].kind;
	}
	return 0;
}

int CSetupDlg::GetIndexIndex(int kind)
{
	for (int ii = 0; ii < cnt_GRPINDEX; ii++)
	{
		if (listGrpIndex[ii].kind == kind)
			return ii;
	}
	return 0;
}
void CSetupDlg::OnAdd() 
{
	AddGraph(m_GrpList.GetItemCount());
}

void CSetupDlg::OnMod() 
{
	m_BtnModify.EnableWindow(FALSE);
	const	int	nItem = GetSelectedGrpItem();
	if (nItem < 0)
		return;

	m_GrpList.DeleteItem(nItem);
	AddGraph(nItem);
	
	m_BtnModify.EnableWindow(FALSE);
}

void CSetupDlg::OnDel() 
{
	const	int	nItem = GetSelectedGrpItem();
	if (nItem < 0)
		return;

	m_GrpList.DeleteItem(nItem);

	m_BtnModify.EnableWindow(FALSE);
}

void CSetupDlg::OnDblclkGrpList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SelectGraph();
	
	*pResult = 0;
}

void CSetupDlg::OnAssign() 
{
	char	pVal[64]{};
	CString	sTmp, sTmp2;
	int	region = 0;
	const	int	gCount = m_GrpList.GetItemCount();

	if (gCount < 2)
		return;

	CStringArray	arRegion, arAdjust;

	arRegion.RemoveAll();
	for (int ii = 0; ii < gCount; ii++)
	{
		m_GrpList.GetItemText(ii, 1, pVal, 64);
		arRegion.Add(pVal);
	}

	for (int ii = 1; ii <= MAX_REGION; ii++)
	{
		bool	bExist = false;
		int	MinRegion = MAX_REGION;
		for (int jj = 0; jj < gCount; jj++)
		{
			sTmp = arRegion.GetAt(jj);
			region = atoi(sTmp);
			if (region < ii)	continue;

			if (region == ii)
			{
				bExist = true;
				break;
			}

			if (region < MinRegion)
				MinRegion = region;
		}

		if (!bExist)
		{
			for (int jj = 0; jj < gCount; jj++)
			{
				sTmp = arRegion.GetAt(jj);
				region = atoi(sTmp);
				if (region < ii)
					continue;

				sTmp.Format("%02d", region - (MinRegion - ii));
				arRegion.RemoveAt(jj);
				arRegion.InsertAt(jj, sTmp);
			}
		}
	}

	int	rCnt = 1;
	for (int ii = 0; ii < arRegion.GetSize(); ii++)
	{
		sTmp = arRegion.GetAt(ii);
		if (atoi(sTmp) > rCnt)
			rCnt = atoi(sTmp);
	}

	if (rCnt < 2)
		return;

	m_GrpOptRegion.GetWindowText(sTmp2);
	if (rCnt * 2 == sTmp2.GetLength())
		sTmp = sTmp2;

	CRegionDlg	dlg(rCnt, sTmp);
	if (dlg.DoModal() == IDOK)
	{
		m_GrpOptRegion.SetWindowText(dlg.m_sRegion);
	}
}

void CSetupDlg::OnDataItemrts() 
{
	if (m_EnvDataItemRts.GetCheck() == BST_CHECKED)
	{
		CItemRtsDlg	dlg;
		dlg.DoModal();
	}	
}

void CSetupDlg::OnShowall() 
{
	m_EnvDataShow.SetWindowText("0");
}
