
// HUBDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "HUB.h"
#include "HUBDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32")
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHUBDlg 대화 상자



CHUBDlg::CHUBDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HUB_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHUBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHUBDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CHUBDlg 메시지 처리기
CString Parser(CString& srcstr, CString substr)
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

CString URLEncode(const char* lpszURL)
{
	CString dest("");

	struct Encode
	{
	private:
		BOOL isexcept(char ch)
		{
			const char excepts[] = {
				'_', '.', 0x00
			};

			for (int i = 0; excepts[i]; i++)
				if (ch == excepts[i])
					return TRUE;
			return FALSE;
		}
	public:
		CString value;

		Encode(char ch)
		{
			if (isalpha(ch) || (ch >= '0' && ch <= '9') || isexcept(ch))
				value = ch;
			else
				value.Format("%%%02X", ch);
		}
	};

	for (int i = 0; i < (int)strlen(lpszURL); i++)
	{
		dest += Encode(lpszURL[i]).value;
	}

	return dest;
}

char* base64Encoder(BYTE input_str[], int len_str)
{
	// TODO: 여기에 구현 코드 추가.
	// Character set of base64 encoding scheme
	char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	// Resultant string
	char* res_str = (char*)malloc(1000 * sizeof(char));

	int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp;
	int i, j, k = 0;

	// Loop takes 3 characters at a time from
	// input_str and stores it in val
	for (i = 0; i < len_str; i += 3)
	{
		val = 0, count = 0, no_of_bits = 0;

		for (j = i; j < len_str && j <= i + 2; j++)
		{
			// binary data of input_str is stored in val
			val = val << 8;

			// (A + 0 = A) stores character in val
			val = val | input_str[j];

			// calculates how many time loop
			// ran if "MEN" -> 3 otherwise "ON" -> 2
			count++;

		}

		no_of_bits = count * 8;

		// calculates how many "=" to append after res_str.
		padding = no_of_bits % 3;

		// extracts all bits from val (6 at a time)
		// and find the value of each block
		while (no_of_bits != 0)
		{
			// retrieve the value of each block
			if (no_of_bits >= 6)
			{
				temp = no_of_bits - 6;

				// binary of 63 is (111111) f
				index = (val >> temp) & 63;
				no_of_bits -= 6;
			}
			else
			{
				temp = 6 - no_of_bits;

				// append zeros to right if bits are less than 6
				index = (val << temp) & 63;
				no_of_bits = 0;
			}
			res_str[k++] = char_set[index];
		}
	}

	// padding is done here
	for (i = 1; i <= padding; i++)
	{
		res_str[k++] = '=';
	}

	res_str[k] = '\0';

	return res_str;
}

#include <wincrypt.h>
int CHUBDlg::showHUB()
{
	CString slog;
	slog.Format("[HUB]  showHUB ");
	OutputDebugString(slog);
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;

	if (CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0) == FALSE) {
		slog.Format("1  %x", GetLastError());
		OutputDebugString(slog);
		//if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET) == FALSE) {
		if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_NEWKEYSET) == FALSE) {
			slog.Format("2 %x", GetLastError());
			OutputDebugString(slog);
			slog.Format("[HUB]   CryptAcquireContext error ");
			OutputDebugString(slog);
			return 0;
		}
	}

	const DWORD AES_KEY_LENGTH = 16;
	struct {
		BLOBHEADER hdr;
		DWORD cbKeySize;
		BYTE rgbKeyData[AES_KEY_LENGTH];
	} keyBlob;

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_128;
	keyBlob.cbKeySize = AES_KEY_LENGTH;
	CopyMemory(keyBlob.rgbKeyData, "!O7#8aksjdf67h53", AES_KEY_LENGTH);

	if (CryptImportKey(hCryptProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey) == FALSE) {
		slog.Format("[HUB]   CryptImportKey error ");
		OutputDebugString(slog);
		return 0;
	}

	DWORD dwMode = CRYPT_MODE_ECB;
	if (CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&dwMode, 0) == FALSE) {
		slog.Format("[HUB]   CryptSetKeyParam1 error ");
		OutputDebugString(slog);
		return 0;
	}

	dwMode = PKCS5_PADDING;
	if (CryptSetKeyParam(hKey, KP_PADDING, (BYTE*)&dwMode, 0) == FALSE) {
		slog.Format("[HUB]   CryptSetKeyParam2 error ");
		OutputDebugString(slog);
		return 0;
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString plainID;

	LONG lResult;
	HKEY hRKey;
	DWORD dwType;
	DWORD dwBytes = 100;
	char buffer[100];

	lResult = RegOpenKeyEx(HKEY_CURRENT_USER,
		"Software\\IBKMAC_STAFF\\AXIS Workstation V04.00\\Workstation",
		//	"Software\\IBK_WORK\\AXIS Workstation V03.00\\Workstation",
		0, KEY_ALL_ACCESS, &hRKey);
	if (lResult != ERROR_SUCCESS)
	{
		slog.Format("[HUB]   Register Open Error");
		OutputDebugString(slog);

		lResult = RegOpenKeyEx(HKEY_CURRENT_USER,
			"Software\\IBK_MAC_STAFF\\AXIS Workstation V04.00\\Workstation",
			//	"Software\\IBK_WORK\\AXIS Workstation V03.00\\Workstation",
			0, KEY_ALL_ACCESS, &hRKey);
		if (lResult != ERROR_SUCCESS)
		{
			return 0;
		}
	}

	// Read Regstry Key 
	lResult = RegQueryValueEx(hRKey, "SignOnID", 0, &dwType, (LPBYTE)buffer, &dwBytes);
	if (lResult == ERROR_SUCCESS)
	{
		slog.Format("[HUB]   Register Open RegQueryValueEx  [%s]", buffer);
		OutputDebugString(slog);
	}
	else
	{
		slog.Format("[HUB]   Register Open RegQueryValueEx fail");
		OutputDebugString(slog);
		return 0;
	}

	plainID.Format("%s", buffer);
	plainID = Parser(plainID, "|");
	plainID.Format("%s^%04d%02d%02d%02d%02d%02d", plainID, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	slog.Format("[HUB]   Register Open RegQueryValueEx  [%s]", plainID);
	OutputDebugString(slog);
	RegCloseKey(hRKey);


	//plainID.Format("%s^%04d%02d%02d%02d%02d%02d", pID, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	//slog.Format("[HUB]   plain=[%s]", plainID);
	//OutputDebugString(slog);

	//plainID = "081144^20211231235959";

	//const char PLAINTEXT[] = "081144^20211231235959";
	//char PLAINTEXT[] = plainID;
	BYTE pData[128];
	char pData2[128];
	DWORD dwStrLen = plainID.GetLength();

	DWORD dwPadding = 16 - (dwStrLen & 0xF);
	DWORD dwDataLen = dwStrLen;
	DWORD dwBufferLen = dwStrLen + dwPadding;
	BOOL result;

	memset(pData, 0, 128);
	memset(pData2, 0, 128);

	CopyMemory(pData, (LPSTR)(LPCTSTR)plainID, dwStrLen);

	result = CryptEncrypt(hKey, 0, TRUE, 0, pData, &dwDataLen, dwBufferLen);

	sprintf_s(pData2, 128, "%s", base64Encoder(pData, dwBufferLen));

	CString strUrl;
	strUrl = " --start-maximized --app=https://hub.ibks.com/api/emp/login/sso?value=" + URLEncode(pData2);
	slog.Format("[HUB]   url=[%s]", strUrl);
	OutputDebugString(slog);

	//	ShellExecute(NULL, "open", "chrome.exe", strUrl, NULL, SW_SHOW);
		//C:\\Chrome\\GoogleChromePortable.exe"
		//strUrl = "naver.com";
		//ShellExecute(NULL, "open", "C:\\Users\\IBKS\\AppData\\Local\\Google\\Chrome\\Application\\chrome.exe", strUrl, NULL, SW_SHOW|SW_SHOWMAXIMIZED);
	ShellExecute(NULL, "open", "C:\\Chrome\\GoogleChromePortable.exe", strUrl, NULL, SW_SHOW | SW_SHOWMAXIMIZED);
	slog.Format("[HUB]   chrome=[%s]", "C:\\Chrome\\GoogleChromePortable.exe");
	OutputDebugString(slog);

	return 1;
}

BOOL CHUBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	CString slog;
	slog.Format("[HUB]   OnInitDialog ");
	OutputDebugString(slog);
	showHUB();
	SetTimer(9898, 3000, NULL);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHUBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHUBDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHUBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHUBDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString slog;
	switch (nIDEvent)
	{
		case 9898:
			
			slog.Format("[HUB]   OnTimer ");
			OutputDebugString(slog);
			PostMessage(WM_CLOSE, 0, 0);
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
