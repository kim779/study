// CMapWnd.cpp: 구현 파일
//

#include "pch.h"
#include "IBXXXX13.h"
#include "CMapWnd.h"
#include "../../../H/interMSG.h"
#include "ioformat.h"

// CMapWnd

IMPLEMENT_DYNAMIC(CMapWnd, CWnd)
#define IDC_BUTTON_INTER				9898				//관심그룹리스트 조회
#define IDC_BUTTON_INTERCODE		9897    //관심그룹종목코드 조회

CMapWnd::CMapWnd()
{

}

CMapWnd::~CMapWnd()
{
	
}

BOOL CMapWnd::CreateMap(CWnd* pParent)
{
	m_pParent = pParent;
	return Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 100, 100), pParent, 100);
}

void CMapWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect rec;
	GetClientRect(rec);

	//dc.FillSolidRect(rec, RGB(0, 255, 0));
}

#include "CDlg.h"
int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	m_pdlg = new CDlg;
	m_pdlg->Create(IDD_DLG, this);
	m_pdlg->ShowWindow(SW_SHOW);
	m_pdlg->m_pWizrd = m_pParent;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	/*CRect rctmp;
	rctmp.SetRect(0, 0, 200, 20);

	rctmp.OffsetRect(10, 10);

	m_pBtn_SearchGroupList = new CfxImgButton();
	m_pBtn_SearchGroupList->Create("관심그룹 리스트 조회", rctmp, this, IDC_BUTTON_INTER, FALSE, TRUE);

	rctmp.OffsetRect(200 + 10, 0);
	m_pBtn_SearchGroupCode = new CfxImgButton();
	m_pBtn_SearchGroupCode->Create("관심그룹 종목 조회", rctmp, this, IDC_BUTTON_INTERCODE, FALSE, TRUE);*/
	return 0;
}


void CMapWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//ON_BN_CLICKED(IDC_BUTTON_INTER, OnBtnInterList)
	//ON_BN_CLICKED(IDC_BUTTON_INTERCODE, OnBtnInterCode)
	ON_MESSAGE(WM_MSG_MAINTOMAP, OnMsgFromMain)
	ON_WM_SIZE()
END_MESSAGE_MAP()


LRESULT CMapWnd::OnMsgFromMain(WPARAM wParam, LPARAM lParam)
{
	int datL = (HIBYTE(LOWORD(wParam)));
	switch (LOBYTE(LOWORD(wParam)))
	{
		case MMSG_RET_SEARCH_INTERGROUP:
		{
m_slog.Format("\r\n------------------------ 관심그룹 리스트 조회 [%d]\n", datL);
OutputDebugString(m_slog);
m_pdlg->AddTextEdit(m_slog);

			m_arrGroup.RemoveAll();

			CString stmp, seq, sgname;
			char* pdata = (char*)lParam;
			int cnt = atoi(CString(pdata, 4));

			int parseL = 4;
			struct _grSearch
			{
				char ngrs[2];
				char grseq[2];
				char gname[30];
			};

			struct _grSearch* stgr;
			char* pSeq, * pgame;
			pSeq = new char[sizeof(stgr->grseq) + 1];
			pgame = new char[sizeof(stgr->gname) + 1];

			for (int ii = 0; ii < cnt; ii++)
			{
				stgr = (struct _grSearch*)&pdata[parseL];
				memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
				memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
				seq.Format("%s", pSeq);

				memset(pgame, 0x00, sizeof(stgr->gname) + 1);
				memcpy(pgame, stgr->gname, sizeof(stgr->gname));
				sgname.Format("%s", pgame);
				m_arrGroup.Add(sgname.TrimRight());
			
				stmp.Format("%s%s", seq, sgname);
				parseL += sizeof(struct _grSearch);

m_slog.Format("[%d]     [%s]\n", ii, stmp);
OutputDebugString(m_slog);
m_pdlg->AddTextEdit(m_slog);
XMSG(m_slog);
			}

			delete[] pSeq; pSeq = nullptr;
			delete[] pgame; pgame = nullptr;
		}
		break;
		case MMSG_RET_SEARCH_GROUPCODE:
		{
m_slog.Format("\r\n------------------------ 관심그룹 코드 조회 [%d] \n", datL);
OutputDebugString(m_slog);
m_pdlg->AddTextEdit(m_slog);
			char* pdata = (char*)lParam;

			CString stmp;
			CString strseq = CString(pdata, 2);
			CString strgname = CString(&pdata[2], 20);
			int cnt = atoi(CString(&pdata[22], 4));
			int parseL = 26;

			struct _jinfo* jinfo;
			char* pgubn, * pCode, * pxprc, * pxnum;
			pgubn = new char[sizeof(jinfo->gubn) + 1];
			pCode = new char[sizeof(jinfo->code) + 1];
			pxprc = new char[sizeof(jinfo->xprc) + 1];
			pxnum = new char[sizeof(jinfo->xnum) + 1];

			for (int ii = 0; ii < cnt; ii++)
			{
				jinfo = (struct _jinfo*)&pdata[parseL];

				memset(pgubn, 0x00, sizeof(jinfo->gubn) + 1);
				memcpy(pgubn, jinfo->gubn, sizeof(jinfo->gubn));  // 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수

				memset(pCode, 0x00, sizeof(jinfo->code) + 1);
				memcpy(pCode, jinfo->code, sizeof(jinfo->code));  // 종목코드[12]

				memset(pxprc, 0x00, sizeof(jinfo->xprc) + 1);
				memcpy(pxprc, jinfo->xprc, sizeof(jinfo->xprc));  // 보유단가[10]

				memset(pxnum, 0x00, sizeof(jinfo->xnum) + 1);
				memcpy(pxnum, jinfo->xnum, sizeof(jinfo->xnum));  	// 보유수량[10]

				stmp.Format("%s\t%s\t%s\t%s", pgubn, pCode, pxprc, pxnum);

m_slog.Format("[%d]    [%s]\n", ii, stmp);
OutputDebugString(m_slog);
m_pdlg->AddTextEdit(m_slog);
XMSG(m_slog);

				parseL += sizeof(struct _jinfo);
			}

			delete[] pgubn; pgubn = nullptr;
			delete[] pCode; pCode = nullptr;
			delete[] pxprc; pxprc = nullptr;
			delete[] pxnum; pxnum = nullptr;
		}
		break;
		case MMSG_RET_INTER_DBBACKUP:
		{
			if (datL < sz_uinfo)
			{
				AfxMessageBox("");
				m_slog.Format("\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!! MMSG_RET_INTER_DBBACKUP error in dll_oub !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!] \n");
				m_pdlg->AddTextEdit(m_slog);
				return 0;
			}

			char datTemp[4084];
			ZeroMemory(datTemp, 4084);
			int	parseL = 0;

			memcpy(datTemp, (char*)lParam, sizeof(_uinfo));
			struct	_uinfo* uinfo = (struct _uinfo*)datTemp;

			parseL += sz_uinfo;


			if (uinfo->retc[0] == 'E')
			{
				struct	_uinfo	infoC;
				FillMemory(&infoC, sizeof(_uinfo), ' ');

				CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
				infoC.dirt[0] = 'U';
				infoC.cont[0] = 'C';		// 취소

				TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
			}

			m_slog.Format("[IBXXXX13] 관심그룹 DB백업 서비스 호출 결과 [%c]\n", uinfo->retc[0]);
			OutputDebugString(m_slog);
			m_pdlg->AddTextEdit(m_slog);
		}
		break;
		case MMSG_RET_INTER_NEWGROUP:
		{

		}
		break;
	}
	return 0;
}


// CMapWnd 메시지 처리기
void CMapWnd::OnBtnInterList()  //관심종목 그룹리스트 조회
{
	m_hMain = AfxGetMainWnd()->m_hWnd;
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_SEARCH_INTERGROUP, 0), this->m_hWnd), 0);
}

void CMapWnd::OnBtnInterCode(int igroup) //그룹의 종목코드 조회
{
	m_hMain = AfxGetMainWnd()->m_hWnd;
	CString stemp;
	stemp.Format("%02d", igroup);
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_SEARCH_GROUPCODE, 2) , this->m_hWnd), (LPARAM)(LPSTR)(LPCTSTR)stemp);
}

void CMapWnd::OnBtnDBBackup()  //관심DB 백업
{
	m_hMain = AfxGetMainWnd()->m_hWnd;
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_INTER_DBBACKUP, 0), this->m_hWnd), 0);
}

void CMapWnd::OnBtnNewGroup(CString strGroupName)  //새그룹
{
	m_hMain = AfxGetMainWnd()->m_hWnd;
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_INTER_NEWGROUP, strGroupName.GetLength()), this->m_hWnd), (LPARAM)(LPSTR)(LPCTSTR)strGroupName);
}

void CMapWnd::OnBtnGroupSave(int igroup) 
{//그룹에 해당하는 종목을 저장한다.  저장한 결과가 성공이면   'V' <-- 최종상태저장  날리고 성공이면 바로  , 'b' <-- DB백업도 연속도 날려준다

	CString strBuffer;
	CString stemp;
	stemp.Format("%02d\n%s", igroup, m_arrGroup.GetAt(igroup - 1));
	
	int count = 5;
	int bufsize = count * sizeof(struct _jinfo) + 1;
	char* sendB = new char[bufsize];
	memset(sendB, 0x00, bufsize);

	struct	_jinfo* jinfo{};
	int datL = 0;

	for (int ii = 0; ii < count; ii++)
	{
		jinfo = (struct _jinfo*)&sendB[datL];
		datL += sz_jinfo;

		memset(jinfo->gubn, ' ', sizeof(jinfo->gubn));
		memset(jinfo->code, ' ', sizeof(jinfo->code));
		memset(jinfo->xprc, ' ', sizeof(jinfo->xprc));
		memset(jinfo->xnum, ' ', sizeof(jinfo->xnum));

	//	jinfo->gubn[0] = "0";  //0 : none, 1 :현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수 
		CopyMemory(jinfo->code, "0", 1);
		switch (ii)
		{
			case 0:
			{
				CopyMemory(jinfo->code, "000020", 6);
				CopyMemory(jinfo->xprc,  "2222", 4);										//종목매입 가격
				CopyMemory(jinfo->xnum, "98", 2);            //보유수량 
			}
			break;
			case 1:
			{
				CopyMemory(jinfo->code, "000040", 6);
			}
			break;
			case 2:
			{
				CopyMemory(jinfo->code, "000050", 6);
			}
			break;
			case 3:
			{
				CopyMemory(jinfo->code, "000060", 6);
			}
			break;
			case 4:
			{
				CopyMemory(jinfo->code, "000070", 6);
			}
			break;
		}
	}

	strBuffer.Format("%s\n%d\n%s", stemp, count, sendB);
	
	m_hMain = AfxGetMainWnd()->m_hWnd;
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_INTER_GROUPCODESAVE, strBuffer.GetLength()), this->m_hWnd), (LPARAM)(LPSTR)(LPCTSTR)strBuffer);
}
