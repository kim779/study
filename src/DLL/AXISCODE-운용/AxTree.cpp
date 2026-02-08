// AxTree.cpp: 구현 파일
//

#include "stdafx.h"
#include "AxTree.h"
#include <AxStd.hpp>

// AxTree
IMPLEMENT_DYNAMIC(AxTree, CTreeCtrl)
/************************************************************
	AxNode Class
************************************************************/

gsl::renter<AxTree*> AxNode::_pTree;

AxNode::AxNode(HTREEITEM item)
{
	if (IS_NOT_NULL(item))
		getNode(item);
}

AxNode::AxNode(const AxNode& rhs)
{
	_htree = rhs._htree;
	_sNode = rhs._sNode;
	_sData = rhs._sData;
	_iImg = rhs._iImg;
	_sImg = rhs._sImg;
}

AxNode::AxNode(const AxNode&& rhs) noexcept
{
	_htree = std::move(rhs._htree);
	_sNode = std::move(rhs._sNode);
	_sData = std::move(rhs._sData);
	_iImg  = std::move(rhs._iImg);
	_sImg  = std::move(rhs._sImg);
}

AxNode::AxNode(const CString sNode, bool bCreate, HTREEITEM hitem)
{
	this->_htree = hitem;
	*this =	CreateNode(sNode, bCreate);
}

void AxNode::operator=(const AxNode&& rhs)
{
	_htree = std::move(rhs._htree);
	_sNode = std::move(rhs._sNode);
	_sData = std::move(rhs._sData);
	_iImg  = std::move(rhs._iImg);
	_sImg  = std::move(rhs._sImg);
}

bool AxNode::getNode(HTREEITEM item)
{
	TVITEM tm{};
	tm.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_IMAGE | TVIF_PARAM;
	tm.hItem = item;
	tm.pszText = _sNode.GetBuffer(1024);
	tm.cchTextMax = 1024;
	_pTree->GetItem(&tm);
	_sNode.ReleaseBuffer();
	
	_iImg = tm.iImage;
	_sImg = tm.iSelectedImage;	
	_htree = item;
	if (auto& imap = _pTree->_dataMap.find(item); imap != _pTree->_dataMap.end())
	{
		_sData = imap->second;
		return true;
	}
	return false;
}

AxNode AxNode::CreateNode(CString node, bool bCreate)
{
	HTREEITEM subItem{};
	CString sKey = node + "/" + _sNode;
	if (_sNode.IsEmpty())
		sKey = node;
	
	auto& item = _pTree->_treemap.find(sKey);
	if (item == _pTree->_treemap.end() && bCreate)
	{
		subItem = _pTree->InsertItem(node, _iImg, _sImg, getItem());
		_pTree->_treemap.emplace(sKey, subItem);
	}
	else 
	{
		subItem = item->second;	
	}
	return std::move(AxNode(subItem));
}


AxNode& AxNode::operator=(int value)
{
ASSERT_VALID(_pTree);
	_sData.Format(_T("%d"), value);
	auto& item =_pTree->_dataMap.emplace(_htree, _sData);
	if (item.second == false)
		item.first->second = _sData;
	return *this;
}

AxNode& AxNode::operator=(CString sValue)
{
ASSERT_VALID(_pTree);
	_sData = sValue;
	auto& item = _pTree->_dataMap.emplace(_htree, _sData);
	if (item.second == false)
		item.first->second = _sData;
	return *this;
}

AxNode& AxNode::operator<<(CString sValue)
{
ASSERT_VALID(_pTree);
	_sData = sValue;
	auto& item = _pTree->_dataMap.emplace(_htree, _sData);
	if (item.second == false)
		item.first->second = _sData;
	return *this;
}

AxNode& AxNode::operator<<(int value)
{
ASSERT_VALID(_pTree);
	_sData.Format(_T("%d"), value);
	auto& item =_pTree->_dataMap.emplace(_htree, _sData);
	if (item.second == false)
		item.first->second = _sData;
	return *this;
}

void AxNode::setData(CString sData)
{
ASSERT_VALID(_pTree);
	_sData = sData;
	if (_htree)
	{
		auto item = _pTree->_dataMap.emplace(_htree, sData);	
		if (item.second == false)
			item.first->second = sData;
	}
		
}

void AxNode::setData(int val)
{
	CString s;
	s.Format(_T("%d"), val);
	setData(s);
}


bool AxNode::Expand(UINT bflag)
{
ASSERT_VALID(_pTree);
	return _pTree->Expand(_htree, bflag ? TVE_EXPAND : TVE_COLLAPSE);
}

AxNode& AxNode::Bold(bool bflag)
{
ASSERT_VALID(_pTree);
	_pTree->SetItemState(_htree, bflag ? TVIS_BOLD : ~TVIS_BOLD, TVIS_BOLD);
	return *this;
}

HTREEITEM AxNode::getParent()
{
ASSERT_VALID(_pTree);
	return _pTree->GetParentItem(_htree);
}

bool AxNode::Delete()
{
ASSERT_VALID(_pTree);
	_pTree->removeKey(this->_sNode);
	return _pTree->DeleteItem(_htree);
}

bool AxNode::ReName(CString name)
{
ASSERT_VALID(_pTree);
	return _pTree->SetItemText(_htree, name);
}

bool AxNode::Select()
{
ASSERT_VALID(_pTree);
	return _pTree->Select(_htree, TVGN_CARET);
}

BOOL AxNode::SelectItem()
{
ASSERT_VALID(_pTree);
	return _pTree->SelectItem(_htree);
	//return _pTree->Select(_htree, TVGN_CARET);
}

/************************************************************
	AxTree Class
************************************************************/
AxTree::AxTree() noexcept
{
	_imType   = imgNONE;
	_dragImg  = nullptr;
}

// AxTree 메시지 처리기
BEGIN_MESSAGE_MAP(AxTree, CTreeCtrl)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &AxTree::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//bool AxTree::SetAt(CString sKey, HTREEITEM htree)
//{
//	auto& item = _treemap.emplace(sKey, htree);	
//	return item.second;
//}
//
//bool AxTree::Lookup(CString sKey, HTREEITEM& htree)
//{
//	auto it = _treemap.find(sKey);
//	if (it != _treemap.end())
//	{
//		htree = it->second;
//		return true;
//	}
//	return false;
//}

BOOL AxTree::clear()
{
	_dataMap.clear();
	_treemap.clear();
	return DeleteAllItems();
}


void AxTree::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
/*
	CMenu menu{}, subMenu{};
	int   cmd = 0;
	//TODO("2019/03 [KJS] Object Event function 추가시 4. 메뉴테이블에 이벤트 추가");
	enum {
		tNONE, tONLOAD, tONSEND, tONRECV, tONCLOSE,
		tONCHANGELAYOUT, tONFLICKING, tONREFRESH, tONCLICK, tONLONGTAP,
		tONCHANGE, tONCHAR, tONSCROLL, tONTAPDOWN, tONTAPUP,
		tFOCUS, tDELETE, tCOPY, tHIGHLIGHT, tCOMMENT,
		tADDLAYOUT, tRENAME
	};

	HTREEITEM item = GetSelectedItem();
	if (item == nullptr)
		return;

	menu.CreatePopupMenu();
	subMenu.CreatePopupMenu();
	GetCursorPos(&point);
	MAP _mapH = _formItem->getmap();

	AxJson  load(FORMAT(L"%s.json", _mapH.lock()->json));
	AxNode  node(this, item);
	AxNode  parentnode(this, node.getParent());

	CString sObj{}, sfun{}, sScript{}, skey{};
	CString sData = node.getData();
	CString sType{};
	int data = -2;	
	UINT uflag = MF_STRING;
	if (!sData.IsEmpty())
	{
		sType = Parser(sData, L"\t");
		data = _wtoi(sData);
		sObj = node.getText();
		switch (sType[0])
		{
		case 'O':
			_imType = gsl::narrow<imgTYPE>((data >= 0) ? _formItem->GetForm(data)->kind : imgFORM);
			switch (_imType)
			{
			case FM_OBJECT:
			{
				for (int ii = 1; ii < 8; ii++)
				{
					sScript.Empty();
					sfun = gsl::at(evFormfun, ii);
					sfun.MakeLower();
					skey = FORMAT(L"_am_%s_am_", sfun);
					if (load[sObj].Exists(skey))
						sScript = load[sObj][skey].Data;
					uflag = MF_STRING;
					if (!sScript.IsEmpty())
						uflag |= MF_CHECKED;
					menu.AppendMenu(uflag, ii, gsl::at(evFormfun, ii));
				}
			}
			break;
			case FM_IMAGEVIEW:
			case FM_LABEL:
			case FM_BUTTON:
			case FM_RADIO:
			case FM_CHECK:
			case FM_OUT:
			case FM_AXPAGER:
			case FM_TAB:
				for (int ii = 0; ii < 2; ii++)
				{
					sScript.Empty();
					sfun = gsl::at(evObjfun, ii);
					sfun.MakeLower();
					skey = FORMAT(L"_am_%s_am_", sfun);
					if (load[sObj].Exists(skey))
						sScript = load[sObj][skey].Data;

					uflag = MF_STRING;
					if (!sScript.IsEmpty())
						uflag |= MF_CHECKED;
					menu.AppendMenu(uflag, ii + 8, gsl::at(evObjfun, ii));
				}
				break;
			case FM_COMBO:
			case FM_TABLE:
			case FM_AXDATE:
			case FM_AXDOUBLEDATE:
			case FM_AXTIMER:
			case FM_CONTROL:
				for (int ii = 0; ii < 3; ii++)
				{
					sScript.Empty();
					sfun = gsl::at(evObjfun, ii);
					sfun.MakeLower();
					skey = FORMAT(L"_am_%s_am_", sfun);
					if (load[sObj].Exists(skey))
						sScript = load[sObj][skey].Data;
					uflag = MF_STRING;
					if (!sScript.IsEmpty())
						uflag |= MF_CHECKED;
					menu.AppendMenu(uflag, ii + 8, gsl::at(evObjfun,ii));
				}
				break;
			case FM_GRID:
			case FM_GRIDEX:
			case FM_AXGRID:
				for (int ii = 0; ii < 6; ii++)
				{
					sScript.Empty();
					sfun = gsl::at(evObjfun, ii);
					sfun.MakeLower();
					skey = FORMAT(L"_am_%s_am_", sfun);
					if (load[sObj].Exists(skey))
						sScript = load[sObj][skey].Data;
					uflag = MF_STRING;
					if (!sScript.IsEmpty())
						uflag |= MF_CHECKED;
					menu.AppendMenu(uflag, ii + 8, gsl::at(evObjfun, ii));
				}
				break;
			case FM_EDIT:
				for (int ii = 0; ii < 5; ii++)
				{
					sScript.Empty();
					sfun = gsl::at(evEditfun,ii);
					sfun.MakeLower();
					skey = FORMAT(L"_am_%s_am_", sfun);
					if (load[sObj].Exists(skey))
						sScript = load[sObj][skey].Data;
						
					uflag = MF_STRING;
					if (!sScript.IsEmpty())
						uflag |= MF_CHECKED;

					switch (ii)
					{
					case 0:
						cmd = tONCHANGE;
						break;
					case 1:
						cmd = tONCHAR;
						break;
					case 2:
						cmd = tONTAPDOWN;
						break;
					case 3:
						cmd = tONTAPUP;
						break;
					case 4:
						cmd = tFOCUS;
						break;
					default:
						cmd = 0;
						break;
					}
					menu.AppendMenu(uflag, cmd, gsl::at(evEditfun, ii));
				}
				break;
			default:
				break;

			}
			menu.AppendMenu(MF_SEPARATOR, 0, L"");
			menu.AppendMenu(MF_STRING, tDELETE, _T("Delete Object"));
			menu.AppendMenu(MF_STRING, tCOPY, _T("Copy Object"));
			menu.AppendMenu(MF_STRING, tHIGHLIGHT, _T("HighLight Object"));
			break;
		case 'E':
			menu.AppendMenu(MF_STRING, tDELETE, _T("Delete Function"));
			menu.AppendMenu(MF_STRING, tCOMMENT, _T("Comment Function"));

			break;
		case 'F':
			menu.AppendMenu(MF_STRING, tADDLAYOUT, _T("Add Layout"));
			break;
		case 'L':
			menu.AppendMenu(MF_STRING, tRENAME, _T("ReName Layout"));
			menu.AppendMenu(MF_STRING, tDELETE, _T("Delete Layout"));
			break;
		default:
			break;
		}
	}

	cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CChildFrame* pActiveChild = dynamic_cast<CChildFrame*>(pMain->MDIGetActive());
	if (pActiveChild == nullptr)
		return;
	// run menu Command 
	// Add Object Event
	if ( IS_WITHIN(tONLOAD, tFOCUS, cmd) )
	{
//TODO("2019/03 [KJS] Object Event function 추가시 5. 메뉴와 함수배열 인덱스 처리");
		switch (sType[0])
		{
		case 'O':
		{
			switch (_imType)
			{
			case FM_OBJECT:
				skey = gsl::at(evFormfun, cmd);
				sfun = skey;
				break;
			case FM_EDIT:
				if (cmd > 12)
					cmd -= 11;
				else
					cmd -= 10;

				skey = gsl::at(evEditfun, cmd);
				sfun.Format(L"%s_%s", sObj, skey);
				break;
			default:
				skey = gsl::at(evObjfun , cmd - 8);
				sfun.Format(L"%s_%s", sObj, skey);
				break;
			}
			skey.MakeLower();
			sScript = load[sObj][FORMAT(L"_am_%s_am_", skey)].Data;
			sScript.Trim();
			if (!sScript.IsEmpty())
				return;


			skey.MakeLower();
			CString ss = FORMAT(L"_am_%s_am_", skey);
			if (load[sObj].Exists(ss))
				sScript = load[sObj][ss].Data;

			if (sScript.IsEmpty())
			{
				node[sfun].Bold() = FORMAT(L"E\t%d", data);
				load[sObj][FORMAT(L"_am_%s_am_", skey)] = FORMAT(L"-- Add %s Script\n", skey);
				load.saveJason();
				pMain->makeOjbectEvent(sfun);
				MainSendMsg(ID_USR_SELCTRLTOCHILD, MAKELPARAM(_wtoi(sData), SC_SCRIPTBAR));
			}
		}
		break;
		default:
			break;
		}
	}
	else
	{
		auto& layout = _formItem->getLayout();
		switch (sType[0])
		{
		case 'O':
			if (data < 0)
				return;
				
			if (cmd == tDELETE)
			{
				pActiveChild->deleteForm(data);
			}
			else if (cmd == tCOPY)
			{
				thread copy( bind(&CChildFrame::Copy, pActiveChild, data) );
				copy.join();
			}
			else if (cmd == tHIGHLIGHT)
			{
				pActiveChild->twinkleObj(data);
			}

			break;
		case 'E':
			{
				sObj = parentnode.getText();
				sfun = node.getText();
				sfun.Replace(sObj, L"");
				sfun.TrimLeft(L"_");
				sfun.MakeLower();

				if (cmd == tDELETE)
				{
					load[sObj][FORMAT(L"_am_%s_am_", sfun)] = L"";
					node.Delete();
					load.saveJason();
				}
				else if (cmd == tCOMMENT)
				{
					static const TCHAR pszText[] = _T("--");
					sData = load[sObj][FORMAT(L"_am_%s_am_", sfun)].Data;
					load[sObj][FORMAT(L"_am_%s_am_", sfun)] = comment(sData);
					load.saveJason();
				}
				MainSendMsg(ID_USR_SELCTRLTOCHILD, MAKELPARAM(_wtoi(sData), SC_SCRIPTBAR));
			}
			break;
		case 'F':
			if (cmd == tADDLAYOUT)
			{
				_imType = imgFORM;
				if (0 >= layout.size())
				{
					(*this)[L"FormLayout"] = L"F\t-1";
					(*this)[L"FormLayout"][L"Original"] = L"L\t-1";
					layout.insert(pair(L"-1", L"Original"));
					pActiveChild->m_view.SetDefaultLayout();
				}
				
				CString sAdd;
				int count = _mapH.lock()->maplayout.size() + 1;
				sAdd.Format(L"layout_%d", count);
				while (_mapH.lock()->maplayout.count(FORMAT(L"%d", count)))
				{
					count++;
					sAdd.Format(L"layout_%d", count);
				}

				(*this)[L"FormLayout"][sAdd] = L"L\t" + FORMAT(L"%d", count);				
				layout.insert(pair(FORMAT(L"%d", count), sAdd));	
				pActiveChild->m_view.SetDefaultLayout();
			}
			break;
		case 'L':
			if (cmd == tRENAME)
			{
				CReNameDlg dlg;
				CString sName = node.getText();
				
				dlg.setLayout(node.getText());
				if (dlg.DoModal() == IDOK)
				{
					layout.erase(sData);

					node.ReName(dlg.getLayoutName());
					layout.insert(pair(sData, dlg.getLayoutName()));
					pActiveChild->m_view.SetDefaultLayout();
				}
			}
			else if (cmd == tDELETE)
			{	
				if (data > -1)
				{
					layout.erase(sData);
					pActiveChild->m_view.SetDefaultLayout();
					node.Delete();
				}
			}
			break;
		default:
			break;
		}
	}
	*/
}

void AxTree::removeKey(CString sKey)
{
	_treemap.erase(sKey);
}

bool AxTree::search(CString sTitle)
{
	map<CString, HTREEITEM>::iterator it;
	CString ss;

	for (it = _treemap.begin(); it != _treemap.end(); it++)
	{
		ss = it->first;
		if (sTitle.CompareNoCase(ss) == 0)
		{
			AxNode node(it->second);
			return node.Select();
		}
	}
	return false;
}

void AxTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	const LPNMTREEVIEW pNMTreeView = LPNMTREEVIEW(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (getSelectedItemCount() <= 0)
		return;

	CPoint	pt = pNMTreeView->ptDrag;
	_dragImg = createDragImage(pt);
	if (_dragImg == nullptr)
		return;

	CPoint	startpt = pNMTreeView->ptDrag;
	startpt -= pt;

	_dragImg->BeginDrag(0, CPoint(0, 0));
	_dragImg->DragEnter(GetDesktopWindow(), startpt);
	SetCapture();
}

int AxTree::getSelectedItemCount()
{
	return 0;
}

HTREEITEM AxTree::getFirstSelectedItem()
{
	for (HTREEITEM hitem = GetRootItem(); hitem != nullptr; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return nullptr;
}

HTREEITEM AxTree::getNextSelectedItem(HTREEITEM hitem)
{
	for (hitem = GetNextVisibleItem(hitem); hitem != nullptr; hitem = GetNextVisibleItem(hitem))
	{
		if (GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return nullptr;
}

std::shared_ptr<CImageList> AxTree::createDragImage(CPoint& point)
{
	CRect	singleRc, completeRc = CRect(0, 0, 0, 0);

	HTREEITEM hitem = getFirstSelectedItem();

	while (hitem)
	{
		GetItemRect(hitem, &singleRc, TRUE);
		singleRc.InflateRect(0, 0, 25, 0);
		if (completeRc.IsRectEmpty())
			completeRc = singleRc;
		else
		{
			completeRc.bottom += singleRc.Height();
			completeRc.right = max(singleRc.right, completeRc.right);
		}

		hitem = getNextSelectedItem(hitem);
	}

	CClientDC dcClient(this);
	CDC	memDC;
	CBitmap bitmap;
	int	nCnt = 0;

	if (!memDC.CreateCompatibleDC(&dcClient))
		return nullptr;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, completeRc.Width(), completeRc.Height()))
		return nullptr;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, completeRc.Width(), completeRc.Height(), RGB(255, 255, 255));
	
	hitem = getFirstSelectedItem();
	
	while (hitem)
	{
		CImageList* pSingleImageList = CreateDragImage(hitem);
		if (pSingleImageList)
		{
			GetItemRect(hitem, &singleRc, TRUE);
			pSingleImageList->Draw(&memDC, 0, CPoint(0, singleRc.Height() * nCnt), ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
			nCnt++;
		}
		hitem = getNextSelectedItem(hitem);
	}
	
	memDC.SelectObject(pOldMemDCBitmap);
	std::shared_ptr<CImageList> pCompleteImageList = make_shared<CImageList>();
	pCompleteImageList->Create(completeRc.Width(), completeRc.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&bitmap, RGB(0, 255, 0));
	bitmap.DeleteObject();

	point.x = completeRc.left;
	point.y = completeRc.top;

	return pCompleteImageList;
}

void AxTree::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (_dragImg != nullptr)
	{
		CPoint	pt = point;
		ClientToScreen(&pt);
		_dragImg->DragMove(pt);
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}


void AxTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (_dragImg != nullptr)
	{
	// 	const gsl::not_null<CMainFrame*> pMain  = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	// 	const gsl::not_null<CChildFrame*> pChild = dynamic_cast<CChildFrame*>(pMain->MDIGetActive());

	 	_dragImg->DragLeave(this);
	 	_dragImg->EndDrag();
	 	::ReleaseCapture();

	 	_dragImg->DeleteImageList();
	}	
	// 	int from{}, to{};

	// 	HTREEITEM item = HitTest(point);
	// 	AxNode node(this, item);
	// 	CString sData = node.getData();
	// 	Parser(sData, L"\t");
	// 	to = _wtoi(sData);


	CTreeCtrl::OnLButtonUp(nFlags, point);
}


// void AxTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	*pResult = 0;
// 	HTREEITEM item = GetSelectedItem();
// 	if (item == nullptr)
// 		return;

// // 	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
// // ASSERT(pMain);

// 	//	AxJson load(FORMAT(L"%s.json", _mapH->json));
// 	AxNode node(this, item);
// 	AxNode parent(this, node.getParent());
// 	CString sData = node.getData();
// 	CString sObj  = parent.getText();
// 	CString sfun  = node.getText();
// 	CString sType;
// 	int type = 0;

// 	// if (!sData.IsEmpty())
// 	// {
// 	// 	sType = Parser(sData, L"\t");
// 	// 	type = _wtoi(sData);
// 	// 	switch (sType[0])
// 	// 	{
// 	// 	case 'O':
// 	// 		break;
// 	// 	case 'E':
// 	// 		break;
// 	// 	case 'T':
// 	// 		pMain->trlayout(L"", false, node.getText());
// 	// 		break;
// 	// 	case 'F':
// 	// 		break;
// 	// 	default:
// 	// 		break;
// 	// 	}
// 	// }
// }

void AxTree::ExpandAll(UINT style)
{
	for_each(_treemap.begin(), _treemap.end(),[&](const auto& item) {
		Expand(item.second, style);
	});
}