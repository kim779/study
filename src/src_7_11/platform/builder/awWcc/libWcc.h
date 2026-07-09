#ifndef	_LIBWCC_H
#define	_LIBWCC_H

#define	MAX_TEXT_LINE		1024*10
#define	MAX_TOKEN_L		1024*10
#define TYPE_NOTDEF 		0x0000

#define	SMT_ENDIF	96
#define	SMT_SWITCH	97
#define SMT_DEFAULT	98
#define	SMT_ENDSWITCH	99

#define	SW_NONE		0
#define	SW_SYMBOL	1
#define	SW_VAR		2
#define	SW_SYSTEM	3
#define SW_EVENT	4

#pragma pack(1)
class CMapToken {
public:
	WORD	len;				// token length
	BYTE	type;				// token type
	CString	name;				// token string name
	CMapToken(){
		len  = 0;
		type = 0;
		name.Empty();
	}
	~CMapToken(){Clear();}
	void Clear()
	{
		len  = 0;
		type = 0;
		name.Empty();
	}
};
#pragma pack()

//
// Token Delimetor
//

#define	TOKF_MSK		0xffff

#define	TOKF_HOS		0x0001		// [(]
#define	TOKF_HOE		0x0002		// [)]
#define	TOKF_QUS		0x0004		// [']
#define	TOKF_QUE		0x0008		// [']
#define	TOKF_DQS		0x0010		// ["]
#define	TOKF_DQE		0x0020		// ["]
#define	TOKF_ORI		0x0040		// [|]
#define	TOKF_COM		0x0080		// [//]
#define TOKF_OPER		0x0100		// [Operator]
#define	TOKF_ESC		0x0200		// [Escape Char.]

//
// Token Type	(ONLY FORM)
//

#define	TOK_USE			0
#define	TOK_NONE		1
#define	TOK_NUM			2
#define	TOK_TEXT		3
#define	TOK_FILE		4
#define TOK_PRP			6		// [. - Property]

#define	TOK_SYM			5		// Resource Only
#define	TOK_OPER		7		// Resource Only

//
// Line Type
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

struct	_mapText {
	WORD	type;
	char	*text;
};

// _mapText type
//
#define TYPE_SYNMSK		0x000f
#define	TYPE_GLOBAL		0x0001
#define	TYPE_FORM		0x0002
#define	TYPE_RESOURCE		0x0004
#define	TYPE_GRAPH		0x0008
#define	TYPE_BEGIN		0x0010
#define	TYPE_END		0x0020
// new below
#define TYPE_COMMENT		0x0080		// [// - Comment]

#ifdef	__cplusplus
extern	"C" {
#endif

__declspec(dllexport) int  APIENTRY wccAllocateMemory(int totL);
__declspec(dllexport) int  APIENTRY wccParseToken(CMapToken* token, int line, WORD* type);
__declspec(dllexport) int  APIENTRY wccBuildToken(CMapToken* token, int line, WORD* type);
__declspec(dllexport) void APIENTRY wccFreeMemory();

__declspec(dllexport) int  APIENTRY wccLoadSource(CString path, CWnd* hwnd, CString srcData = "");
__declspec(dllexport) int  APIENTRY wccBuildObject(CString path, CString nickN = _T(""));

__declspec(dllexport) void APIENTRY wccSetPath(CString& dir, char* fileN, bool source = true);
__declspec(dllexport) void APIENTRY wccSetTempPath(CString& dir, char* fileN, bool source = true);

__declspec(dllexport) int  APIENTRY wccAnalyzeColumn(int);

__declspec(dllexport) char* APIENTRY wccParseResource(int, WORD*);
__declspec(dllexport) void APIENTRY wccSetDominoForm();
__declspec(dllexport) void APIENTRY wccSetCurrentDirectory(CString curdir);
__declspec(dllexport) void APIENTRY wccLastCompileWork();

__declspec(dllexport) void APIENTRY wccToBUFFER(char* buf, int size, int value);
__declspec(dllexport) int  APIENTRY wccToNUMBER(char* buf, int size);

__declspec(dllexport) void APIENTRY wccGetTempName(char* fileN, char* prefix);


#ifdef	__cplusplus
}
#endif

#endif