// build.h: interface for the CBuild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUILD_H__CA8C4F80_53EB_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_BUILD_H__CA8C4F80_53EB_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../awCommon/FormItem.h"

class CBuild  
{
public:
	CBuild();
	virtual ~CBuild();

// Attributes
protected:
	bool	m_bOpen, m_bSrc;
	CFile	m_file;
	CString	m_buf, m_sSrc;

	struct	_mapH*		m_mapH;
	CFormItem		*m_pFormItem;
	struct	_fgraph*	m_grR;

//Implementation
public:
	void	hintType(BYTE isHint);
	CString Parser(CString &srcstr, CString substr);
	void	taborder(WORD idx);
	int	GenerateSource(bool alarm, struct _mapH* mapH);
	CString	GetSource(bool bNotResource, struct _mapH* mapH);

protected:
	void	ioorder(WORD idx);
	void	groupN(WORD idx);
	void	generateHeader();
	void	generateForm();
	void	generateResource();

	void	srcSys(int fN);
	void	srcLabel(int fN);
	void	srcBox(int fN);
	void	srcGroup(int fN);
	void	srcTab(int fN);
	void	srcUsrTab(int fN);
	void	srcPanel(int fN);
	void	srcButton(int fN);
	void	srcAVI(int fN);
	void	srcObject(int fN);
	void	srcGraph(int fN);
	void	srcEdit(int fN);
	void	srcCombo(int fN);
	void	srcGrid(int fN);
	void	srcMemo(int fN);
	void	srcTable(int fN);
	void	srcResizeBar(int fN);
	void	srcTreeView(int fN);
	void	srcSheet(int fN);
	void	srcUserOb(int fN);
	void	srcBrowser(int fN);
	void	srcControl(int fN);

	void	symbolRect(int fN, CRect fmRc = CRect(0, 0, 0, 0));
	void	attributes(DWORD fattr);
	void	subattributes(DWORD fattr, DWORD sattr);
	void	color(DWORD pRGB, DWORD tRGB);
	void	dataType(BYTE type);
	void	alignment(BYTE align);
	void	writeEtc(int fN);
	void	writeAuxiliary(int fN);
	void	writeColumns(struct _repR* repR);
	void	writeRows(struct _repR* repR);

	void	writeFile(CString buf);
	void	writeDataToFile(char* buf);
	void	writeScriptData(char* dir);
	void	addOR(CString& buf);
	void	brace(bool begin);
	void	space(int count);
	void	newline(bool iscontinue);
};

#endif // !defined(AFX_BUILD_H__CA8C4F80_53EB_11D4_A024_00001CD7F9BE__INCLUDED_)
