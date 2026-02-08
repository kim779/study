// CInterWnd.cpp: 구현 파일
//

#include "stdafx.h"
#include "CX_PORTFOLIO.h"
#include "CInterWnd.h"

#include "../../H/interMSG.h"

// CInterWnd

IMPLEMENT_DYNAMIC(CInterWnd, CWnd)

CInterWnd::CInterWnd(char* pStrRoot, char* pStrUser)
{
	CString strRoot, strUser;
	strRoot.Format("%s", pStrRoot);
	char buff[MAX_PATH]{};
	sprintf(buff, "%08u", HashKey((LPCSTR)strRoot));

	char chFileName[500];
	GetModuleFileName(NULL, chFileName, MAX_PATH);
	m_strRoot.Format("%s", chFileName);
	m_strRoot.TrimRight();
	int ifind = m_strRoot.ReverseFind('\\');
	m_strRoot = m_strRoot.Left(ifind);
	m_strRoot.Replace("exe", "user");

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s.ini", m_strRoot, buff, buff);
	readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return ;

	CString sWnd;
	sWnd.Format("%s", readB);
	int ddata = atoi(sWnd);
	HWND hWnd = (HWND)ddata;
    m_pMainFrame = CWnd::FromHandle(hWnd);
}

CInterWnd::~CInterWnd()
{
}


BEGIN_MESSAGE_MAP(CInterWnd, CWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_MSG_MAINTOMAP, OnMsgFromMain)
END_MESSAGE_MAP()



// CInterWnd 메시지 처리기




int CInterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (m_pMainFrame)
	{
		CString stemp;
		CString strHwnd;
		strHwnd.Format("%d", this->m_hWnd);
		::SendMessage(m_pMainFrame->m_hWnd, WM_INTERMSG, MAKEWPARAM(MMSG_SEARCH_INTERGROUP, 0), (LPARAM)strHwnd.operator LPCSTR());

	}
	return 0;
}


LRESULT CInterWnd::OnMsgFromMain(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MMSG_SEARCH_SISE:
	{
		char* pdata = (char*)lParam;

		struct	_grid {
			char	code[12];		 /* 종목코드			 */
			char    hnam[20];                /* 종목명                       */
			char    curr[8];                 /* 현재가                       */
			char    diff[6];                 /* 전일대비      (9999V99)      */
			char    gvol[12];                /* 거래량                       */
		};

		struct  _interMod {
			char	keyf[2];		// ticker id
			char	nrec[3];		// count jcode
			struct	_grid	grid[1];	// data
		};

		CString		code, name, price, diff, gvol;
		_interMod* interMod = (struct _interMod*)pdata;
		int	cnt = atoi(CString(interMod->nrec, sizeof(interMod->nrec)));
		for (int ii = 0; ii < cnt; ii++)
		{
			code = CString(interMod->grid[ii].code, sizeof(interMod->grid[ii].code));	code.TrimRight();
			name = CString(interMod->grid[ii].hnam, sizeof(interMod->grid[ii].hnam));	name.TrimRight();
			price = CString(interMod->grid[ii].curr, sizeof(interMod->grid[ii].curr));	price.TrimRight();
			diff = CString(interMod->grid[ii].diff, sizeof(interMod->grid[ii].diff));	diff.TrimLeft();
			gvol = CString(interMod->grid[ii].gvol, sizeof(interMod->grid[ii].gvol));	gvol.TrimLeft();
			//SetInter((char*)&interMod->grid[ii]);
		}
	}
	break;
	case MMSG_SEARCH_GROUPCODE:
	{
		char* pdata = (char*)lParam;

		//CString stmp, sRet, code;
		//CString strseq = CString(pdata, 2);
		//CString strgname = CString(&pdata[2], 20);
		//int cnt = atoi(CString(&pdata[22], 4));
		//int parseL = 26;

		//struct _jinfo* jinfo{};
		//char* pCode;
		//pCode = new char[sizeof(jinfo->code) + 1];
		//sRet.Empty();

		//if (cnt <= 0)
		//	return 0;

		//for (int ii = 0; ii < cnt; ii++)
		//{
		//	jinfo = (struct _jinfo*)&pdata[parseL];

		//	memset(pCode, 0x00, sizeof(jinfo->code) + 1);
		//	memcpy(pCode, jinfo->code, sizeof(jinfo->code));  // 종목코드[12]

		//	code.Format("%s", pCode);
		//	code.TrimRight();

		//	auto inter = std::make_unique<_inter>();
		//	inter->Code = code;
		//	m_arInter.Add(inter.get());
		//	m_arInterByCode.SetAt(inter->Code, (CObject*)inter.get());

		//	m_arSymData.SetAt(inter->Code, "x");
		//	m_arSym.Add(inter->Code);

		//	parseL += sizeof(struct _jinfo);
		//}
	
	}
	break;
	case MMSG_SEARCH_INTERGROUP:
	{
		CString stmp, seq, sgname, keys;
		char* pdata = (char*)lParam;
		//int cnt = atoi(CString(pdata, 4));
		//if (cnt == 0) return 0;

		//for (int ii = 0; ii < cnt; ii++)
		//{
		//	keys.Format("%02d", ii);
		//	m_arrInterGroup.Add(keys);
		//}

		//int parseL = 4;
		//struct _grSearch
		//{
		//	char ngrs[2];
		//	char grseq[2];
		//	char gname[30];
		//};

		//struct _grSearch* stgr;
		//char* pSeq, * pgame;
		//pSeq = new char[sizeof(stgr->grseq) + 1];
		//pgame = new char[sizeof(stgr->gname) + 1];

		//for (int ii = 0; ii < cnt; ii++)
		//{
		//	stgr = (struct _grSearch*)&pdata[parseL];
		//	memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
		//	memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
		//	seq.Format("%s", pSeq);

		//	memset(pgame, 0x00, sizeof(stgr->gname) + 1);
		//	memcpy(pgame, stgr->gname, sizeof(stgr->gname));
		//	sgname.Format("%s", pgame);
		//	sgname.TrimRight();

		//	parseL += sizeof(struct _grSearch);

		//	m_arrInterGroup.SetAt(ii, sgname);
		//}

		//delete[] pSeq; pSeq = nullptr;
		//delete[] pgame; pgame = nullptr;

		///////////////////////////////////////////////
		//CPoint point;
		//GetCursorPos(&point);
		//CMenuXP		menu;
		//menu.CreatePopupMenu();
		//CStringArray arKey;
		//for (int ii = 0; ii < m_arrInterGroup.GetSize(); ii++)
		//{
		//	CString sGroup = m_arrInterGroup.GetAt(ii);
		//	menu.AppendMenuX(MF_STRING, ii + 1, sGroup);
		//}
		//menu.AppendMenuX(MF_SEPARATOR);
		//menu.AppendMenuX(MF_STRING, 999, "상세설정");
		//if ((int)m_select < m_arrInterGroup.GetSize())
		//	menu.CheckMenuItem(m_select + 1, MF_CHECKED | MF_BYCOMMAND);
		//else
		//	menu.CheckMenuItem(1, MF_CHECKED | MF_BYCOMMAND);

		//const int nRtn = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);

		//if (nRtn == 999)
		//{
		//	(*m_axiscall)(AXI_TICKSETUP, 0, 0);
		//}
		//else if (nRtn > 0 && (nRtn - 1) != (int)m_select)
		//{
		//	CString section;
		//	section.Format("BAR_INFO_%02d", m_id);
		//	m_select = nRtn - 1;
		//	save();
		//	loadInter(section);
		//	//	ReSetPaneInfo();
		//}
	}
	break;
	}

	return 0;
}