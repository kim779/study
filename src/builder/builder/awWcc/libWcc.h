// libWcc.h : awWcc DLL의 외부 참조용 파일입니다.
//

#pragma once
#include "../h/mapxml.H"

#define	MAX_TEXT_LINE		1024*10		// compile.h로 이동
#define TYPE_NOTDEF 		0x0000

class CMapToken {
public:
	WORD	len;				// token length
	BYTE	type;				// token type
	CString	name;				// token string name
	CMapToken()
	{
		len  = 0;
		type = 0;
		name.Empty();
	}
	~CMapToken()
	{
		Clear();
	}

	void Clear()
	{
		len  = 0;
		type = 0;
		name.Empty();
	}
};

//
// token delimiter
//

#define	TOKF_MSK		0xffff

#define	TOKF_HOS		0x0001		// [(]
#define	TOKF_HOE		0x0002		// [)]
#define	TOKF_QUS		0x0004		// [']
#define	TOKF_QUE		0x0008		// [']
#define	TOKF_DQS		0x0010		// ["]
#define	TOKF_DQE		0x0020		// ["]
#define	TOKF_ESC		0x0040		// [Escape Char.]

//
// token Type
//

#define	TOK_USE			0
#define	TOK_NONE		1
#define	TOK_NUM			2
#define	TOK_TEXT		3
#define	TOK_FILE		4
#define TOK_PRP			5		// [. - Property]
#define	TOK_OPER		6		// Resource Only

//
// line type
//
#define LTYPE_NOTDEF		0
#define	LTYPE_ONSTART		1
#define LTYPE_ONSEND		2
#define LTYPE_ONRECEIVE		3
#define LTYPE_ONALERT		4
#define LTYPE_ONSERVICE		5
#define LTYPE_ONFILE		6
#define LTYPE_ONSELECT		7
#define LTYPE_ONTIMER		8
#define LTYPE_ONFOCUS		9
#define LTYPE_ONCLOSE		10
#define LTYPE_ONDEVICE		11
#define LTYPE_ONAPPROVE		12
#define LTYPE_ONKEY		13
#define	LTYPE_ONTRIGGER		14
#define	LTYPE_ONDECLARE		15
#define	LTYPE_ONINDECLARE	16
#define LTYPE_SCPBEGIN		17
#define LTYPE_SCPEND		18
#define LTYPE_ONEDIT		19
#define LTYPE_ONCLICK		20
#define	LTYPE_ONDBLCLK		21
#define LTYPE_ONCHANGE		22
#define	LTYPE_ONCHAR		23
#define	LTYPE_ONKEYCHANGE	24
#define	LTYPE_ONEND		25
#define	LTYPE_PROC		30
#define	LTYPE_PROCEND		31
#define	LTYPE_DIVEND		32
#define LTYPE_ONENDSUB		33
#define LTYPE_ONENDFUNC		34

#define	OBJECT_ONCLICK		35
#define	OBJECT_ONDBLCLK		36
#define	OBJECT_ONCHANGE		37
#define OBJECT_ONCHAR		38
#define OBJECT_ONKEYCHANGE	39



struct	_mapText {
	WORD	type;
	char	*text;
};

// _mapText.type
//
#define TYPE_SYNMSK		0x000f
#define	TYPE_GLOBAL		0x0001
#define	TYPE_FORM		0x0002
#define	TYPE_RESOURCE		0x0004
#define	TYPE_GRAPH		0x0008
#define	TYPE_BEGIN		0x0010
#define	TYPE_END		0x0020
#define TYPE_COMMENT		0x0080		// [// - Comment]

#ifdef	__cplusplus
extern	"C" {
#endif

__declspec(dllexport) int   APIENTRY wccLoadSource(CString path, CWnd* hwnd, CString srcData = _T(""));
__declspec(dllexport) bool  APIENTRY wccmakeBinaryFile(CString strTmp,class mapForm* mapH);

__declspec(dllexport) void  APIENTRY wccSetDominoForm();
__declspec(dllexport) void  APIENTRY wccSetCurrentDirectory(CString curdir);

__declspec(dllexport) void  APIENTRY wccSetTempPath(CString& dir, char* fileN, bool source = true);
__declspec(dllexport) void  APIENTRY wccGetTempName(char* fileN, char* prefix);
__declspec(dllexport) void  APIENTRY wccGetXmlNode(CNode &pElementNode, CString ElementKind);
__declspec(dllexport) int   APIENTRY wccGetFormNode(CNode &pElementNode, int* idx);
__declspec(dllexport) int   APIENTRY wccGetCellNode(CNode &pElementNode, int* idx);
#ifdef	__cplusplus
}
#endif