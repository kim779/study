// EncCharacter.h : main header file
//

#if !defined(AFX_ENCCHARACTER_H__1D1576F3_42E4_4990_A008_1EE950900E7A__INCLUDED_)
#define AFX_ENCCHARACTER_H__1D1576F3_42E4_4990_A008_1EE950900E7A__INCLUDED_

#define ENC_KEY 174		// 파일 암호화 키값
#define ENC_STRKEY "systemtrding_hanwha"	// 문자열 암호화 키값

unsigned char enctab[] = {
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

int WINAPI lsrand(int key)
{
	return (key % 255);
}

void WINAPI lEncryption(int key, unsigned char *buff, unsigned char *encdata, int len)
{
	int c, r, i;

	c = lsrand(key);
	for (i=0; i < len; i++)
	{
		do 
		{
			r = (c+(3*i )+i) % 256;
		} while (r >= 256);
		*encdata++ = (unsigned char)( *buff++ ^ enctab[r] );
	}
}

const char* szCharacter[][8] = {
/*0*/			{_T("SAdQ"), _T("QtRT"), _T("TnR3"), _T("2qF1"), _T("eSAQ"), _T("eQRT"), _T("aTR3"), _T("r2F1")},
/*1*/			{_T("HsJ8"), _T("JHf7"), _T("F4gM"), _T("LLg2"), _T("HdJ8"), _T("JfH7"), _T("Fd4M"), _T("LfL2")},
/*2*/			{_T("DZ6t"), _T("BG7d"), _T("JK4f"), _T("33Fk"), _T("DZf6"), _T("BGb7"), _T("JKc4"), _T("33gF")},
/*3*/			{_T("GCvM"), _T("CeCM"), _T("6eH2"), _T("HvLP"), _T("GCMs"), _T("CCMg"), _T("6H2g"), _T("dHLP")},
/*4*/			{_T("AcLZ"), _T("RGd6"), _T("TBsM"), _T("EXbE"), _T("qALZ"), _T("aRG6"), _T("dTBM"), _T("EsXE")},
/*5*/			{_T("ZIPv"), _T("EAAm"), _T("V43o"), _T("SYSa"), _T("ZaIP"), _T("EdAA"), _T("Vu43"), _T("SYaS")},
/*6*/			{_T("RcAR"), _T("3r45"), _T("Hp74"), _T("HrUY"), _T("RAdR"), _T("34c5"), _T("H7j4"), _T("HUYa")},
/*7*/			{_T("BAgT"), _T("GGf4"), _T("HEgR"), _T("HIgM"), _T("nBAT"), _T("GGb4"), _T("HERg"), _T("gHIM")},
/*8*/			{_T("SQQh"), _T("JKOx"), _T("OIPf"), _T("H87k"), _T("SiQQ"), _T("dJKO"), _T("sOIP"), _T("Hh87")},
/*9*/			{_T("7tK6"), _T("DcFS"), _T("OwGG"), _T("MhP3"), _T("7Ky6"), _T("DaFS"), _T("OfGG"), _T("MPj3")},

			{_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")}, 
			{_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")}, 

/*A*/			{_T("Re8S"), _T("3qFG"), _T("SeFN"), _T("6yTF"), _T("yR8S"), _T("y3FG"), _T("fSFN"), _T("e6TF")},
/*B*/			{_T("GGtD"), _T("H6d3"), _T("H7dX"), _T("6JhK"), _T("GhGD"), _T("Hh63"), _T("Hd7X"), _T("6sJK")},
/*C*/			{_T("YJDh"), _T("4FAj"), _T("4KLs"), _T("HY6j"), _T("YJfD"), _T("4FfA"), _T("4KdL"), _T("HYd6")},
/*D*/			{_T("8sHJ"), _T("NhBR"), _T("6s6H"), _T("JkQW"), _T("8HJd"), _T("NBRf"), _T("66Hs"), _T("JQWf")},
/*E*/			{_T("Y1c2"), _T("TGn5"), _T("6YaS"), _T("3Yi4"), _T("aY12"), _T("fTG5"), _T("a6YS"), _T("w3Y4")},
/*F*/			{_T("MPQv"), _T("43Xv"), _T("7UJz"), _T("5GSu"), _T("MdPQ"), _T("4g3X"), _T("7rUJ"), _T("5aGS")},
/*G*/			{_T("AnDE"), _T("H469"), _T("Tw0O"), _T("VrVD"), _T("ADfE"), _T("H6h9"), _T("T0fO"), _T("VVdD")},
/*H*/			{_T("BTg5"), _T("FGhA"), _T("REsH"), _T("6Jf3"), _T("BT5h"), _T("FGAt"), _T("REHg"), _T("6J3a")},
/*I*/			{_T("JNTr"), _T("7P0n"), _T("H3Gx"), _T("HFFg"), _T("yJNT"), _T("d7P0"), _T("fH3G"), _T("iHFF")},
/*J*/			{_T("CwVZ"), _T("DmF4"), _T("Be31"), _T("GtHL"), _T("CuVZ"), _T("DfF4"), _T("Bd31"), _T("GkHL")},
/*K*/			{_T("VQhQ"), _T("JKhY"), _T("ZEd5"), _T("XFg7"), _T("VQjQ"), _T("JKgY"), _T("ZEe5"), _T("XFh7")},
/*L*/			{_T("MEWo"), _T("G5Hf"), _T("N1Vc"), _T("H90b"), _T("MEWg"), _T("G5Hh"), _T("N1Vu"), _T("H90g")},
/*M*/			{_T("EqQK"), _T("F3Bs"), _T("RrR6"), _T("8eKA"), _T("kEQK"), _T("jF3B"), _T("jRR6"), _T("q8KA")},
/*N*/			{_T("REkW"), _T("TdOP"), _T("5Yf2"), _T("PPf0"), _T("RhEW"), _T("ThOP"), _T("5hY2"), _T("PaP0")},
/*O*/			{_T("DWJb"), _T("JYu3"), _T("B1Gv"), _T("MMDv"), _T("DWgJ"), _T("JYf3"), _T("B1gG"), _T("MMgD")},
/*P*/			{_T("StZZ"), _T("GYIt"), _T("Ut75"), _T("HyKK"), _T("SZZf"), _T("GYId"), _T("U75d"), _T("HKKn")},
/*Q*/			{_T("1Tj5"), _T("UtI9"), _T("BDgF"), _T("TQhP"), _T("w1T5"), _T("rUI9"), _T("wBDF"), _T("iTQP")},
/*R*/			{_T("95Fm"), _T("BWjE"), _T("CC3b"), _T("BYUf"), _T("9f5F"), _T("BeWE"), _T("CeC3"), _T("BtYU")},
/*S*/			{_T("BrNY"), _T("TERs"), _T("4tFG"), _T("BdH2"), _T("BNfY"), _T("TEdR"), _T("4FrG"), _T("BHd2")},
/*T*/			{_T("FDfE"), _T("BqNM"), _T("V7gH"), _T("MNf3"), _T("FDEc"), _T("BNMs"), _T("V7Hf"), _T("MN3a")},
/*U*/			{_T("G7Kq"), _T("KDwN"), _T("NB4n"), _T("J03v"), _T("aG7K"), _T("kKDN"), _T("hNB4"), _T("wJ03")},
/*V*/			{_T("NdBG"), _T("K45v"), _T("DyE2"), _T("GbH9"), _T("NlBG"), _T("Ki45"), _T("DgE2"), _T("GfH9")},
/*W*/			{_T("XFj4"), _T("XcF8"), _T("VFh3"), _T("JJn5"), _T("XFg4"), _T("XFy8"), _T("VFd3"), _T("JJd5")},
/*X*/			{_T("7HGu"), _T("F4vF"), _T("55Gb"), _T("DE1w"), _T("7HGg"), _T("F4Fy"), _T("55Gs"), _T("DE1g")},
/*Y*/			{_T("LpWI"), _T("F11u"), _T("Fy9N"), _T("BrG6"), _T("fLWI"), _T("yF11"), _T("Fi9N"), _T("BaG6")},
/*Z*/			{_T("2DoQ"), _T("5sD3"), _T("LKj9"), _T("GFf9"), _T("2fDQ"), _T("5tD3"), _T("LKy9"), _T("GFd9")}
};

#endif // !defined(AFX_ENCCHARACTER_H__1D1576F3_42E4_4990_A008_1EE950900E7A__INCLUDED_)
