#ifndef __SIGNKOREA_PC_ID_H__
#define __SIGNKOREA_PC_ID_H__

// verison 1.0.0.1  : byte -> unsigned char  , error code 
// version 3.0.0.0  : win8 flip bug 
#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_DESCRIPTION_LENGTH 128

typedef struct _PC_INFO_CONTEXT {
	
	unsigned char DataFlag ;           // information return code 
		#define NO_PRESENT                    0x00
		#define PHYSICALMAC_PRESENT           0x01
		#define HARDDISK_PRESENT              0x02
		#define CPUID_PRESENT                 0x04
		#define NKEY_PRESENT                  0x10 
		#define HKEY_PRESENT                  0x20 
	//---------------------------
	char szOSDesc[MAX_DESCRIPTION_LENGTH];    //OS버전 정보 문자열
	char szCPUDesc[MAX_DESCRIPTION_LENGTH] ;  //CPUID 문자열
	//--------------------------
	unsigned char bHDDriverType ;                  //  0x01 Fixed , 0x02 Removable 
	unsigned char bHDControlerType ;               //  0x01 Master, 0x02 Slave , 0x03~ ATA(SCSI) TYPE
	char szHDModelNumber[MAX_DESCRIPTION_LENGTH] ;  
	char szHDSerialNumber[20+1] ;  
	//--------------------------
	char szAdapterName[MAX_DESCRIPTION_LENGTH];
	char szAdapterDesc[MAX_DESCRIPTION_LENGTH * 2];
	char szPhysicalMAC[12+1] ;            //  "%02X%02X%02X%02X%02X%02X"
	char szCurrentMAC[12+1] ;             //  "%02X%02X%02X%02X%02X%02X"
	char szIPAddress[15+1] ;              //  "000.000.000.000
	char szSubnetMask[15+1] ;             //  "000.000.000.000 
	char szDefaultGateway[15+1] ;         //  "000.000.000.000 
	unsigned int DhcpEnabled;             //   0x00  NO  0x01 YES
	//---------------------------
	char szNKey[28+1];                      // MAC 정보 식별자 
	char szHKey[28+1];                      // HARD DISK 정보 식별자
	char szReserved[MAX_DESCRIPTION_LENGTH]; // 예비 영역

} PC_INFO_CONTEXT;


int GetPCIdentity( PC_INFO_CONTEXT * pContext, int Mode);

    /* Mode */
	#define ALL                    0x00000000
	#define NETDEVICE			   0x00000001   //1
	#define HARDDISK			   0x00000010   //16
	#define CPUID				   0x00000100   //256 
    
	/* Mode :  HARD_DISK Search option  VER 2.0.0.0*/
	#define HARDDISK_OPTION	       0x000000E0   //32    0010  win98 flip의 bug fetch
	#define SEARCH_WIN8_NOFLIP     0x00000020  //+32    
	#define SEARCH_SEQ_NO_RIGHTS   0x00000040  //+64

	/* Mode :  MAC Search option  VER 3.0.0.1*/
	#define NETDEVICE_OPTION       0x0000000E   //14  842
	#define SEARCH_LOCAL_FIRST     0x00000002   //+2  0010  
	#define SEARCH_SKIP_HOTSPOT    0x00000006   //+6  0110  
	/**********************************************************
    HKEY 정보   (HARD DISK SERIAL)   first FIXED DISK    
    NKEY 정보   (MAC ADDRESS )       default NIC physical ADDRESS  
	**********************************************************/


	// API RETURN CODE
	#define  P_OK                  0x00000000 
	#define  P_PARAMETER_IS_NULL   0x00000003 
	#define  P_INVALID_WINDOWS     0x00000005

//--------------------------------------------------------------
int GetDeviceErrorCode(int Device) ;  // 디바이스 오류 확인시 호출 

	// API RETURN CODE

	//NETDEVICE ERROR
	#define  N_NIC_NOT_FOUND       0x00010005
 	#define  N_LOADLIBRARY		   0x00010001
	#define  N_GET_FUNC_ADDRESS    0x00010002
	#define  N_GET_ADAOTERINFO     0x00010003
	#define  N_GET_PMAC_FAIL       0x00010006

	//HARDDISK ERROR
	#define  H_SEARCH_ATA_FAIL     0x00020010 
	#define  H_SEARCH_SCSI_FAIL    0x00020020 
	#define  H_IOCTL_EXCEPTION     0x00020001 
	#define  H_SCSI_EXCEPTION      0x00020002 

	//CPUID ERROR
	#define  P_GET_CPUID_FAIL      0x00040001 
	#define  P_CPUID_EXCEPTION     0x00040001 


#ifdef __cplusplus
}
#endif

#endif