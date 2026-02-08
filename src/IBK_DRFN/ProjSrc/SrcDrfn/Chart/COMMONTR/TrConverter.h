#if !defined(AFX_TRCONVERTER__BY__JUNOK_LEE__4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_TRCONVERTER__BY__JUNOK_LEE__4B9B_978C_E7AC60B1F611__INCLUDED_


class CTrConverter
{
public:
	// 주식
	static int  GetSize_S03090_P0602();
	static int  MakeTrDataIn_S03090_P0602(BYTE *pTrData, char* szSaveBuf);
	static void MakeFidDataIn_S03090_P0602(BYTE *pTrData, char* szSaveBuf, int &nPos, int nLineType);

	// 업종
	static int  GetSize_D03092_P0603();
	static int  MakeTrDataIn_D03092_P0603(BYTE *pTrData, char* szSaveBuf);
	static void MakeFidDataIn_D03092_P0603(BYTE *pTrData, char* szSaveBuf, int &nPos, int nLineType);

	// 선옵
	static int  GetSize_D90001_P0604();
	static int  MakeTrDataIn_D90001_P0604(BYTE *pTrData, char* szSaveBuf);
	static void MakeFidDataIn_D90001_P0604(BYTE *pTrData, char* szSaveBuf, int &nPos, int nLineType);

public:
	static CString GetCodeName(LPVOID	aTRData);
};

#endif //AFX_TRCONVERTER__BY__JUNOK_LEE__4B9B_978C_E7AC60B1F611__INCLUDED_
