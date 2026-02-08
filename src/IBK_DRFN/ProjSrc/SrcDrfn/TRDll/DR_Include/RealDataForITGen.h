#ifndef _REALDATA_FOR_ITGEN
#define _REALDATA_FOR_ITGEN

typedef struct stRealDataITGen
{
	BYTE	chCount;
	char	szKeyCode[4];
	int		nSize;
	LPSTR	pDataBuf;
	int		lRealStruct;
	BYTE	chRealType;
} REALDATAITGEN;

#endif