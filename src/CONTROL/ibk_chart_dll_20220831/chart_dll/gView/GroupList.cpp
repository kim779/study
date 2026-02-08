// GroupList.cpp : implementation file
//

#include "stdafx.h"
#include "axisGView.h"
#include "GroupList.h"
#include "../../h/jmcode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCodeTreeCtrl::CCodeTreeCtrl(CWnd *pView, CString rootPath, CString user)
{
	m_hCallWnd = NULL;
	m_pView = pView;
	m_rootPath = rootPath;
	m_userName = user;
	m_itemSELECT = NULL;
	resetArray(m_arHJCode);
	resetArray(m_arUPCode);
	resetArray(m_arBUFFER);
}

CCodeTreeCtrl::~CCodeTreeCtrl()
{
	resetArray(m_arHJCode);
	resetArray(m_arUPCode);
	resetArray(m_arBUFFER);
}

BEGIN_MESSAGE_MAP(CCodeTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CCodeTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HTREEITEM CCodeTreeCtrl::AddItem(HTREEITEM hParent, CString input, DWORD nData, HTREEITEM hAt)
{
	TV_INSERTSTRUCT		TreeCtrlItem;
	HTREEITEM		hItem = NULL;

	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.iImage = 1; 	//이미지index
	TreeCtrlItem.item.iSelectedImage = 1;
	TreeCtrlItem.hInsertAfter = hAt;	

	TreeCtrlItem.hParent = hParent;
	TreeCtrlItem.item.pszText = (LPSTR)(LPCTSTR)input;
	TreeCtrlItem.item.lParam = nData;
	TreeCtrlItem.item.cchTextMax = input.GetLength();
	hItem = this->InsertItem(&TreeCtrlItem);	
	return hItem;
}

bool CCodeTreeCtrl::loadHJCode()
{
	if (m_arHJCode.GetSize() > 0)
		return true;
	
	int	nFSize = 0, nSize = sizeof(struct hjcode), res = 0;

	CFile	file;
	struct  hjcode  HJCode;
	struct hjcode*	pObject = NULL;

	CString path;
	
	path.Format("%s\\%s\\%s", m_rootPath, TABDIR, HJCODE);
	
  	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{
		AfxGetMainWnd()->MessageBox("종목 화일을 읽을 수 없습니다.", COMPANYNAME);
		return false;
	}

	nFSize = file.GetLength() / nSize;
	
	for ( int ii = 0 ; ii < nFSize ; ii++ )
	{
		if (file.Read(&HJCode, nSize) < 0)
			continue;

		pObject = new struct hjcode;
		/*pObject->Name = CString(HJCode.hnam, HNameLen);
		res = pObject->Name.Find('\0');

		if (res != -1)
			pObject->Name = pObject->Name.Left(res);

		pObject->Name.MakeUpper();
		pObject->Name.TrimRight();
		pObject->Name = pObject->Name.Mid(1, pObject->Name.GetLength());
		*/
		pObject->size = HJCode.size;
		pObject->ucdm = HJCode.ucdm;
		pObject->ucds = HJCode.ucds;
		pObject->kpgb = HJCode.kpgb;
		pObject->jjug = HJCode.jjug;
		pObject->kosd = (char)HJCode.kosd;		
		pObject->ssgb = (char)HJCode.ssgb;
		pObject->ucmd = HJCode.ucmd;
		pObject->wsgb = HJCode.wsgb;
		pObject->jsiz = HJCode.jsiz;
		pObject->itgb = HJCode.itgb;		
		pObject->star = HJCode.star;

		bool	bContinue = false;
		switch (pObject->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (pObject->ssgb)
			{
			case jmSINJU:	case jmSINJS:	case jmELW:	case jmHYFND:
				bContinue = true;
			default:
				break;
			}

			if (bContinue)
			{
				delete pObject;
				continue;
			}

			/**
			pObject->Code = CString(HJCode.code, HCodeLen);
			pObject->Code = pObject->Code.Mid(1, HCodeLen); 
			pObject->Code.TrimRight();
			**/
			break;
		default:
			delete pObject;
			continue;
		}
		
		m_arHJCode.Add(pObject);
      	}

	file.Close();
	return true;
}

bool CCodeTreeCtrl::loadUPCode()
{
	if (m_arUPCode.GetSize() > 0)
		return false;

	int	nFSize = 0, nSize = sizeof(struct upcode);
	CFile	file;
	struct  upcode  UPCodh;
	struct	upcode*	pObject = NULL;
	CString path;

	path.Format("%s\\%s\\%s", m_rootPath, TABDIR, UPCODE);

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		AfxMessageBox("업종 화일을 읽을 수 없습니다.");
		return FALSE;
	}

	nFSize = file.GetLength() / nSize;

	for (int ii = 0; ii < nFSize; ii++)
	{
		file.Read(&UPCodh, nSize);
		pObject = new struct upcode;
		memcpy(pObject, &UPCodh, nSize);
		m_arUPCode.Add(pObject);
	}

	file.Close();
	return true;
}

void CCodeTreeCtrl::readCode()
{
	loadHJCode();
	loadUPCode();
}

void CCodeTreeCtrl::setTree()
{
	DWORD	dword = 0;
	HTREEITEM	hItem = NULL;
	HTREEITEM	hParent = NULL;

	int	nKind = 0, nSubKind = 0;
	int	ii = 0;

	// History
	nKind = TRCD_HISTORY;
	dword = makeItemData(nKind, 0, 0);
	hItem = AddItem(TVI_ROOT, _T("history"), dword);
	hParent = GetParentItem(hItem);

	// 전종목
	nKind = TRCD_ALLCODE;
	dword = makeItemData(nKind, 0, 0);
	hItem = AddItem(TVI_ROOT, _T("전종목"), dword);
	hParent = GetParentItem(hItem);	

	{
		HTREEITEM	hParents[2];
		// KOSPI UPJONG
		nKind = TRCD_KOSPIUJ;
		dword = makeItemData(nKind, 0, 0);
		hParents[0] = AddItem(TVI_ROOT, _T("KOSPI"), dword);
	
		// KOSDAQ UPJONG
		nKind = TRCD_KOSDAQUJ;
		dword = makeItemData(nKind, 0, 0);
		hParents[1] = AddItem(TVI_ROOT, _T("KOSDAQ"), dword);		
		
		struct upcode*	pupcode = NULL;
		int	nSize = m_arUPCode.GetSize();
		int	nIndex;
		CString	tmpstr;
		
					
		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			pupcode = (struct upcode*)m_arUPCode.GetAt(ii);
			nIndex = pupcode->ucod;

			if (pupcode->jgub == upKOSPI)
			{
				dword = makeItemData(TRCD_KOSPIUJ, 0, nIndex);
				AddItem(hParents[0], pupcode->hnam, dword);
			}
			else if (pupcode->jgub == upKOSDAQ)
			{
				dword = makeItemData(TRCD_KOSDAQUJ, 0, nIndex);
				AddItem(hParents[1], pupcode->hnam, dword);
			}				
		}		

		// Add KOSPI
		char*	titleKOSPI[] = {"KOSPI200", "KOSPI100", "KOSPI50", "KOSPI IT"};
		char*	titleKOSDAQ[] = {"KOSDAQ50", "KOSDAQ IT", "KOSTAR", "오락문화"};
		int	idxKOSPI[] = {ID_KOSPI200, ID_KOSPI100, ID_KOSPI50, ID_KOSPIIT};
		int	idxKOSDAQ[] = {ID_KOSDAQ50, ID_KOSDAQIT, ID_KOSTAR, ID_ORAKMUNHWA};

		nSize = sizeof(titleKOSPI) / sizeof(char*);

		
		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			dword = makeItemData(TRCD_KOSPIUJ, 0, idxKOSPI[ii]);
			AddItem(hParents[0], titleKOSPI[ii], dword);
		}

		nSize = sizeof(titleKOSDAQ) / sizeof(char*);

		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			dword = makeItemData(TRCD_KOSDAQUJ, 0, idxKOSDAQ[ii]);
			AddItem(hParents[1], titleKOSDAQ[ii], dword);
		}		
	}

	CString	filename;
	
	// 계열사 
	nKind = TRCD_COMPANY;
	dword = makeItemData(nKind, 0, 0);	
	hItem = AddItem(hParent, _T("계열사"), dword);
	filename.Format("%s\\%s\\%s", m_rootPath, TABDIR, GROUP_FILE);	
	setTreeList(hItem, "20", filename);

	// 관심종목그룹 
	nKind = TRCD_INTEREST;
	dword = makeItemData(nKind, 0, 0);	
	hItem = AddItem(hParent, _T("관심종목그룹"), dword);	
	filename.Format("%s\\%s\\%s%s", m_rootPath, TABDIR, m_userName, ".ins");
	setTreeList(hItem, "10", filename);

	// 테마리스트 
	nKind = TRCD_THEME;
	dword = makeItemData(nKind, 0, 0);	
	hItem = AddItem(hParent, _T("테마리스트"), dword);	
	filename.Format("%s\\%s\\%s", m_rootPath, TABDIR, THEME_FILE);	
	setTreeList(hItem, "40", filename);	
}

void CCodeTreeCtrl::setTreeList(HTREEITEM root, CString kind, CString fileName)
{
	char	inBuf[80];
	DWORD	dword = GetItemData(root);
	CString	key, itemData, Name, strTmp;

	BOOL	bKwansim = FALSE;
	if (kind.Compare(_T("10")) == 0)
	{
		CFileFind	finder;
		bKwansim = finder.FindFile(fileName);
		if (!bKwansim)	return;
	}

	if (bKwansim)
	{
		for (int ii = 0 ; ii < 100 ; ii++)
		{
			key.Format("%02d", ii);
			GetPrivateProfileString("FOLDER", key, NULL, inBuf, sizeof(inBuf), fileName);
			strTmp = inBuf;
			int	index = strTmp.Find(';');
			if (index < 0)	continue;

			Name = strTmp.Left(index);
			if (!Name.IsEmpty())
			{
				itemData = CString(key);
				dword = makeItemData(getDataKind(dword), getDataSubKind(dword), atoi(itemData));
				AddItem(root, Name, dword);				
			}
		}
	}
	else
	{
		for (int ii = 0 ; ii < 200 ; ii++)
		{
			key.Format("%02d", ii);
			GetPrivateProfileString ("FOLDER", key, NULL, inBuf, 80, fileName);

			if (inBuf[0])
			{
				strTmp = CString(inBuf);				
				Name = GetStrToken(0, strTmp, ";");
				Name.TrimRight();
				key = GetStrToken(1, strTmp, ";");
				key.TrimRight();				
				itemData = CString(key);				
				dword = makeItemData(getDataKind(dword), getDataSubKind(dword), atoi(itemData));
				AddItem(root, Name, dword);
			}
		}
	}
}

int CCodeTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	readCode();
	setTree();

	SelectItem(NULL);

	return 0;
}


PCODEINFO NewCode(CString Code)
{
	PCODEINFO pc = new CODEINFO;
	/**
	ZeroMemory(pc, sz_Code);
	memcpy(pc->code, Code, Code.GetLength());
	**/
	strcpy(pc->code, Code);

	return pc;
}

// 2006.04.27 schbang	return 값 적용
bool CCodeTreeCtrl::FillBuffer(HTREEITEM hItem)
{
	m_itemSELECT = hItem;
	resetArray(m_arBUFFER);

	if (m_itemSELECT == NULL)
	{
		return false;
	}

	DWORD	dword = GetItemData(hItem);
	int	nKind = getDataKind(dword);
	int	nSubKind = getDataSubKind(dword);
	int	nIndex = getDataIndex(dword);

	int	ii = 0, jj = 0, nSize = 0;
	struct _codeinfo*	pCode = NULL;
	CString	filename;
	bool	bNotSort = false;

	m_selectKIND = nKind;
	switch (nKind)
	{
	case TRCD_HISTORY:
		{
			bNotSort = true;

			CString	symbol = _T("1301");
			CString	history = (char *)m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, historyCC),
						(long)symbol.operator LPCTSTR());

			CString	Str, Code, Name;
			int	index;
			JCODE*	pObject = NULL;
			for (;;)
			{
				index = history.Find("\t");
				if (index <= 0)
					break;

				Str = history.Left(index);
				history = history.Mid(index + 1);

				index = Str.Find(" ");
				if (index > 0)
				{
					Code = Str.Left(index);
					//Name = Str.Mid(index + 1);

					m_arBUFFER.Add(NewCode(Code));
				}
			}
		}
		break;
	case TRCD_ALLCODE:
		{
			JCODE*	pObject = NULL;
			nSize = m_arHJCode.GetSize();

			/**i
			for ( ii = 0 ; ii < nSize ; ii++ )
			{
				pObject = m_arHJCode.GetAt(ii);

				if (pObject->kosd == jmKOSPI || pObject->kosd == jmKOSDAQ || pObject->kosd == jm3RD)
					m_arBUFFER.Add(NewCode(pObject->Code));
			}
			**/
		}
		break;		
	case TRCD_KOSPIUJ:
	case TRCD_KOSDAQUJ:
		{
			JCODE* p;
			CODEFILTERPROC proc = GetCodeFilterProc(nKind, nSubKind, nIndex);
			if (proc) {
				for (int i =0; i < nSize; i++) {
					p = m_arHJCode.GetAt(i);
					if (proc(p, nIndex))
						m_arBUFFER.Add(NewCode(p->Code));
				}
			}
		}
		break;
	case TRCD_COMPANY:
		{	
			bNotSort = true;

			CString	key;
			CString	tmpstr;
			CString	strbuf;
			char	buf[1024];

			filename.Format("%s\\%s\\%s", m_rootPath, TABDIR, GROUP_FILE);
			key.Format("%03d", nIndex);
			GetPrivateProfileString("GRCODE", key, _T(""), buf, sizeof(buf), filename);
			strbuf = buf;

			for ( ii = 0 ; ; ii++ )
			{
				tmpstr = GetStrToken(ii, strbuf, ";");				

				if (tmpstr.IsEmpty())
					break;

				m_arBUFFER.Add(NewCode(tmpstr));
			}
		}
		break;
	case TRCD_INTEREST:
		{
			bNotSort = true;

			char buf[1024];
			CString dir, sName, kName, tmpS, sRtn = "", sFileName;
			CFileFind	finder;

			sName.Format("%02d", nIndex);
			filename.Format("%s\\%s\\%s%s", m_rootPath, TABDIR, m_userName, ".ins");
			BOOL	bFind = finder.FindFile(filename);
			if (!bFind)
			{
				//AfxGetMainWnd()->MessageBox("관심종목 화일을 읽을 수 없습니다.");
				return false;
			}

			CString	jcode, jname;
			for (int ii = 0; ii <= 99; ii++)
			{
				kName.Format("%02d", ii);	
				GetPrivateProfileString(sName, kName, "", buf, sizeof(buf), filename);
				tmpS = buf;

				if (!tmpS.IsEmpty())
				{
					int	index = tmpS.Find(":");
					if (index < 0)	continue;
					jcode = tmpS.Left(index);
					tmpS = tmpS.Mid(index);

					index = tmpS.Find(";");
					if (index < 0)	continue;
					jname = tmpS.Left(index);

					m_arBUFFER.Add(NewCode((jcode.GetAt(0) == 'A' ? jcode.Mid(1): jcode)));
				}
			}
		}
		break;
	case TRCD_THEME:
		{
			bNotSort = true;

			CString	key;
			CString	tmpstr;
			CString	strbuf;
			char	buf[1024];

			filename.Format("%s\\%s\\%s", m_rootPath, TABDIR, THEME_FILE);
			key.Format("%04d", nIndex);
			GetPrivateProfileString("TMCODE", key, _T(""), buf, sizeof(buf), filename);
			strbuf = buf;

			for ( ii = 0 ; ; ii++ )
			{
				tmpstr = GetStrToken(ii, strbuf, ";");				

				if (tmpstr.IsEmpty())
					break;

				m_arBUFFER.Add(NewCode(tmpstr));
			}	
		}
		break;
	}

	if (!bNotSort)
		sortCode();

	return true;
}

int compare(const void * elem1, const void * elem2)
{
	struct _codeinfo*	e1 = ((struct _codeinfo*)elem1);
	struct _codeinfo*	e2 = ((struct _codeinfo*)elem2);

	return strcmp(e1->code, e2->code);
}

void CCodeTreeCtrl::sortCode()
{
	int	nCount = m_arBUFFER.GetSize();

	if (nCount <= 0)
		return;

	struct _codeinfo*	pSort = new _codeinfo[nCount];
	struct _codeinfo*	pCode = NULL;
	int	ii = 0;
	int	nSize = sizeof(struct _codeinfo);
	
	memset(pSort, 0x00, nSize * nCount);

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		pCode = m_arBUFFER.GetAt(ii);
		memcpy(&pSort[ii], pCode, nSize);
	}

	qsort(pSort, nCount, nSize, compare);
	resetArray(m_arBUFFER);

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		pCode = new _codeinfo;
		memset(pCode, 0x00, nSize);
		memcpy(pCode, &pSort[ii], nSize);
		m_arBUFFER.Add(pCode);
	}

	delete [] pSort;
	pSort = NULL;
}

CString CCodeTreeCtrl::GetStrToken(int nIndex, CString data, char* token)
{
	CString retval = _T("");
	char*	dataptr = NULL;
	char*	value = NULL;
	int	nValue = 0;
	int	nLen = 	data.GetLength() + 1;
	int	ii = 0;

	if (data.IsEmpty())
		return "";

	dataptr = (char*)malloc(nLen);
	memset(dataptr, 0x00, nLen);
	memcpy(dataptr, data, nLen - 1);

	retval = value = strtok(dataptr, token);

	if ( ii == nIndex )
	{
		free(dataptr);
		return retval;
	}

	while(value)
	{
		ii++;
		retval = value = strtok(NULL, token);
		
		if (ii == nIndex)
		{
			free(dataptr);
			return retval;
		}
	}

	free(dataptr);
	return "";	
}

void CCodeTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM	hItem = GetSelectedItem();

	if (!ItemHasChildren(hItem))
	{
		if (FillBuffer(hItem))
		{
			CString	slist = _T("");
			int nSize = m_arBUFFER.GetSize();
			if (nSize > 0)
			{
				struct _codeinfo *codeinfo = m_arBUFFER.GetAt(0);
				for (int ii = 0; ii < nSize; ii++)
				{
					codeinfo = m_arBUFFER.GetAt(ii);
					slist += CString(codeinfo->code, strlen(codeinfo->code)) + "\t";
				}
			}
			((CGroupList *)GetParent())->SelectCode(slist, m_selectKIND);
		}


		if (GetToolTips())
			GetToolTips()->ShowWindow(SW_HIDE);

		((CGroupList *)GetParent())->SetShow(false);
		GetParent()->ShowWindow(SW_HIDE);
	}

	*pResult = 0;
}

void CCodeTreeCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CTreeCtrl::OnActivate(nState, pWndOther, bMinimized);
	
/*	if (nState == WA_INACTIVE)
	{
		if (GetParent()->IsWindowVisible())
			GetParent()->ShowWindow(SW_HIDE);
	}
*/
}

DWORD CCodeTreeCtrl::makeItemData(int nKind, int nSubKind, int nIndex)
{
	DWORD	ret = 0;
	WORD	loWord = 0;

	loWord = MAKEWORD(nSubKind, nKind);
	ret = MAKEWPARAM(nIndex, loWord);
	return ret;	
}

WORD CCodeTreeCtrl::getDataInfo(int nWhat, DWORD input)
{
	WORD	ret = 0;
	switch (nWhat)
	{
	case DTIF_KIND:
		ret = HIBYTE(HIWORD(input));
		break;
	case DTIF_SUBKIND:
		ret = LOBYTE(HIWORD(input));
		break;
	case DTIF_INDEX:
		ret = LOWORD(input);
		break;
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CGroupList

CGroupList::CGroupList(CWnd *pView, CWnd* pMain, CFont *pFont, CString rootPath, CString userName)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, GLST_CLASS_NAME, &wndcls)))
	{
		wndcls.style		= CS_SAVEBITS;
		wndcls.lpfnWndProc	= ::DefWindowProc;
		wndcls.cbClsExtra	= wndcls.cbWndExtra = 0;
		wndcls.hInstance	= hInst;
		wndcls.hIcon		= NULL;
		wndcls.hCursor		= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName	= NULL;
		wndcls.lpszClassName	= GLST_CLASS_NAME;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}

	m_pView = pView;
	m_pMain = pMain;
	m_rootPath = rootPath;
	m_userName = userName;
	m_pFont = pFont;

	m_pTree = NULL;
}

CGroupList::~CGroupList()
{
}


BEGIN_MESSAGE_MAP(CGroupList, CWnd)
	//{{AFX_MSG_MAP(CGroupList)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGroupList message handlers

void CGroupList::Create()
{
	DWORD dwStyle = WS_POPUP|WS_THICKFRAME|LBS_NOTIFY; 
	DWORD dwExStyle = WS_EX_TOPMOST;

	CreateEx(dwExStyle, GLST_CLASS_NAME, NULL, dwStyle, 
                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		            NULL, NULL, NULL );

//	Create(NULL, NULL, WS_CHILD|WS_THICKFRAME|WS_VISIBLE|LBS_NOTIFY,
//		CRect(0, 0, 0, 0), this, 0);
}

int CGroupList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_pTree == NULL)
	{
		UINT	dwStyle = TVS_SHOWSELALWAYS|TVS_HASBUTTONS|TVS_HASLINES|TVS_INFOTIP|TVS_LINESATROOT|TVS_FULLROWSELECT|TVS_DISABLEDRAGDROP;
		m_pTree = new CCodeTreeCtrl(m_pView, m_rootPath, m_userName);
		m_pTree->Create(dwStyle, CRect(0,0,0,0), this, 3);
		m_pTree->ModifyStyle(0, WS_CHILD|WS_VISIBLE);
		m_pTree->ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	}

	return 0;
}

void CGroupList::OnDestroy() 
{
	if (m_pTree)
	{
		if (m_pTree->m_hWnd)
			m_pTree->DestroyWindow();
		delete m_pTree;
		m_pTree = NULL;		
	}

	CWnd::OnDestroy();
}

void CGroupList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	if (m_pTree)
	{
		if (bShow)
			m_pTree->ShowWindow(SW_SHOW);
		else
			m_pTree->ShowWindow(SW_HIDE);
	}
}

void CGroupList::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_pTree)
		m_pTree->MoveWindow(0, 0, cx, cy);
}

void CGroupList::SetShow(bool bShow)
{
	if (bShow)
	{
		m_bActivate = true;
		ShowWindow(SW_SHOW);
	}
	else
	{
		m_bActivate = false;
	}
}

void CGroupList::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)
	{
		if (IsWindowVisible())
			ShowWindow(SW_HIDE);
		m_pMain->PostMessage(GEV_VIEW, MAKEWPARAM(viewCode, hideList), 0);
	}
}

void CGroupList::SelectCode(CString sList, int kind)
{
	m_pMain->SendMessage(GEV_VIEW, MAKEWPARAM(viewGroupList, kind), (long)sList.operator LPCTSTR());
}
