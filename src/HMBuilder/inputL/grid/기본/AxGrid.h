#pragma once
#include "../Object/fmObject.h"
#include "../../h/fmProperties.h"
#include "../iForm/iForm.h"

// AxGrid

class AxGrid : public CWnd, CfmObject
{
	DECLARE_DYNAMIC(AxGrid)

public:

	AxGrid(class CfmProperties* prop, class CiForm* ifm, CWnd* view);
	virtual ~AxGrid();
	virtual void OnFinalRelease();
private:


public:
	
	CString	GetName(int col = -1);		// 객체명
	int	GetRtsName(int col = -1);	// rts 심볼명
	int	GetIndexByName(CString name);	// for grid

	CRect	GetRect(bool original = false);
	void	SetRect(CRect rect, CRect pRc);
	void	Adjust(CSize size);
	void	Adjust(CPoint pt, float hR, float vR);
	void	Offset(CSize offs);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();

	void	SetFont(int point, int style, CString fonts, bool resize);

	void	SetFocus(bool focus);
	void	SetVisible(bool visible, int col = -1);
	bool	IsVisible(int col = -1);
	void	SetEnable(bool enable);
	bool	IsEnable();

	void	SetFgColor(int rgb, int col = -1, int row = -1);
	void	SetBkColor(int rgb, int col = -1, int row = -1);
	int	GetFgColor(int col = -1, int row = -1);
	int	GetBkColor(int col = -1, int row = -1);
	void	SetDomino(bool redraw = true);

	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	CString	GetHeader();
	int	GetHeaderLength();
	void	SetHeader(CString data);

	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	int	GetRowCount();
	int	GetValidRowCount();
	int	GetColumnCount();

	void	InsertRow(int index, CString data);
	void	RemoveRow(int index);

	bool	IsChanged(bool reset = true);

	void	SetCurSel(int index, bool redraw = true);
	int	GetCurSel();

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func = -1);

	void	Refresh();
	void	Blink(bool set, int rgb = 0);
	void	Blink(bool set, int row, int col, int rgb = 0);

	void	SetLayout(CRect rect, bool visible);

	


protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


