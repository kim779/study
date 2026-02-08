/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 1006 $
 * 
 */

#ifndef AOSSDKCOM_H
#define AOSSDKCOM_H

/*****************************************************************************
 *
 *	Edit 타입을 정의합니다.
 *
 *	typeNumber : 이 값이 설정되면 Number 인 경우만 처리합니다.
 * 
 ***************************************************************************/
enum AosMkdEditType
{
	typeNumber	= 0x0001,
	typeChar	= 0x0002,
	typeSpecial = 0x0004
};

/*****************************************************************************
 *
 *	Edit Control 의 동작 모드를 정의합니다.
 *
 *	modeUserInsert : 이 값이 설정되면 Insert Key를 사용할 수 있습니다. 이 값이 
 *		설정되면 SetInsertMode 함수를 사용하여 Insert 상태를 변경할 수 
 *		있습니다.
 *
 *	modeUseEsc : 이 값이 설정되면 ESC Key를 사용할 수 있습니다. ESC 키가 눌리면 
 *		값을 내부 메모리를 Clear시킵니다. 내부 값만 Clear 되므로 
 *		표시되는 값을 Clear 시켜야 합니다.
 *
 *	modeDeleteLimit : 이 값이 설정되면 커서의 인덱스가 0 인 상태인 경우 
 *	    Backspace로 값을 지우고자 할 경우 사용됩니다. 보통은 커서가
 *		맨 앞에 있는 경우에는 Backspace로 값을 지울 수 없으나 지우고자 할 경우는 
 *		이 값을 설정하여야 합니다.
 *
 *	modeProtectLevelLow : 이 값이 설정되면 Text 타입에 한하여
 *  낮은 수준의 보호를 하게 됩니다.
 *  보호레벨이 낮으므로 입력값의 검증 정도의 용도로 사용하기를 권합니다. 
 * 
 ***************************************************************************/

enum AosMkdEditMode
{
	modeUseInsert				= 0x0001,
	modeUseEsc					= 0x0002,
	modeDeleteLimit			= 0x0004,
	modeProtectLevelLow	= 0x0008
};

/*****************************************************************************
 *
 *	AOS MKD 의 초기 모드를 설정합니다.
 *
 *	modeProtect : 이 값이 설정되면 보호 모드로 동작하게 됩니다.
 *
 *	modeBypass  : 이 값이 설정되면 보호 모드로 동작하지 않습니다.
 * 
 ***************************************************************************/
 
enum AosMkdInitMode
{
	modeProtect		= 0x0001,
	modeBypass		= 0x0002
};

/*****************************************************************************
 *
 *	AOS MKD Interface
 *
 ***************************************************************************/
 
class IAosMkdA
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, BOOL bIntegratedWithAos = FALSE) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectEditControl(HWND hWnd) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall GetText(HWND hWnd, CHAR* szOutput, DWORD* pdwSize) = 0;
	virtual DWORD _stdcall SetText(HWND hWnd, CONST CHAR* lpszText) = 0;
	virtual DWORD _stdcall GetTextType(HWND hwnd, DWORD* pdwType) = 0;
	virtual DWORD _stdcall SetInsertMode(HWND hwnd, BOOL bInsert) = 0;
	virtual DWORD _stdcall Uninitialize2() = 0;
};

class IAosMkdW
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, BOOL bIntegratedWithAos = FALSE) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectEditControl(HWND hWnd) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall GetText(HWND hWnd, WCHAR* szOutput, DWORD* pdwSize) = 0;
	virtual DWORD _stdcall SetText(HWND hWnd, CONST WCHAR* lpszText) = 0;
	virtual DWORD _stdcall GetTextType(HWND hwnd, DWORD* pdwType) = 0;
	virtual DWORD _stdcall SetInsertMode(HWND hwnd, BOOL bInsert) = 0;
	virtual DWORD _stdcall Uninitialize2() = 0;
};


class IAosMkdS4
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, HWND hMainWnd, BOOL bIntegratedWithAos) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall StartProtect() = 0;
	virtual DWORD _stdcall StopProtect() = 0;
	virtual DWORD _stdcall Uninitialize2() = 0;
};

/*****************************************************************************
 *
 *	AOS Verify Interface
 *
 ***************************************************************************/

class IAosVerify
{
public:
	virtual BOOL _stdcall IsRunningPd(DWORD dwServiceCode) = 0;
	virtual BOOL _stdcall VerifyTrustSelfA(LPCSTR szSignedName) = 0;
	virtual BOOL _stdcall VerifyTrustSelfW(LPCWSTR swSignedName) = 0;
	virtual BOOL _stdcall VerifyTrustSDK() = 0;
};

#endif // AOSSDKCOM_H
