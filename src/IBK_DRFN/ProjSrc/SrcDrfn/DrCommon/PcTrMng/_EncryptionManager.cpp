//=============================================================================
// 화일명     : Encryption.cpp
// 화일내용   : 암호화처리 
// 화일작성자 : 이준옥
// 화일작성일 : 2005/08/12
//=============================================================================
#include "stdafx.h"
#include "PcTrMng.h"

unsigned char enctab[]={
	198, 254, 12,  183, 166, 61,  168, 99,  228, 252,
	47,  154, 36,  136, 173, 85,  112, 241, 59,  216,
	247, 253, 24,  66,  82,  111, 215, 119, 175, 141,
	201, 248, 242, 20,  226, 9,   95,  207, 120, 48,
	118, 161, 62,  74,  205, 128, 133, 69,  67,  174,
	86,  40,  212, 176, 75,  3,   30,  31,  58,  97,
	76,  203, 206, 134, 41,  79,  223, 43,  100, 81,
	185, 110, 186, 131, 250, 224, 237, 163, 78,  148,
	235, 6,   234, 144, 83,  4,   181, 96,  7,   221,
	127, 202, 209, 46,  35,  114, 92,  14,  197, 56,
	180, 211, 73,  106, 17,  246, 150, 42,  16,  52,
	45,  116, 60,  77,  91,  230, 1,   8,   15,  178,
	145, 210, 21,  193, 129, 255, 170, 29,  208, 94,
	189, 231, 227, 165, 196, 158, 34,  88,  191, 11,
	22,  137, 171, 179, 98,  164, 177, 80,  233, 132,
	101, 200, 13,  38,  49,  50,  142, 225, 113, 194,
	169, 90,  44,  23,  229, 167, 84,  71,  151, 19,
	236, 109, 28,  139, 214, 219, 195, 232, 39,  93,
	149, 89,  125, 243, 70,  153, 222, 218, 251, 51,
	156, 187, 18,  37,  115, 122, 57,  239, 244, 172,
	204, 108, 147, 10,  199, 68,  5,   155, 238, 162,
	159, 26,  65,  104, 107, 53,  103, 54,  27,  157,
	140, 138, 0,   55,  213, 117, 33,  102, 182, 190,
	160, 121, 245, 105, 135, 184, 126, 146, 130, 188,
	63,  143, 240, 32,  25,  124, 152, 249, 220, 64,
	217, 72,  123, 192, 87,  2
};

int CEncryptionManager::lsrand(int key)
{
	return (key % 255);
}

void CEncryptionManager::Encryption(int key, LPCSTR buff, LPSTR encdata, int len)
{
	int c, r;
	int i;

	c = lsrand(key);
	for(i=0;i<len;i++){
		do{
			r = ((c+(3*i)+i)%256);
		}while(r >= 256);
		*encdata++= (*buff++^enctab[r]);
	}

	return ;
}

void CEncryptionManager::Decryption(int key, LPCSTR buff, LPSTR encdata, int len)
{
	Encryption(key, buff, encdata, len);
}

const char* szCharacter[][4] = {
			{_T("SAQ"), _T("QRT"), _T("TR3"), _T("2F1")},
			{_T("HJ8"), _T("JH7"), _T("F4M"), _T("LL2")},
			{_T("DZ6"), _T("BG7"), _T("JK4"), _T("33F")},
			{_T("GCM"), _T("CCM"), _T("6H2"), _T("HLP")},
			{_T("ALZ"), _T("RG6"), _T("TBM"), _T("EXE")},
			{_T("ZIP"), _T("EAA"), _T("V43"), _T("SYS")},
			{_T("RAR"), _T("345"), _T("H74"), _T("HUY")},
			{_T("BAT"), _T("GG4"), _T("HER"), _T("HIM")},
			{_T("SQQ"), _T("JKO"), _T("OIP"), _T("H87")},
			{_T("7K6"), _T("DFS"), _T("OGG"), _T("MP3")},

			{_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T("")}, 

			{_T("R8S"), _T("3FG"), _T("SFN"), _T("6TF")},
			{_T("GGD"), _T("H63"), _T("H7X"), _T("6JK")},
			{_T("YJD"), _T("4FA"), _T("4KL"), _T("HY6")},
			{_T("8HJ"), _T("NBR"), _T("66H"), _T("JQW")},
			{_T("Y12"), _T("TG5"), _T("6YS"), _T("3Y4")},
			{_T("MPQ"), _T("43X"), _T("7UJ"), _T("5GS")},
			{_T("ADE"), _T("H69"), _T("T0O"), _T("VVD")},
			{_T("BT5"), _T("FGA"), _T("REH"), _T("6J3")},
			{_T("JNT"), _T("7P0"), _T("H3G"), _T("HFF")},
			{_T("CVZ"), _T("DF4"), _T("B31"), _T("GHL")},
			{_T("VQQ"), _T("JKY"), _T("ZE5"), _T("XF7")},
			{_T("MEW"), _T("G5H"), _T("N1V"), _T("H90")},
			{_T("EQK"), _T("F3B"), _T("RR6"), _T("8KA")},
			{_T("REW"), _T("TOP"), _T("5Y2"), _T("PP0")},
			{_T("DWJ"), _T("JY3"), _T("B1G"), _T("MMD")},
			{_T("SZZ"), _T("GYI"), _T("U75"), _T("HKK")},
			{_T("1T5"), _T("UI9"), _T("BDF"), _T("TQP")},
			{_T("95F"), _T("BWE"), _T("CC3"), _T("BYU")},
			{_T("BNY"), _T("TER"), _T("4FG"), _T("BH2")},
			{_T("FDE"), _T("BNM"), _T("V7H"), _T("MN3")},
			{_T("G7K"), _T("KDN"), _T("NB4"), _T("J03")},
			{_T("NBG"), _T("K45"), _T("DE2"), _T("GH9")},
			{_T("XF4"), _T("XF8"), _T("VF3"), _T("JJ5")},
			{_T("7HG"), _T("F4F"), _T("55G"), _T("DE1")},
			{_T("LWI"), _T("F11"), _T("F9N"), _T("BG6")},
			{_T("2DQ"), _T("5D3"), _T("LK9"), _T("GF9")}
};

 void CEncryptionManager::EncryptCharacterz(int nKey, char cCharacter, LPSTR szDestination)
{
	int nCharIndex = (UINT)cCharacter - 0x30;
//	int nIndex = ::rand() % 4;
	int nIndex = nKey % 4;

	const char** szSelCharacter = szCharacter[nCharIndex];
	::strcat(szDestination, szSelCharacter[nIndex]);
}

void CEncryptionManager::DecryptCharacterz(LPSTR szEncCharacter, LPSTR szDestination)
{
	for(int i = 0; i <= 42; i++)
	{
		const char** szSelCharacter = szCharacter[i];
		for(int j = 0; j < 4; j++)
		{
			if(::strcmp(szSelCharacter[j], szEncCharacter) == 0)
			{
				char szDecChar[2] = {0, };
				szDecChar[0] = i + 0x30;

				::strcat(szDestination, szDecChar);
				return;
			}
		}
	}
}

BOOL CEncryptionManager::EX_EncryptWord(int nKey, LPSTR szSource, LPSTR szDestination)
{
	int nLenSource = ::strlen(szSource);
	if(nLenSource <= 0)
		return FALSE;

//	time_t t;
//	::srand((unsigned)time(&t)); 

	for(int i = 0; i < nLenSource; i++)
		EncryptCharacterz(nKey, szSource[i], szDestination);

	return TRUE;

}

BOOL CEncryptionManager::EX_DecryptWord(LPSTR szSource, LPSTR szDestination)
{
	int nLenSource = ::strlen(szSource);
	if(nLenSource <= 0 || (nLenSource % 3 != 0))
		return FALSE;

	char szEcryptedChar[4];
	for(int i = 0; i < nLenSource; i += 3)
	{
		::memset(szEcryptedChar, 0x00, sizeof(szEcryptedChar));
		::memcpy(szEcryptedChar, szSource + i, 3);

		DecryptCharacterz(szEcryptedChar, szDestination);
	}

	return TRUE;
}


