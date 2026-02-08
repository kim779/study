#include "stdafx.h"
#include "IB0000A2.h"
#include "mapForm.h"

#include "dlgGrNew.h"
#include "libDefine.h"
#include "../../h/memDC.h"
#include "../../h/ledger.h"
#include "dlgDirect.h"
#include "../../control/fx_misc/misctype.h"

//#define	_FILE_DEBUG
#ifdef	_FILE_DEBUG
#include <fstream.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ENCTEST

#ifdef	_FILE_DEBUG
	ofstream dFile("C:\\IB0000A2.trc",ios::app);
#endif

IMPLEMENT_DYNCREATE(CMapForm, CAxMapForm)

const COLORREF clrGridHead		= 74;
const COLORREF clrGridLine		= 75;
const COLORREF clrGridBG		= 68;
const COLORREF clrGridFG		= 69;
const COLORREF clrGridFocusBG	= 78;
const COLORREF clrGrid1stRow	= clrGridBG;

#define TR_ACCT	"PIDOACCN"

#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		1
#define GAP_BLOCK		5
#define GAP_PANEL		4
#define PANAL_HEIGHT	26
//판넬
#define clPanel	66
#define clPanelLine 165

//contents 외곽
#define clContents 181
#define clGuide	26
#define	clBack	64
#define clBox	65
#define	clText	69
#define IDC_PANEL	10104

CMapForm::CMapForm()
	: CAxMapForm(CMapForm::IDD)
{
	//{{AFX_DATA_INIT(CMapForm)
	//}}AFX_DATA_INIT
	m_panel = NULL;
	m_bitGuide = NULL;
	initConstruct();
}

void CMapForm::initConstruct()
{
	m_bCustomer = true;//**false;
	m_sRootDir = _T("");
	m_szFilename = _T("");

	m_sStaffNum = _T("");
	m_sPassWord = _T("");
	m_sDeptCode = _T("");

	m_gubn   = ' ';

	m_pGrAcc = NULL;
	m_pGrEdit = NULL;

	m_sFontName    = "굴림체";
	m_nFontPoint   = 9;
	m_pFont = NULL;
	
	m_bFirst = true;
	deleteAllAcSave();
	deleteAllGpSave();
	m_mapPass.RemoveAll();
	m_arDelGp.RemoveAll();

	m_sendB = NULL;

	m_sendB2 = NULL;
	m_nextTRPos = -1;
	m_totTRCnt = -1;
	m_subTRCnt = -1;
	m_calBufL = -1;

	m_dlgDirect = NULL;

	m_sInputAccPass = _T("");
	m_sInputAccNum = _T("");
	m_sInputAccName = _T("");
	m_passRow = -1;
	m_clrBG = Axis::GetColor(34);// RGB(200, 200, 200);
	m_selType = SEL_NOT;
	m_bChange = false;

	m_useAlias = FALSE;
}


void CMapForm::destroy()
{
#ifdef	_FILE_DEBUG
	dFile.close();
#endif
	deleteAllAcSave();
	deleteAllGpSave();
	m_arDelGp.RemoveAll();
	m_mapPass.RemoveAll();
		
	if (m_pGrAcc)
		saveGrid(m_pGrAcc.get(), GRACC);
	if (m_pGrEdit)
	{
		//AfxMessageBox("asdf1");
		saveGrid(m_pGrEdit.get(), GREDIT);
		//AfxMessageBox("asdf2");
		//AfxMessageBox("asdf3");
	}
	
	if (m_sendB)
	{
		m_sendB = NULL;
	}
	if (m_sendB2)
	{
		m_sendB2 = NULL;
	}

}

CMapForm::~CMapForm()
{
	
}

void CMapForm::DoDataExchange(CDataExchange* pDX)
{
	CAxMapForm::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapForm)
	DDX_Control(pDX, IDC_BTNINITIAL, m_btnInital);
	DDX_Control(pDX, IDC_BTNGRDOWN, m_btnGrDown);
	DDX_Control(pDX, IDC_BTNGRUP, m_btnGrUp);
	DDX_Control(pDX, IDC_BTNGRCOPY, m_btnGrCopy);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_PASSCHECK, m_passCheck);
	DDX_Control(pDX, IDC_STGRNUM, m_stGrNum);
	DDX_Control(pDX, IDC_STGRNAME, m_stGrName);
	DDX_Control(pDX, IDC_RESULT, m_result);
	DDX_Control(pDX, IDC_BTNOK, m_btnOk);
	DDX_Control(pDX, IDC_BTNDNBOT, m_btnDnBot);
	DDX_Control(pDX, IDC_BTNUPTOP, m_btnUpTop);
	DDX_Control(pDX, IDC_BTNCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTNDIRECT, m_btnDirect);
	DDX_Control(pDX, IDC_BTNUP, m_btnUp);
	DDX_Control(pDX, IDC_BTNREFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTNINSERT, m_btnInsert);
	DDX_Control(pDX, IDC_BTNGRNEW, m_btnGrNew);
	DDX_Control(pDX, IDC_BTNGRDEL, m_btnGrDel);
	DDX_Control(pDX, IDC_BTNGRCHG, m_btnGrChg);
	DDX_Control(pDX, IDC_BTNDN, m_btnDn);
	DDX_Control(pDX, IDC_BTNDEL, m_btnDel);
	DDX_Control(pDX, IDC_BTNALLDEL, m_btnAllDel);
	DDX_Control(pDX, IDC_BTNALLADD, m_btnAllAdd);
	DDX_Control(pDX, IDC_BTNADD, m_btnAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapForm, CAxMapForm)
	//{{AFX_MSG_MAP(CMapForm)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTNADD, OnBtnadd)
	ON_BN_CLICKED(IDC_BTNREFRESH, OnBtnrefresh)
	ON_CBN_SELCHANGE(IDC_CBGR, OnSelchangeCbgr)
	ON_BN_CLICKED(IDC_BTNGRNEW, OnBtngrnew)
	ON_BN_CLICKED(IDC_BTNGRCHG, OnBtngrchg)
	ON_BN_CLICKED(IDC_BTNGRDEL, OnBtngrdel)
	ON_BN_CLICKED(IDC_BTNINSERT, OnBtninsert)
	ON_BN_CLICKED(IDC_BTNDEL, OnBtndel)
	ON_BN_CLICKED(IDC_BTNUP, OnBtnup)
	ON_BN_CLICKED(IDC_BTNDN, OnBtndn)
	ON_BN_CLICKED(IDC_BTNALLADD, OnBtnalladd)
	ON_BN_CLICKED(IDC_BTNALLDEL, OnBtnalldel)
	ON_BN_CLICKED(IDC_BTNDIRECT, OnBtndirect)
	ON_BN_CLICKED(IDC_BTNCANCEL, OnBtncancel)
	ON_BN_CLICKED(IDC_BTNOK, OnBtnok)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_BN_CLICKED(IDC_BTNUPTOP, OnBtnuptop)
	ON_BN_CLICKED(IDC_BTNDNBOT, OnBtndnbot)
	ON_BN_CLICKED(IDC_BTNGRCOPY, OnBtngrcopy)
	ON_BN_CLICKED(IDC_BTNGRDOWN, OnBtngrdown)
	ON_BN_CLICKED(IDC_BTNGRUP, OnBtngrup)
	ON_BN_CLICKED(IDC_RDHM, OnRdhm)
	ON_BN_CLICKED(IDC_RDFU, OnRdfu)
	ON_BN_CLICKED(IDC_BTNINITIAL, OnBtninitial)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_NOTIFY(GVNM_LMOUSEDOWN, IDC_GRACC, OnGrAccClick)
	ON_NOTIFY(GVNM_DBLCLICKED, IDC_GRACC, OnGrAccblClick)
	ON_NOTIFY(GVNM_LMOUSEDOWN, IDC_GREDIT, OnGrEditClick)
	ON_NOTIFY(GVNM_DBLCLICKED, IDC_GREDIT, OnGrEditdblClick)
	ON_NOTIFY(GVNM_ENDSORT, IDC_GREDIT, OnGrEditEndSort)
	ON_NOTIFY(GVNM_BEGINEDIT, IDC_GREDIT, OnBeginEdit)
	ON_NOTIFY(GVNM_ENDEDIT, IDC_GREDIT, OnEndEdit)
	ON_MESSAGE(WM_KEYMSG, OnKeyMsg)
	ON_MESSAGE(WM_ACCSEARCH, OnAccNameSearch)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CMapForm::AssertValid() const
{
	CAxMapForm::AssertValid();
}

void CMapForm::Dump(CDumpContext& dc) const
{
	CAxMapForm::Dump(dc);
}
#endif //_DEBUG


LRESULT CMapForm::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int len = 0;
	switch(LOBYTE(LOWORD(wParam))) 
	{
	case DLL_OUB:
		len = HIWORD(wParam);
		if (HIWORD(wParam))
		{
			switch(HIBYTE(LOWORD(wParam)))
			{
			case SC_ACCCUST:	// 고객계좌조회
				oubAccUser((char *)lParam);
				break;
			case SC_ACCLIST:	// 직원계좌조회
				oubAccList((char *)lParam, len);
				break;
			case SC_ACCLISTBYGROUP:	// 그룹계좌조회
				oubGroup((char *)lParam);
				break;
			case SC_SAVEACC:	// 그룹계좌저장
			case SC_SAVEACCONLY:	// 그룹계좌저장
				oubGroupSave((char *)lParam, HIBYTE(LOWORD(wParam)));
				break;
			case SC_INPUTACCOUNTVALID:// 입력 계좌 비번 검사
				oubAccInspect((char *)lParam, len);
				break;
			case SC_ACCOUNTNAME:	// 계좌명검증
				oubAccInspectName((char *)lParam, len);
				break;
			case SC_ACCPASSVALID:	// 계좌유효성검사(비번edit입력시)
				oubAccInspectEdit((char *)lParam, len);
				break;
			case SC_DLGACCOUNTNAME:	// 계좌명 조회
				oubAccountName((char *)lParam, len);
				break;
			}
		}
		break;
	case DLL_ALERT:
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		domino((char *)lParam);
		break;
	case DLL_SETPAL:
		redrawCtrl();
		break;
	case DLL_GUIDE:
		m_result.SetWindowText((char*)lParam);
		return true;
	}
	return FALSE;
}

LRESULT CMapForm::OnAccNameSearch(WPARAM wParam, LPARAM lParam)
{
	queryAccNameSearch((char *)lParam);
	return TRUE;
}

void CMapForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

#ifdef	_FILE_DEBUG
	dFile	<< "===그룹계좌==============================================================" << endl;
#endif
	Variant(titleCC, "[계좌그룹등록]");
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(SCRWIDTH, SCRHEIGHT));

	m_sRootDir = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	CString	name = Variant(nameCC, "");
	m_szFilename.Format("%s\\user\\%s\\IB0000A2.INI", m_sRootDir, name);
	
	if (m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
		m_bCustomer = true;
	//m_bitGuide = Axis::GetBitmap("알림_IC.BMP");
	m_useAlias = LoadConfig_UseAlias();

	m_sStaffNum = Variant(userCC, "");
	m_sPassWord = "TRUE";
	m_sDeptCode = Variant(deptCC, "");

	CString	gubn = Variant(systemCC, "");
	if (gubn.GetLength() > 0) 
		m_gubn = gubn.GetAt(0);

	m_clrBG = GetIndexColor(CLRBG);
	m_clrGuide = GetIndexColor(34);

	m_bEnter = true;			// VK_RETURN을 TAB으로 사용

	initControl();
	enableGroup(FALSE);
	enableAcc(FALSE);

	m_bFirst = true;
	//***queryAcc();			// 계좌조회
	LoadAccountListFromWizard();
}

void CMapForm::OnDestroy() 
{
	destroy();
	CAxMapForm::OnDestroy();
}

HBRUSH CMapForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CAxMapForm::OnCtlColor(pDC, pWnd, nCtlColor);
	const int nID = pWnd->GetDlgCtrlID();
	switch (nID)
	{
	case IDC_PASSCHECK:
	case IDC_STATIC1:	case IDC_STATIC2:	
	case IDC_STCTITLE:	case IDC_STATIC4:	case IDC_STCWARN:
	case IDC_RDHM:		case IDC_RDFU:
		hbr = (HBRUSH)getAxBrush(m_clrBG)->operator HBRUSH();
		break;
	}
	pDC->SetBkMode(TRANSPARENT);
	return hbr;
}

void CMapForm::OnPaint() 
{
	CPaintDC dc(this);

	xxx::CMemDC memdc(&dc);
	CRect rc;
	GetClientRect(&rc);

	memdc.SetBkMode(TRANSPARENT);

	rc.SetRect(0, 0, rc.Width(), rc.Height() );
	
	memdc.FillSolidRect(rc, m_clrGuide);

	
	
	
}

void CMapForm::OnSize(UINT nType, int cx, int cy)
{
	CAxMapForm::OnSize(nType, cx, cy);
}

void CMapForm::OnRdhm() 
{
	selectRadio(IDC_RDHM);
}

void CMapForm::OnRdfu() 
{
	selectRadio(IDC_RDFU);
}


void CMapForm::OnBtnrefresh() 
{
	deleteAllAcSave();
	deleteAllGpSave();
	m_mapPass.RemoveAll();
	m_arDelGp.RemoveAll();

	updateGrAcc();
	updateTree();
	updateGrEdit(-1);

	m_bFirst = true;

	//**queryAcc();
	LoadAccountListFromWizard();

	m_bChange = true;
}

void CMapForm::OnSelchangeCbgr() 
{
	HTREEITEM item;
	updateGrEdit(getCurGr(item));
}

void CMapForm::OnBtngrdown() 
{
	HTREEITEM mvItem{}, ntItem{};
	HTREEITEM rootItem = m_tree.GetRootItem();

	const int moveIdx = getCurGr(mvItem);
	if (moveIdx < 0)
	{
		setMessage("이동할 그룹을 선택하세요.");
		return;
	}
	CString mvName = m_tree.GetItemText(mvItem);
	const int mvIdx = getGr(mvName);
	if (mvIdx < 0)	return;
	_gpSave *gpSaveCur = m_arGpSave.GetAt(mvIdx);
	if (!gpSaveCur)	return;
	
	ntItem = m_tree.GetNextVisibleItem(mvItem);
	if (ntItem == rootItem || ntItem == NULL)
		return;

	CString ntName = m_tree.GetItemText(ntItem);
	const int ntIdx = getGr(ntName);
	
	_gpSave *gpSaveNew = new _gpSave();
	gpSaveNew->Copy(*gpSaveCur);
	m_arGpSave.InsertAt(ntIdx+1, gpSaveNew);
	LIB_DELETE(gpSaveCur);
	m_arGpSave.RemoveAt(moveIdx, 1);
	
	HTREEITEM item = m_tree.InsertItem(mvName, imgTGROUP, imgTGRSEL, rootItem, ntItem);
	m_tree.DeleteItem(mvItem);
	m_tree.SelectItem(item);
	m_bChange = true;
}


void CMapForm::OnBtngrup() 
{
	HTREEITEM mvItem{}, ntItem{};
	HTREEITEM rootItem = m_tree.GetRootItem();

	const int moveIdx = getCurGr(mvItem);
	if (moveIdx < 0)
	{
		setMessage("이동할 그룹을 선택하세요.");
		return;
	}
	CString mvName = m_tree.GetItemText(mvItem);
	const int mvIdx = getGr(mvName);
	if (mvIdx < 0)	return;

	ntItem = m_tree.GetPrevVisibleItem(mvItem);
	if (ntItem == rootItem || ntItem == NULL)
		return;

	CString ntName = m_tree.GetItemText(ntItem);
	const int ntIdx = getGr(ntName);

	_gpSave *gpSaveCur = m_arGpSave.GetAt(ntIdx);
	if (!gpSaveCur)	return;
	_gpSave *gpSaveNew = new _gpSave();
	gpSaveNew->Copy(*gpSaveCur);
	m_arGpSave.InsertAt(mvIdx+1, gpSaveNew);
	LIB_DELETE(gpSaveCur);
	m_arGpSave.RemoveAt(ntIdx, 1);

	HTREEITEM item = m_tree.InsertItem(ntName, imgTGROUP, imgTGRSEL, rootItem, mvItem);
	m_tree.DeleteItem(ntItem);
	m_tree.SelectItem(mvItem);
	m_bChange = true;
}

void CMapForm::OnBtngrnew() 
{
	if (m_arGpSave.GetSize() >= MAX_GP)
	{
		setMessage("최대 %d개의 그룹을 등록하실 수 있습니다.", MAX_GP);
		return;
	}
	
	CDlgGrNew dlg;
	dlg.SetTitle("새그룹 등록");
	if (dlg.DoModal() != IDOK)
		return;
	
	// 그룹명 check
	CString newName = dlg.GetNewGrName();
	if (checkIsGrName(newName))
	{
		setMessage("[%s] : 동일한 그룹명이 있습니다. 등록을 취소합니다.", newName);
		return;
	}
	

	// 그룹등록
	_gpSave *gpSave = new _gpSave();
	gpSave->sGrName = newName;
	m_arGpSave.Add(gpSave);

	// tree선택
	const int data = m_arGpSave.GetUpperBound();
	HTREEITEM item = insertTreeItem(newName, data);
	m_tree.SelectItem(item);
	updateGrEdit(data);

	setMessage("[%s] 그룹을 추가하였습니다.", newName);
	m_bChange = true;
}

void CMapForm::OnBtngrcopy() 
{
	HTREEITEM item;
	const int curSel = getCurGr(item);
	if (curSel < 0)
	{
		setMessage("복사할 그룹을 선택하세요.");
		return;
	}

	_gpSave *gpSaveCur = m_arGpSave.GetAt(curSel);
	if (!gpSaveCur)	return;

	CDlgGrNew dlg;
	dlg.SetTitle("그룹명 등록");
	if (dlg.DoModal() != IDOK)
		return;

	CString newName = dlg.GetNewGrName();
	CString cmpName = _T("");
	CString curName = m_tree.GetItemText(item);

	// 그룹명 check
	if (checkIsGrName(newName))
	{
		setMessage("[%s] : 동일한 그룹명이 있습니다. 그룹등록을 취소합니다.", newName);
		return;
	}
	
	struct _group group;
	memset(&group, 0x00, sizeof(group));
	const int maxLen = sizeof(group.gnam);
	if (newName.GetLength() > maxLen)
	{
		newName = newName.Left(maxLen);
	}

	_gpSave *gpSaveNew = new _gpSave();
	gpSaveNew->Copy(*gpSaveCur);
	gpSaveNew->sGrName = newName;
	gpSaveNew->sGroupID = "";
	m_arGpSave.Add(gpSaveNew);

	// tree선택
	const int data = m_arGpSave.GetUpperBound();
	item = insertTreeItem(newName, data);
	m_tree.SelectItem(item);
	updateGrEdit(data);
	m_bChange = true;
}


void CMapForm::OnBtngrchg() 
{
	HTREEITEM item;
	const int curSel = getCurGr(item);
	if (curSel < 0)
	{
		setMessage("이름을 변경할 그룹을 선택하세요.");
		return;
	}

	CDlgGrNew dlg;
	dlg.SetTitle("그룹명 변경");
	if (dlg.DoModal() != IDOK)
		return;	

	CString newName = dlg.GetNewGrName();
	CString cmpName = _T("");
	CString curName = m_tree.GetItemText(item);

	// 그룹명 check
	if (newName == curName)
	{
		setMessage("[%s] : 현재 그룹명과 동일합니다.", newName);
		OnBtngrchg();
		return;
	}
	if (checkIsGrName(newName))
	{
		setMessage("[%s] : 동일한 그룹명이 있습니다.", newName);
		OnBtngrchg();
		return;
	}
	
	// 그룹명 변경
	_gpSave *gpSaveCur = m_arGpSave.GetAt(curSel);
	gpSaveCur->sGrName = newName;
	m_tree.SetItemText(item, newName);
	m_tree.SelectItem(item);

	updateGrEdit(curSel);

	setMessage("[%s]을 [%s]으로 그룹명을 변경하였습니다.", curName, newName);
	m_bChange = true;
}

void CMapForm::OnBtninitial() 
{
	if (MessageBox("등록된 모든 그룹이 삭제됩니다. \n\n등록된 모든 그룹을 삭제하시겠습니까?", "그룹삭제", MB_OKCANCEL|MB_ICONEXCLAMATION) != IDOK)
	{
		setMessage("전체그룹 삭제가 취소되었습니다.");
		return;
	}
	
	_gpSave *gpSave = NULL;
	for (int ii = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		gpSave = m_arGpSave.GetAt(ii);
		if (!gpSave)	continue;
		// 저장시 삭제 알려주어야 한다.
		m_arDelGp.Add(Format("%s\t%s", gpSave->sGroupID, gpSave->sGrName));
		LIB_DELETE(gpSave);
	}
	m_arGpSave.RemoveAll();
	updateTree();
	updateGrEdit(-1);
	setMessage("모든그룹을 삭제하였습니다.");
	m_bChange = true;
}


void CMapForm::OnBtngrdel() 
{
	HTREEITEM item;
	const int curSel = getCurGr(item);
	if (curSel < 0)
	{
		setMessage("삭제할 그룹이 없습니다.");
		return;
	}
	
	CString delKey;
	CString curName = m_tree.GetItemText(item);

	_gpSave *gpSave = m_arGpSave.GetAt(curSel);
	
	// 저장시 삭제 알려주어야 한다.
	delKey.Format("%s\t%s", gpSave->sGroupID, gpSave->sGrName);
	m_arDelGp.Add(delKey);

	CDebug dbg;
	dbg << FormatString("delete group[%s]", delKey) << mdebug::endl;

	LIB_DELETE(gpSave);
	m_arGpSave.RemoveAt(curSel);
	m_tree.DeleteItem(item);
	
	HTREEITEM rootItem = m_tree.GetRootItem();
	if (m_tree.ItemHasChildren(rootItem))
	{
		m_tree.SelectItem(m_tree.GetChildItem(rootItem));
		updateGrEdit(0);
	}
	else
	{
		updateGrEdit(-1);
	}
	
	
	setMessage("[%s] 그룹을 삭제하였습니다.", curName);
	m_bChange = true;
}

void CMapForm::OnBtnadd() 
{
	addGrEdit();
	m_bChange = true;
}

void CMapForm::OnBtninsert() 
{
	int selGrEdit = getGrEditSelectRow();
	if (selGrEdit < 0)
		selGrEdit = 0;
	addGrEdit(selGrEdit);
	m_bChange = true;
}

void CMapForm::OnBtndirect() 
{
	if (!m_dlgDirect)
		m_dlgDirect = std::make_unique<class CDlgDirect>(this);

	if (m_dlgDirect && m_dlgDirect->DoModal() != IDOK)
		return;
	m_dlgDirect->GetAccInfo(m_sInputAccNum, m_sInputAccName, m_sInputAccPass);
	queryAccInspectName(m_sInputAccNum);
	m_bChange = true;
}

void CMapForm::OnBtndel() 
{
	delGrEdit();
	m_bChange = true;
	
}

void CMapForm::OnBtnup() 
{
	changeGrEdit(true);
	m_bChange = true;
}

void CMapForm::OnBtndn() 
{
	changeGrEdit(false);
	m_bChange = true;
}

void CMapForm::OnBtnuptop() 
{
	changeGrEdit(true, true);
	m_bChange = true;
}

void CMapForm::OnBtndnbot() 
{
	changeGrEdit(false, true);
	m_bChange = true;
}

void CMapForm::OnBtnalladd() 
{
	addGrEditAll();
	m_bChange = true;
}

void CMapForm::OnBtnalldel() 
{
	delGrEditAll();
	m_bChange = true;
}

void CMapForm::OnBtncancel() 
{
	m_pParent->PostMessage(WM_USER, closeDLL);
}

void CMapForm::OnBtnok() 
{
	queryGroupSave(SC_SAVEACC, m_selType);	// 저장 OK되면 화면 닫음
}

void CMapForm::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM item;
	int sel = getCurGr(item);
	if (sel < 0)
	{
		HTREEITEM item = m_tree.GetChildItem(m_tree.GetRootItem());
		if (item)
		{
			m_tree.SelectItem(item);
			sel = 0;
		}
	}
	updateGrEdit(sel);
	*pResult = 0;
}


//=================================================================================================
void CMapForm::OnGrAccClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	if (pResult)	*pResult = 0;
}


void CMapForm::OnGrAccblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	addGrEdit();
}

void CMapForm::OnGrEditClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	if (pgvitem->col != IECOL_PRI)
		return;
	const int row = pgvitem->row;
	if (row < 1)
		return;

	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	_acSave *acSave = NULL;
	
	for (int ii = 1; ii < m_pGrEdit->GetRowCount(); ii++)
	{
		acSave = gpSave->arAcEdit.GetAt(ii-1);
		if (ii == row)
		{
			acSave->sPriAcc = "1";
			
		}
		else
		{
			acSave->sPriAcc = "";
			m_pGrEdit->SetItemParam(ii, IECOL_PRI, 0);
		}
	}
	m_bChange = true;
}



void CMapForm::OnGrEditEndSort(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	sortGpSave();
	m_bChange = true;
}

void CMapForm::OnGrEditdblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
}

void CMapForm::OnBeginEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM* pgvitem = (NM_GVITEM*)pNMHDR;

	if (pgvitem->hdr.idFrom != IDC_GREDIT)
		return;


	const int row = pgvitem->row-1;
	if (row < 0)
		return;
	switch (pgvitem->col)
	{
	case IECOL_PASSWORD:
	case IECOL_RATE:
	case IECOL_ALIAS:
		break;
	default:
		return;
	}
	
	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	const int selGrEdit = getGrEditSelectRow();
	if (selGrEdit < 0)
		return;

	_acSave *acSave = NULL;
	acSave = gpSave->arAcEdit.GetAt(row);
	if (!acSave)	return;

	m_passRow = -1;
	m_pGrEdit->SetPass(acSave->sPassword);
	m_bChange = true;
	
}


void CMapForm::OnEndEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM* pgvitem = (NM_GVITEM*)pNMHDR;
	if (pgvitem->hdr.idFrom != IDC_GREDIT)
		return;

	m_bChange = true;
	const int row = pgvitem->row-1;
	if (row < 0)
		return;
	
	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	_acSave *acSave = NULL;
	acSave = gpSave->arAcEdit.GetAt(row);
	if (!acSave)	return;

	CString accS;
	const int val = 0;
	CString tmpS = m_pGrEdit->GetItemText(pgvitem->row, pgvitem->col);
	tmpS.TrimRight();
	tmpS.TrimLeft();
	double rate = 0.0;
	switch (pgvitem->col)
	{
	case IECOL_PASSWORD:		// Pass
		accS  = m_pGrEdit->GetItemText(pgvitem->row, IECOL_ACCNUM);
		accS.Remove('-');
		tmpS = m_pGrEdit->GetPass();
		acSave->sPassword = tmpS;
		m_pGrEdit->SetItemText(pgvitem->row, pgvitem->col, getPassString(tmpS));
		setMapPass(accS, tmpS);
		
		if (m_passCheck.GetCheck())
		{
			m_passRow = pgvitem->row;	// 패스워드 검증 row
			queryAccInspect(accS, tmpS, SC_ACCPASSVALID);
		}
		else
		{
			if (pgvitem->row+1 < m_pGrEdit->GetRowCount())
				m_pGrEdit->SetFocusCellEdit(pgvitem->row+1, pgvitem->col, false);
		}
		break;
	case IECOL_RATE:		// 비율
		rate = atof(tmpS);
		if (rate < 0)   rate = 0;
		tmpS.Format("%.f", rate);
		acSave->sAllocRate = tmpS;
		m_pGrEdit->SetItemText(pgvitem->row, pgvitem->col, tmpS);
		if (pgvitem->row+1 < m_pGrEdit->GetRowCount())
			m_pGrEdit->SetFocusCellEdit(pgvitem->row+1, pgvitem->col, false);
		break;
	case IECOL_ALIAS:		// 계좌별칭
		acSave->sAccntNick = tmpS;
		if (pgvitem->row+1 < m_pGrEdit->GetRowCount())
			m_pGrEdit->SetFocusCellEdit(pgvitem->row+1, pgvitem->col, false);
		break;

	default:
		break;
	}
}

LRESULT CMapForm::OnKeyMsg(WPARAM wParam, LPARAM lParam)
{
	OnTabNext(wParam, lParam);
	return 0;
}

//=================================================================================================
void CMapForm::initControl()
{
	m_pFont = getAxFont(m_sFontName, m_nFontPoint);

	// fxStatic
	CRect rcMap;
	GetClientRect(rcMap);
	CRect rcTop;
	rcTop.SetRect(GAP_SIDE, GAP_TOP, rcMap.right - GAP_SIDE, GAP_TOP + PANAL_HEIGHT);

	CRect rcPanel;
	rcPanel.SetRect(rcMap.left, rcMap.top, rcMap.right, rcMap.bottom -22 /* + GAP_TOP + GAP_BLOCK*/);
	m_panel = std::make_unique<CfxStatic>();
	m_panel->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, rcPanel, this, IDC_PANEL);
	m_panel->Init(ssGRADIENT, 0, _T(""), _T(""), GetIndexColor(clPanel), GetIndexColor(clPanel), GetIndexColor(clPanel), 0);
	//m_panel->SetBkColor(GetIndexColor(clContents));
	//"", WS_VISIBLE | WS_CHILD | SS_RIGHT, itemRC, this, ID_STATIC_TITLE);

	HBITMAP BitMap[4] = { 0, };
	CStringArray strImage;
	//panel
	strImage.Add("AXPANEL1_LT.bmp");
	strImage.Add("AXPANEL1_RT.bmp");
	strImage.Add("AXPANEL1_LB.bmp");
	strImage.Add("AXPANEL1_RB.bmp");
	
	int nLineTick[4];
	nLineTick[0] = GAP_SIDE;
	nLineTick[1] = GAP_TOP;
	nLineTick[2] = GAP_SIDE;
	nLineTick[3] = GAP_BLOCK;
	
	for(int i = 0; i < strImage.GetSize(); i++)
	{	
		//CString sImageDir = m_sRoot + "\\image\\" + strImage.GetAt(i);
		BitMap[i] = getBitmap2(strImage.GetAt(i));
	}
	//if(MessageBox("asdf","asdf",MB_YESNO)==IDYES){}
	m_panel->SetCornerRound(BitMap, 5, &nLineTick[0]);
	m_panel->SetBorderColor(GetIndexColor(clBack));
	// button
	setImgBtn(&m_btnRefresh, 4, TRUE);
	
	setImgBtn(&m_btnAdd, 4, TRUE);
	setImgBtn(&m_btnInsert, 4, TRUE);
	setImgBtn(&m_btnDel, 4, TRUE);
	setImgBtn(&m_btnUp, 4, TRUE);
	setImgBtn(&m_btnDn, 4, TRUE);
	setImgBtn(&m_btnUpTop, 4, TRUE);
	setImgBtn(&m_btnDnBot, 4, TRUE);
	setImgBtn(&m_btnAllDel, 4, TRUE);
	setImgBtn(&m_btnAllAdd, 4, TRUE);
	setImgBtn(&m_btnDirect, 4, TRUE);

	setImgBtn(&m_btnGrNew, 4, TRUE);
	setImgBtn(&m_btnGrDel, 4, TRUE);
	setImgBtn(&m_btnGrChg, 4, TRUE);
	setImgBtn(&m_btnGrCopy, 4, TRUE);
	setImgBtn(&m_btnGrDown, 1, TRUE);
	setImgBtn(&m_btnGrUp, 1, TRUE);
	setImgBtn(&m_btnInital, 4, TRUE);
	
	setImgBtn(&m_btnCancel, 4, TRUE);
	setImgBtn(&m_btnOk, 4, TRUE);

	// check button
	m_passCheck.SetCheck(TRUE);

	// static
	m_result.setColor(GetIndexColor(34), GetIndexColor(69), GetIndexColor(34));
	m_result.SetWindowText("");
	
	m_stGrNum.setColor(RGB(240, 240, 240), RGB(219, 0, 0), RGB(150, 150, 150));
	m_stGrNum.SetWindowText("");
	m_stGrName.setColor(RGB(240, 240, 240), RGB(219, 0, 0), RGB(150, 150, 150));
	m_stGrName.SetWindowText("");


	// 계좌 grid
	CRect rc;
	GetDlgItem(IDC_STGRACC)->GetWindowRect(rc);
	ScreenToClient(rc);
	m_pGrAcc = std::make_unique<CXXGrid>();
	m_pGrAcc->Create(rc, this, IDC_GRACC, GVSC_VERT, NULL, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
	initGrid(m_pGrAcc.get(), IDC_GRACC);

	GetDlgItem(IDC_STGREDIT)->GetWindowRect(rc);
	ScreenToClient(rc);
	rc.right = rc.left + 250 + 20;
	m_pGrEdit = std::make_unique<CXXGrid>();
	m_pGrEdit->Create(rc, this, IDC_GREDIT, GVSC_VERT, NULL, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
	initGrid(m_pGrEdit.get(), IDC_GREDIT);

	selectRadio(IDC_RDHM, false, true);

	// group Tree
	m_tree.Init(this, m_pFont);
	m_tree.SetDragEnabled(true);

	// 고객은 비밀번호 입력 안됨
	if (m_bCustomer)
	{
		GetDlgItem(IDC_BTNDIRECT)->ShowWindow(SW_HIDE);
	    GetDlgItem(IDC_PASSCHECK)->ShowWindow(SW_HIDE);
	}
}

void CMapForm::initGrid(CXXGrid* pGrid, UINT idc)
{
	_GVITEM gvitem;
	LOGFONT lf{};
	COLORREF gridHead{}, gridLine{};
	int ii = 0;
	CString	sectionS;
	int colCnt = 0;
	
	gridHead = GetIndexColor(clrGridHead);
	gridLine = GetIndexColor(clrGridLine);
	
	pGrid->SetBkColor(GetIndexColor(clrGrid1stRow));

	pGrid->SetGridColor(gridLine);
	pGrid->SetGridFocusLine(TRUE, TRUE);

	if (m_pFont)	m_pFont->GetLogFont(&lf);
	lf.lfHeight = m_nFontPoint * 10;

	gvitem.row = 0;
	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR|GVMK_MISC|GVMK_PARAM;
	
	gvitem.font = lf;
	gvitem.format = GVFM_HEADER;
	gvitem.fgcol = GetIndexColor(76);
	gvitem.bkcol = gridHead;

	switch (idc)
	{
	case IDC_GRACC:
		colCnt = sizeof(grInfoAcc)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
		for (ii = 0; ii < colCnt; ii++)
		{
			gvitem.symbol = grInfoAcc[ii].symbols;
			gvitem.text = grInfoAcc[ii].heads;
			gvitem.attr = grInfoAcc[ii].attr;
			gvitem.col = ii;
			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoAcc[ii].width);
		}
		sectionS = GRACC;
		break;
	case IDC_GREDIT:
		colCnt = sizeof(grInfoEdit)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
		pGrid->m_nPassCol = IECOL_PASSWORD;

		gvitem.param = 10;	// ?
		for (ii = 0; ii < colCnt; ii++)
		{
			gvitem.symbol = grInfoEdit[ii].symbols;
			gvitem.text = grInfoEdit[ii].heads;
			gvitem.attr = grInfoEdit[ii].attr;
			if (ii == 0)
				gvitem.attr &= ~GVAT_CHECK;
			gvitem.col = ii;
			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoEdit[ii].width);
		}
		sectionS = GREDIT;
		break;
	}
	
	pGrid->SetRowHeight(0, GRIDROW_HEIGHT);
	pGrid->SetRowColResize(FALSE, FALSE);
	pGrid->SetBKSelColor(GetIndexColor(clrGridFocusBG));
//	pGrid->SetGridColor(GetIndexColor(65));
//	pGrid->SetBkColor(GetIndexColor(CLRBG));
	pGrid->Adjust();
	loadGrid(pGrid, sectionS);	
}


void CMapForm::setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit)
{
	CString sImageDir;
	sImageDir.Format("%s\\image\\%dbtn", m_sRootDir, nLength);
	
	pButton->m_hBitmap    = getAxBitmap(sImageDir + ".bmp")->operator HBITMAP();
	pButton->m_hBitmap_dn = getAxBitmap(sImageDir + "_dn.bmp")->operator HBITMAP();
	pButton->m_hBitmap_hv = getAxBitmap(sImageDir + "_en.bmp")->operator HBITMAP();
	
	if (bImageFit)
		pButton->FitImageSize();

}

bool CMapForm::loadGrid(CXXGrid* pGrid, CString section)
{
	return true;
	char	buf[1024]{};
	CString	str, strUnit;
	int	nCount = 0;

	nCount = (int)GetPrivateProfileInt(section, "COUNT", 0, m_szFilename);
	if (!nCount) 	return false;

	GetPrivateProfileString(section, "SIZE", _T(""), buf, sizeof(buf), m_szFilename);
	
	CWordArray	arArray;
	BOOL		bSize = TRUE;
	str = buf;

	if (str.IsEmpty())
		return false;

	for (int ii = 0 ; ii < nCount ; ii++)
	{
		strUnit = Parser(str, ",");

		if (strUnit.IsEmpty())
		{
			bSize = FALSE;
			break;
		}
		arArray.Add(atoi(strUnit));
	}

	if (bSize)
	{
		for (int ii = 0 ; ii < min(nCount, pGrid->GetColumnCount()); ii++)
		{
			pGrid->SetColumnWidth(ii, arArray.GetAt(ii));
		}
	}
	arArray.RemoveAll();
	return true;
}

bool CMapForm::saveGrid(CXXGrid* pGrid, CString section)
{
	return true;
	CString	str = _T(""), str2 = _T("");
	const int	nCol = pGrid->GetColumnCount();

	str.Format("%d", nCol);
	WritePrivateProfileString(section, "COUNT", str, m_szFilename);

	for ( int ii = 0 ; ii < nCol ; ii++ )
	{
		if (ii == (nCol - 1))
			str.Format("%d", pGrid->GetColumnWidth(ii));
		else
			str.Format("%d, ", pGrid->GetColumnWidth(ii));
		str2 += str;
	}

	if (!str2.IsEmpty())
		WritePrivateProfileString(section, "SIZE", str2, m_szFilename);
	return true;
}


void CMapForm::enableGroup(BOOL bEnable)
{
}

void CMapForm::enableAcc(BOOL bEnable)
{
}

CString CMapForm::getLedger(CString sTran, char cGubn, CString sSvcn, CString sLastKey /*= ""*/)
{
	struct	_ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');
	
	switch (cGubn)
	{
	case 'I':
		cGubn = '2';	break;
	case 'U':
		cGubn = '3';	break;
	case 'D':
		cGubn = '4';	break;
	case 'Q':
	default:
		cGubn = '1';
	}

	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	if (!sLastKey.IsEmpty())
		CopyMemory(&ledger.next, sLastKey, sLastKey.GetLength());

	return CString((char*)&ledger, L_ledgerH);
}


void CMapForm::queryAcc()
{
	if (m_bCustomer)
	{
		struct _acSet	acSet;
		memset(&acSet, ' ', L_acSet);
		acSet.func[0] = 'Q';
		CopyMemory(acSet.usid, m_sStaffNum, m_sStaffNum.GetLength());
		sendTR(TR_ACCT, (char *)&acSet, L_acSet, SC_ACCCUST);
	}
	else
	{
		if (m_sStaffNum.IsEmpty() || m_sPassWord.IsEmpty())
			return;

		CString sData = _T("");

		if (m_gubn == 'D')
			sData = getLedger("2705", 'Q', "SI511101_Q");
		else
			sData = getLedger("2705", 'Q', "SI511100_Q");

		queryStaffAcc(sData);
	}

	enableAllBtn(FALSE);
}

void CMapForm::queryStaffAcc(CString ledgerS)
{
	CString sData = ledgerS;

	if (m_gubn == 'D')
	{
		sData += m_sStaffNum + "\t";
	}
	else
	{
		CString passX = "TRUE";
		sData += m_sDeptCode + "\t" + m_sStaffNum + "\t\t" + passX + "\t";
	}

	SendTR("PIBOTUXQ", NULL, sData, SC_ACCLIST);
}

void CMapForm::queryGroup()
{
	m_bChange = false;
	deleteAllGpSave();
	m_mapPass.RemoveAll();

	//_gmid* pgmid = new _gmid;
	//memset(pgmid, ' ', sizeof(struct _gmid));
	//pgmid->xflg  = '1';				// 조회
	//memcpy(pgmid->usrid, (LPSTR)(LPCTSTR)m_sStaffNum, m_sStaffNum.GetLength());
	//memcpy(pgmid->grec, "0000", sizeof(pgmid->grec));

	//switch (m_selType)
	//{
	//case SEL_FU:
	//	pgmid->sflag = '2';				// 선물옵션 조회
	//	break;
	//default:
	//	pgmid->sflag = '1';				// 현물 조회
	//	break;
	//}
	//CString sData;
	//sData.Format("%s", pgmid);
	//TRSend(TR_GROUP, NULL, sData, SC_ACCLISTBYGROUP);

	_gmid	gmid;
	memset((char*)&gmid, ' ', sizeof(struct _gmid));
	gmid.xflg  = '1';				// 조회
	memcpy(gmid.usrid, (LPSTR)(LPCTSTR)m_sStaffNum, m_sStaffNum.GetLength());
	memcpy(gmid.grec, "0000", 4);

	switch (m_selType)
	{
	case SEL_FU:
		gmid.sflag = '2';				// 선물옵션 조회
		break;
	default:
		gmid.sflag = '1';				// 현물 조회
		break;
	}

	CString sData = CString((char*)&gmid, L_gmid);
	SendTR(TR_GROUP, US_KEY, sData, SC_ACCLISTBYGROUP);
	//SendTR(TR_GROUP, NULL, sData, SC_ACCLISTBYGROUP);
}
void CMapForm::queryGroupSave(int sendKey, int selType)
{
#ifdef	_FILE_DEBUG
	dFile	<< "저장 start =>" << endl;
#endif

	m_nextTRPos = -1;
	m_totTRCnt = -1;
	m_subTRCnt = -1;

	int ii = 0, jj = 0;
	int pos = L_gmid;

	_gpSave *gpSave = NULL;
	_acSave *acSave = NULL;
	
	int gpCnt = 0;
	CString tmpS;
	
	_group	group;
	std::unique_ptr<_gmid> pgmid;
	_accn	accn;

	if (m_sendB)
	{
		m_sendB = NULL;
	}

	m_calBufL = L_gmid + L_group * m_arDelGp.GetSize();
	for (ii = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		gpSave = m_arGpSave.GetAt(ii);

		if (!gpSave)	
			continue;

		for (jj = 0; jj < gpSave->arAcEdit.GetSize(); jj++)
		{
			m_calBufL += L_accn;
		}

		m_calBufL += L_group;
	}

	m_sendB = std::make_unique<char[]>(m_calBufL + 1);

	memset(m_sendB.get(), ' ', m_calBufL + 1);

	pgmid = std::make_unique<_gmid>();
	memset(pgmid.get(), ' ', L_gmid);

	pgmid->xflg  = '2';				// 저장
	//sprintf(gmid.usrid, "%-12s", m_sStaffNum.GetString());	// 사원번호
	memcpy(pgmid->usrid, (LPSTR)(LPCTSTR)m_sStaffNum, m_sStaffNum.GetLength());


	switch (selType)
	{
	case SEL_FU:
		pgmid->sflag = '2';				// 선물옵션
		break;
	default:
		pgmid->sflag = '1';				// 주식
		break;
	}
#ifdef	_FILE_DEBUG
	dFile	<< "그룹삭제 편집" << endl;
#endif

	// 삭제된 그룹
	int find = 0;
	CString sGroupID, sGrName;

	for (ii = 0; ii < m_arDelGp.GetSize(); ii++)
	{
		tmpS = m_arDelGp.GetAt(ii);
		find = 0;
		if ((find = tmpS.Find('\t', find)) == 0)
			continue;
		sGroupID = tmpS.Left(find);
		sGrName = tmpS.Mid(find+1);

		memset(&group, ' ' , L_group);
		group.xflg = 'D';		// Delete
		SetString(group.gpid, sizeof(group.gpid), sGroupID);
		strncpy(group.seqn, "000", sizeof(group.seqn));
		SetString(group.gnam, sizeof(group.gpid), sGrName);
		strncpy(group.nrec, "0000", sizeof(group.nrec));
		memcpy(&m_sendB.get()[pos], &group, L_group);
		pos += L_group;
		
		gpCnt++;
	}
#ifdef	_FILE_DEBUG
	dFile	<< "그룹 U/I 편집" << endl;
#endif

	bool bAccPri = false;
	int seqn = 0;
	int arAcEditCnt = 0;
	CString passS;

	for (ii = 0, seqn = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		gpSave = m_arGpSave.GetAt(ii);

		memset(&group, ' ' , L_group);
		if (gpSave->sGroupID.IsEmpty())
			group.xflg = 'I';	// Insert
		else
			group.xflg = 'U';	// update

		arAcEditCnt = gpSave->arAcEdit.GetSize();
		
		if (arAcEditCnt == 0)
		{
			group.xflg = 'D';	// delete : 등록계좌 없으면 delete, 새로 등록한 그룹인 경우는 서버에 저장안함
			if (gpSave->sGroupID.IsEmpty())
				continue;
		}

		SetString(group.gpid, sizeof(group.gpid), gpSave->sGroupID);
		FormatCopy(&group.seqn, "%03d", ++seqn);
		SetString(group.gnam, sizeof(group.gnam), gpSave->sGrName);
		FormatCopy(&group.nrec, "%04d", arAcEditCnt);
		memcpy(&m_sendB.get()[pos], &group, L_group);

		pos += L_group;

		gpCnt++;

		// 대표계좌등록
		bAccPri = false;
		for (jj = 0; jj < arAcEditCnt; jj++)
		{
			acSave = gpSave->arAcEdit.GetAt(jj);
			if (acSave->sPriAcc == "1")
			{
				bAccPri = true;
				break;
			}
		}
		if (arAcEditCnt > 0 && !bAccPri)
			acSave->sPriAcc = "1";

		// 계좌저장
		for (jj = 0; jj < arAcEditCnt; jj++)
		{
			acSave = gpSave->arAcEdit.GetAt(jj);
			memset(&accn, ' ', L_accn);

			sprintf(accn.seqn, "%03d", jj + 1);
			SetString(accn.accn, sizeof(accn.accn), acSave->sAccntNum);

			if (!m_bCustomer)
			{
				passS = getMapPass(acSave->sAccntNum);
				if (!passS.IsEmpty())
					SetString(accn.pass, sizeof(accn.pass), passS);
				else
					SetString(accn.pass, sizeof(accn.pass), "          ");
			}
			else
				SetString(accn.pass, sizeof(accn.pass), "          ");

			SetString(accn.acnm, sizeof(accn.acnm), acSave->sAccntName);

			SetString(accn.rate, sizeof(accn.rate), Format("%.f", atof(acSave->sAllocRate)));
			SetString(accn.multi, sizeof(accn.multi), acSave->sAllocMulti);
			SetString(accn.alis, sizeof(accn.alis), acSave->sAccntNick);
			if (!acSave->sPriAcc.IsEmpty())
				accn.prea = acSave->sPriAcc.GetAt(0);
			else
				accn.prea = ' ';

			memcpy(&m_sendB.get()[pos], &accn, L_accn);
			pos += L_accn;
		}
	}

	sprintf(pgmid->grec, "%04d", gpCnt);		// 총 그룹개수

	memcpy(&m_sendB.get()[0], pgmid.release(), L_gmid);
TRACE("m_calBufL[%d] MAX_SEND[%d]\n", m_calBufL, MAX_SEND);
	if (m_calBufL > MAX_SEND-1)
	{
		m_nextTRPos = 0;
		m_subTRCnt = 1;
		m_totTRCnt = int(m_calBufL / (MAX_SEND + L_ihead)) + 1;
TRACE("연속 m_totTRCnt1[%d]\n", m_totTRCnt);
		const int val = m_totTRCnt * (MAX_SEND - L_ihead);
		if (val < m_calBufL)
			m_totTRCnt++;
	//	m_totTRCnt = m_calBufL / (MAX_SEND + L_ihead) + 1;


#ifdef	_FILE_DEBUG
	dFile	<< "연속 저장 tot[" << m_totTRCnt << "] sub[" << m_subTRCnt << "] calBufL[" << m_calBufL << "] L_ihead[" << L_ihead << "]" << endl;
#endif

		nextSend('F', sendKey);

	}
	else
	{
		sendTR(TR_GROUP, m_sendB.get(), pos, sendKey); 
#ifdef	_FILE_DEBUG
	dFile	<< "그룹저장 all" << endl;
#endif
	}
}

void CMapForm::nextSend(char next, int sendKey)
{
#ifdef	_FILE_DEBUG
	dFile	<< "그룹저장 next start  => [" << next << "]" << endl;
#endif
	if (m_sendB2)
	{	
		m_sendB2 = NULL;
	}
	
	int sendL = MAX_SEND-L_ihead;
	if (m_subTRCnt == m_totTRCnt)
	{
		sendL = m_calBufL - m_nextTRPos;
		next = 'E';
	}

	CString tmpS;
	m_sendB2 = std::make_unique<char[]>(MAX_SEND + 1);
	memset(m_sendB2.get(), ' ', MAX_SEND + 1);

	_ihead*	ihead = (struct _ihead *)m_sendB2.get();
	ihead->xflg[0] = next;

	FormatCopy(&ihead->csiz, "%06d", sendL);
	FormatCopy(&ihead->fsiz, "%06d", m_calBufL);

	memcpy(&m_sendB2[L_ihead], &m_sendB.get()[m_nextTRPos], sendL);
	m_nextTRPos += sendL;
	m_subTRCnt++;
	
	sendTR(TR_GROUP, m_sendB2.get(), sendL+L_ihead, sendKey);

#ifdef	_FILE_DEBUG
	dFile	<< "그룹저장 next send[" << next << "]" << endl;
#endif

}

void CMapForm::queryAccNameSearch(CString accNum)
{
	CString sData = getLedger("XXXX", 'Q', "SX945310_Q");	// 계좌명 조회
	sData += GetAcctSymbol(accNum);
	SendTR("PIBOTUXQ", NULL, sData, SC_DLGACCOUNTNAME);
}

void CMapForm::queryAccInspectName(CString accNum)
{
	if (accNum.GetLength() != LEN_ACCT)
	{
		setMessage((LPSTR)(LPCTSTR)Format("계좌는 %d자리를 입력하셔야 합니다.", LEN_ACCT));
		return;
	}
	if (m_selType == SEL_FU && !IsFOptionAcc(accNum))
	{
		setMessage("선물/옵션 계좌만 입력이 가능합니다.");
		return;
	}
	if (m_selType == SEL_HM && !IsStockAcc(accNum))
	{
		setMessage("주식 계좌만 입력이 가능합니다.");
		return;
	}

	CString sData = getLedger("XXXX", 'Q', "SX945310_Q");	// 계좌명 검증
	sData += GetAcctSymbol(accNum);
	SendTR("PIBOTUXQ", NULL, sData, SC_ACCOUNTNAME);
}

void CMapForm::queryAccInspect(CString accNum, CString accPass, int key)
{
	CString sData = getLedger("2705", 'Q', "SXACTCHK_Q");
	sData += accNum + "\t";
#ifdef ENCTEST
	CString passX;
	passX.Format("%s\t%s", accPass, accNum.Left(8));						
	passX = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
#endif
	sData += passX + "\t";

	SendTR("PIBOTUXQ", NULL, sData, key);
}

void CMapForm::sendTR(CString trC, char* pBytes, int nBytes, char key)
{
	if (!m_pParent)
		return;

	const int	len = L_userTH + nBytes;
	std::unique_ptr<char[]> sndB = std::make_unique<char[]>(len + 1);
	struct _userTH* udat = (struct _userTH*)sndB.get();

	ZeroMemory(sndB.get(), sizeof(sndB));

	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = key;
	udat->stat = US_ENC;

	CopyMemory(&sndB[L_userTH], pBytes, nBytes);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes), (LPARAM)sndB.get());
}

void CMapForm::oubAccUser(char* buf)
{
	deleteAllAcSave();

	struct _acSet* acSet  = (struct _acSet*) buf;
	
	CString	msg = CString(acSet->emsg, sizeof(acSet->emsg));
	msg.TrimRight();	
	m_result.SetWindowText(msg);

	int nRec = atoi(CString(acSet->nrec, sizeof(acSet->nrec)));

	CString accNum, accName;
	buf += L_acSet;
	for (int ii = 0; ii < nRec ; ii++)
	{
		_acInfo *acInfo = (_acInfo *)buf;
		buf += L_acInfo;

		accNum = GetString(acInfo->accn, sizeof(acInfo->accn));
		accName = GetString(acInfo->acnm, sizeof(acInfo->acnm));
		switch (m_selType)
		{
		case SEL_FU:
			if (!IsFOptionAcc(accNum))
				continue;
			break;
		default:
			if (!IsStockAcc(accNum))
				continue;
			break;
		}

		if (accName.IsEmpty() || accNum.IsEmpty())
			break;
		
		AddAcctSave(accNum, accName);
	}

	updateGrAcc();			// 계좌 grid update
	m_pGrAcc->Sort(1, true);	// 계좌명 sort
	m_bFirst = true;
	queryGroup();			// 그룹조회
}

void CMapForm::oubAccList(char* buf, int len)
{
	processError(buf);
	if (strncmp(LEDGER_ECODE(buf), "000000", 6))
		return;

	bool bNext = false;
	CString sData = _T("");
	if (LEDGER_CONF(buf) == '1')
	{
		sData = CString(buf, L_ledgerH);
		bNext = true;
		m_bFirst = false;
	}
	
	buf += L_ledgerH;
	if (m_bFirst)
		deleteAllAcSave();
	oubAccListEx(buf, len-L_ledgerH);
	if (bNext)			// 다음 데이타 조회
	{
		queryStaffAcc(sData);
		return;
	}

	updateGrAcc();			// 계좌 grid update
	m_pGrAcc->Sort(1, true);	// 계좌명 sort
	m_bFirst = true;
	queryGroup();			// 그룹조회
}

void CMapForm::oubAccListEx(char* buf, int len)
{
	CString datS = CString(buf, len);
	
	CString infoS, accNum, accName;
	while (!datS.IsEmpty())
	{
		infoS = parser(datS, "\n");
		if (infoS.IsEmpty())
			break;


		if (m_gubn == 'D')
		{
			accNum = parser(infoS, "\t");			// 계좌번호
			accName = parser(infoS, "\t");			// 고객명
		}
		else
		{
			parser(infoS, "\t");				// 관리자
			accNum = parser(infoS, "\t");			// 계좌번호
			accName = parser(infoS, "\t");			// 고객명
		}

//TRACE("accNum[%s] accName[%s]\n", accNum, accName);

		switch (m_selType)
		{
		case SEL_FU:
			if (!IsFOptionAcc(accNum))
				continue;
			break;
		default:
			if (!IsStockAcc(accNum))
				continue;
			break;
		}

		if (accName.IsEmpty() || accNum.IsEmpty())
			continue;

		AddAcctSave(accNum, accName);
	}
}


void CMapForm::oubGroup(char* buf)
{
	enableAllBtn(TRUE);
	deleteAllGpSave();
	m_mapPass.RemoveAll();

	_gmod *gmod = (_gmod *)buf;
	if (gmod->okgb != 'Y')
		return;
	int grec = atoi(CString(gmod->grec, sizeof(gmod->grec)));
	if (grec == 0)
	{
		updateTree();
		return;
	}
	
	CString tmpS;
	int accn = 0;
	buf += L_gmod;
	for (int ii = 0; ii < grec; ii++)
	{
		_group *group = (_group *)buf;

		_gpSave *gpSave = new _gpSave();
		gpSave->sGroupID = GetString(group->gpid, sizeof(group->gpid));
		gpSave->sGrSortNum = GetString(group->seqn, sizeof(group->seqn));
		gpSave->sGrName = GetString(group->gnam, sizeof(group->gnam));
		gpSave->sAccntCnt = GetString(group->nrec, sizeof(group->nrec));
		accn = atoi(gpSave->sAccntCnt);
		
		buf += L_group;
		for (int jj = 0; jj < accn; jj++)
		{
			_accn *accn = (_accn *)buf;
			
			_acSave *acSave = new _acSave;
			acSave->sSortNum = GetString(accn->seqn, sizeof(accn->seqn));
			acSave->sAccntNum = GetString(accn->accn, sizeof(accn->accn));
			acSave->sAccntName = GetAcctNameFromAllAccount(acSave->sAccntNum, GetString(accn->acnm, sizeof(accn->acnm)));//(GetString(accn->acnm, sizeof(accn->acnm)));
			tmpS = GetString(accn->rate, sizeof(accn->rate));
			acSave->sAllocRate.Format("%.f", atof(tmpS));
			acSave->sAccntNick = GetString(accn->alis, sizeof(accn->alis));
			
			if (m_bCustomer)
			{
				acSave->sPassword = Variant(passCC, acSave->sAccntNum);
			}
			else
			{
				acSave->sPassword = GetString(accn->pass, sizeof(accn->pass));
				setMapPass(acSave->sAccntNum, acSave->sPassword);
			}
			acSave->sPassword.TrimRight();
			acSave->sPriAcc = accn->prea;
			acSave->sAllocMulti =  GetString(accn->multi, sizeof(accn->multi));
			gpSave->arAcEdit.Add(acSave);
			buf += L_accn;
		}
		m_arGpSave.Add(gpSave);
	}
	updateTree();
}

void CMapForm::oubGroupSave(char* buf, int sendKey)
{
	bool bClose = true;
	if (sendKey == SC_SAVEACCONLY)
		bClose = false;
#ifdef	_FILE_DEBUG
	dFile	<< "그룹저장oub dat[" << buf << "]" << endl;
#endif

	_gmod* gmod = (_gmod *)buf;

	if (m_subTRCnt <= 0)
	{
#ifdef	_FILE_DEBUG
	dFile	<< "그룹저장oub all[" << gmod->okgb << "]" << endl;
#endif
		if (gmod->okgb == 'N')	// 저장오류
			setMessage("저장오류입니다.");
		else
		{
			if (bClose)
			{
				m_rtnStr = "OnGroupChange\tok";			// 스크립트로 변경되었다고 알려줌
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)m_rtnStr);
				m_pParent->PostMessage(WM_USER, closeDLL);
#ifdef	_FILE_DEBUG
	dFile	<< "그룹저장 ok close " << endl;
#endif
			}
			else
			{
				setMessage("그룹이 저장되었습니다.");
				enableAllBtn(FALSE);
				queryGroup();
				m_bFirst = true;
				//**queryAcc();			// 계좌조회
				LoadAccountListFromWizard();
			}
		}
		return;
	}
#ifdef	_FILE_DEBUG
	dFile	<< "그룹저장oub next[" << gmod->okgb << "]" << endl;
#endif


	// 14K넘는 경우
	switch (gmod->okgb)
	{
	case 'N':	// 저장오류
		setMessage("저장오류입니다.");
		break;
	case 'M':	// NEXT
		if (m_subTRCnt > m_totTRCnt)
			setMessage("저장오류입니다.");
		else
			nextSend('N', sendKey); 
		break;
	case 'Y':	// 저장 ok
		if (bClose)
		{
			m_rtnStr = "OnGroupChange\tok";			// 스크립트로 변경되었다고 알려줌
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)m_rtnStr);
			m_pParent->PostMessage(WM_USER, closeDLL);
#ifdef	_FILE_DEBUG
	dFile	<< "그룹저장 ok close " << endl;
#endif

		}
		else
		{
			setMessage("그룹이  저장되었습니다.");
			enableAllBtn(FALSE);
			queryGroup();
			m_bFirst = true;
			//**queryAcc();			// 계좌조회
			LoadAccountListFromWizard();
		}
		break;
	}
}


void CMapForm::oubAccInspectName(char *buf, int len)
{
	processError(buf);
	buf += L_ledgerH;

	CString accName, accNum;

	CString sData = CString(buf, len-L_ledgerH);
	accName = Parser(sData, "\t");
	accName.TrimLeft();
	if (accName.IsEmpty())
	{
		setMessage("등록할 수 없는 계좌입니다.");
		return;
	}

	if (m_sInputAccPass.IsEmpty() || !m_passCheck.GetCheck())
	{
		addInputAccount();
		return;
	}

	queryAccInspect(m_sInputAccNum, m_sInputAccPass, SC_INPUTACCOUNTVALID);

}

void CMapForm::oubAccInspectEdit(char *buf, int len)
{
	processError(buf);
	buf += L_ledgerH;

	CString result = CString(buf, len-L_ledgerH);
	CString valS = parser(result, "\t");
	if (atoi(valS) == 0)	// 검증성공
	{
		if (m_passRow < m_pGrEdit->GetRowCount())
			m_pGrEdit->SetFocusCellEdit(m_passRow+1, IECOL_PASSWORD, false);
		return;
	}
	
	if (m_passRow < 1)
		return;

	setMessage("비밀번호가 일치하지 않습니다.");	
	m_pGrEdit->SetItemText(m_passRow, IECOL_PASSWORD, "");
	CString accS  = m_pGrEdit->GetItemText(m_passRow, IECOL_ACCNUM);
	accS.Remove('-');
	setMapPass(accS, "");

	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	_acSave *acSave = NULL;
	acSave = gpSave->arAcEdit.GetAt(m_passRow-1);
	if (!acSave)	return;

	acSave->sPassword = "";

	m_passRow = -1;

}

void CMapForm::oubAccInspect(char *buf, int len)
{
	processError(buf);
	buf += L_ledgerH;
	CString result = CString(buf, len-L_ledgerH);
	CString valS = parser(result, "\t");
	if (atoi(valS) == 0)
	{
		addInputAccount();
		return;
	}
	setMessage("비밀번호가 일치하지 않습니다.");

}

void CMapForm::oubAccountName(char *buf, int len)
{
	buf += L_ledgerH;

	CString sData = CString(buf, len - L_ledgerH);
	CString sAccName = Parser(sData, "\t");
	if (m_dlgDirect)
		m_dlgDirect->SetAccName(sAccName);
}

void CMapForm::processError(void *pLedger)
{
	struct _ledgerH *ledger = NULL;
	ledger = (_ledgerH*)pLedger;
	CString msg(ledger->emsg, sizeof(ledger->emsg));
	msg.TrimRight();

	switch (ledger->eact[0])
	{
	default:
	case '0':	// 상태바
		m_result.SetWindowText(msg);
		break;
	}
}

void CMapForm::updateGrAcc()
{
	if (!m_pGrAcc)	return;
	
	m_pGrAcc->Clear();
	m_pGrAcc->SetRowCount(1);

	_acSave	*acSave = NULL;
	_GVITEM gvitem;
	LOGFONT lf{};
	CString	accS;
	
	int nRow = m_pGrAcc->GetRowCount();

	if (m_pFont)
		m_pFont->GetLogFont(&lf);
	lf.lfHeight = m_nFontPoint*10;
	gvitem.font   = lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.bkcol  = GetIndexColor(clrGridBG);
	gvitem.fgcol  = GetIndexColor(clrGridFG);

	for (int ii = 0; ii < m_arAcSave.GetSize(); ii++)
	{
		acSave = m_arAcSave.GetAt(ii);

		nRow = m_pGrAcc->GetRowCount();
		m_pGrAcc->InsertRow(" ", nRow);
		m_pGrAcc->SetRowHeight(nRow, GRIDROW_HEIGHT);
		gvitem.row    = nRow;

		// 계좌번호(편집)
		gvitem.text = EncodeAcctNo(acSave->sAccntNum);
		gvitem.col = IACOL_ACC;
		gvitem.format = grInfoAcc[IACOL_ACC].format;
		gvitem.attr   = grInfoAcc[IACOL_ACC].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_pGrAcc->SetItem(&gvitem);

		// 계좌이름
		gvitem.text = (m_useAlias && !acSave->sAccntNick.IsEmpty()) ? acSave->sAccntNick: acSave->sAccntName;
		gvitem.col = IACOL_NAME;
		gvitem.format = grInfoAcc[IACOL_NAME].format;
		gvitem.attr   = grInfoAcc[IACOL_NAME].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_pGrAcc->SetItem(&gvitem);

		// Index
		gvitem.text.Format("%d", ii);
		gvitem.col = IACOL_IDX;
		gvitem.format = grInfoAcc[IACOL_IDX].format;
		gvitem.attr   = grInfoAcc[IACOL_IDX].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_pGrAcc->SetItem(&gvitem);
	}
}

void CMapForm::updateGrEdit(int gpIdx)
{
	if (!m_pGrEdit)	return;

	updateGrContent(gpIdx);	// 그룹명, 등록계좌수

	m_pGrEdit->Clear();
	m_pGrEdit->SetRowCount(1);

	if (gpIdx < 0)
		return;

	const int edCnt = m_arGpSave.GetSize();
	if (edCnt <= 0 || gpIdx >= edCnt)
		return;
	
	_acSave* acSave = NULL;
	_gpSave	*gpSave = NULL;
	gpSave = m_arGpSave.GetAt(gpIdx);
	int row = 0;

	for (int ii = 0; ii < gpSave->arAcEdit.GetSize(); ii++)
	{
		acSave = gpSave->arAcEdit.GetAt(ii);
		row = insertGrEditRow(acSave, m_pGrEdit->GetRowCount());
	}
	updateGrEditIdx(gpSave);	// index
	updateGrEditPri(gpSave);	// 대표계좌
}

void CMapForm::updateGrEditIdx(_gpSave *gpSave)
{
	if (!gpSave)	return;

	CString idxS;
	_acSave *acSave = NULL;
	for (int ii = 0; ii < gpSave->arAcEdit.GetSize(); ii++)
	{
		idxS.Format("%d", ii+1);
		acSave = gpSave->arAcEdit.GetAt(ii);
		acSave->sSortNum = idxS;
		m_pGrEdit->SetItemText(ii+1, IECOL_SEQ, idxS);
	}
}
	
void CMapForm::updateGrEditPri(_gpSave *gpSave)
{
	if (!gpSave)	return;

	bool bPri = false;
	for (int row = 1; row < m_pGrEdit->GetRowCount(); row++)
	{
		if (m_pGrEdit->GetItemParam(row, IECOL_PRI))
			bPri = true;
	}
	if (!bPri && gpSave->arAcEdit.GetSize() > 0)
	{
		m_pGrEdit->SetItemParam(1, IECOL_PRI, 1);
		gpSave->arAcEdit.GetAt(0)->sPriAcc = "1";
	}
}

void CMapForm::updateGrContent(int gpIdx)
{
	if (gpIdx < 0 || gpIdx >= m_arGpSave.GetSize())
	{
		m_stGrNum.SetWindowText("");
		m_stGrName.SetWindowText("");
		return;
	}
	
	CString numS;
	_gpSave	*gpSave = m_arGpSave.GetAt(gpIdx);
	m_stGrName.SetWindowText("  " + gpSave->sGrName);
	numS.Format("%d 개  ", gpSave->arAcEdit.GetSize());
	m_stGrNum.SetWindowText(numS);
}

void CMapForm::updateTree()
{
	m_tree.DeleteAllItems();
	CString titleS = "";
	switch (m_selType)
	{
	case SEL_FU:
		titleS = "선물옵션그룹";
		break;
	default:
		titleS = "주식그룹";
		break;
	}
	HTREEITEM rootItem = m_tree.InsertItem(titleS, imgTROOT, imgTROOT, TVI_ROOT);
	HTREEITEM selItem = NULL, item = NULL;
	for (int ii = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		_gpSave *gpSave = m_arGpSave.GetAt(ii);
		item = m_tree.InsertItem(gpSave->sGrName, imgTGROUP, imgTGRSEL, rootItem);
		//m_tree.SetItemData(item, ii);
		if (ii == 0)
			selItem = item;
	}
	m_tree.Expand(rootItem, TVE_EXPAND);
	if (selItem)
		m_tree.SelectItem(selItem);
}

void CMapForm::deleteAllAcSave()
{
	_acSave *acSave = NULL;
	for (int ii = 0; ii < m_arAcSave.GetSize(); ii++)
	{
		acSave = m_arAcSave.GetAt(ii);
		if (acSave)	LIB_DELETE(acSave);
	}
	m_arAcSave.RemoveAll();
}

void CMapForm::deleteAllGpSave()
{
	for (int ii = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		_gpSave *gpSave = m_arGpSave.GetAt(ii);
		if (gpSave)	LIB_DELETE(gpSave);
		
	}
	m_arGpSave.RemoveAll();

	
}


void CMapForm::setMapPass(CString acc, CString pass)
{
	pass.TrimRight();
	acc.Remove('-');
	//if (!m_mapPass.Lookup(acc, pass))
	//{
		m_mapPass.SetAt(acc, pass);
	//}
}

CString CMapForm::getMapPass(CString acc)
{
	CString pass = _T("");
	m_mapPass.Lookup(acc, pass);
	return pass;
}

CString CMapForm::getPassString(CString sPass)
{
#ifdef	_DEBUG
	return sPass;
#endif
	CString sRtn = "";
	for (int ii = 0; ii < sPass.GetLength(); ii++)
		sRtn += '*';
	return sRtn;
}


CString CMapForm::parser(CString &srcstr, CString substr)
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

int CMapForm::getCurGr(HTREEITEM &item)
{
	item = m_tree.GetSelectedItem();
	if (item == NULL || item == m_tree.GetRootItem())
		return -1;
	CString grName = m_tree.GetItemText(item);
	return getGr(grName);
}

int CMapForm::getGr(CString grName)
{
	_gpSave *gpSave = NULL;
	for (int ii = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		gpSave = m_arGpSave.GetAt(ii);
		if (!gpSave)	continue;
		if (gpSave->sGrName == grName)
			return ii;
	}
	return -1;
}

CString CMapForm::getCurGrName()
{
	CString grName = "";
	HTREEITEM item = m_tree.GetSelectedItem();
	if (item == m_tree.GetRootItem())
		return grName;
	grName = m_tree.GetItemText(item);
	return grName;
}
	
_gpSave* CMapForm::getCurGpSave(int& gpIdx)
{
	HTREEITEM item;
	gpIdx = getCurGr(item);
	if (gpIdx < 0)
	{
		setMessage("선택한 그룹이 없습니다.");
		return NULL;
	}
	return m_arGpSave.GetAt(gpIdx);
}

int CMapForm::getGrAccSelectRow()
{
	int minRow, maxRow, minCol, maxCol;
	m_pGrAcc->GetSelectedCellRange(minRow, maxRow, minCol, maxCol);
	if (maxRow < 0 || maxCol < 0)
		return -1;
	return maxRow;
}

_acSave* CMapForm::getCurAcSave()
{
	_acSave* acSave = NULL;
	if (!m_pGrAcc)	return acSave;
	const int acSel = getGrAccSelectRow();
	if (acSel < 0)
	{
		setMessage("선택한 계좌가 없습니다.");
		return acSave;
	}
	if (acSel > m_arAcSave.GetSize())
	{
		setMessage("계좌 데이타 오류입니다.");
		return acSave;
	}
	CString accNum = m_pGrAcc->GetItemText(acSel, IACOL_ACC);
	accNum.TrimRight();
	if (accNum.IsEmpty())	return NULL;
	accNum.Remove('-');
	for (int ii = 0; ii < m_arAcSave.GetSize(); ii++)
	{
		acSave = m_arAcSave.GetAt(ii);
		if (acSave->sAccntNum == accNum)
			return acSave;
	}
	return NULL;
	/*
	_acSave* acSave = NULL;
	int acSel = getGrAccSelectRow();
	if (acSel < 0)
	{
		setMessage("선택한 계좌가 없습니다.");
		return acSave;
	}
	if (acSel > m_arAcSave.GetSize())
	{
		setMessage("계좌 데이타 오류입니다.");
		return acSave;
	}
	acSave = m_arAcSave.GetAt(acSel-1);
	return acSave;
	*/
}

int CMapForm::getGrEditSelectRow()
{
	int minRow, maxRow, minCol, maxCol;
	m_pGrEdit->GetSelectedCellRange(minRow, maxRow, minCol, maxCol);
	if (maxRow < 0 || maxCol < 0)
		return -1;
	return maxRow;
}

bool CMapForm::checkGrEdit(_gpSave* gpSave, CString accNum)
{
	if (!gpSave)	return false;

	const int acCnt = gpSave->arAcEdit.GetSize();
	if (acCnt >= MAX_EDIT)
	{
		setMessage("최대 %d개의 계좌를 등록하실 수 있습니다.", MAX_EDIT);
		return false;
	}

	CString cmpNum = accNum;
	for (int ii = 0; ii < gpSave->arAcEdit.GetSize(); ii++)
	{
		_acSave* acSave = gpSave->arAcEdit.GetAt(ii);
		if (acSave->sAccntNum == cmpNum)
		{
			setMessage("[%s]에 (%s)가 이미 등록되어 있습니다.", gpSave->sGrName, EncodeAcctNo(accNum));
			return false;
		}
	}
	return true;
}

int CMapForm::insertGrEditRow(_acSave* acSave, int row)
{
	_GVITEM gvitem;
	LOGFONT lf{};
	CString	accS;
	
	if (m_pFont)
		m_pFont->GetLogFont(&lf);
	lf.lfHeight = m_nFontPoint*10;
	gvitem.font   = lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.bkcol  = GetIndexColor(clrGridBG);
	gvitem.fgcol  = GetIndexColor(clrGridFG);

	row = m_pGrEdit->InsertRow(" ", row);
	m_pGrEdit->SetRowHeight(row, GRIDROW_HEIGHT);
	gvitem.row    = row;

	// 대표계좌
	gvitem.text = "";
	gvitem.col = IECOL_PRI;
	gvitem.format = grInfoEdit[IECOL_PRI].format;
	gvitem.attr = grInfoEdit[IECOL_PRI].attr;
	gvitem.param = atoi(acSave->sPriAcc);
	m_pGrEdit->SetItem(&gvitem);

	// 번호		
	gvitem.text = "";
	gvitem.col = IECOL_SEQ;
	gvitem.format = grInfoEdit[IECOL_SEQ].format;
	gvitem.attr   = grInfoEdit[IECOL_SEQ].attr;
	gvitem.param  = 0;
	m_pGrEdit->SetItem(&gvitem);

	// 계좌번호편집
	gvitem.text = EncodeAcctNo(acSave->sAccntNum);
	gvitem.col = IECOL_ACCNUM;
	gvitem.format = grInfoEdit[IECOL_ACCNUM].format;
	gvitem.attr   = grInfoEdit[IECOL_ACCNUM].attr;
	gvitem.attr &= ~GVAT_SORTSTR;
	m_pGrEdit->SetItem(&gvitem);
	
	// 계좌이름
	gvitem.text = acSave->sAccntName;
	gvitem.col = IECOL_ACCNAME;
	gvitem.format = grInfoEdit[IECOL_ACCNAME].format;
	gvitem.attr   = grInfoEdit[IECOL_ACCNAME].attr;
	gvitem.attr &= ~GVAT_SORTSTR;
	m_pGrEdit->SetItem(&gvitem);

	// 비밀번호
	gvitem.text = getPassString(acSave->sPassword);
	gvitem.col = IECOL_PASSWORD;
	gvitem.format = grInfoEdit[IECOL_PASSWORD].format;
	gvitem.attr   = grInfoEdit[IECOL_PASSWORD].attr;
	if (m_bCustomer)	// 고객은 비밀번호 변경할 수 없음
		gvitem.attr &= ~GVAT_EDIT;
	else
		gvitem.bkcol  = RGB(255, 255, 170);

	gvitem.param  = 0;
	m_pGrEdit->SetItem(&gvitem);

	// 비율
	gvitem.bkcol  = RGB(255, 255, 170);
	gvitem.text = acSave->sAllocRate;
	gvitem.col = IECOL_RATE;
	gvitem.format = grInfoEdit[IECOL_RATE].format;
	gvitem.attr   = grInfoEdit[IECOL_RATE].attr;
	gvitem.attr &= ~GVAT_SORTVAL;
	m_pGrEdit->SetItem(&gvitem);

	// 계좌별칭
	gvitem.text   = acSave->sAccntNick;
	gvitem.col    = IECOL_ALIAS;
	gvitem.format = grInfoEdit[IECOL_ALIAS].format;
	gvitem.attr   = grInfoEdit[IECOL_ALIAS].attr;
	gvitem.attr &= ~GVAT_SORTSTR;
	m_pGrEdit->SetItem(&gvitem);

	return row;
}

void CMapForm::setMessage(char* format, ...)
{
	CString tmpS;
	va_list valist;
	va_start(valist, format);
	tmpS.FormatV(format, valist);
	va_end(valist);

	m_result.SetWindowText(tmpS);
}


void CMapForm::addGrEdit(int idx/* = -1*/)
{
	_acSave *acSave = getCurAcSave();
	if (!acSave)	return;
	
	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	CString accNum = acSave->sAccntNum;
	if (!checkGrEdit(gpSave, accNum))
		return;

	// 그룹 정보에 추가
	_acSave *newAcSave = new _acSave();
	newAcSave->sAccntNum = accNum;
	newAcSave->sAccntName = GetAcctName(acSave->sAccntName);
	if (m_bCustomer)
		newAcSave->sPassword = Variant(passCC, accNum);
	else
		newAcSave->sPassword = getMapPass(accNum);
	if (idx == -1)	// add
		gpSave->arAcEdit.Add(newAcSave);
	else 
	{
		idx--;
		if (idx < 0)
			idx = 0;
		gpSave->arAcEdit.InsertAt(idx, newAcSave);
	}

	updateGrEdit(gpIdx);
	m_bChange = true;
}

void CMapForm::addGrEditAll()
{
	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	_acSave *acSave = NULL;
	CString accNum;
	for (int row = 1; row < m_pGrAcc->GetRowCount(); row++)
	{
		accNum = m_pGrAcc->GetItemText(row, IACOL_ACC);
		accNum.TrimRight();
		if (accNum.IsEmpty())	continue;
		accNum.Remove('-');

		if (!checkGrEdit(gpSave, accNum))
			continue;

		for (int ii = 0; ii < m_arAcSave.GetSize(); ii++)
		{
			acSave = m_arAcSave.GetAt(ii);
			if (acSave->sAccntNum == accNum)
			{
				// 그룹 정보에 추가
				_acSave *newAcSave = new _acSave();
				newAcSave->sAccntNum = accNum;
				newAcSave->sAccntName = GetAcctName(acSave->sAccntName);
				if (m_bCustomer)
					newAcSave->sPassword = Variant(passCC, accNum);
				else
					newAcSave->sPassword = getMapPass(accNum);
				
				gpSave->arAcEdit.Add(newAcSave);
				break;
			}
		}
	}
	/*
	CString accNum;
	 for (int ii = 0; ii < m_arAcSave.GetSize(); ii++)
	{
		_acSave *acSave = m_arAcSave.GetAt(ii);
		accNum = acSave->sAccntNum;
		if (!checkGrEdit(gpSave, accNum))
			continue;

		// 그룹 정보에 추가
		_acSave *newAcSave = new _acSave();
		newAcSave->sAccntNum = accNum;
		newAcSave->sAccntName = acSave->sAccntName;
		if (m_bCustomer)
			newAcSave->sPassword = Variant(passCC, accNum);
		else
			newAcSave->sPassword = getMapPass(accNum);
		gpSave->arAcEdit.Add(newAcSave);
	}
*/
	updateGrEdit(gpIdx);
}

void CMapForm::addInputAccount()
{
	// 고객은 사용 못함
	//if (m_bCustomer)
	//	return;

	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	CString accNum = m_sInputAccNum;
	if (!checkGrEdit(gpSave, accNum))
		return;

	// 그룹 정보에 추가
	_acSave *newAcSave = new _acSave();
	newAcSave->sAccntNum = accNum;
	newAcSave->sAccntName = GetAcctName(m_sInputAccName);
	if (m_sInputAccPass.IsEmpty())
	{
		CString passS = getMapPass(accNum);
		newAcSave->sPassword = passS;
	}
	else
	{
		newAcSave->sPassword = m_sInputAccPass;
		setMapPass(accNum, m_sInputAccPass);
	}
	gpSave->arAcEdit.Add(newAcSave);
	
	updateGrEdit(gpIdx);

}


void CMapForm::delGrEdit()
{
	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;
	
	const int selGrEdit = getGrEditSelectRow();
	if (selGrEdit < 0)
	{
		setMessage("삭제할 계좌를 선택하십시오.");
		return;
	}

	const int delIdx = selGrEdit-1;
	if (delIdx < 0 || delIdx >= gpSave->arAcEdit.GetSize())
	{
		setMessage("그룹정보 오류입니다.");
		return;
	}
	
	_acSave *acSave = gpSave->arAcEdit.GetAt(delIdx);
	if (acSave)	LIB_DELETE(acSave);
	gpSave->arAcEdit.RemoveAt(delIdx);

	updateGrEdit(gpIdx);
}

void CMapForm::delGrEditAll()
{
	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	for (int ii = 0; ii < gpSave->arAcEdit.GetSize(); ii++)
	{
		_acSave *acSave = gpSave->arAcEdit.GetAt(ii);
		if (acSave)	LIB_DELETE(acSave);
	}
	gpSave->arAcEdit.RemoveAll();
	updateGrEdit(gpIdx);
}



void CMapForm::changeGrEdit(bool bUp, bool bBound /*= false*/)
{
	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	const int selGrEdit = getGrEditSelectRow();
	if (selGrEdit < 0)
	{
		setMessage("이동할 계좌를 선택하십시오.");
		return;
	}

	const int moveIdx = selGrEdit-1;
	if (moveIdx < 0 || moveIdx >= gpSave->arAcEdit.GetSize())
	{
		setMessage("그룹정보 오류입니다.");
		return;
	}

	int next = 0;
	_acSave *acSave = gpSave->arAcEdit.GetAt(moveIdx);
	_acSave *ntSave = NULL, *tmSave = NULL;
	if (bBound)
	{
		if (bUp)
			next = 0;
		else
			next = gpSave->arAcEdit.GetUpperBound();
		if (next == moveIdx)
			return;
	}
	else
	{
		if (bUp)
		{
			next = moveIdx-1;
			if (next < 0)
				return;
		}
		else
		{
			next = moveIdx+1;
			if (next > gpSave->arAcEdit.GetUpperBound())
				return;
		}
	}
	ntSave = gpSave->arAcEdit.GetAt(next);

	tmSave = new _acSave();
	tmSave->Copy(*acSave);

	LIB_DELETE(acSave);
	gpSave->arAcEdit.RemoveAt(moveIdx);
	gpSave->arAcEdit.InsertAt(next, tmSave);
	
	updateGrEdit(gpIdx);
	m_pGrEdit->SetSelectRow(next+1);
}

void CMapForm::sortGpSave()
{
	int gpIdx = 0;
	_gpSave *gpSave = getCurGpSave(gpIdx);
	if (!gpSave)	return;

	CMapStringToOb mapTemp;
	_acSave *acSave = NULL, *tmSave = NULL;
	
	mapTemp.RemoveAll();
	for (int ii = 0; ii < gpSave->arAcEdit.GetSize(); ii++)
	{
		acSave = gpSave->arAcEdit.GetAt(ii);
		tmSave = new _acSave();
		tmSave->Copy(*acSave);
		mapTemp.SetAt(acSave->sAccntNum, (CObject *)tmSave);
	}

	gpSave->_RemoveAr();

	CString key;
	for (int ii = 1; ii <= m_pGrEdit->GetRowCount(); ii++)
	{
		key = m_pGrEdit->GetItemText(ii, IECOL_ACCNUM);
		if (key.GetLength() > LEN_ACCT)
			key = DecodeAcctNo(key);

		if (!mapTemp.Lookup(key, (CObject *&)tmSave))
			continue;
		acSave = new _acSave();
		acSave->sPriAcc= tmSave->sPriAcc;
		acSave->sSortNum= tmSave->sSortNum;
		acSave->sAccntNum= tmSave->sAccntNum;
		acSave->sAccntName= GetAcctName(tmSave->sAccntName);
		acSave->sPassword= tmSave->sPassword;
		acSave->sAllocRate= tmSave->sAllocRate;
		acSave->sAllocMulti= tmSave->sAllocMulti;
		acSave->sAccntNick= tmSave->sAccntNick;
		gpSave->arAcEdit.Add(acSave);
	}
	
	for (POSITION pos = mapTemp.GetStartPosition(); pos != NULL; )
	{
		mapTemp.GetNextAssoc(pos, key, (CObject*&)tmSave);
		LIB_DELETE(tmSave);
	}
	mapTemp.RemoveAll();

	updateGrEdit(gpIdx);

}

void CMapForm::enableAllBtn(BOOL bEnable)
{
	const UINT idc[] = 
	{
		IDC_BTNREFRESH, 
		IDC_BTNGRNEW, IDC_BTNGRCHG, IDC_BTNGRDEL, IDC_BTNGRCOPY,
		IDC_BTNDIRECT, IDC_BTNGRUP, IDC_BTNGRDOWN,
		IDC_BTNADD, IDC_BTNINSERT, IDC_BTNALLADD, 
		IDC_BTNDEL, IDC_BTNALLDEL,
		IDC_BTNUP, IDC_BTNDN, IDC_BTNUPTOP, IDC_BTNDNBOT,
		IDC_BTNINITIAL, IDC_BTNOK,
		IDC_RDHM, IDC_RDFU, 	
	};

	for (int ii = 0; ii < sizeof(idc)/sizeof(UINT); ii++)
	{
		GetDlgItem(idc[ii])->EnableWindow(bEnable);
	}

}

void CMapForm::redrawCtrl()
{
	if (!m_pGrAcc || !m_pGrEdit)
		return;
	
	for (int ii = 0; ii < sizeof(grInfoAcc)/sizeof(_grInfo); ii++)
	{
		m_pGrAcc->SetItemBkColor(0, ii, GetIndexColor(clrGridHead));//clrGridBG));
		
	}
	for (int ii = 0; ii < sizeof(grInfoEdit)/sizeof(_grInfo); ii++)
	{
		m_pGrEdit->SetItemBkColor(0, ii, GetIndexColor(clrGridHead));//clrGridBG));
	}
	
	m_panel->Init(ssGRADIENT, 0, _T(""), _T(""), GetIndexColor(clPanel), GetIndexColor(clPanel), GetIndexColor(clPanel), 0);
	m_panel->SetBorderColor(GetIndexColor(clBack));
	m_clrBG = GetIndexColor(CLRBG);
	Invalidate();
	

}

void CMapForm::domino(CString str)
{
	CString sym, tmpS;
	while (!str.IsEmpty())
	{
		tmpS = Parser(str, "\n");
		sym = Parser(tmpS, "\t");
		
		if (!sym.CompareNoCase("OnGroupSelect"))
		{
			if (tmpS == "1")	// 주식
			{
				selectRadio(IDC_RDHM, true, true);
				((CButton *)GetDlgItem(IDC_RDFU))->SetCheck(FALSE);
				GetDlgItem(IDC_RDFU)->EnableWindow(FALSE);
			}
			else if (tmpS == "2")	// 선물
			{
				selectRadio(IDC_RDFU, true, true);
				((CButton *)GetDlgItem(IDC_RDHM))->SetCheck(FALSE);
				GetDlgItem(IDC_RDHM)->EnableWindow(FALSE);
			}
		}
		
	}
	
}

HTREEITEM CMapForm::insertTreeItem(CString name, int data)
{
	struct _accn accn;
	memset(&accn, 0x00, L_accn);
	if (name.GetLength() > sizeof(accn.acnm))
		name = name.Left(sizeof(accn.acnm));
	HTREEITEM rootItem = m_tree.GetRootItem();
	HTREEITEM item = m_tree.InsertItem(name, imgTGROUP, imgTGRSEL, rootItem);
	//m_tree.SetItemData(item, data);
	return item;
}


bool CMapForm::checkIsGrName(CString newGrName)
{
	_gpSave *gpSave = NULL;
	for (int ii = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		gpSave = m_arGpSave.GetAt(ii);
		if (gpSave->sGrName == newGrName)
			return true;
	}
	return false;	
}

void CMapForm::selectRadio(UINT idc, bool bSend /* = true */, bool bFirst /* = false*/)
{
	const int oldSelType = m_selType;

	switch (idc)
	{
	case IDC_RDHM:
		((CButton *)GetDlgItem(IDC_RDHM))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RDFU))->SetCheck(FALSE);
		m_selType = SEL_HM;
		break;
	case IDC_RDFU:
		((CButton *)GetDlgItem(IDC_RDHM))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RDFU))->SetCheck(TRUE);
		m_selType = SEL_FU;
		break;
	}
	if (bSend)
	{
		if (oldSelType != m_selType)
		{
			if (!bFirst)
			{
				if (m_bChange)
				{
					CString tmpS;
					if (oldSelType == SEL_FU)
						tmpS = "선물/옵션 그룹을 저장하시겠습니까?";
					else
						tmpS = "주식 그룹을 저장하시겠습니까?";
					if (MessageBox(tmpS, "그룹저장", MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
						queryGroupSave(SC_SAVEACCONLY, oldSelType);
				}
			}
			enableAllBtn(FALSE);
			queryGroup();
			m_bFirst = true;
			updateGrEdit(-1);		// 20070525 수정
			//**queryAcc();			// 계좌조회
			LoadAccountListFromWizard();

		}
	}

}


CString CMapForm::GetAcctSymbol(const char* szAcct)
{
	CString acct(szAcct);
	if (acct.GetLength() < LEN_ACCT)
		return CString("");

	const char tab = '\t';

	return acct.Left(acctLeft) + tab + acct.Mid(acctLeft, acctMid) + tab + 
			acct.Mid(acctLeft + acctMid, acctRight) + tab;
}

CString CMapForm::GetAcctName(const char* szName)
{
	CString acctName, acctNick;
	DecodeAcctName(szName, acctName, acctNick);

	if (m_useAlias)
	{
		if (acctNick.IsEmpty())
			return acctName;
		else
			return acctNick;
	}
	else
		return acctName;
}

const char* account = "Account";
const char* useAlias = "UseAlias";

BOOL CMapForm::LoadConfig_UseAlias()
{
	CProfile profile(GetUserConfigFileName());
	
	return profile.GetInt(account, useAlias);
}

CString CMapForm::GetUserConfigFileName()
{
	CString home(Variant(homeCC, "")), user(Variant(nameCC, ""));
	
	NormalizeFolderName(home);

	home += "user\\";
	home += user;

	return home + "\\" + user + ".ini";
}

CString CMapForm::GetAcctNameFromAllAccount(const char* acct, const char* name)
{
	CString str(acct);
	str.Replace("-", "");

	for (int i = 0; i < m_arAcSave.GetSize(); i++)
	{
		if (m_arAcSave[i]->sAccntNum == str)
		{
			if (m_useAlias && !m_arAcSave[i]->sAccntNick.IsEmpty())
				return m_arAcSave[i]->sAccntNick;
			else
				return m_arAcSave[i]->sAccntName;
		}
	}
	return CString(name);
}

void CMapForm::AddAcctSave(const char* no, const char* nm)
{
	CString name, alias;
	DecodeAcctName(nm, name, alias);

	_acSave* p = new _acSave;
	p->sAccntName = name;
	p->sAccntNum = no;
	p->sAccntNick = alias;
	m_arAcSave.Add(p);
}

void CMapForm::LoadAccountListFromWizard()
{
	CString str = Variant(accountCC, "1");
	CString buf, account, name, password, alias, mkgb;
	const int row = 1;

	deleteAllAcSave();
	while (TRUE)
	{
		buf = Parse(str, "\n");
		if (buf.IsEmpty())
			break;

		account = Parse(buf);
		if (account.IsEmpty())
			break;

		password = Parse(buf);
		DecodeAcctName(Parse(buf), name, alias);

		switch (m_selType)
		{
		case SEL_FU:
			if (!IsFOptionAcc(account))
				continue;
			break;
		default:
			if (!IsStockAcc(account))
				continue;
			break;
		}

		if (name.IsEmpty() || account.IsEmpty())
			continue;

		mkgb = "";
		if (account.GetLength()>=5)
			mkgb = account.Mid(3, 2);
		if (mkgb=="25")	// 해외선물 계좌
			continue;
		if (mkgb=="27") // FX마진 계좌
			continue;


		AddAcctSave(account, name);
	}
	updateGrAcc();			// 계좌 grid update
	m_pGrAcc->Sort(1, true);	// 계좌명 sort
	m_bFirst = true;
	queryGroup();			// 그룹조회
}

HBITMAP CMapForm::getBitmap2(CString bmps)
{
	CString	path;

	path.Format("%s\\%s\\%s", m_sRootDir, IMAGEDIR, bmps);
	
	return ((CBitmap *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}
