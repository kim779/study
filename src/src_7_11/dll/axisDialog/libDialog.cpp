#include "stdafx.h"
#include "libdialog.h"
#include "axisdialog.h"
#include "columns.h"

#include "../../h/axisfire.h"

/*	1. src : 선택 가능한 전체 column에 관한 데이타
		심볼명1 | 헤드문자열1 | 심볼명2 | 헤드문자열2 ....

	2. items : 현재 사용중인 column에 관한 데이타
		심볼명1 | 심볼명 2 ...

	3. [기본설정으로 되돌리기]에 사용될 default column에 관한 데이타 
		심볼명1 | 심볼명 2 ...

	return 심볼명1 | 심볼명 2 ...
*/

__declspec(dllexport) void* APIENTRY axDialog(int kind, char * root, char* src, char* items, char* defs)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CColumns* columns;
	columns = new CColumns(root, src, items, defs, NULL);
	if (columns->DoModal() == IDOK)
	{
		CAxisdialogApp* app = (CAxisdialogApp *)AfxGetApp();
		app->m_returns = columns->GetItems();
		delete columns;
		return (char *)app->m_returns.operator LPCTSTR();
	}
	delete columns;
	
	return NULL;
}
