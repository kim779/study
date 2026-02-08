#pragma once

#include "afxtempl.h"
#include "../../../h/memDC.h"
#include "../../../h/ledger.h"
#include "../../../h/symbol.h"
#include "../../../h/axisvar.h"
#include "imgBtn.h"

enum { FK_NOT = 0, FK_COMMA, FK_FLOAT, FK_FLOATx, FK_FUTURE, FK_ACCNx, FK_ACCN, FK_TIME, FK_RMVCOMMA, FK_RMVACCN, FK_TRIMLEFT };

void		_SetImgBtn(CWnd *pParent, CString imgDir, CImgBtn *pButton, int nLength, BOOL bImageFit);
void		_SetImgBtnX(CWnd *pParent, CString imgDir, CImgBtn *pButton, CString sImgName, BOOL bImageFit);
void		_SetImgBtnT(CWnd *pParent, CString imgDir, CImgBtn *pButton, CString sImgName, BOOL bImageFit);
CPen*		_GetAxPen(CWnd *pParent, COLORREF clr, int width, int style);
COLORREF	_GetIndexColor(CWnd *pParent, int index);
CFont*		_GetAxFont(CWnd *pParent, CString fName = "굴림체", int point = 9, bool bItalic = false, int nBold = 0);
CBitmap*	_GetAxBitmap(CWnd *pParent, CString path);
CString		_Parser(CString &srcstr, CString substr);
CString		_GetAccEdit(CString accn);
CString		_GetRemoveString(CString str);
CString		_GetString(char *pChar, int nMinLen);
CString		_GetStringX(char *pChar, int nMinLen, int digit, bool bColor = false);
double		_Str2Double(CString string, bool bABS = true);
int		_Str2Int(CString string, bool bABS = true);
CString		_GetFormatData(int fkind, CString str);

// macro
#define	LIB_DELETE(XXX) { delete XXX; XXX = NULL; }
#define	BUF_DELETE(XXX)	{ delete[] XXX; XXX = NULL; }

// color
#define	CLRBG			64		// background
#define	CLRGRROWB1		68		// grid row1 back
#define	CLRGRROWB2		77		// grid row2 back
#define	CLRGRROWF		69		// grid row fore
#define	CLRGRROWSF		56		// grid row fore(special)
#define	CLRGRHB			74		// grid head back
#define	CLRGRHSB		0		// grid head select
#define	CLRGRHF			76		// grid head fore
#define	CLRGRHJMGA		163		// grid head 주문가
#define	CLRGRHMCHG		164		// grid head 미체결량
#define	CLRGRHCHEG		162		// grid head 체결량
#define	CLRGRSEL		78		// grid select
#define	CLRGRLINE		65		// grid line
#define	CLRREFUSE		39		// 거부
#define	CLRMDF			57		// 매도 font
#define	CLRMSF			56		// 매수 font
#define	CLRMDB			162		// 매도 back
#define	CLRMSB			161		// 매수 back
#define	CLRMDT			82		// 매도 title
#define	CLRMST			85		// 매수 title
#define	CLRSEL			PALETTERGB(255, 255, 170)	// Y 표시 있을때 색상
#define	CLRGRROWDISF		PALETTERGB(150, 150, 150)	// 모두 체결된 경우

// 주문확인
#define	CLRCFMTF		PALETTERGB(255, 255, 255)	// title F
#define	CLRCFMJJBG		163				// 정정바탕
#define	CLRCFMJJGHB		163				// 정정grid head back
#define	CLRCFMJJGL		208				// 정정grid lind
#define	CLRCFMJJGRB		68				// 정정grid row back
#define	CLRCFMJJGRS		164				// 정정grid row select

#define	CLRCFMCCBG		164				// 취소바탕
#define	CLRCFMCCGHB		164				// 취소grid head back
#define	CLRCFMCCGL		208				// 취소grid lind
#define	CLRCFMCCGRB		68				// 취소grid row back
#define	CLRCFMCCGRS		164				// 취소grid row selec

#define	CLRCFMMDBG		72				// 매도바탕
#define	CLRCFMMDGHB		72				// 매도grid head back
#define	CLRCFMMDGL		65				// 매도grid lind
#define	CLRCFMMDGRB		68				// 매도grid row back
#define	CLRCFMMDGRS		72				// 매도grid row selec

// Y character
#define	SELCHAR			"V"		// Select 표시
#define	MARK_EXPAND		"▼"		// 확장
#define	MARK_REDUCE		"▲"		// 축소
//======================================================================
// grid 
//======================================================================
struct _grInfo {
	char*	heads;
	int	width;
	char*	symbols;
	UINT	format;
	DWORD	attr;
	char*	defaultVal;
	int	param;
};
#define GRIDROW_HEIGHT  18

//======================================================================
// 주문 결과 저장
//======================================================================
struct	_jmResult
{
	char    accn[10];			// 계좌번호
	char	anam[40];			// 계좌명
	char	jmno[6];			// 주문번호
	char	code[12];			// 종목코드
	char	name[32];			// 종목명
	char	emsg[80];			// 오류메세지
	char	ojno[6];			// 원주문번호
	char    jqty[8];			// 주문수량
	char    jprc[10];			// 주문단가 : 선물옵션 (112.13->11213)
	char	mmgb[1];			// 매매구분 1:매도,2:매수
};
#define	sz_jmResult	sizeof(struct _jmResult)

//=================================================================================================
// 그룹정보
//=================================================================================================
class 	_acSave	
{
public:
	CString sAccntNum;				//계좌번호
	CString sAccntName;				//계좌이름
public:
	_acSave()
	{
		sAccntNum = _T("");
		sAccntName = _T("");
	};

	_acSave(_acSave const &other)
	{
		Copy(other);
	};

	~_acSave() {}

	inline Copy(_acSave const &other)
	{
		sAccntNum = other.sAccntNum;
		sAccntName = other.sAccntName;
	};
};

//=================================================================================================
class	_gpSave	
{
public:
	CString sGrName;				// 그룹명
	CString sAccntCnt;				// 계좌갯수
	CString	sGroupID;				// 그룹아이디
	CArray <_acSave *, _acSave *>	arAcEdit;	// 계좌 array
public:
	_gpSave()
	{
		sGrName = _T("");
		sAccntCnt = _T("");
		sGroupID = _T("");
		arAcEdit.RemoveAll();
	};

	~_gpSave()
	{
		_RemoveAr();
	};

	inline	void _RemoveAr()
	{
		for (int ii = 0; ii < arAcEdit.GetSize(); ii++)
		{
			_acSave* acSave = arAcEdit.GetAt(ii);
			if (acSave)
			{
				delete acSave;
				acSave = NULL;
			}
		}
		arAcEdit.RemoveAll();
	};
};

