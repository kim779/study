#ifndef __STSDKMKD25_H_
#define __STSDKMKD25_H_


/*****************************************************************************
 *
 *	Edit 타입을 정의합니다.
 *
 *	AstxtypeNumber : 이 값이 설정되면 Number 인 경우만 처리합니다.
 * 
 **************************************************************************/
enum AstxMkdEditType
{
	AstxtypeNumber				= 0x0001,
	AstxtypeChar				= 0x0002,
	AstxtypeSpecial				= 0x0004
};

/*****************************************************************************
 *
 *	Edit Control 의 동작 모드를 정의합니다.
 *
 *	AstxmodeUserInsert : 이 값이 설정되면 Insert Key를 사용할 수 있습니다. 
 *
 *	AstxmodeUseEsc : 이 값이 설정되면 ESC Key를 사용할 수 있습니다. ESC 키가 눌리면 
 *		값을 내부 메모리를 Clear시킵니다. 내부 값만 Clear 되므로 
 *		표시되는 값을 Clear 시켜야 합니다.
 *
 *	AstxmodeDeleteLimit : 이 값이 설정되면 커서의 인덱스가 0 인 상태인 경우 
 *	    Backspace로 값을 지우고자 할 경우 사용됩니다. 보통은 커서가
 *		맨 앞에 있는 경우에는 Backspace로 값을 지울 수 없으나 지우고자 할 경우는 
 *		이 값을 설정하여야 합니다.
 *
 *	AstxmodeProtectLevelLow : 이 값이 설정되면 Text 타입에 한하여
 *  낮은 수준의 보호를 하게 됩니다.
 *  보호레벨이 낮으므로 입력값의 검증 정도의 용도로 사용하기를 권합니다. 
 * 
 ***************************************************************************/

enum AstxMkdEditMode
{
	AstxmodeUseInsert			= 0x0001,
	AstxmodeUseEsc				= 0x0002,
	AstxmodeDeleteLimit			= 0x0004,
	AstxmodeProtectLevelLow		= 0x0008
};

/*****************************************************************************
 *
 *	AhnLab Safe Transaction MKD 의 초기 모드를 설정합니다.
 *
 *	AstxmodeProtect : 이 값이 설정되면 보호 모드로 동작하게 됩니다.
 *
 *	AstxmodeBypass  : 이 값이 설정되면 보호 모드로 동작하지 않습니다.
 *
 *	AstxmodeProtectSafeNet  : 이 값이 설정되면 SafeNet 모드로 동작합니다. 
 *
 *	AstxmodeProtectInSafeNet  : 이 값이 설정되면 InSafeNet 모드로 동작합니다.
 * 
 *	AstxmodeProtectInAstx  	: 이 값이 설정되면 ASTx 연동 모드로 동작합니다. 
 * 
 ***************************************************************************/
 
enum AstxMkdInitMode
{
	AstxmodeProtect				= 0x0001,
	AstxmodeBypass				= 0x0002,
	AstxmodeProtectSafeNet		= 0x0004,
	AstxmodeProtectInSafeNet	= 0x0008,
	AstxmodeProtectInAstx		= 0x0010
};

/*****************************************************************************
 *
 *	ASTx MKD Interface
 *
 ***************************************************************************/

typedef int (__stdcall* PFN_AKSDK_Callback)(long lCode, LPARAM lParam);

class IAstxMkd8A
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, PFN_AKSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectEditControl(HWND hWnd) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall GetText(HWND hWnd, CHAR* szOutput, DWORD* pdwSize) = 0;
	virtual DWORD _stdcall SetText(HWND hWnd, CONST CHAR* lpszText) = 0;
	virtual DWORD _stdcall GetEncText(DWORD dwCustomCode, HWND hWnd, CHAR* szOutput, DWORD* pdwSize, BYTE bzRandom[]) = 0;
};

class IAstxMkd8W
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, PFN_AKSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectEditControl(HWND hWnd) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall GetText(HWND hWnd, WCHAR* szOutput, DWORD* pdwSize) = 0;
	virtual DWORD _stdcall SetText(HWND hWnd, CONST WCHAR* lpszText) = 0;
	virtual DWORD _stdcall GetEncText(DWORD dwCustomCode, HWND hWnd, WCHAR* szOutput, DWORD* pdwSize, BYTE bzRandom[]) = 0;

};

class IAstxAkA
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, PFN_AKSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall GetEncText(IN DWORD dwCustomCode, IN CHAR* szE2EWindowID, IN CHAR* szE2EFormID, IN CHAR* szE2EInputID, OUT CHAR* szOutput, OUT DWORD* pdwSize, OUT BYTE bzRandom[]) = 0;

};

class IAstxAkW
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, PFN_AKSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall GetEncText(IN DWORD dwCustomCode, IN WCHAR* szE2EWindowID, IN WCHAR* szE2EFormID, IN WCHAR* szE2EInputID, OUT WCHAR* szOutput, OUT DWORD* pdwSize, OUT BYTE bzRandom[]) = 0;

};

#endif // __STSDKMKD25_H_
