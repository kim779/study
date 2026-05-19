
#pragma once

class CBuild
{
public:
	CBuild();
	virtual ~CBuild();

protected:
	bool	m_open;
	bool	m_getsource;
	CFile	m_file;
	int	m_iTab;

	CString	m_source;

	class mapForm*	m_mapH;

public:
	int	GenerateSource(bool alarm, class mapForm* mapForm);
	CString	GetSource(bool notResource, class mapForm* mapForm);
	void	SetTab(bool bTab);
	void	InitTab();

protected:
	void	generateHeader();
	void	generateForm();
	void	generateResource();
	void	generateTRlayout();
	void	generateRTSSymbol();
	void	generateLayoutEdit();

	void	srcSys(int fN);
	void	srcLabel(int fN);
	void	srcBox(int fN);
	void	srcGroup(int fN);
	void	srcTab(int fN);
	void	srcPanel(int fN);
	void	srcButton(int fN);
	void	srcForm(int fN);
	void	srcEdit(int fN);
	void	srcCombo(int fN);
	void	srcGrid(int fN,char kind);
	void	srcMemo(int fN);
	void	srcTable(int fN);
	void	srcTreeView(int fN);
	void	srcBrowser(int fN);
	void	srcObject(int fN);

	void	symbolRect(int fN);
	void	attributes(DWORD fattr);
	void	subattributes(DWORD fattr, DWORD sattr);
	void	color(DWORD pRGB, DWORD tRGB);
	void	dataType(BYTE type);
	void	alignment(BYTE align, CString buf);

	void	hintType(BYTE isHint, CString buf);
	CString	parse(CString& src, CString subs);
	void	tabOrder(WORD idx);
	void	ioOrder(WORD idx);
	void	groupN(WORD idx);

	void	writeEtc(int fN);
	void	writeAuxiliary(int fN);
	void	writeColumns(CObArray* arryCellProp);
	void	writeRows(CObArray* arryCellProp);

	void	writeWithNL(CString buf, bool iscontinue = false);
	void	writeFile(CString buf, bool bTab = true);
	void	writeDataToFile(char* buf);
	void	writeScriptData(char* dir);
	void	addOR(CString& buf);
	void	brace(bool begin);
	void	space(int count);
	void	newline(bool iscontinue = false);
};


