// FieldSet.cpp : implementation file
//

#include "stdafx.h"
//#include "../TrConVol/trconvolview.h"
#include "MainFrmWnd.h"
#include "FieldSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFieldSet dialog


//CFieldSet::CFieldSet(CWnd* pParent /*=NULL*/)
//	: CDialog(CFieldSet::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CFieldSet)
//	m_nColNum = 0;
//	//}}AFX_DATA_INIT
//}

CFieldSet::CFieldSet(CWnd* pParent, StFieldSet *pstMainFieldSet)
	: CDialog(CFieldSet::IDD, pParent)
{
	m_bChange = FALSE;
//	m_stFieldSet = stMainFieldSet;
	STCOPY_P_StFieldSet(pstMainFieldSet, (&m_stFieldSet));

	m_pMainData = pstMainFieldSet;
	m_pWndMainFrm = pParent;

//	memset(&m_stFieldSetPast, 0x00, sizeof(StFieldSet));
//	m_stFieldSetPast = stMainFieldSet;
//	int nSize = sizeof(StFieldSet) + 1;
//	m_lpPast = new char[nSize];	
//	memset(m_lpPast, 0x00, nSize);
//	memcpy(m_lpPast, &m_stFieldSetPast, nSize - 1 );

}


void CFieldSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFieldSet)
	DDX_Control(pDX, IDC_CHK_CONVOLPOWER, m_chkConvolPower);
	DDX_Control(pDX, IDC_EDIT_COLNUM, m_editColNum);
	DDX_Control(pDX, IDC_SPIN1, m_spinCol);
	DDX_Control(pDX, IDC_CHK_ACCUMDEALVOL, m_chkAccumDealVol);
	DDX_Control(pDX, IDC_CHK_CHANGE, m_chkChange);
	DDX_Control(pDX, IDC_CHK_CHGRATE, m_chkChgRate);
	DDX_Control(pDX, IDC_CHK_CODEITEM, m_chkCodeItem);
	DDX_Control(pDX, IDC_CHK_CONVOL, m_chkConVol);
	DDX_Control(pDX, IDC_CHK_CONVOLRATE, m_chkConVolRate);
	DDX_Control(pDX, IDC_CHK_DEALVOLRATE, m_chkDealVolRate);
	DDX_Control(pDX, IDC_CHK_NAMEITEM, m_chkNameItem);
	DDX_Control(pDX, IDC_CHK_PRICENOW, m_chkPriceNow);
	DDX_Control(pDX, IDC_CHK_TIME, m_chkTime);
	DDX_Control(pDX, IDC_COUNT, m_chkCount);
	DDX_Text(pDX, IDC_EDIT_COLNUM, m_nColNum);
	DDV_MinMaxInt(pDX, m_nColNum, 10, 40);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFieldSet, CDialog)
	//{{AFX_MSG_MAP(CFieldSet)
	ON_BN_CLICKED(IDCANCEL, OnBtnCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_COUNT, /*&CFieldSet::*/OnBnClickedCount)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFieldSet message handlers

BOOL CFieldSet::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetCheckField();	
	m_spinCol.SetRange(10, 40);

	int nSpinPos = 0;
	char *pBuff;
	INT_CONVERT(pBuff, m_stFieldSet.chColNum, sizeof(m_stFieldSet.chColNum), nSpinPos)

	if(nSpinPos < 10 || nSpinPos >40)
		nSpinPos = 24;
	m_spinCol.SetPos(nSpinPos);

	SetInit();
	
	return TRUE;  
}

void CFieldSet::SetInit()
{
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pWndMainFrm;
	m_strFieldPath.Format("%s%s\\%s\\%s", pMainFrmWnd->Variant(homeCC), USRDIR, pMainFrmWnd->Variant(nameCC), CONVOL_FILE);
	
//	GetLoad_FieldInfo(m_strFieldPath, &m_stFieldSetPast);
	
}

void CFieldSet::SetCheckField()
{
	char *pBuff;
	int nData;
	INT_CONVERT(pBuff,				m_stFieldSet.chkTime,		1, nData)
	m_chkTime.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkCodeItem,	1, nData)
	m_chkCodeItem.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkNameItem,	1, nData)
	m_chkNameItem.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkPriceNow,	1, nData)
	m_chkPriceNow.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkChange,		1, nData)
	m_chkChange.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkChgRate,	1, nData)
	m_chkChgRate.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkConVol,		1, nData)
	m_chkConVol.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkAccumDealVol, 1, nData)
	m_chkAccumDealVol.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkDealVolRate, 1, nData)
	m_chkDealVolRate.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkConVolRate, 1, nData)
	m_chkConVolRate.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkConVolPower, 1, nData)
	m_chkConvolPower.SetCheck(nData);
	INT_CONVERT(pBuff,				m_stFieldSet.chkCount, 1, nData)
	m_chkCount.SetCheck(nData);
}

void CFieldSet::GetCheckField()
{
//	memset(&m_stFieldSet, 0x00, sizeof(StFieldSet)+1);
	m_stFieldSet.chkTime[0]		= '0' + m_chkTime.GetCheck();
	m_stFieldSet.chkTime[0]		= '0' + m_chkTime.GetCheck();
	m_stFieldSet.chkCodeItem[0] = '0' + m_chkCodeItem.GetCheck();
	m_stFieldSet.chkNameItem[0] = '0' + m_chkNameItem.GetCheck();
	m_stFieldSet.chkPriceNow[0] = '0' + m_chkPriceNow.GetCheck();
	m_stFieldSet.chkChange[0]	= '0' + m_chkChange.GetCheck();
	m_stFieldSet.chkChgRate[0]	= '0' + m_chkChgRate.GetCheck();
	m_stFieldSet.chkConVol[0]	= '0' + m_chkConVol.GetCheck();
	m_stFieldSet.chkAccumDealVol[0] = '0' + m_chkAccumDealVol.GetCheck();
	m_stFieldSet.chkDealVolRate[0]	= '0' + m_chkDealVolRate.GetCheck();
	m_stFieldSet.chkConVolRate[0]	= '0' + m_chkConVolRate.GetCheck();
	m_stFieldSet.chkConVolPower[0]  = '0' + m_chkConvolPower.GetCheck();
	m_stFieldSet.chkCount[0]	= '0' + m_chkCount.GetCheck();

	CString strColNum;
	m_editColNum.GetWindowText(strColNum);
	int nColNum = atoi(strColNum);
	strColNum.Format("%03d", nColNum);	
	memcpy(&m_stFieldSet.chColNum,		strColNum, strColNum.GetLength());
}

BOOL CFieldSet::CheckChange()
{
	LPSTR szSrc = &m_stFieldSet.chkTime[0];
	LPSTR szTar = &m_pMainData->chkTime[0];

	m_bChange = FALSE;
	if(strncmp(szSrc, szTar, sizeof(StFieldSet)-1)==0) return FALSE;

	m_bChange = TRUE;
	return TRUE;
//	char* lpNow;
//
//	int nSize = sizeof(StFieldSet) + 1;
//
//	lpNow  = new char[nSize];
//	memcpy(lpNow,  (void*)&m_stFieldSet,     nSize - 1);
//
//	for(int nIndex = 0; nIndex < nSize - 1; nIndex++)
//	{	
//		if(lpNow[nIndex] != m_lpPast[nIndex])
//		{
//			m_bChange = TRUE;
//			break;
//		}
//	}
//	delete [] lpNow;
}

void CFieldSet::OnDestroy() 
{
	CDialog::OnDestroy();

//	if(m_lpPast)
//	{
//		delete []m_lpPast;
//		m_lpPast = NULL;
//	}	
}


BOOL CFieldSet::GetLoad_FieldInfo(CString strFullPath, StFieldSet	*pstFieldSet)
{
	BOOL bLoad = TRUE;
	CString szKey, szValue;
	int nData;
//	char* pBuff;	
	TRY
	{
	
		nData = ::GetPrivateProfileInt("FieldSet", "chkTime", 1, strFullPath);
		_itoa(nData, pstFieldSet->chkTime, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkCodeItem", 0, strFullPath);
		_itoa(nData, pstFieldSet->chkCodeItem, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkNameItem", 1, strFullPath);
		_itoa(nData, pstFieldSet->chkNameItem, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkPriceNow", 1, strFullPath);
		_itoa(nData, pstFieldSet->chkPriceNow, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkChange", 1, strFullPath);
		_itoa(nData, pstFieldSet->chkChange, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkChgRate", 1, strFullPath);
		_itoa(nData, pstFieldSet->chkChgRate, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkConVol", 1, strFullPath);
		_itoa(nData, pstFieldSet->chkConVol, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkAccumDealVol", 1, strFullPath);
		_itoa(nData, pstFieldSet->chkAccumDealVol, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkDealVolRate", 0, strFullPath);
		_itoa(nData, pstFieldSet->chkDealVolRate, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkConVolRate", 0, strFullPath);
		_itoa(nData, pstFieldSet->chkConVolRate, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkConVolPower", 1, strFullPath);
		_itoa(nData, pstFieldSet->chkConVolPower, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chkCount", 0, strFullPath);
		_itoa(nData, pstFieldSet->chkCount, 10);

		nData = ::GetPrivateProfileInt("FieldSet", "chColNum", 24, strFullPath);
		_itoa(nData, pstFieldSet->chColNum, 10);
	}
	CATCH(CFileException, pEx)
	{
		bLoad = FALSE;
	}
	END_CATCH

/*
	CFile clsFile;
	if (clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeRead))
	{
		LPSTR lpBuff = NULL;
		DWORD nLen = clsFile.GetLength();
		if(nLen == 0) return FALSE;
		
		lpBuff = new char[nLen];
		memset(lpBuff, 0, nLen);
		clsFile.Read(lpBuff, nLen);
		clsFile.Close();
		
		CMemFile memFile = CMemFile(nLen);
		memFile.Attach((unsigned char*)lpBuff, nLen);
		
		CArchive ar(&memFile, CArchive::load);
	
		int nData;
		
		ar >> nData;
		itoa(nData, pstFieldSet->chkTime, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chkCodeItem, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chkNameItem, 10);
		
		ar >> nData;
		itoa(nData, pstFieldSet->chkPriceNow, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chkChange, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chkChgRate, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chkConVol, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chkAccumDealVol, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chkDealVolRate, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chkConVolRate, 10);

		ar >> nData;
		itoa(nData, pstFieldSet->chColNum, 10);

		ar.Close();
		if (lpBuff) 
			delete lpBuff;
		return TRUE;
	}
*/	
	return bLoad;
}

BOOL CFieldSet::SetSave_FieldInfo(CString strFullPath, StFieldSet	*pstFieldSet)
{
	CString szKey, szValue;
	int nData;
	char* pBuff;
	
	BOOL bSave = TRUE;	

	TRY
	{
	
		INT_CONVERT(pBuff, pstFieldSet->chkTime, sizeof(pstFieldSet->chkTime), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkTime", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkCodeItem, sizeof(pstFieldSet->chkCodeItem), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkCodeItem", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkNameItem, sizeof(pstFieldSet->chkNameItem), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkNameItem", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkPriceNow, sizeof(pstFieldSet->chkPriceNow), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkPriceNow", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkChange, sizeof(pstFieldSet->chkChange), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkChange", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkChgRate, sizeof(pstFieldSet->chkChgRate), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkChgRate", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkConVol, sizeof(pstFieldSet->chkConVol), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkConVol", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkAccumDealVol, sizeof(pstFieldSet->chkAccumDealVol), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkAccumDealVol", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkDealVolRate, sizeof(pstFieldSet->chkDealVolRate), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkDealVolRate", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkConVolRate, sizeof(pstFieldSet->chkConVolRate), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkConVolRate", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkConVolPower, sizeof(pstFieldSet->chkConVolPower), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkConVolPower", szValue, strFullPath);

		INT_CONVERT(pBuff, pstFieldSet->chkCount, sizeof(pstFieldSet->chkCount), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chkCount", szValue, strFullPath);
		
		INT_CONVERT(pBuff, pstFieldSet->chColNum, sizeof(pstFieldSet->chColNum), nData);
		szValue.Format("%d", nData);
		::WritePrivateProfileString("FieldSet", "chColNum", szValue, strFullPath);
	}
	CATCH(CFileException, pEx)
	{
		bSave = FALSE;
	}
	END_CATCH
 
/*	CFile clsFile;
	if(!clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite)) 
		return FALSE;
	CArchive ar(&clsFile, CArchive::store);

	int nData;
	char* pBuff;
	INT_CONVERT(pBuff, pstFieldSet->chkTime, sizeof(pstFieldSet->chkTime), nData);
	ar << nData;

	INT_CONVERT(pBuff, pstFieldSet->chkCodeItem, sizeof(pstFieldSet->chkCodeItem), nData);
	ar << nData;

	INT_CONVERT(pBuff, pstFieldSet->chkNameItem, sizeof(pstFieldSet->chkNameItem), nData);
	ar << nData;

	INT_CONVERT(pBuff, pstFieldSet->chkPriceNow, sizeof(pstFieldSet->chkPriceNow), nData);
	ar << nData;

	INT_CONVERT(pBuff, pstFieldSet->chkChange, sizeof(pstFieldSet->chkChange), nData);
	ar << nData;
	INT_CONVERT(pBuff, pstFieldSet->chkChgRate, sizeof(pstFieldSet->chkChgRate), nData);
	ar << nData;
	INT_CONVERT(pBuff, pstFieldSet->chkConVol, sizeof(pstFieldSet->chkConVol), nData);
	ar << nData;
	INT_CONVERT(pBuff, pstFieldSet->chkAccumDealVol, sizeof(pstFieldSet->chkAccumDealVol), nData);
	ar << nData;
	INT_CONVERT(pBuff, pstFieldSet->chkDealVolRate, sizeof(pstFieldSet->chkDealVolRate), nData);
	ar << nData;
	INT_CONVERT(pBuff, pstFieldSet->chkConVolRate, sizeof(pstFieldSet->chkConVolRate), nData);
	ar << nData; 
	INT_CONVERT(pBuff, pstFieldSet->chColNum, sizeof(pstFieldSet->chColNum), nData);
	ar << nData;

	ar.Close();
	clsFile.Close();
*/
	return bSave;

}


void CFieldSet::OnOk() 
{
	GetCheckField();
	if(!CheckChange())
	{
		CDialog::OnCancel();		
	}
	STCOPY_P_StFieldSet((&m_stFieldSet), m_pMainData);

	CDialog::OnOK();

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pWndMainFrm;
	if(m_bChange)
		pMainFrmWnd->OnFieldSetBtnOK(&m_stFieldSet);
}

void CFieldSet::OnBnClickedCount()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFieldSet::OnBtnCancel()
{
	OnCancel();
}

void CFieldSet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pWndMainFrm;
	
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, pMainFrmWnd->GetAxColor(66));
}

HBRUSH CFieldSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pWndMainFrm;

		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(pMainFrmWnd->m_clrMainSkin);
		hbr = pMainFrmWnd->GetAxBrush(pMainFrmWnd->GetAxColor(66));
	}

	return hbr;
}

void CFieldSet::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	OnSize();
}

void CFieldSet::OnSize()
{
	CRect reClient, reLeft;
	GetClientRect(reClient);

	reLeft = reClient;
	
	reLeft.top = reClient.bottom - 25;
	reLeft.bottom = reLeft.top + 20;

	reLeft.right = reClient.right - 5;
	reLeft.left = reLeft.right - 60;
	m_btnCancel.MoveWindow(&reLeft);

	reLeft.right = reLeft.left - 2;
	reLeft.left = reLeft.right - 60;
	m_btnOk.MoveWindow(&reLeft);

	Invalidate();
}

int CFieldSet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pWndMainFrm;
	
	m_btnOk.Create(_T("확인"), CRect(0, 0, 0, 0), this, IDOK);
	pMainFrmWnd->SetBtnImg(&m_btnOk, 4, FALSE);
	
	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDCANCEL);
	pMainFrmWnd->SetBtnImg(&m_btnCancel, 4, FALSE);
	
	return 0;
}
