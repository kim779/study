/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 10073 $
 * 
 */

#ifndef STSDKCFG_H
#define STSDKCFG_H

// AhnLab Safe Transaction MKD Reigstry
#define ASTX_REG_HKEY				HKEY_LOCAL_MACHINE
#define ASTX_REG_ROOT				_T("SOFTWARE\\Ahnlab\\Safe Transaction")
#define ASTX_REG_INSTALLPATH		_T("InstallPath")
#define ASTX_SDK_NAME				_T("stsdk.dll")
#define ASTX_SDK_WOW64_DIR_NAME		_T("Nz32")
#define ASTX_SDK_WOW64_NAME			_T("stsdk32.dll")

#define ASTX_SDK_CUSTOM_POLICY_URL	_T("http://webclinic.ahnlab.com/astx/policy/customer_stsdk_default.html")


#define STSDK_FUNC_CODE_PB			0x00000004	// ASTx Protect Browser
#define STSDK_FUNC_CODE_FW			0x00000080	// ASTx Firewall



#endif //STSDKCFG_H