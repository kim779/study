#pragma once



#define			MAXCOUNT		100
#define			DELIMITER		_T(";")
#define			SUBDELIMITER	_T("^")
#define			NEWLINE			0x0a

#define			LBLDATATYPE			0
#define			INDATATYPE			1
#define			OUTDATATYPE			2
#define			GRIDOUTDATATYPE		3
#define			LOGODATATYPE		6
#define			DOMINODATATYPE		7


#define			LEFTALIGN		0
#define			CENTERALIGN		1
#define			RIGHTALIGN		2


#define			SPACEDRAW				0
#define			GRIDDRAW				1
#define			LISTDRAW				2
#define			TEXTDRAW				3
#define			MULTILINETEXTDRAW		4




/*
-0:output
-1:sign number + comma
-2:number + comma
-3:account
-4:yyyymmdd
-5:mmdd
-6:hhmmss
-7:mmss
-8:number(only int) + comma
*/
#include "../H/gsl/gsl"

static			int TXTDRAWTYPE(CString sData)
{
	sData.MakeUpper(); 
	if(sData == "LBL") return 0;
	if(sData == "IN") return 1;
	if(sData == "OUT") return 2;//out
	if(sData == "GRIDOUT") return 3;

	if(sData == "SYM") return 5;
	if(sData == "LGO") return 6;
	if(sData == "D") return 7;//domino symbol
	
	return 4; 
}

static			int DRAWTYPE(CString sData)
{
	if(sData == "SPACE") return 0;
	if(sData == "GRID") return 1;
	if(sData == "LIST") return 2;

	if(sData == "TEXT") return 3;
	if(sData == "MULTILINETEXT") return MULTILINETEXTDRAW;
	
	return 4;//NULL
}

struct QueryList
{
	CString		strSVCName;
	int			iLedgerRCnt{};
	int			inCount{};
	int			inInLens[MAXCOUNT]{};
	CString		strInLens[MAXCOUNT];
	CString		strInDatas[MAXCOUNT];
	CString		strInTypes[MAXCOUNT];
};
/*
[OUTPUT]
--simple:0, simple+grid:1
--IDX:ZeroBase
TYPE=1
OUTCOUNT=43
OUTLENS=5;20;8;8;8;1;12;5;15;15;15;15;15;15;15;15;15;15;15;15;15;15;15;16;15;15;5;8;40;15;15;15;15;15;15;9;40;9.99;15;15;15;15;15;
GRIDROWSIDX=26
GRIDSTARTIDX=27
GRID_ROWTYPECNT=2
VIEWGRIDIDX_01=27;28;29;30;31;32;33;34;
VIEWGRIDIDX_02=35;36;37;38;39;40;41;42;
*/

struct OutFormList
{
	int			iType{};
	int			iOutCount{};
	int			iGridCntIdx{};
	int			iGridStartIdx{};
	int			iRowTypeCnt{};
	int			iFloatLen{};


	int			iLineDataSize{};
	int			iSimpleCount{}, iGridCount{}, iLastSimpleCount{};
	CString		strOutLens[MAXCOUNT];

	int			iOutLens[MAXCOUNT]{};
	int			iOutGridLens[MAXCOUNT]{};

	int			iOutFormat[MAXCOUNT]{};
	int			iOutGridFormat[MAXCOUNT]{};

	CString		strOutToken[MAXCOUNT];
	CString		strOutGridToken[MAXCOUNT];

};

struct OutDataList
{
	int		iType=0;//simple:0, grid:1
	int		iColCnt=0;

	CString		strData[MAXCOUNT];

	OutDataList(int ival, int iCnt, CString * sList)
	{
		if (sList != NULL) {
			iType = ival;
			iColCnt = iCnt;
			for (int i = 0; i < iCnt; i++) {
				strData[i] = sList[i];
			}
		}
	};

	OutDataList()
	{
	};
};

typedef CArray<OutDataList, OutDataList> COutDataListArray;

/*
struct QueryList
{
	CString		strSVCName;
	
	int			inCount;
	CString		strInLens[MAXCOUNT];
	CString		strInDATA[MAXCOUNT];

	int			outCount;
	int			iOutType; //0-simple, 1-grid, 2-simple+grid
	CString		strOutIndex[MAXCOUNT];

	int			outDataCount;
	CString		strOutData[MAXCOUNT];

	QueryList(CString sData, CString sData1, CString sData2, int iData, CString sData3)
	{
		strSVCName = sData;
		inCount =  SplitString(sData1, DELIMITER, strInLens);
		int icnt = SplitString(sData2, DELIMITER, strInDATA);
		inCount = min(inCount, icnt);

		outCount = iData;
		SplitString(sData3, DELIMITER, strOutIndex);
	};

	QueryList()
	{
		inCount = 0;
		outDataCount = outCount = 0;
		strSVCName = "";
	};
};
typedef CArray<QueryList, QueryList> CQueryListArray;

*/
struct Header
{
	CString		sTitle;
	CString		sLeftLbl[MAXCOUNT];
	CString		sLeftText[MAXCOUNT];

	CString		sRightLbl[MAXCOUNT];
	CString		sRightText[MAXCOUNT];
};

struct Line
{
	int				iFieldCnt{};
	int				iDataType[MAXCOUNT]{};//label:0?input:1?output:2,gridout:3
	CString			sText[MAXCOUNT];//LINE01DATA=°èÁÂ¹øÈ£;IN^ACT;°èÁÂ¸í;OUT^2;

	int				iTextAlign[MAXCOUNT]{};
	double			lWidthDif[MAXCOUNT]{};
};


