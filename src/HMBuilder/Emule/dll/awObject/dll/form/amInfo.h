#include "stdafx.h"
#include <deque>
using namespace std;


class CiBase 
{
public:
	CiBase(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
	{
		vector<string> sLua;
		m_pInfo		 = pinfo;
		m_pscript	 = pscript;
		m_psub		 = psub;
		m_pMapScript = pMAP;
	
		m_pForm = new struct _formR();
		FillMemory(m_pForm, sizeof(struct _formR), 0);

		string ss;
		if (!m_pInfo)
			return;	

		char	*pch[] = {
			  "CLASS",
			  "mNAME", 
		      "mLEFT", 
			  "mTOP", 
			  "mRIGHT", 
			  "mBOTTOM"
		};

		Msg("");
		UINT ii;
		for (ii = 1 ; ii < mBOTTOM + 1 ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);

			Msg("%s %s", pch[ii], ss.data());
			switch(ii)
			{
			case mNAME: 
				m_pForm->name = new char[ss.length() + 1];
				strcpy_s(m_pForm->name, ss.length() + 1, ss.data());
				break;
			case mLEFT: 
				m_pForm->left = atoi(ss.data());
				break;
			case mTOP:
				m_pForm->top = atoi(ss.data());
				break;
			case mRIGHT: 
				m_pForm->right = atoi(ss.data());
				break;
			case mBOTTOM:
				m_pForm->bottom = atoi(ss.data());
				break;
			default:
				break;
			}
		}	

		map <string, int> mScript;
		char* pcSCR[] = {	
			"_am_onclick_am_",
			"_am_onlongtap_am_",
			"_am_onchange_am_",
			"_am_onchar_am_",
		};

		for each(char* pc in pcSCR)
			mScript[pc] = 1;
	
		for (UINT ii = 1 ; ii < pscript->size(); ii++)
		{
			ss = (*pscript)[ii];

			if (ss.empty()) 
				continue;
/*			
			CString str = CString(ss.data());
			str.Replace(12,  0x08);
			ss.clear();
			ss.append(str);
			CString cPRO = FORMAT("%c", mCLASS);
			
//			int nCount = 
				boost::regex_split(std::back_inserter(sLua), ss, boost::regex(cPRO.GetBuffer(0))); 
*/			
			
			int nCount = Split(sLua, ss, 12);
			
			while(1)
			{
				if (sLua[0].empty())
					sLua.erase(sLua.begin());
				else 
					break;
			}
			

			if (sLua.empty() || sLua[0].empty())
				break;

			transform(sLua[0].begin(), sLua[0].end(), sLua[0].begin(), tolower);
	
			if (mScript[sLua[0]] && sLua.size() == 2)
			{	
				char* pch = new char[sLua[1].length() + 1];
				strcpy_s(pch, sLua[1].length() + 1, sLua[1].data()); 
				string sName = string(m_pForm->name);
				algorithm::trim(sName);
				(*pMAP)[sLua[0] + sName] = pch; 	
			}
			sLua.clear();			
		}

	};
	~CiBase()
	{
	};

public:

	enum	{ 
			  mNAME = 1, 
		      mLEFT, 
			  mTOP, 
			  mRIGHT, 
			  mBOTTOM, 
			};

	/*	스크립트 처리 이후...
			  // script
			  _am_ontap_am_,
			  _am_onlongtap_am_,
			  _am_onchange_am_,
	*/
public:
	struct	_formR* GetForm(){ if (m_pForm) return m_pForm;  return NULL; }


protected:
	
	struct	_formR* m_pForm;
	
	map<string, char*>*	m_pMapScript;
	vector<string>*		m_pInfo;
	vector<string>*		m_pscript;
	vector<string>*		m_psub;
};


class CiLabel : public CiBase
{
public:
	CiLabel(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP) 
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		char* pch[] =
		{ 
			"mSTR",
			"mPROPERTIES",
			"mALIGNIMAGE",
			"mPATHIMAGE",
			"mDESC",
			"mALIGNMENT",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mATTR",
			"mPRGB",
			"mTRGB",
			"mBORDER",
			"mLINECOLOR",
			"mFONTHEIGHT",
			"기타",
			"기타",
		};


		m_pForm->kind = FM_LABEL;
		for (ii = mSTR ; ii < m_pInfo->size()/*mLINECOLOR + 1*/ ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

			Msg("%s %s", pch[ii - mSTR], ss.data());

			switch (ii)
			{
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mALIGNIMAGE:
				m_pForm->alignImage = ATOI(ss);
				break;
			case mPATHIMAGE:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mDESC:
				m_pForm->desc = new char[nLen];
				strcpy_s(m_pForm->desc, nLen, ss.data());
				break;
			case mALIGNMENT:			
				m_pForm->alignment = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mBORDER:
				m_pForm->borders = ATOI(ss);
				break;
			case mLINECOLOR:
				m_pForm->bRGB = ATOL(ss);
				break;
			default:
				break;
			}
		}
		Msg("");
	};
	~CiLabel(){};

	enum	
	{ 
		mSTR = mBOTTOM + 1,		// display text
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mALIGNIMAGE,			// IA_STRETCH, IA_CENTER, IA_TILE
		mPATHIMAGE,				// (imagefilename)
		mDESC,					// (voice reading)
		mALIGNMENT,				// (textalign)
		mFNAME,					// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mATTR,					// attr
		mPRGB,					// backcolor
		mTRGB,					// forecolor
		mBORDER,				
		mLINECOLOR,		
		mFONTHEIGHT,
	};
};


class CiBox : public CiBase
{
public:
	CiBox(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;
		
		char	*pch[] = {
			"mSIZE",
			"mPROPERTIES",
			"mATTR",
			"mPRGB",
			"mTRGB",
			"기타",
			"기타",
			"기타",
		};


		m_pForm->kind = FM_BOX;
		for (ii = mSIZE ; ii < mTRGB + 1 ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
			Msg("%s	%s", pch[ii - mSIZE], ss.data());
	
			switch (ii)
			{
			case mSIZE:
				m_pForm->size = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			default:
				break;
			}
		}
		Msg("");
	};
	~CiBox(){};

	enum	
	{
		mSIZE = mBOTTOM + 1,
	    mPROPERTIES,				// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR,					// attr
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
	};
};


class CiGroup : public CiBase
{
public:
	CiGroup(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP) : CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		char *pch[] = {
			"mSTR",
			"mALIGNMENT",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mPROPERTIES",
			"mATTR",
			"mPRGB",
			"mTRGB",
			"기타",
			"기타",
			"기타",
			"기타",
		};
		Msg("");

		m_pForm->kind = FM_GROUP;
		for (ii = mSTR ; ii < mTRGB + 1 ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

			Msg("% %s", pch[ii - mSTR], ss.data());

			switch (ii)
			{
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mALIGNMENT:
				m_pForm->properties = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOI(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOI(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOI(ss);
				break;
			default:
				break;
			}

		}
		Msg("");

	};
	~CiGroup(){};

	enum	
	{
		mSTR = mBOTTOM + 1,			// string record
		mALIGNMENT,				// (textalign)
		mFNAME,					// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR,					// attr
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
	};
};


class CiImageView : public CiBase
{
public:
	CiImageView(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		char* pch[] = 
		{ 
			"mSTR",
			"mPATHIMAGE",
			"mDESC",
			"mPROPERTIES",
			"mALIGNIMAGE",
			"mBKCOLOR",
			"mBORDER",
			"mLINECOLOR",
			"기타",
			"기타",
			"기타",
			"기타",

		};

		m_pForm->kind = FM_IMAGEVW;
		for (ii = mSTR ; ii < m_pInfo->size()/*mLINECOLOR + 1*/ ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

			Msg("%s %s", pch[ii - mSTR], ss.data());
				
			switch (ii)
			{
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPATHIMAGE:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mDESC:
				m_pForm->desc = new char[nLen];
				strcpy_s(m_pForm->desc, nLen, ss.data());
				break;
			case mPROPERTIES:
				m_pForm->properties = (WORD)ATOI(ss);
				break;
			case mALIGNIMAGE:
				m_pForm->alignImage = ATOI(ss);
				break;
			case mBKCOLOR:
				m_pForm->pRGB = ATOI(ss);
				break;
			case mBORDER:
				m_pForm->borders = ATOI(ss);
				break;
			case mLINECOLOR:
				m_pForm->bRGB = ATOL(ss);
				break;
			default:
				break;
			}
		}
				Msg("");
	};
	~CiImageView(){};

	enum 
	{ 
		mSTR = mBOTTOM + 1,	 // string record
		mPATHIMAGE,			 // (imagefilename)
		mDESC,				 // (voice reading)
		mPROPERTIES,		 // PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mALIGNIMAGE,		 // IA_STRETCH, IA_CENTER, IA_TILE	
		mBKCOLOR,
		mBORDER,
		mLINECOLOR,
	};
};


class CiButton : public CiBase
{
public:
	CiButton(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		char *pch[] = { 
				"mSTR",
				"mPATHIMAGE",
				"mDESC",
				"mALIGNMENT",
				"mFNAME",
				"mFPOINT",
				"mFSTYLE",
				"mPROPERTIES",
				"mALIGNIMAGE",
				"mATTR",
				"mPRGB",
				"mTRGB",
				"mTABORDER",
				"mSCOLOR"
				"기타",
				"기타",
			};	
				
	
		for (ii = mSTR ; ii < mTABORDER + 1 ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
			m_pForm->kind = FM_BUTTON;
			m_pForm->type = BTN_PUSH;

			Msg("%s %s", pch[ii - mSTR], ss.data());
			
			switch (ii)
			{
			case mSTR:
					m_pForm->str = new char[nLen];
					strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPATHIMAGE:
					m_pForm->str2 = new char[nLen];
					strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mDESC:
					m_pForm->desc = new char[nLen];
					strcpy_s(m_pForm->desc, nLen, ss.data());
				break;
			case mALIGNMENT:
				m_pForm->alignment = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mALIGNIMAGE:
				m_pForm->alignImage = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mSCOLOR:
				break;
			default:
				break;
			}
		}
		Msg("");

	};
	~CiButton(){};

	enum	
	{ 
		mSTR = mBOTTOM + 1,	 // string record
		mPATHIMAGE,			 // (imagefilename)
		mDESC,				 // (voice reading)
		mALIGNMENT,				// (textalign)
		mFNAME,					// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mPROPERTIES,		 // PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mALIGNIMAGE,		 // IA_STRETCH, IA_CENTER, IA_TILE	
		mATTR,					// attr
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mTABORDER,				// taborder
		mSCOLOR,				// SelectedColor
	};
};

class CiCheck : public CiBase
{
public:
	CiCheck(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		char *pch[]= {
			"mSTR",
			"mPATHIMAGE",
			"mDESC",
			"mALIGNMENT",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mIOK",
			"mPROPERTIES",
			"mmALIGNIMAGE",
			"mATTR",
			"mPRGB",
			"mTRGB",
			"mmTABORDER",
			"mTOUT",
			"mTIN",
		};

		m_pForm->kind = FM_CHECK;
		m_pForm->type = BTN_CHECK;

		for (ii = mSTR ; ii < mTABORDER + 1 ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

			Msg("%s %s", pch[ii - mSTR], ss.data());
		
			switch (ii)
			{
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPATHIMAGE:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mDESC:
				m_pForm->desc = new char[nLen];
				strcpy_s(m_pForm->desc, nLen, ss.data());
				break;
			case mALIGNMENT:
				m_pForm->alignment = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mIOK:
				m_pForm->iok = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mALIGNIMAGE:
				m_pForm->alignImage = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mTOUT:
			case mTIN:
			default:
				break;
			}
		}
			Msg("");
	};
	~CiCheck(){};

	enum	
	{ 
		mSTR = mBOTTOM + 1,	 // string record
		mPATHIMAGE,			 // (imagefilename)
		mDESC,				 // (voice reading)
		mALIGNMENT,				// (textalign)
		mFNAME,					// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mIOK,				// in / out kind
		mPROPERTIES,		 // PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mALIGNIMAGE,		 // IA_STRETCH, IA_CENTER, IA_TILE	
		mATTR,					// attr
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mTABORDER,				// taborder
		mTOUT,					// total out#
		mTIN						// total in#
	};
};

class CiRadio : public CiBase
{
public:
	CiRadio(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		char *pch[] = 
		{ 
			"mSTR",
			"mPATHIMAGE",
			"mDESC",
			"mGROUP",
			"mALIGNMENT",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mIOK",
			"mPROPERTIES",
			"mALIGNIMAGE",
			"mATTR",
			"mPRGB",
			"mTRGB",
			"mTABORDER",
			"mTOUT",
			"mTIN"
		};


		m_pForm->kind = FM_RADIO;
		m_pForm->type = BTN_RADIO;
		for (ii = mSTR ; ii < mTABORDER + 1 ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

			Msg("%s %s", pch[ii - mSTR], ss.data());
		

			switch (ii)
			{
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPATHIMAGE:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mDESC:
				m_pForm->desc = new char[nLen];
				strcpy_s(m_pForm->desc, nLen, ss.data());
				break;
			case mGROUP:
				m_pForm->auxS = new char[nLen];
				strcpy_s(m_pForm->auxS, nLen, ss.data());
				break;
			case mALIGNMENT:
				m_pForm->alignment = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mIOK:
				m_pForm->iok = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mALIGNIMAGE:
				m_pForm->alignImage = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			default:
			case mTOUT:
			case mTIN:
				break;
			}
		}
		Msg("");
	};
	~CiRadio(){};

	enum	
	{ 
		mSTR = mBOTTOM + 1,	 // string record
		mPATHIMAGE,			 // (imagefilename)
		mDESC,				 // (voice reading)
		mGROUP,					// Group
		mALIGNMENT,				// (textalign)
		mFNAME,					// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mIOK,				// in / out kind
		mPROPERTIES,		 // PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mALIGNIMAGE,		 // IA_STRETCH, IA_CENTER, IA_TILE	
		mATTR,					// attr
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mTABORDER,				// taborder
		mTOUT,					// total out#
		mTIN					// total in#
	};

};


class CiEdit : public CiBase
{
public:
	CiEdit(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		char* pch[] =	
		{ 
			"mSIZE", 
			"mTYPE",
			"mOFFS",
			"mPROPERTIES",
			"mSTR",
			"mPATHIMAGE",
			"mHINT",
			"mIOK",
			"mALIGNIMAGE",
			"mDESC",
			"mFORMAT",
			"mUPCOL",
			"mDNCOL",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mATTR",
			"mATTR2",
			"mPRGB",
			"mTRGB",
			"mTABORDER",
			"mORDER",
			"mTOUT",
			"mTIN",
			"mBORDER",
			"mLINECOLOR",
			"mRTSSYMBOL",
			"기타",
			"기타",
			"기타",
			"기타",
		};


		m_pForm->kind = FM_EDIT;
		for (ii = mSIZE ; ii < m_pInfo->size() ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

			Msg("%s %s", pch[ii - mSIZE], ss.data());
	
			
			switch (ii)
			{
			case mSIZE:
				m_pForm->size = ATOI(ss);
				break;
			case mTYPE:
				m_pForm->type = ATOI(ss);
				break;
			case mOFFS:
				m_pForm->offs = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPATHIMAGE:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mHINT:
				m_pForm->dat = new char[nLen];
				strcpy_s(m_pForm->dat, nLen, ss.data());
				break;
			case mIOK:
				m_pForm->iok = ATOI(ss);
#ifdef _DEBUG
				m_pForm->iok |= EIO_INOUT;
#endif
				break;
			case mALIGNIMAGE:
				m_pForm->alignImage = ATOI(ss);
				break;
			case mDESC:
				m_pForm->desc = new char[nLen];
				strcpy_s(m_pForm->desc, nLen, ss.data());
				break;
			case mFORMAT:
				m_pForm->editS = new char[nLen];
				strcpy_s(m_pForm->editS, nLen, ss.data());
			//	m_pForm->isEdit = ATOI(ss);
				break;
			case mUPCOL:
				m_pForm->onEdit = MAKELONG(0, ATOI(ss));
				break;
			case mDNCOL:
				m_pForm->onEdit = MAKELONG(ATOI(ss), HIWORD(m_pForm->onEdit));
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;	
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mATTR2:
				m_pForm->attr2 = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			case mTOUT:
			case mTIN:
				break;
			case mBORDER:
				m_pForm->borders = ATOI(ss);
				break;
			case mLINECOLOR:
				m_pForm->bRGB = ATOL(ss);
				break;
			case mRTSSYMBOL:
				m_pForm->rts = new char[nLen];
				strcpy_s(m_pForm->rts, nLen, ss.data());
				break;
			default:
				break;
			}
		}
		Msg("");
	};

	~CiEdit(){};

	enum	
	{ 
		mSIZE = mBOTTOM + 1,	// I/O data size
		mTYPE,				// dattype
		mOFFS,				// float digit
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mSTR,	 // string record
		mPATHIMAGE,			 // (imagefilename)
		mHINT,				 // hint text
		mIOK,				    // in / out kind
		mALIGNIMAGE,			 // (ALIGN)
		mDESC,				 // (voice reading)
		mFORMAT,				// Edit FORMAT
		mUPCOL,		
		mDNCOL,
		mFNAME,				// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mATTR,					// attr
		mATTR2,
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mTABORDER,				// taborder
		mORDER,
		mTOUT,					// total out#
		mTIN,						// total in#
		mBORDER,
		mLINECOLOR,
		mRTSSYMBOL
	};
};

class CiOut : public CiBase
{
public:
	CiOut(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		char* pch[] = 
		{ 
			"mSIZE", 
			"mTYPE",
			"mOFFS",
			"mPROPERTIES",
			"mSTR",
			"mALIGN",
			"mBORDERS",
			"mLINECOLOR",
			"mDESC",
			"mFORMAT",
			"mUPCOL",
			"mDNCOL",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mATTR",
			"mATTR2",
			"mPRGB",
			"mTRGB",
			"mTABORDER",
			"mORDER",
			"mTOUT",
			"mTIN",
			"mRTSSYMBOL",
			"mIMAGE",
			"기타",
			"기타",
		};


		m_pForm->kind = FM_OUT;
		for (ii = mSIZE ; ii < m_pInfo->size() ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
		
			Msg("%s %s", pch[ii - mSIZE], ss.data());
		
			switch (ii)
			{
			case mSIZE:
				m_pForm->size = ATOI(ss);
				break;
			case mTYPE:
				m_pForm->type = ATOI(ss);
				break;
			case mOFFS:
				m_pForm->offs = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mALIGN:
				m_pForm->alignment = ATOI(ss);
				break;
			case mBORDERS:
				m_pForm->borders = ATOI(ss);
				break;
			case mLINECOLOR:
				m_pForm->bRGB = ATOL(ss);
				break;
			case mDESC:
				m_pForm->desc = new char[nLen];
				strcpy_s(m_pForm->desc, nLen, ss.data());
				break;
			case mFORMAT:
				m_pForm->editS = new char[nLen];
				strcpy_s(m_pForm->editS, nLen, ss.data());
				break;
			case mUPCOL:
				m_pForm->onEdit = MAKELONG(0, ATOI(ss));
				break;
			case mDNCOL:
				m_pForm->onEdit = MAKELONG(ATOI(ss), HIWORD(m_pForm->onEdit));
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mATTR2:
				m_pForm->attr2 = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			case mTOUT:
			case mTIN:
				break;
			case mRTSSYMBOL:
				m_pForm->rts = new char[nLen];
				strcpy_s(m_pForm->rts, nLen, ss.data());
				break;
			case mIMAGE:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			default:
				break;
			}
		
		}
		Msg("");
			
	};
	~CiOut(){};

	enum
	{ 
		mSIZE = mBOTTOM + 1,	// I/O data size
		mTYPE,				// dattype
		mOFFS,				// float digit
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mSTR,	 // string record
		mALIGN, 
		mBORDERS,
		mLINECOLOR,
		mDESC,				 // (voice reading)	
		mFORMAT,				// Edit FORMAT
		mUPCOL,		
		mDNCOL,
		mFNAME,				// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mATTR,					// attr
		mATTR2,
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mTABORDER,				// taborder
		mORDER,
		mTOUT,					// total out#
		mTIN,						// total in#
		mRTSSYMBOL,
		mIMAGE,					// Image;
	};

};


class CiCombo : public CiBase
{
public:
	CiCombo(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;
		m_pForm->kind = FM_COMBO;


		char* pch[]= 
		{ 
			"mSIZE", 
			"mLENGTH",
			"mCOUNT",
			"mINDEX",
			"mKEY",
			"mTYPE",
			"mSTR",
			"mDAT",
			"mPATHIMAGE",
			"mFORMAT",
			"mIOK",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mPROPERTIES",
			"mATTR",
			"mATTR2",
			"mPRGB",
			"mTRGB",
			"mTABORDER",
			"mORDER",
			"mIN",
			"mOUT",
			"mSELECTEDCOLOR",
			"기타",
			"기타",
		};



		for (ii = mSIZE ; ii <  m_pInfo->size() ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
		
			Msg("%s %s", pch[ii - mSIZE], ss.data());
			
			switch (ii)
			{
			case mSIZE:
				m_pForm->size = ATOI(ss);
				break;
			case mLENGTH:
				m_pForm->keys = MAKELONG(0, ATOI(ss));
				break;
			case mCOUNT:
				m_pForm->keys = MAKELONG(ATOI(ss), HIWORD(m_pForm->keys));
				break;
			case mINDEX:
				m_pForm->checked = ATOI(ss);
				break;
			case mKEY:
				m_pForm->onEdit = ATOL(ss);
				break;
			case mTYPE:
				m_pForm->type = ATOI(ss);
				break;
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mDAT:
				m_pForm->dat = new char[nLen];
				strcpy_s(m_pForm->dat, nLen, ss.data());
				break;
			case mPATHIMAGE:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mFORMAT:
				m_pForm->editS = new char[nLen];
				strcpy_s(m_pForm->editS, nLen, ss.data());
				break;
			case mIOK:
				m_pForm->iok = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mATTR2:
				m_pForm->attr2 = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			case mTOUT:
			case mTIN:
				break;
			case mSELCOL:
				m_pForm->spRGB = ATOI(ss);
				break;
			default:
				break;
			}
		}
		Msg("");
	};
	~CiCombo(){};

	enum	
	{ 
		mSIZE = mBOTTOM + 1,	// I/O data size
		mLENGTH,				// data size
		mCOUNT,					// visible count
		mINDEX,					// en
		mKEY,					// match key
		mTYPE,					// datatype
		mSTR,					// string record
		mDAT,	
		mPATHIMAGE,			 // (imagefilename)
		mFORMAT,				// Edit FORMAT
		mIOK,				    // in / out kind
		mFNAME,				// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR,					// attr
		mATTR2,
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mTABORDER,				// taborder
		mORDER,	
		mTOUT,					// total out#
		mTIN,					// total in#
		mSELCOL,
	};
};


class CiMemo : public CiBase
{
public:
	CiMemo(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;
		m_pForm->kind = FM_MEMO;
		for (ii = mBORDERS ; ii < mORDER + 1 ; ii++)
		{

			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
		
			switch (ii)
			{
			case mBORDERS:
				m_pForm->borders = ATOI(ss);
				break;
			case mTYPE:
				m_pForm->type = ATOI(ss);
				break;
			case mLEN:  //????
				m_pForm->keys = ATOL(ss);
				break;
			case mIOK:
				m_pForm->iok = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data()); 
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			default: 
				break;
			}
		}
	};
	~CiMemo(){};

	enum	
	{
		mBORDERS = mBOTTOM + 1,
		mTYPE,				// datatype
		mLEN,				// data length
		mIOK,
		mFNAME,				// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR,					// attr
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mTABORDER,				// taborder
		mORDER,
	};
};


class CiGrid : public CiBase
{
public:
	CiGrid(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP, vector<struct _cellR*>* pCell)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		m_pCELL = pCell;
		UINT ii = 0;
		int nLen;
		string ss;
		m_pForm->kind = FM_GRID;

		char* pch[]= 
		{ 
			"mVROW",
			"mHEIGHT",
			"mROW",
			"mCOL",
			"mTRGB",
			"mPRGB",
			"mLRGB",
			"mFOCUS",
			"mFKIND",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mSTR",
			"mPROPERTIES",
			"mATTR2",
			"mTABORDER",
			"mORDER",
			"mTIN",
			"mTOUT",
			"mFROW",
			"mFCOL",
			"mHHEIGHT",
			"mROWOF",
			"mFIR",
			"mSEC",
			"mFH",
			"기타",
			"기타",
			"기타",
		};


		for (ii = mVROW ; ii < m_pInfo->size() ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
	
			Msg("%s %s", pch[ii - mVROW], ss.data());
			switch (ii)
			{
			case mVROW:
				m_pForm->keys = MAKELONG(0, ATOI(ss));
				break;
			case mHEIGHT:
				m_pForm->keys = MAKELONG(ATOI(ss), HIWORD(m_pForm->keys));
				break;
			case mROW:
				m_pForm->row = ATOI(ss);
				break;
			case mCOL:
				m_pForm->col = ATOI(ss);
				break;
			case mTRGB:
//				m_pForm->stRGB = ATOL(ss);
				m_pForm->tRGB = ATOL(ss);
				break;
			case mPRGB:
//				m_pForm->spRGB = ATOL(ss);
				m_pForm->pRGB = ATOL(ss);
				break;
			case mLRGB:
				m_pForm->bRGB = ATOL(ss);
				break;
			case mFOCUS:
				m_pForm->onEdit = ATOL(ss);
				break;
			case mFKIND:
				m_pForm->isEdit = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR2:
				m_pForm->attr2 = ATOL(ss);
				m_pForm->attr2 |= GO_ALWAYS;
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			case mTOUT:
			case mTIN:
				break;
			case mFROW:
				m_pForm->frow = ATOI(ss);
				break;
			case mFCOL:
				m_pForm->type = ATOI(ss);
				break;
			case mHHEIGHT:
				m_pForm->hHeight = ATOI(ss);
				break;
			case mROWOF:
				m_pForm->rowof = ATOI(ss);
				if (m_pForm->rowof != 0)
					m_pForm->attr2 |= GO_ROW;
				break;
			case mFIR:
				m_pForm->stRGB = ATOL(ss);
				break;
			case mSEC:
				m_pForm->spRGB = ATOL(ss);
				break;
			case mFH:
				break;

			default:
				break;
			}
		}
		Msg("");
		CString str,  cPRO = FORMAT("%c", mCLASS);
		vector<string>::iterator it;
		vector<string> vCELL;
		for (it = psub->begin(); it != psub->end(); it++)
		{
			str = CString(it->data(), it->length());
			str.Replace(12, 0x08);	
			it->clear();
			it->append(str);
//			int nCount = /// 컨트롤의 개수
//				boost::regex_split(std::back_inserter(vCELL), *it, boost::regex(cPRO.GetBuffer(0))); 
		
			int nCount = Split(vCELL, *it, mCLASS);

			m_pCELL->push_back(MakeSub(&vCELL));
			vCELL.clear();
		}

		Msg("");
	};
	~CiGrid(){};

	enum	
	{ 
		mVROW = mBOTTOM + 1,		// Visible Row
		mHEIGHT,
		mROW,
		mCOL,
		mTRGB,					// forecolor	
		mPRGB,					// backcolor
		mLRGB,					// lineColor
		mFOCUS,					// focusColor
		mFKIND,					// foucs kind
		mFNAME,				// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mSTR,					// variant
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR2,
		mTABORDER,				// taborder
		mORDER,	
		mTIN,					// total out#
		mTOUT,						// total in#
		mFROW,					// Fixed row
		mFCOL,					// Fixed column
		mHHEIGHT,				// head height
		mROWOF,
		mFIR,
		mSEC,
		mFH,
	};

	
	enum cell 
	{
		cmNAME = 0,
		cmSIZE,
		cmWIDTH,
		cmKEY,	//match key
		cmHEAD,  // head text
		cmALIGNH,
		cmALIGNV,
		cmHPRGB,
		cmHTRGB,
		cmOFFS,			// float digit
		cmTYPE,
		cmIOK,
		cmATTR,
		cmATTR2,
		cmPRGB,
		cmTRGB,
		cmAUXS,		// domino columnsymbol
		cmPROPERTIES,
		cmFORMAT,
		cmUPCOL,
		cmDNCOL,
		cmKIND,		// 컴보
		cmSTR,      // displaystring
		cmDAT,      // Combo dat
		cmSTR2,		// CheckString
		cmTXT,		// buttonstring
		cmIMAGE,     // image file name
		cmRTSSYMBOL,
	};

public:

	vector<struct _cellR*>* m_pCELL;
	void SetCellR(vector<struct _cellR*>* pCell){ m_pCELL = pCell; }
	struct _cellR* MakeSub(vector<string>* pDeque)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		struct _cellR* pCell = new struct _cellR;
		FillMemory(pCell, sizeof(struct _cellR), 0);

		char* pch[] = 
		{
			"cmNAME",
			"cmSIZE",
			"cmWIDTH",
			"cmKEY",
			"cmHEAD",
			"cmALIGNH",
			"cmALIGNV",
			"cmHPRGB",
			"cmHTRGB",
			"cmOFFS",
			"cmTYPE",
			"cmIOK",
			"cmATTR",
			"cmATTR2",
			"cmPRGB",
			"cmTRGB",
			"cmAUXS",
			"cmPROPERTIES",
			"cmFORMAT",
			"cmUPCOL",
			"cmDNCOL",
			"cmKIND",
			"cmSTR",
			"cmDAT",
			"cmSTR2",
			"cmTXT",
			"cmIMAGE",
			"cmRTSSYMBOL",
			"기타",
			"기타",
			"기타",
			"기타",
		};

		Msg("");

		for (ii = cmNAME ; ii < pDeque->size() ; ii++)
		{
			ss = (*pDeque)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

			Msg("%s %s", pch[ii-cmNAME], ss.data());
			
			switch (ii)
			{	
			case cmNAME:
				strcpy_s(pCell->name, nLen, ss.data());
				break;
			case cmSIZE:
				pCell->size = ATOI(ss);
				break;
			case cmWIDTH:
				pCell->width = ATOI(ss);
				break;
			case cmKEY:
				break;
			case cmHEAD:
				strcpy_s(pCell->head, nLen, ss.data());
				break;
			case cmALIGNH:
				pCell->alignH = ATOI(ss);
				break;
			case cmALIGNV:
				pCell->alignC = ATOI(ss);
				break;
			case cmHPRGB:
				pCell->hpRGB = ATOL(ss);
				break;
			case cmHTRGB:
				pCell->htRGB = ATOL(ss);
				break;
			case cmOFFS:
				pCell->digit = ATOL(ss);
				break;
			case cmTYPE:
				pCell->type = ATOI(ss);
				break;
			case cmIOK:
				pCell->iok = ATOI(ss);
				break;
			case cmATTR:
				pCell->attr = ATOL(ss);
				if (pCell->attr & FA_FLOATx)
				{
					pCell->properties |= FA_FLOAT;
					pCell->digit = 0;
				}
				break;
			case cmATTR2:
				pCell->attr2 = ATOL(ss); 
				break;
			case cmPRGB:
				pCell->pRGB = ATOL(ss);
				break;
			case cmTRGB:
				pCell->tRGB = ATOL(ss);
				break;
			case cmAUXS:
				strcpy_s(pCell->auxS, nLen, ss.data());
				break;
			case cmPROPERTIES:
				pCell->properties = ATOI(ss);
				break;
			case cmFORMAT:
				strcpy_s(pCell->editS, nLen, ss.data());
				break;
			case cmUPCOL:
				pCell->onEdit = MAKELONG(0, ATOI(ss));
				break;
			case cmDNCOL:
				pCell->onEdit = MAKELONG(ATOI(ss), HIWORD(pCell->onEdit));
				break;
			case cmKIND:
				pCell->kind = ATOI(ss);
				break;
			case cmSTR:
				strcpy_s(pCell->str, nLen, ss.data());
				break;
			case cmDAT:
				strcpy_s(pCell->dat, nLen, ss.data());
				break;
			case cmSTR2:
				strcpy_s(pCell->str2, nLen, ss.data());
				break;
			case cmTXT:
				strcpy_s(pCell->txt, nLen, ss.data());
				break;
			case cmIMAGE:
				strcpy_s(pCell->str3, nLen, ss.data());
				break;
			case cmRTSSYMBOL:
				strcpy_s(pCell->rts, nLen, ss.data());
				break;
			default:
				break;
			}
			
		}
		return pCell;

		Msg("");
	}
};



class CiTable : public CiBase
{
public:
	CiTable(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP, vector<struct _cellR*>* pCell)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		m_pCELL = pCell;		

		UINT ii = 0;
		int nLen;
		string ss;

		char* pch[] =	
		{ 
			"mROW",
			"mCOL",
			"mLRGB",
			"mHEIGHT",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mPROPERTIES",
			"mATTR2",
			"mIOK",
			"mORDER",
			"mTOUT",
			"mTIN",
			"기타",
			"기타",
			"기타",
			"기타",
		};

		m_pForm->kind = FM_TABLE;
		for (ii = mROW ; ii < m_pInfo->size()/*mORDER + 1*/ ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

			Msg("%s %s", pch[ii - mROW], ss.data());
		
						
			switch (ii)
			{
			case mROW:
				m_pForm->row = ATOI(ss);
				break;
			case mCOL:
				m_pForm->col = ATOI(ss);
				break;
			case mLRGB:
				m_pForm->bRGB = ATOL(ss);
				break;
			case mHEIGHT:
				m_pForm->keys = ATOL(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR2:
				m_pForm->attr2 = ATOL(ss);
				break;
			case mIOK:
				m_pForm->iok = ATOI(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			case mTOUT:
			case mTIN:
			default:
				break;
			}
		}

		CString str,  cPRO = FORMAT("%c", mCLASS);
		vector<string>::iterator it;
		vector<string> vCELL;
		for (it = psub->begin(); it != psub->end(); it++)
		{
			str = CString(it->data(), it->length());
			str.Replace(12, 0x08);	
			it->clear();
			it->append(str);
	//		int nCount = /// 컨트롤의 개수
	//			boost::regex_split(std::back_inserter(vCELL), *it, boost::regex(cPRO.GetBuffer(0))); 
		
			int nCount = Split(vCELL, *it, mCLASS);
			m_pCELL->push_back(MakeSub(&vCELL));
			vCELL.clear();
		}
		Msg("");
	};
	~CiTable(){};

	public:
	enum	
	{ 
		mROW = mBOTTOM + 1,	
		mCOL,
		mLRGB,				// lineColor
		mHEIGHT,
		mFNAME,				// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR2,
		mIOK,				// taborder
		mORDER,		
		mTOUT,					// total out#
		mTIN						// total in#

	};

	enum cell 
	{
		cmNAME = 0,
		cmSIZE,
		cmWIDTH,
		cmHEAD,  // head text
		cmALIGNH,
		cmALIGNV,
		cmHPRGB,
		cmHTRGB,
		cmOFFS,			// float digit
		cmTYPE,
		cmIOK,
		cmATTR,
		cmATTR2,

		cmPRGB,
		cmTRGB,
		cmAUXS,		// domino columnsymbol

		cmPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		cmFORMAT,
		cmUPCOL,
		cmDNCOL,
		cmRTSSYMBOL,

	};

public:
	vector<struct _cellR*>* m_pCELL;

	void SAFEDELETECELL(struct _cellR* t)
	{
		SAFE_DELETE(t);
	}
	struct _cellR* MakeSub(vector<string>* pDeque)
	{
		UINT ii = 0;
		int nLen;
		string ss;

		struct _cellR* pCell = new struct _cellR;
		FillMemory(pCell, sizeof(struct _cellR), 0);
		
		
		char* pch[] =
		{
			"cmNAME",
			"cmSIZE",
			"cmWIDTH",
			"cmHEAD",
			"cmALIGNH",
			"cmALIGNV",
			"cmHPRGB",
			"cmHTRGB",
			"cmOFFS",
			"cmTYPE",
			"cmIOK",
			"cmATTR",
			"cmATTR2",
			"cmPRGB",
			"cmTRGB",
			"cmAUXS",
			"cmPROPERTIES",
			"cmFORMAT",
			"cmUPCOL",
			"cmDNCOL",
			"cmRTSSYMBOL",
			"기타",
			"기타",
			"기타",
			"기타",

		};

		for (ii = cmNAME ; ii < pDeque->size() ; ii++)
		{
			ss = (*pDeque)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;

		//	if (ii != (cmUPCOL + 1))
			Msg("%s %s", pch[ii - cmNAME], ss.data());
		
		
			switch (ii)
			{
			case cmNAME:
				strcpy_s(pCell->name, nLen, ss.data());
				break;
			case cmSIZE:
				pCell->size = ATOI(ss);
				break;
			case cmWIDTH:
				pCell->width = ATOI(ss);
				break;
			case cmHEAD:
				strcpy_s(pCell->head, nLen, ss.data());
				break;
			case cmALIGNH:
				pCell->alignH = ATOI(ss);
				break;
			case cmALIGNV:
				pCell->alignC  = ATOI(ss);
				break;
			case cmHPRGB:
				pCell->hpRGB = ATOL(ss);
				break;
			case cmHTRGB:
				pCell->htRGB = ATOL(ss);
				break;
			case cmOFFS:
				pCell->digit = ATOL(ss);
				break;
			case cmTYPE:
				pCell->type = ATOI(ss);
				break;
			case cmIOK:
				pCell->iok = ATOI(ss);
				break;
			case cmATTR:
				pCell->attr = ATOI(ss);
				if (pCell->attr & FA_FLOATx)
				{
					pCell->properties |= FA_FLOAT;
					pCell->digit = 0;
				}
				break;
			case cmATTR2:
				pCell->attr2 = ATOL(ss);
				break;
			case cmPRGB:
				pCell->pRGB = ATOL(ss);
				break;
			case cmTRGB:
				pCell->tRGB = ATOL(ss);
				break;
			case cmAUXS:
				strcpy_s(pCell->auxS, nLen, ss.data());
				break;
			case cmPROPERTIES:
				pCell->properties = ATOI(ss);
				break;
			case cmFORMAT:
				strcpy_s(pCell->editS, nLen, ss.data());
				break;
			case cmUPCOL:
				pCell->onEdit = MAKELONG(0, ATOI(ss));
				break;
			case cmDNCOL:
				pCell->onEdit = MAKELONG(ATOI(ss), HIWORD(pCell->onEdit));
				break;
			case cmRTSSYMBOL:
				strcpy_s(pCell->rts, nLen, ss.data());
				break;
			default:
				break;
			}
		}
		Msg("");
			
		return pCell;
	};
};


class CiObject : public CiBase
{
public:
	CiObject(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;
		m_pForm->kind = FM_OBJECT;


		char *pch[] =	
		{
			"mSTR", 
			"mPROPERTIES",
			"mATTR",
			"mATTR2",
			"mTABORDER",
			"mORDER",
			"기타",
			"기타",
			"기타",
			"기타",
		};


		for (ii = mSTR ; ii < mORDER + 1 ; ii++)
		{

			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
		
			Msg("%s %s", pch[ii - mSTR], ss.data());
			
			switch (ii)
			{
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mATTR2:
				m_pForm->attr2 = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			default:
				break;
			}
		}
		Msg("");

	};
	~CiObject(){};

	enum	
	{
		mSTR = mBOTTOM + 1,	
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR,
		mATTR2,
		mTABORDER,				// taborder
		mORDER,		
	};
};


class CiTab : public  CiBase
{
public:
	CiTab(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;
		m_pForm->kind = FM_TAB;

		char* pch[] =	
		{ 
			"mINDEX", 
			"mCOUNT", 
			"mVCOUNT",
			"mATTR",
			"mFNAME", 
			"mFPOINT", 
			"mFSTYLE", 
			"mPRGB", 
			"mTRGB", 
			"mPROPERTIES", 
			"mALIGNIMAGE", 
			"mPATHIMAGE", 
			"mSPRGB", 
			"mSTRGB", 
			"mDAT", 
			"mINFO", 
			"mVCT",
			"mOBJ",
			"기타",
			"기타",
		};

		for (ii = mINDEX ; ii < m_pInfo->size(); ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
		
			Msg("%s %s", pch[ii - mINDEX], ss.data());
	
			switch (ii)
			{
			case mINDEX:
				m_pForm->checked = ATOI(ss);
				break;
			case mCOUNT:
				m_pForm->keys = MAKELONG(ATOI(ss), 0);
				break;
			case mVCOUNT:
				m_pForm->keys = MAKELONG(LOWORD(m_pForm->keys), ATOI(ss));
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss); 
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->fName, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mALIGNIMAGE:
				m_pForm->alignImage = ATOI(ss);
				break;
			case mPATHIMAGE:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mSPRGB:
				m_pForm->spRGB = ATOL(ss);
				break;
			case mSTRGB:
				m_pForm->stRGB = ATOL(ss);
				break;
			case mDAT:
				m_pForm->dat = new char[nLen];
				strcpy_s(m_pForm->dat, nLen, ss.data());
				break;
			case mINFO:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mVCT:
				m_pForm->size = ATOI(ss);
				break;
			case mOBJ:
				m_pForm->auxS = new char[nLen];
				strcpy_s(m_pForm->auxS, nLen, ss.data());
				break;
			}
		}
		Msg("");

	};
	~CiTab(){};

	enum	
	{ 
		mINDEX = mBOTTOM + 1,		// select index
		mCOUNT,
		mVCOUNT, 
		mATTR,
		mFNAME,				// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle)
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mALIGNIMAGE,			 // (ALIGN)
		mPATHIMAGE,			 // (imagefilename)
		mSPRGB,					// backcolor
		mSTRGB,					// forecolor	
		mDAT,
		mINFO,
		mVCT,
		mOBJ,					// OBJ
	};
};


class CiBrowser : public CiBase
{
public:
	CiBrowser(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;
		m_pForm->kind = FM_BROWSER;

		char* pch[] = 
		{
			"mIOK",
			"mSTR",
			"mPROPERTIES",
			"mATTR",
			"mORDER",
			"mTOUT",
			"mTIN",
		};


		for (ii = mIOK ; ii < mORDER + 1 ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
		
			Msg("%s %s", pch[ii - mIOK], ss.data());
		
			switch (ii)
			{
			case mIOK:
				m_pForm->iok = ATOI(ss);
				break;
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			case mTOUT:
			case mTIN:
			default:
				break;
			}
		}

				Msg("");
	};
	~CiBrowser(){};

	enum	
	{
		mIOK = mBOTTOM + 1,	
		mSTR,
		mPROPERTIES,			// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR,
		mORDER,			
		mTOUT,					// total out#
		mTIN						// total in#
	};
};


class CiControl : public CiBase
{
public:
	CiControl(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAP)
		: CiBase(pinfo, pscript, psub, pMAP)
	{
		UINT ii = 0;
		int nLen;
		string ss;
		m_pForm->kind = FM_CONTROL;

		char* pch[] = 	
		{ 
			"mPROPERTIES", 
			"mATTR",
			"mATTR2",
			"mSTR",
			"mPATH",
			"mIOK",
			"mFNAME",
			"mFPOINT",
			"mFSTYLE",
			"mPRGB",
			"mTRGB",
			"mTABORDER",
			"mORDER",
			"mTOUT",
			"mTIN",
			"기타",
			"기타",
		};


		for (ii = mPROPERTIES ; ii < m_pInfo->size() ; ii++)
		{
			ss = (*m_pInfo)[ii];
			algorithm::trim(ss);
			nLen = ss.length() + 1;
		
			Msg("%s %s", pch[ii - mPROPERTIES], ss.data());
		
			switch (ii)
			{
			case mPROPERTIES:
				m_pForm->properties = ATOI(ss);
				break;
			case mATTR:
				m_pForm->attr = ATOL(ss);
				break;
			case mATTR2:
				m_pForm->attr2 = ATOL(ss);
				break;
			case mSTR:
				m_pForm->str = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mPATH:
				m_pForm->str2 = new char[nLen];
				strcpy_s(m_pForm->str, nLen, ss.data());
				break;
			case mIOK:
				m_pForm->iok = ATOI(ss);
				break;
			case mFNAME:
				m_pForm->fName = new char[nLen];
				strcpy_s(m_pForm->str2, nLen, ss.data());
				break;
			case mFPOINT:
				m_pForm->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pForm->fStyle = ATOI(ss);
				break;
			case mPRGB:
				m_pForm->pRGB = ATOL(ss);
				break;
			case mTRGB:
				m_pForm->tRGB = ATOL(ss);
				break;
			case mTABORDER:
				m_pForm->index = ATOI(ss);
				break;
			case mORDER:
				m_pForm->iorder = ATOI(ss);
				break;
			case mTOUT:
			case mTIN:
			default:
				break;
			}
		}
		Msg("");
		ss.clear();
	};
	~CiControl(){};

	enum	
	{ 
		mPROPERTIES= mBOTTOM + 1,				// PR_ENABLE, PR_VISIBLE, PR_CHECKED, PR_IMAGE, PR_IMAGETEXT, PR_HOVER, PR_SORT, PR_EDIT
		mATTR,
		mATTR2,
		mSTR,		// Control name
		mPATH,
		mIOK,
		mFNAME,				// (fontname)
		mFPOINT,				// (fontsize)
		mFSTYLE,				// (fontstyle
		mPRGB,					// backcolor
		mTRGB,					// forecolor	
		mTABORDER,				// taborder
		mORDER,				
		mTOUT,					// total out#
		mTIN						// total in#
	};
};


class CiMap 
{
public:
	CiMap(vector<string>* pinfo, vector<string>* pscript, vector<string>* psub, map<string, char*>* pMAPSCR)
	{
		vector<string> sLua;
		m_pMAP = new struct _mapH;
		FillMemory(m_pMAP, sizeof(struct _mapH), 0);
		string ss;
		UINT ii = 0;

		m_pMAP->type = VW_FIXED;
		for (ii = 1 ; ii < mBGCLR + 1 ; ii++)
		{
			ss = (*pinfo)[ii];
			algorithm::trim(ss);

			switch(ii)
			{
			case mMAPN:
				strcpy_s(m_pMAP->mapN, ss.length() + 1, ss.data());
				break;
			case mHEIGHT:
				m_pMAP->height = ATOI(ss);
				break;
			case mWIDTH:
				m_pMAP->width =  ATOI(ss);
				break;
			case mMAPK:		
				m_pMAP->mapK =   ATOI(ss);
				break;
			case mMAPDESC:
				strcpy_s(m_pMAP->mapdesc, ss.length() + 1, ss.data()); 
				break;
			case mFNAME:
				strcpy_s(m_pMAP->fName, ss.length() + 1, ss.data());
				break;
			case mFPOINT:
				m_pMAP->fPoint = ATOI(ss);
				break;
			case mFSTYLE:
				m_pMAP->fStyle = ATOI(ss);
				break;
			case mOPTION:
				m_pMAP->options = ATOL(ss);
				break;
			case mCAPTION:	
				strcpy_s(m_pMAP->caption, ss.length() + 1, ss.data());
				break;
			case mTOPATH:	
				m_pMAP->toPath = ATOI(ss);
				break;
			case mMAPM:		
				m_pMAP->mapM = ATOI(ss);
				break;
			case mTRXC:
				strcpy_s(m_pMAP->trxC, ss.length() + 1, ss.data());
				break;
			case mREPEATV:	
				m_pMAP->repeatV = ATOI(ss);
				break;
			case mFLASH:		
				m_pMAP->flashClr = ATOL(ss); 
				break;
			case mFLASHSYM:	
				strcpy_s(m_pMAP->flashSym, ss.length() + 1, ss.data());
				break;
			case mUPCLR:
				m_pMAP->upClr = ATOL(ss);
				break;
			case mDNCLR:
				m_pMAP->dnClr = ATOL(ss);
				break;
			case mCONTRASTN:	
				strcpy_s(m_pMAP->contrastN, ss.length() + 1, ss.data());
				break;
			case mBGK:		
				m_pMAP->bgK = ATOI(ss);
				break;
			case mBGPATH:	
				strcpy_s(m_pMAP->bgFileN, ss.length() + 1, ss.data());
				break;
			case mBGCLR:		
				m_pMAP->bgClr = ATOL(ss);
				break;
			case mTYPE:
				m_pMAP->type = ATOI(ss);
				break;
			default:
				break;
			}
		}

		map<string, int> mScr;

		char* pScript[] = {	
			"_am_onload_am_",
			"_am_onsend_am_",
			"_am_onreceive_am_",
			"_am_onalert_am_",
			"_am_ontimer_am_",
			"_am_onclose_am_",
			"_am_ondevice_am_",
			"_am_onerror_am_",
			"_am_onflicking_am_",
			"_am_ondeclaration_am_",
			"_am_onindeclaration_am_", 
		};
		
		for each(char* pc in pScript)
			mScr[pc] = 1;

		
		for (UINT ii = 1 ; ii < pscript->size(); ii++)
		{
			ss = (*pscript)[ii];
/*
			CString str = CString(ss.data());
			str.Replace(12,  0x08);
			ss.clear();
			ss.append(str);
			CString cPRO = FORMAT("%c", mCLASS);
			
//			int nCount = /// 컨트롤의 개수
//				boost::regex_split(std::back_inserter(sLua), ss, boost::regex(cPRO.GetBuffer(0))); 
*/
			int nCount = Split(sLua, ss, 12);
			while(1)
			{
				if (sLua[0].empty())
					sLua.erase(sLua.begin());
				else 
					break;
			}

			if (sLua.empty() || sLua[0].empty())
				break;

			transform(sLua[0].begin(), sLua[0].end(), sLua[0].begin(), tolower);
			if (mScr[sLua[0]] && sLua.size() == 2)
			{	
				char* pch = new char[sLua[1].length() + 1];
				strcpy_s(pch, sLua[1].length() + 1, sLua[1].data()); 
				(*pMAPSCR)[sLua[0]] = pch;
			}
			sLua.clear();
		}
		

	};
	~CiMap(){};	

public:

	struct _mapH *m_pMAP;
	struct _mapH* GetMapH(){ if (m_pMAP) return m_pMAP;  return NULL; }

	enum
	{
		mMAPN = 1,
		mHEIGHT,
		mWIDTH,
		mMAPK,			// map kind    NORMAL, TEMPLATE, PROCEDURES
		mMAPDESC,
		mFNAME,
		mFPOINT,
		mFSTYLE,
		mOPTION,
		mCAPTION,		// title
		mTOPATH,		// AXIS, BP1, BP2
		mMAPM,			// FORM, MENU, SEND
		mTRXC,
		mREPEATV,		// repeat interval
		mFLASH,			// flash index
		mFLASHSYM,		// flash key string (comma delimiter)
		mUPCLR,
		mDNCLR,
		mCONTRASTN,		// contrast image file name
		mBGK,			// background kind
		mBGPATH,		// background image
		mBGCLR,			// background color
		mTYPE,			// form type
	};



	/*
	enum script
	{
	
	};
	*/
};


