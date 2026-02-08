// CHiddenWnd.cpp: 구현 파일
//

#include "pch.h"
#include "IBXXXX12.h"
#include "CHiddenWnd.h"
#include "../../../H/axisfire.h"
#include "../../../H/interMSG.h"

#include "ioformat.h"
// CHiddenWnd

#define TR_GROUPCODE  17
#define TR_GROUPARR  18

IMPLEMENT_DYNAMIC(CHiddenWnd, CWnd)

CHiddenWnd::CHiddenWnd()
{

}

CHiddenWnd::~CHiddenWnd()
{
}


BEGIN_MESSAGE_MAP(CHiddenWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_USER + 999, OnMsgFromMain)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()



// CHiddenWnd 메시지 처리기
LRESULT CHiddenWnd::OnMsgFromMain(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case MMSG_SEARCH_INTERGROUP:
			Request_GroupList();
			break;
		case MMSG_SEARCH_GROUPCODE:
		{
			int igroup = atoi((char*)lParam);

			Request_GroupCode(igroup, TR_GROUPCODE);
		}
		break;
	}
	return 0;
}

LRESULT CHiddenWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString str;
	int key = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
		{
			key = HIBYTE(LOWORD(wParam));
			int datL = HIWORD(wParam);

			if (key == TR_GROUPARR)   //접속자의 관심그룹 조회하기
			{
				CString stemp;
				CString strHwnd;
				strHwnd.Format("%d", this->m_hWnd);
				::SendMessage(m_hWnd, WM_USER, MAKEWPARAM(MMSG_RET_SEARCH_INTERGROUP, datL), lParam);

				return 1;
			}
			else if (key == TR_GROUPCODE)  //관심그룹의 종목코드 조회
			{
				CString stemp;
				CString strHwnd;
				strHwnd.Format("%d", this->m_hWnd);
				::SendMessage(m_hWnd, WM_USER, MAKEWPARAM(MMSG_RET_SEARCH_GROUPCODE, datL), lParam);
			}
		}
	}

	return 0;
}

void CHiddenWnd::Request_GroupList()
{
	int	sendL = 0;
	char	sendB[16 * 1024] = { 0, }, tempB[32];
	int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	sendTR("PIDOMYST", sendB, sendL, TR_GROUPARR);
}

void CHiddenWnd::Request_GroupCode(int iseq, int iTRKey)
{
	CString slog;

	int index = iseq;
	int	sendL = 0;
	CString stmp;
	char	sendB[16 * 1024] = { 0, }, tempB[32];
	int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	stmp.Format("%02d", index);

	memcpy((char*)&sendB[sz_uinfo], stmp, 2);

	sendTR("PIDOMYST", sendB, sendL, TR_GROUPCODE);

}

BOOL CHiddenWnd::CreateMap(CWnd* pParent)
{
	m_pParent = pParent;
	return Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 100, 100), pParent, 100);
}

int CHiddenWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_hWnd = AfxGetMainWnd()->m_hWnd;
	CString stemp;
	CString strHwnd;
	strHwnd.Format("%d", this->m_hWnd);
	::SendMessage(m_hWnd, WM_USER, MMSG_SET_INTEREMAP, (LPARAM)(LPCTSTR)strHwnd);
	return 0;
}

void CHiddenWnd::sendTR(CString trCode, char* datB, int datL, int key)
{
	char* sendB = new char[L_userTH + datL + 1];

	struct	_userTH* uTH;
	uTH = (struct _userTH*)sendB;

	memcpy(uTH->trc, trCode, trCode.GetLength());
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)sendB);

	delete[] sendB;
}

void CHiddenWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect rec;
	GetClientRect(rec);

	dc.FillSolidRect(rec, RGB(255, 0, 0));
}
