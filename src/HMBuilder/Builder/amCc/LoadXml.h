
#pragma once

#include <MSXML2.H>
#pragma comment (lib, "msxml2.lib")
#pragma comment (lib, "comsuppw.lib")

#include "afxcoll.h"

#include "../amCom/FormItem.h"
#include "../h/mapvar.h"
// CLoadXml 명령 대상입니다.

class CLoadXml
{
public:
	CLoadXml();
	virtual ~CLoadXml();
	
protected:
	struct	_mapH*		m_mapH;
	CFormItem*		m_pFormItem;
	IXMLDOMDocument*	m_pReadDoc;
	CMapStringToString	m_cMapHeader;
	CMapStringToString	m_cControlForm;
	CMapStringToString	m_cScript;
	BSTR			m_ParentNodename;

	int	m_multi;
	int	m_rtype;
	int	m_formP,  m_colP;
	int	m_formN;
	bool	m_column;
	CString m_curdir;
	int	m_fN;
	bool	m_bfirst;
	int	m_colcnt;
	int	m_col, m_row;
	CMap <int, int, int, int> m_groupArr;
	enum	{NONE = 0, GLOBAL, CONTROL, TRLAYOUT, RTSSYMBOL, LAYOUT} m_readKind;
	CCellItem *m_cellItem;
	bool	m_bgridtable;

public:
	CMapStringToString*	m_control;

public:
	void Load(struct _mapH* mapH, CString curdir = _T(""));
	void LoadXml(CString sPath = _T(""));
	void XmlParser(IXMLDOMNode* pNode);
	bool GetXmlAttribute(IXMLDOMNode* pNode);
	void removeIXMLDOMElement(IXMLDOMElement* pElement = NULL);
	void removeIXMLDOMNode(IXMLDOMNode* pNode = NULL);
	void removeIXMLDOMAttribute(IXMLDOMAttribute* pAttribute = NULL);
	void removeIXMLDOMNamedNodeMap(IXMLDOMNamedNodeMap* pNodename = NULL);

	void SetFormData(CString parentnode, CString node, CString sKey, CString dat);
	void SetGlobal(CString sKey, CString dat);
	void SetControl(CString parentnode, CString node, CString sKey, CString dat, int& fN);
	void SetScript(CString sKey, CString dat, int fN = -1);
	void SetMulticolControl(CString parentnode, CString node, CString sKey, CString dat, int& fN);
	void setTrLayout(CString sKey, CString dat);
	void setRtsSymbol(CString sKey, CString dat);
	void setLayout(CString sKey, CString dat);

	char* SetStr(char *map, CString data);
	CString Parser(CString &srcstr, CString substr);
	int getFormIndex(char* symbol, int len, int count);
	int getColumnIndex(char* symbol, int len, int pos);
	void SaveScriptFile(CString sPath, CString sScript, int size  = -1);

	CString	FmKind(CString tmp);
	void RemoveCMap();
	int  formcount();
	int  ARGBtoRGB(int color);
	void newCellitem();

	void SetTrRtsInfo();

	void checkGridMultiLine(int fN);
};


