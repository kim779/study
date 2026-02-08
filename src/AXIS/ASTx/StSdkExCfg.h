#ifndef __STSDKEXCFG_H_
#define __STSDKEXCFG_H_


#define STSDKEX_ASTX_REG_ROOT			_T("Software\\Ahnlab\\Safe Transaction")
#define STSDKEX_ASTX_REG_INSTALLPATH		_T("InstallPath")
#define STSDKEX_SDK_NAME				_T("stsdk.dll")
#define STSDKEX_SDK_WOW64_DIR_NAME		_T("Nz32")
#define STSDKEX_SDK_WOW64_NAME			_T("stsdk32.dll")
#define STSDKEX_SDK_TMPPATH				_T("RunTmp")
#define STSDKEX_ASTX_INSTALL_PATH			_T("AhnLab\\Safe Transaction")
#define STSDKEX_ASTX_KEY_FILE				_T("StSess.exe")
#define STSDKEX_FILENAME_STSESS					_T("StSess.exe")
#define STSDKEX_FILENAME_STSDKEX				_T("StSdkEx.dll")
#define STSDKEX_FILENAME_STSDKEX64				_T("StSdkEx64.dll")

// master_url		http://safetx.ahnlab.com/master/win/default/all/astx_setup.exe
// master_url_nt32	http://safetx.ahnlab.com/master/win/default/nt32/astx_setup.exe
// master_url_x64	http://safetx.ahnlab.com/master/win/default/x64/astx_setup.exe
#define	STSDKEX_MASTER_URL				_T("https://safetx.ahnlab.com/master/win/default")
#define	STSDKEX_MASTER_NAME				_T("astx_setup.exe")
#define	STSDKEX_MASTER_PLATFORM_NT32	_T("nt32")
#define	STSDKEX_MASTER_PLATFORM_X64		_T("x64")

#define	STSDKEX_SIGNER_NAME				_T("AhnLab, Inc.")

#endif	// __STSDKEXCFG_H_
