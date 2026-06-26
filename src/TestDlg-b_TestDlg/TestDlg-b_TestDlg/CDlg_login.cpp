// CDlg_login.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_login.h"
#include "afxdialogex.h"


// CDlg_login 대화 상자

IMPLEMENT_DYNAMIC(CDlg_login, CDialogEx)

CDlg_login::CDlg_login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
{

}

CDlg_login::~CDlg_login()
{
}

void CDlg_login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_login, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SIGNR, &CDlg_login::OnBnClickedBtnSignr)
	ON_BN_CLICKED(IDC_BTN_CHECK, &CDlg_login::OnBnClickedBtnCheck)
END_MESSAGE_MAP()


// CDlg_login 메시지 처리기

//	signR.signK
//
#define	signON		0x01		// sign/on
#define	signOX		0x02		// sign/off
#define	signER		0x03		// error

#define	maskSEC		0x07		//0111		security mask				
#define	maskNO		0x00		//				security no change
#define	maskOR		0x01		//0001		OR
#define	maskAND		0x02		//0010		AND
#define	maskXOR		0x03		//0011		XOR
#define	maskSYS		0x20		//0010 0000 	 system bit

typedef struct	_signR {
	unsigned char	signK;		// sign
	unsigned char	mask;		// security mask
	unsigned char	absS;		// absolute
	unsigned char	incS[4];	// inclusive
	unsigned char	excS[4];	// exclusive

	char	termN[8];		// terminal name;
	unsigned char	flag;		// terminal flag
	unsigned char	dev;		// device flag

	char	mapN[8];		// map name
	char	sign[12];		// sign identification
	char	name[20];		// sign name
	char	menu[12];		// menu

	char	trx[3];			// transaction timeout(sec.)
	char	usage[3];		// usage time (min.)
	char	idle[3];		// idle timeout (min.)
	char	guide[70];		// guide msg
	char	service[10];		// service#

	char	info[64];		// information
					// server time + '\t' + ....
	char	infox[192];		// information
}STSindR;
#define	L_signR		sizeof(struct _signR)
unsigned char m_incS[4]{};
unsigned char m_excS[4]{};
void CDlg_login::OnBnClickedBtnSignr()
{
	
	STSindR* signR = new STSindR;
	int	idx;
	
	signR->incS[0] = 55;
	signR->incS[1] = 56;
	signR->incS[2] = 57;
	signR->incS[3] = 58;

	signR->mask = 1;

	switch (signR->mask & maskSEC)  //0111
	{
	case maskOR:		//0001	
		for (idx = 0; idx < sizeof(signR->incS); idx++)
		{
			m_incS[idx] |= signR->incS[idx];
			m_excS[idx] |= signR->excS[idx];
		}
		break;
	case maskAND:  //0010
		for (idx = 0; idx < sizeof(signR->incS); idx++)
		{
			m_incS[idx] &= signR->incS[idx];
			m_excS[idx] &= signR->excS[idx];
		}
		break;
	case maskXOR:  //0011
		for (idx = 0; idx < sizeof(signR->incS); idx++)
		{
			m_incS[idx] ^= signR->incS[idx];
			m_excS[idx] ^= signR->excS[idx];
		}
		break;
	case maskNO:
	default:
		break;
	}
}

#define L_SECB 4
void CDlg_login::OnBnClickedBtnCheck()
{ //쉬프트 연산 한칸 이동은 산술적으로 곱하기2 거나 나누기2이다
	int itest = 22;

	int itemp = 25;
	if (itest & itemp)  //한비트라도 참이면 참
	{
		TRACE("1");
	}
	else
	{
		TRACE("2");
	}

	OnBnClickedBtnSignr();
	int	iBit, kk;
	DWORD	terms, maps, masks, ieSecurity;

	terms = maps = 0;
	for (kk = 0; kk < L_SECB; kk++)
	{
		terms <<= 8;
		terms |= (BYTE)m_incS[kk];

		maps <<= 8;
		//maps |= (BYTE)mapH->incS[kk];
		maps |= (BYTE)m_excS[kk];
	}

	ieSecurity = terms & maps;
	//if (!ieSecurity && terms && maps)
	{
		iBit = 0;
		for (masks = 0x80000000; masks; masks >>= 1)
		{
			if (terms & masks)
			{
				if (!(maps & masks))
				{
					//SecurityErrorMsg(iBit);
					return ;
				}
			}
			iBit++;
		}
	}


}
