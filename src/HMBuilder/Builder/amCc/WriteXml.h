
#pragma once

#include <MSXML2.H>
#pragma comment (lib, "msxml2.lib")
#pragma comment (lib, "comsuppw.lib")

#include "Status.h"
#include "../amCom/FormItem.h"
#include "../amCom/CellItem.h"
// CWriteXml 명령 대상입니다.

#define	GB_CLASS	0x08
#define GB_PROPERT	0x0c
#define GB_SUITEM	0x7f
#define GB_SCRIPT	0x0e

#define	GB_TR		0x08
#define GB_SYMBOL	0x0c
#define GB_INOUT	0x0e

class CWriteXml
{
public:
	CWriteXml();
	virtual ~CWriteXml();

public:
	IXMLDOMDocument* m_pWriteDoc;
	IXMLDOMElement*  m_FormElement;
	IXMLDOMElement*  m_TrLayoutElement;
	IXMLDOMElement*  m_RtsSymbolElement;

	IXMLDOMElement*  m_LayoutElement;

	CStatus		*m_status;
	bool		m_bOpen, m_bSrc;
	CFile		m_file;
	CString		m_buf, m_sSrc;
	CFormItem	*m_pFormItem;
	struct _mapH	*m_mapH;
	CString		m_Xmlpath;
	CString		m_Binpath;
	int		m_inputidx;
	int		m_outputidx;
//	CMapStringToString	m_control;

	enum		{NONE = 0, PROPERT, PROPERTLAST, SCRIPT, SUBITEM, NEWFORM, TRLAYOUT} m_writeType;

	CMapStringToString	m_trlayoutxml;
	CMapStringToString	m_rtssymbolxml;
	CMapStringToString*	m_control;

public:
	CString	GetSource(bool bNotResource, struct _mapH* mapH);
	int	GenerateSource(bool alarm, struct _mapH* mapH, bool getsource = false);
	void	GenerateHeader(IXMLDOMElement* pElement = NULL);
	void	GenerateForm();
	void	generateFormScript(IXMLDOMElement* pElement);
	void	generateControlScript(int fN, IXMLDOMElement* pElement);
	void	writeScriptData(char* dir, CString name, IXMLDOMElement* pElement = NULL);
	void	writeDataToFile(char* buf);

	IXMLDOMElement* BaseWrite(CString kind, int fN, IXMLDOMElement* pElement);
	IXMLDOMElement* MakeElement(IXMLDOMElement* pElement, CString name = _T(""), CString sValue = _T(""), bool bresource = true);
	IXMLDOMElement* MakeElementDiff(IXMLDOMElement* pElement, CString name, CString sXML = _T(""), CString sRXML = _T(""));
	void	MakChildeNode(bool bChild, IXMLDOMNode* pNode, CString name = _T(""), CString text = _T(""));
	void	SetElementToAttribute(IXMLDOMElement* pElement, CString name, CString value);
	void	SetElementToText(IXMLDOMElement* pElement, CString text);

	void	removeIXMLDOMElement(IXMLDOMElement* pElement = NULL);
	void	removeIXMLDOMNode(IXMLDOMNode* pNode = NULL);
	void	removeIXMLDOMAttribute(IXMLDOMAttribute* pAttribute = NULL);
	void	removeIXMLDOMNamedNodeMap(IXMLDOMNamedNodeMap* pNodename = NULL);

	void	srcLabel(int fN, IXMLDOMElement* pElement = NULL);
	void	srcBox(int fN, IXMLDOMElement* pElement = NULL);
	void	srcGroup(int fN, IXMLDOMElement* pElement = NULL);
	void	srcImageVW(int fN, IXMLDOMElement* pElement = NULL);
	void	srcButton(int fN, IXMLDOMElement* pElement = NULL);
	void	srcCheck(int fN, IXMLDOMElement* pElement = NULL);
	void	srcRadio(int fN, IXMLDOMElement* pElement = NULL);
	void	srcEdit(int fN, IXMLDOMElement* pElement = NULL);
	void	srcCombo(int fN, IXMLDOMElement* pElement = NULL);
	void	srcGrid(int kind, int fN, IXMLDOMElement* pElement = NULL);
	void	srcTable(int fN, IXMLDOMElement* pElement = NULL);
	void	srcObject(int fN, IXMLDOMElement* pElement = NULL);
	void	srcTab(int fN, IXMLDOMElement* pElement = NULL);
	void	srcBrowser(int fN, IXMLDOMElement* pElement = NULL);
	void	srcControl(int fN, IXMLDOMElement* pElement = NULL);

	void	symbolRect(int fN, IXMLDOMElement* pElement);
	void	alignment(BYTE align, IXMLDOMElement* pElement = NULL);
	void	alignmentImage(BYTE align, IXMLDOMElement* pElement = NULL);
	void	attributes(DWORD attr, IXMLDOMElement* pElement = NULL);
	void	cellFont(struct _cellR *cellR, IXMLDOMElement* pElement = NULL);
	void	formFont(int fN, IXMLDOMElement* pElement = NULL);	
	void	formcolor(DWORD pRGB, DWORD tRGB, IXMLDOMElement* pElement = NULL);
	void	formselcolor(DWORD spRGB, DWORD stRGB, IXMLDOMElement* pElement = NULL);
	void	taborder(WORD idx, IXMLDOMElement* pElement = NULL);
	void	ioorder(int fN, IXMLDOMElement* pElement = NULL);
	void	dataType(BYTE type, IXMLDOMElement* pElement = NULL);
	void	subattributes(DWORD attr, DWORD attr2, IXMLDOMElement* pElement = NULL);
	void	writeAuxiliary(DWORD properties, IXMLDOMElement* pElement = NULL);
	void	writeIOK(int fN, IXMLDOMElement* pElement = NULL);
	void	writeBoders(int fN, IXMLDOMElement* pElement = NULL);
	void	writeGridOption(int fN, IXMLDOMElement* pElement = NULL);
	void	writeRows(struct _repR* repR, IXMLDOMElement* pElement = NULL);
	void	writeColumns(int kind, struct _repR* repR, IXMLDOMElement* pElement = NULL);
	void	writeCellKind(BYTE kind, IXMLDOMElement* pElement = NULL);
	void	writeEtc(int fN, IXMLDOMElement* pElement);
	void	groupN(WORD idx, IXMLDOMElement* pElement = NULL);
	void	writeFile(CString buf = _T(""), CString name = _T(""));
	void	Encrypt(CString path);

	void	writeTrlayout();
	void	writeRtsSymbol();
	void	writeLayout();

	CString Parser(CString &srcstr, CString substr);
	CString ParserDescription(CString &srcstr, CString substr);
	DWORD	RGBtoARGB(DWORD color);
	int	FontHeight(char* fontN, int point, bool italic, int bold = false);
	void	SetpixFont(int fN);
	void	SetCellpixFont(struct _cellR* cellR);

	void	convertRuntimexml(CString key, CString data);
	void	convertRuntimerts(CString  key, CString data);
	CString	makertsinfo(CString data, bool isGrid);

//	void	loadFile();

	void	SetTrlayoutxml(int kind, CString data);
	CString	GetTrLayOut();
	void	SetRtsinfoxml(int kind, CString data);
	CString RemoveComment(CString strScript);

	CString AnsiToUTF8(CString InputStr);
};


