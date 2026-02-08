// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_PCIdentity.h"
#include "MainWnd.h"
#include "Sha256.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

void CMainWnd::WriteLog( LPCSTR log)
{
	CString sRoot = (char*)m_parent->SendMessage(WM_USER,MAKEWPARAM(variantDLL, homeCC), (LPARAM)0);

	FILE *fp = fopen( sRoot + "\\PCIdentity.log", "a+");

	OutputDebugString(sRoot + "\\PCIdentity.log");
	if (!fp) return;
	
	CTime time = CTime::GetCurrentTime();
	fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));
	
	va_list argptr;
	va_start(argptr, log);
	vfprintf(fp, log, argptr);
	va_end(argptr);
	fprintf(fp, "\n");
	
	fclose(fp);
}

CMainWnd::CMainWnd(CWnd* parent)
{
	EnableAutomation();
	
	m_parent = parent;
	//m_pContext = NULL;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetPCData", GetPCData, VT_BSTR, VTS_I2)
	DISP_FUNCTION(CMainWnd, "GetOSDesc", GetOSDesc, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetCPUDesc", GetCPUDesc, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Load", Load, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetMAC", GetMAC, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {42298071-E779-49E7-B77D-352349BD40A6}
static const IID IID_IMainWnd =
{ 0x42298071, 0xe779, 0x49e7, { 0xb7, 0x7d, 0x35, 0x23, 0x49, 0xbd, 0x40, 0xa6 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

void CMainWnd::SetProperties(LPCTSTR sParam) 
{
	// TODO: Add your dispatch handler code here

}

BSTR CMainWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// 	int nResult = GetPCIdentity(m_pContext,0);
// 
// 	if(nResult == P_PARAMETER_IS_NULL)
// 	{
// 		AfxMessageBox("Input parameter is NULL");
// 		
// 		return 0;
// 	}
// 	else if(nResult == P_INVALID_WINDOWS)
// 	{
// 		AfxMessageBox("지원가능 OS 확인 불가");
// 
// 		return 0;
// 	}
	//LoadData();
	
	return 0;
}

void CMainWnd::LoadData()
{
	CString			err;

	int info;
	int err_code;
	
	/* 단말정보 획득 API 호출 */
	info = GetPCIdentity(&m_pContext, 32+6);
	/* mode =
		ALL			0x00000000
		NETDEVICE		0x00000001	//1
		HARDDISK		0x00000010	//16
		CPUID			0x00000100	//256 
    
		HARD_DISK Search option  VER 2.0.0.0
		HARDDISK_OPTION		0x000000E0	//32    0010  win98 flip의 bug fetch
		SEARCH_WIN8_NOFLIP	0x00000020	//+32    
		SEARCH_SEQ_NO_RIGHTS	0x00000040	//+64

		MAC Search option  VER 3.0.0.1
		NETDEVICE_OPTION       0x0000000E	//14  842
		SEARCH_LOCAL_FIRST     0x00000002	//+2  0010  
		SEARCH_SKIP_HOTSPOT    0x00000006	//+6  0110  
	*/

	if(info != P_OK)	// 호출 성공이 아닐 경우 에러 처리
	{
		AfxMessageBox("API ERROR");
		CString s;
		s.Format("PC IDENTITY ERROR [%d]\n",info);
		OutputDebugString(s);
		WriteLog(s);
		return;
	}

	/*
		# API RETURN CODE
			P_OK                  0x00000000  - 호출 성공시 반환 값
			P_PARAMETER_IS_NULL   0x00000003  - Input PARAMETER IS NULL
			P_INVALID_WINDOWS     0x00000005  - 지원 가능 OS 확인 불가
	
	*/
	
	m_os = m_pContext.szOSDesc;				// 윈도우 버전 정보
	
	/* CPU */
	if(m_pContext.DataFlag & 0x04)
	{
		m_cpu = m_pContext.szCPUDesc;			// CPU ID
		
	}
	else
	{
		err_code = GetDeviceErrorCode(CPUID);
		err.Format("CPU 정보 추출 실패. Error_Code : [%d]", err_code);
		WriteLog(err);
		//MessageBox(err);
	}
	
	/* NETWORK */
	if(m_pContext.DataFlag & 0x01)
	{

		m_n_mac_key			= m_pContext.szNKey;				// MAC 정보 식별자
		m_n_adap_name		= m_pContext.szAdapterName;		// 네트워크 어댑터 장치
		m_n_adap_desc		= m_pContext.szAdapterDesc;		// 네트워크 어댑터 Description
		m_n_physical_mac	= m_pContext.szPhysicalMAC;		// 제조사에서 설정한 물리적 주소
		m_n_current_mac		= m_pContext.szCurrentMAC;		// 네트워크에서 사용하는 물리적 주소
		m_n_ip_address		= m_pContext.szIPAddress;			// IP Address
		m_n_subnetmask		= m_pContext.szSubnetMask;		// IP Address의 SubnetMask
		m_n_gateway	= m_pContext.szDefaultGateway;			// GateWay
		
		m_n_dhcp_enabled.Format("%d", m_pContext.DhcpEnabled);	// dynamic host configuration protocol (DHCP)  네트워크 어댑터 설정여부
		if(m_pContext.DhcpEnabled & 0x01)
		{
			m_n_dhcp_enabled2 = "YES";
		}
		else
		{
			m_n_dhcp_enabled2 = "NO";
		}
	}
	else
	{
		err_code = GetDeviceErrorCode(NETDEVICE);
		err.Format("NETWORK 정보 추출 실패. Error_Code : [%d]", err_code);
		//MessageBox(err);
		WriteLog(err);
	}

	/* HARD_DISK */
	if(m_pContext.DataFlag & 0x02)
	{
		m_h_hdd_key			= m_pContext.szHKey;					// 하드디스크 정보 식별자
		m_h_serial_number	= m_pContext.szHDSerialNumber;		// 하드디스크 고유번호

		m_h_drive_type.Format("%d", m_pContext.bHDDriverType);	// 하드디스크 타입 (현재 Fixed Disk 만 추출, USB Type의 이동형 디스크 추출 제외)
		if(m_pContext.bHDControlerType & 0x01)
		{
			m_h_drive_type2 = "Fixed";
		}
		else
		{
			m_h_drive_type2 = "Removable";
		}
		m_h_controler_type.Format("%d", m_pContext.bHDControlerType);		// IDE 계열인 경우 Master / Slave만 구분
		if(m_pContext.bHDControlerType & 0x01)
		{
			m_h_controler_type2 = "Master";
		}
		else if(m_pContext.bHDControlerType & 0x02)
		{
			m_h_controler_type2 = "Slave";
		}
		else
		{
			m_h_controler_type2 = "ATA(SCSI) TYPE";
		}

		m_h_model_number.Format("%s", m_pContext.szHDModelNumber);	// 제조업체가 부여한 모델명
	
	}
	else
	{
		err_code = GetDeviceErrorCode(HARDDISK);
		err.Format("HARD_DISK 정보 추출 실패. Error_Code : [%d]", err_code);
		//MessageBox(err);
		WriteLog(err);
	}

	//WriteLog("PCIdentity Success!");
}

CString CMainWnd::GetData(int nType)
{
	CString strReturn;

	switch (nType)
	{
	case 0://bHDDriverType
		strReturn = m_h_drive_type;
		break;
	case 1://bHDControlerType
		strReturn = m_h_drive_type2;
		break;
	case 2://szHDModelNumber
		strReturn = m_h_model_number;
		break;
	case 3://szHDSerialNumber
		strReturn = m_h_serial_number;
		break;
	case 4://HDD - szHKey
		strReturn = m_h_hdd_key;
		break;
	case 5://szAdapterName
		strReturn = m_n_adap_name;
		break;
	case 6://szAdapterDesc
		strReturn = m_n_adap_desc;
		break;
	case 7://szPhysicalMAC
		strReturn = m_n_physical_mac;
		break;
	case 8://szCurrentMAC
		strReturn = m_n_current_mac;
		break;
	case 9://szIPAddress
		strReturn = m_n_ip_address;
		break;
	case 10://szSubnetMask
		strReturn = m_n_subnetmask;
		break;
	case 11://szDefaultGateway
		strReturn = m_n_gateway;
		break;
	case 12://DhcpEnabled
		strReturn = m_n_dhcp_enabled;
		break;
	case 13:
		strReturn = m_n_mac_key;
		break;
	}

	return strReturn;
}
#if 0
#include <nb30.h>
#include <Iphlpapi.h>

#pragma comment (lib, "IpHlpApi.lib")

CString CMainWnd::GetMacAddress()
{
    CString strMacAddress = _T("00.00.00.00.00.00");
    IP_ADAPTER_INFO AdapterInfo[5];       // Allocate information
    // for up to 16 NICs
    DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer
	
    DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
        AdapterInfo,                 // [out] buffer to receive data
        &dwBufLen);                  // [in] size of receive data buffer
    //    assert(dwStatus == ERROR_SUCCESS);  // Verify return value is
    // valid, no buffer overflow
    if(dwStatus != ERROR_SUCCESS)
    {
        strMacAddress.Format(_T("Error : %d "),dwStatus) ;
        return strMacAddress ;
    }
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
    // MAX_ADAPTER_ADDRESS_LENGTH  8
    strMacAddress.Format(_T("%02X.%02X.%02X.%02X.%02X.%02X"),
        pAdapterInfo->Address[0],
        pAdapterInfo->Address[1],
        pAdapterInfo->Address[2],
        pAdapterInfo->Address[3],
        pAdapterInfo->Address[4],
        pAdapterInfo->Address[5]);
	
    return strMacAddress;
}
#endif

BSTR CMainWnd::GetPCData(short nType) 
{
	CString strResult;
	
	strResult = GetData(nType);
	return strResult.AllocSysString();
}

BSTR CMainWnd::GetOSDesc() 
{
	CString strResult;
	
	strResult = m_os;

	return strResult.AllocSysString();
}

BSTR CMainWnd::GetCPUDesc() 
{
	CString strResult;
	
	strResult = m_cpu;

	return strResult.AllocSysString();
}

void CMainWnd::Load() 
{
	// TODO: Add your dispatch handler code here
	LoadData();
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CMainWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnClose();
}

BSTR CMainWnd::GetMAC() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult = GetData(7);

// 	CString s;
// 	s.Format("MAC ADDRESS [%s]\n",strResult);
// 	OutputDebugString(s);

	char* mac;

	mac = (char*)String2Binary(strResult,12);
	
// 	s.Format("MAC ADDRESS [%s]\n",mac);
// 	OutputDebugString(s);

	char* base64encode = NULL;
	
	int base64_size = base64_encode(mac,6,&base64encode);

	CString encode(base64encode);

// 	s.Format("MAC ADDRESS [%s]\n",encode);
// 	OutputDebugString(s);

	return encode.AllocSysString();
}

LPBYTE CMainWnd::String2Binary(LPCTSTR strData, int nDataLen) 
{ 
	// Reject if not an even number of digits 
	if (nDataLen % 2 != 0) 
		return NULL; 
	
	
	// Convert the hex string to binary 
	BYTE* pData = new BYTE[nDataLen / 2]; 
	for (int i = 0; i < nDataLen; i+=2) 
	{ 
		BYTE bByte; 
		BYTE bSource = toupper(strData[i]); 
		if (bSource >= 'A' && bSource <= 'F') 
			bByte = bSource - 'A' + 10; 
		else if (bSource >= '0' && bSource <= '9') 
			bByte = bSource - '0'; 
		else 
		{ 
			// Invalid hex digit 
			delete[] pData; 
			return NULL; 
		} 
		bByte *= 16; 
		
		
		bSource = toupper(strData[i+1]); 
		if (bSource >= 'A' && bSource <= 'F') 
			bByte += bSource - 'A' + 10; 
		else if (bSource >= '0' && bSource <= '9') 
			bByte += strData[i+1] - '0'; 
		else 
		{ 
			// Invalid hex digit 
			delete[] pData; 
			return NULL; 
		} 
		pData[i/2] = bByte; 
	} 
	return pData; 	
}
