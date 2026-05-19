// LayoutEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "LayoutEditDlg.h"
#include "afxdialogex.h"


// CLayoutEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLayoutEditDlg, CDialogEx)

CLayoutEditDlg::CLayoutEditDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CLayoutEditDlg::IDD, pParent)
, m_strLayoutName(_T(""))
{

}

CLayoutEditDlg::~CLayoutEditDlg()
{
}

void CLayoutEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAYOUT_LIST, m_LayoutList);
}


BEGIN_MESSAGE_MAP(CLayoutEditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &CLayoutEditDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CLayoutEditDlg::OnBnClickedBtnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LAYOUT_LIST, &CLayoutEditDlg::OnLvnItemchangedLayoutList)
	ON_BN_CLICKED(IDC_BTN_RENAME, &CLayoutEditDlg::OnBnClickedBtnRename)
	ON_BN_CLICKED(IDOK, &CLayoutEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLayoutEditDlg 메시지 처리기입니다.
BOOL CLayoutEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_LayoutList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_LayoutList.InsertColumn(0, _T("No"), LVCFMT_CENTER, 30);
	m_LayoutList.InsertColumn(1, _T("Layout Name"), LVCFMT_LEFT, 147);

	int idx = 0;
	CString sIndex, sData;
	for (int ii = 0; ii < m_arList.GetCount(); ii++)
	{
		sIndex.Format("%d", ii);
		//		if (!sIndex.CompareNoCase("-1"))
		//			continue;

		m_mapData.Lookup(sIndex, sData);
		m_LayoutList.InsertItem(ii, sIndex);
		m_LayoutList.SetItemText(ii, 1, sData);
	}

	m_mapData.SetAt("-1", "Original");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// Init Layout
void CLayoutEditDlg::SetLayoutList(CMapStringToString* pMap, CStringArray* parLayout)
{
	if (!pMap || !parLayout)
		return;

	m_arList.Copy(*parLayout);

	CString sIndex, sData;
	for (POSITION pos = pMap->GetStartPosition(); pos;)
	{
		pMap->GetNextAssoc(pos, sIndex, sData);
		m_mapData.SetAt(sIndex, sData);
	}
}

void CLayoutEditDlg::OnBnClickedBtnAdd()
{
	if (m_LayoutList.GetItemCount() > 20)
	{
		AfxMessageBox("Maximum Layout count is twenty.");
		return;
	}

	CString sIndex = "0", sData;
	CString stmp;
	int index = 0;

	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowTextA(sData);

	//stmp = sData.MakeUpper();
	if (!sData.CompareNoCase("DEFAULT"))
	{
		AfxMessageBox("Default is Exception Keyword!");
		return;
	}

	for (int ii = 0; ii < m_LayoutList.GetItemCount(); ii++)
	{
		// 중복 체크
		sIndex.Format("%d", ii);
		if (m_mapData.Lookup(sIndex, stmp))
		{
			if (!sData.CompareNoCase(stmp))
			{
				AfxMessageBox("Duplicate Name.");
				return;
			}
		}
	}

	for (int ii = 0; ii < m_LayoutList.GetItemCount(); ii++)
	{
		sIndex.Format("%d", ii);
		if (!m_mapData.Lookup(sIndex, stmp))
			break;
		index++;
	}

	sIndex.Format("%d", index);
	m_LayoutList.InsertItem(m_LayoutList.GetItemCount(), sIndex);
	m_LayoutList.SetItemText(m_LayoutList.GetItemCount() - 1, 1, sData);
	m_mapData.SetAt(sIndex, sData);
}


void CLayoutEditDlg::OnBnClickedBtnDelete()
{
	CString sData, slayout;
	int nSelectedItem = m_LayoutList.GetNextItem(-1, LVNI_SELECTED);

	slayout = m_LayoutList.GetItemText(nSelectedItem, 1);
	if (m_strLayoutName.CompareNoCase(slayout) == 0)
	{
		MessageBox("Current layout not delete", "Message");
		return;
	}

	sData.Format("Do you want to delete the Layout(%s)", m_LayoutList.GetItemText(nSelectedItem, 1));
	if (MessageBox(sData, "Delete File", MB_OKCANCEL) == IDOK)
	{
		sData = m_LayoutList.GetItemText(nSelectedItem, 0);
		m_mapData.RemoveKey(m_LayoutList.GetItemText(nSelectedItem, 0));
		m_LayoutList.DeleteItem(nSelectedItem);
	}
}


void CLayoutEditDlg::OnLvnItemchangedLayoutList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVNI_SELECTED))
	{
		int nSel = pNMLV->iItem;
		if (nSel < 0)
			return;
		CString sData;
		sData = m_LayoutList.GetItemText(nSel, 1);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowTextA(sData);
	}
	*pResult = 0;
}


void CLayoutEditDlg::OnBnClickedBtnRename()
{
	CString sData, sIndex;
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowTextA(sData);
	int nSelectedItem = m_LayoutList.GetNextItem(-1, LVNI_SELECTED);

	sIndex = m_LayoutList.GetItemText(nSelectedItem, 0);
	m_mapData.SetAt(sIndex, sData);
	m_LayoutList.SetItemText(nSelectedItem, 1, sData);
}


void CLayoutEditDlg::OnBnClickedOk()
{
	// 첫 번째
	m_arList.RemoveAll();
	m_arList.Add("-1");
	for (int ii = 0; ii < m_LayoutList.GetItemCount(); ii++)
		m_arList.Add(m_LayoutList.GetItemText(ii, 0));

	CDialogEx::OnOK();
}
