#include "pch.h"
#include "CHDwnd.h"
#include "../../../H/axisfire.h"
#include "../../../H/interMSG.h"

#include "ioformat.h"

#define TR_GROUPCODE					17
#define TR_GROUPARR					18
#define TR_DBBACKUP					19
#define TR_NEWGROUP					20
#define TR_GROUPCODESAVE		21

#define TM_TRWAIT		9999
#define TR_WAIT_TIME	5000

CHDwnd::CHDwnd()
{
	m_hwndArr.RemoveAll();
}

CHDwnd::~CHDwnd()
{

}

BEGIN_MESSAGE_MAP(CHDwnd, CWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_MSG_MAINTOMAP, OnMsgFromMain)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CHDwnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TM_TRWAIT:
	{
		KillTimer(nIDEvent);
		if (m_hwndArr.GetSize() > 0)
		{
			m_hwndArr.RemoveAt(0);
			CheckMainMsg();
		}
	}
	break;
	}
	CWnd::OnTimer(nIDEvent);
}

CString CHDwnd::Parser(CString& srcstr, CString substr)
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

void CHDwnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect rec;
	GetClientRect(rec);
	dc.FillSolidRect(rec, RGB(255, 0, 0));
}

int CHDwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_hMain = AfxGetMainWnd()->m_hWnd;
	CString stemp;
	CString strHwnd;
	strHwnd.Format("%d", this->m_hWnd);
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_SET_INTEREMAP, -1), this->m_hWnd), (LPARAM)(LPCTSTR)strHwnd);
	return 0;
}

int CHDwnd::KeyTran(int ikey)
{
	int iretkey = 0;
	switch (ikey)
	{
		case MMSG_SEARCH_INTERGROUP:			 iretkey = MMSG_RET_SEARCH_INTERGROUP; break;
		case MMSG_SEARCH_GROUPCODE:			 iretkey = MMSG_RET_SEARCH_GROUPCODE;  	break;
		case MMSG_INTER_DBBACKUP:					 iretkey = MMSG_RET_INTER_DBBACKUP; break;
		case MMSG_INTER_NEWGROUP:				 iretkey = MMSG_RET_INTER_DBBACKUP;  break;
		case MMSG_INTER_GROUPCODESAVE:		 iretkey = MMSG_RET_INTER_GROUPCODESAVE;  break;
	}
	return iretkey;
}

void CHDwnd::sendTR(CString trCode, char* datB, int datL, int key)
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

BOOL CHDwnd::CreateMap(CWnd* pParent)
{
	m_pParent = pParent;
	return Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 100, 100), pParent, 100);
}

LRESULT CHDwnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString str;
	int Trkey = 0;
	int isize = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
		{
			Trkey = HIBYTE(LOWORD(wParam)); 
			isize = HIWORD(wParam);
			switch (Trkey)
			{
				case TR_GROUPARR:
				case TR_GROUPCODE:
				case TR_DBBACKUP:
				case TR_NEWGROUP:
				case TR_GROUPCODESAVE:
				{
					if (m_hwndArr.GetSize() > 0)
					{
						st_RequestHwnd* pst = m_hwndArr.GetAt(0);
						int hkey = KeyTran(pst->key);
						if(hkey)
							::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MAKEWORD(hkey, isize), pst->hwnd), lParam);
						KillTimer(TM_TRWAIT);
						m_hwndArr.RemoveAt(0);
						CheckMainMsg();
					}
				}
				break;
				default:
				{

				}
				break;
			}
		}
		break;
	}

	return 0;
}

void CHDwnd::CheckMainMsg()
{
	if (m_hwndArr.GetSize() > 0)
	{
		SetTimer(TM_TRWAIT, TR_WAIT_TIME, nullptr);
		st_RequestHwnd* pst = m_hwndArr.GetAt(0);
		switch (pst->key)
		{
			case MMSG_SEARCH_INTERGROUP:
				Request_GroupList();
			break;
			case MMSG_SEARCH_GROUPCODE:
				Request_GroupCode(pst->pMid);
			break;
			case MMSG_INTER_DBBACKUP:
				DB_uploadBackup();
			break;
			case MMSG_INTER_NEWGROUP:
			{
				DB_NewGroupAppend(pst->pMid);
			}
			break;
			case MMSG_INTER_GROUPCODESAVE:
			{
				DB_GroupCodeSave(pst->pMid);
			}
			break;
		}
	}
}

LRESULT CHDwnd::OnMsgFromMain(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
		case MMSG_SEARCH_INTERGROUP:
		case MMSG_SEARCH_GROUPCODE:
		case MMSG_INTER_DBBACKUP:
		case MMSG_INTER_NEWGROUP:
		case MMSG_INTER_GROUPCODESAVE:
		{
			struct st_RequestHwnd* pst = new struct st_RequestHwnd;
			pst->hwnd = (HWND)HIWORD(wParam);
			pst->key = LOBYTE(LOWORD(wParam));
			int isize = HIBYTE(LOWORD(wParam));
			if (isize > 0)
			{
				pst->pMid = new char[isize + 1];
				memset(pst->pMid, 0x00, isize + 1);
				memcpy(pst->pMid, (char*)lParam, isize);
			}
			m_hwndArr.Add(pst);
			CheckMainMsg();
		}
		break;
	}
	return 0;
}

void  CHDwnd::DB_GroupCodeSave(char* pdata)
{
	int	sendL = 0;
	char	sendB[16 * 1024], tempB[32];

	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += sz_updn;

	CString ogrs, mode, gnam, stmp;
	FillMemory((char*)updn, sz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'G';	//그룹종목전송

	memcpy(tempB, "00001", 5);
	CopyMemory(updn->uinfo.nblc, tempB, sizeof(updn->uinfo.nblc));

	updn->uinfo.retc[0] = 'O';

	int count = 0;

	CString sbuffer;
	sbuffer.Format("%s", pdata);
	ogrs = Parser(sbuffer, "\n");
	gnam = Parser(sbuffer, "\n");
	count = atoi(Parser(sbuffer, "\n"));

	mode = "";

	int ilen= count * sz_jinfo;
	memcpy((char*)&sendB[sendL], (LPSTR)(LPCTSTR)sbuffer, ilen);
	sendL += ilen;

	//ogrs = m_sheet->getManageGroupdata(gno, 0);		//old
	//mode = m_sheet->getManageGroupdata(gno, 1);		//mode (D : 삭제) N : 그룹 추가 
	//gnam = m_sheet->getManageGroupdata(gno, 3);		//name

	CopyMemory(updn->ginfo.gnox, ogrs.operator LPCTSTR(), sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
	stmp.Format("%04d", count);
	CopyMemory(updn->ginfo.jrec, stmp, sizeof(updn->ginfo.jrec));

	sendTR(trUPDOWN, sendB, sendL, TR_GROUPCODESAVE);
}

void CHDwnd::DB_NewGroupAppend(char* pdata)
{
	CString strGroupName;
	strGroupName.Format("%s", pdata);

	int	sendL = 0;
	char	sendB[16 * 1024], tempB[32];

	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += sz_updn;

	CString ogrs, mode, gnam;
	FillMemory((char*)updn, sz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'G';  //그룹종목전송  새그룹을 올릴때도 사용하며 이때는 같이 올리는 종목코드들은 없다

	memcpy(tempB, "00001", 5);
	CopyMemory(updn->uinfo.nblc, tempB, sizeof(updn->uinfo.nblc));

	updn->uinfo.retc[0] = 'O';

	int count = 0;

	CopyMemory(updn->ginfo.gnox, "-1", sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, strGroupName, min(sizeof(updn->ginfo.gnam), strGroupName.GetLength()));
	CopyMemory(updn->ginfo.jrec, "0000", sizeof(updn->ginfo.jrec));

	sendB[sendL] = 0x00;
	sendTR(trUPDOWN, sendB, sendL, TR_NEWGROUP);
}

void CHDwnd::Request_GroupList()
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

void CHDwnd::DB_uploadBackup()
{
	int	sendL = 0;
	char	sendB[16 * 1024], tempB[32];
	//	char	strUinfo[500];

	struct _uinfo* uinfo = (struct _uinfo*)&sendB[sendL];
	sendL += sz_uinfo;

	FillMemory((char*)uinfo, sz_uinfo, ' ');

	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'b';

	memcpy(tempB, "00000", 5);
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));

	uinfo->retc[0] = 'O';

	sendB[sendL] = 0x00;
	sendTR(trUPDOWN, sendB, sendL, TR_DBBACKUP);
}

void CHDwnd::Request_GroupCode(char* pdata)
{
	CString slog;

	int index = atoi(pdata);
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




