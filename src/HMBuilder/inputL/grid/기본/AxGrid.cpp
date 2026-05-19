// AxGrid.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "oxGrid.h"
#include "AxGrid.h"


// AxGrid

IMPLEMENT_DYNAMIC(AxGrid, CWnd)



AxGrid::AxGrid(CfmProperties* prop, CiForm* ifm, CWnd* view) : CfmObject(prop, ifm, view)
{
	EnableAutomation();
}

AxGrid::~AxGrid()
{
}

void AxGrid::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(AxGrid, CWnd)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(AxGrid, CWnd)
END_DISPATCH_MAP()

// 참고: IID_IAxGrid에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {B8B87AE6-F708-40B7-A3E0-C2410A6DEFFB}
static const IID IID_IAxGrid =
{ 0xB8B87AE6, 0xF708, 0x40B7, { 0xA3, 0xE0, 0xC2, 0x41, 0xA, 0x6D, 0xEF, 0xFB } };

BEGIN_INTERFACE_MAP(AxGrid, CWnd)
	INTERFACE_PART(AxGrid, IID_IAxGrid, Dispatch)
END_INTERFACE_MAP()


// AxGrid 메시지 처리기입니다.


CString	AxGrid::GetName(int col)
{
	return CfmObject::GetName(col);
}


int AxGrid::GetRtsName(int col)	// rts 심볼명
{
	return CfmObject::GetRtsName();
}

int AxGrid::GetIndexByName(CString name) 
{ 
	return 0; 
}

CRect AxGrid::GetRect(bool original)
{
	return CfmObject::GetRect();
}

void AxGrid::SetRect(CRect rect, CRect pRc)
{
}

void AxGrid::Adjust(CSize size)
{
}


void AxGrid::Adjust(CPoint pt, float hR, float vR)
{
}


void AxGrid::Offset(CSize offs)
{
}


void AxGrid::Resize(float hR, float vR, bool font, bool redraw)
{
}

void AxGrid::Reload()
{
}

void AxGrid::SetFont(int point, int style, CString fonts, bool resize)
{
}

void AxGrid::SetFocus(bool focus)		
{ 
	m_focus = focus; 
}


void AxGrid::SetVisible(bool visible, int col)
{
}

bool AxGrid::IsVisible(int col)
{
	return CfmObject::IsVisible();
}


void AxGrid::SetEnable(bool enable)
{
}

bool AxGrid::IsEnable()
{
	return IsWindowEnabled() ? true : false;
}

void AxGrid::SetFgColor(int rgb, int col, int row)
{
}


void AxGrid::SetBkColor(int rgb, int col, int row)
{
}

int AxGrid::GetFgColor(int col, int row )
{
	return CfmObject::GetFgColor(col, row);
}

int AxGrid::GetBkColor(int col, int row)
{
	return CfmObject::GetBkColor(col, row);
}

void AxGrid::SetDomino(bool redraw)			
{
}

void AxGrid::ReadAll(CString& data, bool edit, bool head) 
{
}

void AxGrid::ReadData(CString& data, bool edit, int col, int row) 
{
}	// row == -1 : HEAD


CString	AxGrid::GetHeader() 
{ 
	return _T(""); 
}

int AxGrid::GetHeaderLength() 
{ 
	return 0; 
}

void AxGrid::SetHeader(CString data) 
{
}

void AxGrid::WriteAll(CString str, bool redraw) 
{
}

void AxGrid::WriteData(CString data, bool redraw, int col , int row ) 
{
}

void AxGrid::ClearData(bool selectOnly )		
{
}

int AxGrid::GetRowCount()					
{ 
	return 0; 
}
int AxGrid::GetValidRowCount()				
{
	return 0; 
}
int AxGrid::GetColumnCount()				
{
	return 0; 
}

void AxGrid::InsertRow(int index, CString data)
{
}

void AxGrid::RemoveRow(int index) 
{
}

bool AxGrid::IsChanged(bool reset)			
{ 
	return true;
}

void AxGrid::SetCurSel(int index, bool redraw)	
{
}

int AxGrid::GetCurSel()					
{
	return 0;
}

void AxGrid::GetEnum(CString& text, int& value, int func)	
{
}	// 추후사용 용도

void AxGrid::SetEnum(CString text, int value, int fun)		
{
}	// 추후사용 용도

void AxGrid::Refresh()
{
}

void AxGrid::Blink(bool set, int rgb)
{
}

void AxGrid::Blink(bool set, int row, int col, int rgb)
{
}

void AxGrid::SetLayout(CRect rect, bool visible) 
{
}

//	virtual function