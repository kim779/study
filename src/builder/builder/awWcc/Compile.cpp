// Compile.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "awWcc.h"
#include "Compile.h"
#include "Status.h"
#include "misc.hxx"
#include "../../h/wccSymbol.h"

//#include "XMLFile.h"
#include "../h/mapform.h"
#include "BinaryMngr.h"
#include "../awTool/awObjectLoad.h"


// CCompile
CCompile::CCompile()
{
	for (int ii = 0; ii < MAX_TEXT_LINE; ii++)
	{
		m_text[ii].text = NULL;
		m_text[ii].type = TYPE_NOTDEF;
	}
	m_textB  = NULL;
	m_textB2 = NULL;
	m_status = NULL;
	m_valueR = NULL;
	m_pageR = NULL;
	m_XMLMnger = NULL;
	m_pWnd = NULL;

	initVariables();
}

CCompile::~CCompile()
{
	if (m_textB)
		delete[] m_textB;
	if (m_textB2)
		delete[] m_textB2;
	if (m_status)
		delete m_status;

	if (m_XMLMnger) delete m_XMLMnger;
}

int CCompile::LoadSource(char* szPath, CWnd* hwnd, CString srcData)
{
	/////////////////////////////////////////
	//XMLMnger.LoadFile("C:\\TEST\\code.xml");
	m_pWnd = hwnd;

	CString dir;
	if (!srcData.IsEmpty())
	{
		dir.Format("%s\\temp.xml", m_curdir);

		CString strReadLine, strData;
		CStdioFile file;
		CFileException e;
		int ii = 0;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite, &e))
		{
			file.WriteString(srcData);
			file.Close();
		}
		szPath = (char*)dir.operator LPCSTR();
	}
	if (m_XMLMnger) delete m_XMLMnger;
	m_XMLMnger = new CXMLMnger();

	CString strPath(""), strTmp;
	strPath.Format("%s", szPath);
	strPath.MakeReverse();
	strTmp = strPath.Left(4);
	strTmp.MakeReverse();
	strPath.MakeReverse();
	if (strTmp.MakeUpper() != ".XML")
		strPath += ".XML";
	return m_XMLMnger->LoadFile((char*)strPath.operator LPCSTR());
	/////////////////////////////////////////
}

bool CCompile::makeBinaryFile(CString strTmp, mapForm* mapH)
{
	if (mapH->media == MEDIA_HTS)
		return makeHTSBinaryFile(strTmp, mapH);
	else
		return makeMTSBinaryFile(strTmp, mapH);
}

bool CCompile::makeHTSBinaryFile(CString strTmp, mapForm* mapH)
{
	CFile file;
	CString strScript("");
	if (!file.Open(strTmp + ".ax", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		setMessage(-1, 7);
		return false;
	}
	BYTE ScpKind = VBSCRIPT;

	CBinaryMngr BinaryMngr;
	BinaryMngr.AddBinaryData(&mapH->vers	,	sizeof(mapH->vers	));
	BinaryMngr.AddBinaryData(&mapH->height	,	sizeof(mapH->height	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->width	,	sizeof(mapH->width	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->fStyle	,	sizeof(mapH->fStyle	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->fPoint	,	sizeof(mapH->fPoint	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->fName	,	sizeof(mapH->fName	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->options	,	sizeof(mapH->options	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->caption	,	sizeof(mapH->caption	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->type	,	sizeof(mapH->type	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->mapN	,	sizeof(mapH->mapN	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->helpN	,	sizeof(mapH->helpN	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->publicR	,	sizeof(mapH->publicR	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->repeatV	,	sizeof(mapH->repeatV	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->imageK	,	sizeof(mapH->imageK	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->bkClr	,	sizeof(mapH->bkClr	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->imageV	,	sizeof(mapH->imageV	), SEQ_DATA);
	BinaryMngr.AddBinaryData(&ScpKind	,	sizeof(ScpKind		), SEQ_DATA);

	BinaryMngr.AddMapScript(mapH->onDeclarationN,	ONDECLARE);
	BinaryMngr.AddMapScript(mapH->onInDeclarationN, ONINDECLARE);
	BinaryMngr.AddMapScript(mapH->onStartN,		ONSTART);
	BinaryMngr.AddMapScript(mapH->onSendN,		ONSEND);
	BinaryMngr.AddMapScript(mapH->onReceiveN,	ONRECEIVE);
	BinaryMngr.AddMapScript(mapH->onAlertN,		ONALERT);
	BinaryMngr.AddMapScript(mapH->onServiceN,	ONSERVICE);
	BinaryMngr.AddMapScript(mapH->onFileN,		ONFILE);
	BinaryMngr.AddMapScript(mapH->onSelectN,	ONSELECT);
	BinaryMngr.AddMapScript(mapH->onTimerN,		ONTIMER);
	BinaryMngr.AddMapScript(mapH->onFocusN,		ONFOCUS);
	BinaryMngr.AddMapScript(mapH->onCloseN,		ONCLOSE);
	BinaryMngr.AddMapScript(mapH->onApproveN,	ONAPPREVE);
	BinaryMngr.AddMapScript(mapH->onKeyN,		ONKEY);
	BinaryMngr.AddMapScript(mapH->onDeviceN,	ONDEVICE);
	BinaryMngr.AddMapScript(mapH->onChangeLayoutN, ONCHANGELAYOUT);
	
	BinaryMngr.AddTRlayoutData(mapH->trinfo);
	BinaryMngr.AddRTSData(mapH->rtsinfo);
	//////////////////////////////////////////////////////////////////
	if (mapH->maplayout != NULL)
	{
		CMapStringToString* map = mapH->maplayout;
		CString sIndex, sData, ss;
		struct _Layout* playout = nullptr;

		CObjectLoad*	ObjLoad = NULL;
		CformProp*	Prop = NULL;

		CString strInfo("");
		for (POSITION pos = map->GetStartPosition(); pos;)
		{
			map->GetNextAssoc(pos, sIndex, sData);
			if (_T("Original") == sData)
				continue;
			strInfo = sData;
			strInfo += ",";
			for (int ii = 0; ii < mapH->forms.GetCount(); ii++)
			{
				ObjLoad = (CObjectLoad*)mapH->forms.GetAt(ii);
				Prop = (CformProp*)ObjLoad->getProp();

				// Delete layout...
				for (POSITION pos1 = Prop->layout->GetStartPosition(); pos1;)
				{
					Prop->layout->GetNextAssoc(pos1, ss, (void*&)playout);
					if (sData == ss)
					{
						strInfo.AppendFormat("%s,%d,%d,%d,%d,%d^", 
							Prop->name,
							playout->left, playout->top, playout->right - playout->left, playout->bottom - playout->top, (playout->properties & stVISIBLE) ? 1:0);
					}
				}
			}
			BinaryMngr.AddScreenLayoutData(strInfo);
		}
	}

	//////////////////////////////////////////////////////////////////
	CformProp* Prop = NULL;
	cellOb* CellProp = NULL;
	int pos = 0;
	int count = 0;
	for (int ii = 0; mapH->forms.GetCount() > ii; ii++)
	{
		Prop = ((CObjectLoad*)mapH->forms.GetAt(ii))->getProp();
		pos = 0;
		switch ((int)Prop->kind)
		{
		case fmLABEL:
			BinaryMngr.AddBinaryData(&Prop->kind		,	sizeof(Prop->kind),		SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR()  , Prop->name.GetLength(), SEQ_DATA);	
			BinaryMngr.AddBinaryData(&Prop->rect.left	,	sizeof(Prop->rect.left),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top	,	sizeof(Prop->rect.top),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right	,	sizeof(Prop->rect.right),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom	,	sizeof(Prop->rect.bottom),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes	,	sizeof(Prop->attributes),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles		,	sizeof(Prop->styles),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor	,	sizeof(Prop->paintColor),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor	,	sizeof(Prop->textColor),	SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint	,	sizeof(Prop->fontPoint),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle	,	sizeof(Prop->fontStyle),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment	,	sizeof(Prop->alignment),	SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint	,	sizeof(Prop->typeHint),		SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL,	ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL,	ONDBLCLICK);
			break;
		case fmBOX:
			BinaryMngr.AddBinaryData(&Prop->kind		,	sizeof(Prop->kind),		SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR()  , Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left	,	sizeof(Prop->rect.left),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top	,	sizeof(Prop->rect.top),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right	,	sizeof(Prop->rect.right),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom	,	sizeof(Prop->rect.bottom),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size		,	sizeof(Prop->paintColor),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType	,	sizeof(Prop->textColor),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes	,	sizeof(Prop->size),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles		,	sizeof(Prop->charType),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor	,	sizeof(Prop->attributes),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint	,	sizeof(Prop->group),		SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmGROUP:
			BinaryMngr.AddBinaryData(&Prop->kind		,	sizeof(Prop->kind),		SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR()  , Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left	,	sizeof(Prop->rect.left),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top	,	sizeof(Prop->rect.top),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right	,	sizeof(Prop->rect.right),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom	,	sizeof(Prop->rect.bottom),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size		,	sizeof(Prop->size),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType	,	sizeof(Prop->charType),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes	,	sizeof(Prop->attributes),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles		,	sizeof(Prop->styles),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor	,	sizeof(Prop->paintColor),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint	,	sizeof(Prop->typeHint),		SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmIMVIEW:
			BinaryMngr.AddBinaryData(&Prop->kind		,	sizeof(Prop->kind),		SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR()  , Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left	,	sizeof(Prop->rect.left),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top	,	sizeof(Prop->rect.top),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right	,	sizeof(Prop->rect.right),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom	,	sizeof(Prop->rect.bottom),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle	,	sizeof(Prop->borderStyle),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes	,	sizeof(Prop->attributes),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles		,	sizeof(Prop->styles),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor	,	sizeof(Prop->paintColor),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor	,	sizeof(Prop->textColor),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor	,	sizeof(Prop->borderColor),	SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage	,	sizeof(Prop->alignmentImage),	SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint	,	sizeof(Prop->typeHint),		SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmEDIT:
			BinaryMngr.AddBinaryData(&Prop->kind					,	sizeof(Prop->kind),			SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR()		,	Prop->name.GetLength(),			SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left				,	sizeof(Prop->rect.left),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top				,	sizeof(Prop->rect.top),			SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right				,	sizeof(Prop->rect.right),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom				,	sizeof(Prop->rect.bottom),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size					,	sizeof(Prop->size),			SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType				,	sizeof(Prop->charType),			SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType					,	sizeof(Prop->ioType),			SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle				,	sizeof(Prop->borderStyle),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes				,	sizeof(Prop->attributes),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes				,	sizeof(Prop->auxAttributes),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles					,	sizeof(Prop->styles),			SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor				,	sizeof(Prop->paintColor),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor				,	sizeof(Prop->textColor),		SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR()	,	Prop->fontName.GetLength(),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint				,	sizeof(Prop->fontPoint),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle				,	sizeof(Prop->fontStyle),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues				,	sizeof(Prop->keyValues),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyParam				,	sizeof(Prop->keyParam),			SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR()		,	Prop->strings.GetLength(),		SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->keyNames.operator LPCSTR()	,	Prop->keyNames.GetLength(),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->offsets					,	sizeof(Prop->offsets),			SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->whatEdit				,	sizeof(Prop->whatEdit),			SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->editFormat.operator LPCSTR()	,	Prop->editFormat.GetLength(),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxValues				,	sizeof(Prop->auxValues),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index					,	sizeof(Prop->index),			SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->flashKeys.operator LPCSTR()	,	Prop->flashKeys.GetLength(),		SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint				,	sizeof(Prop->typeHint),			SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR()	,	Prop->hintText.GetLength(),		SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onCharL, ONCHARCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onKeyL, ONKEYINPUT);
			break;
		case fmOUTPUT:
			BinaryMngr.AddBinaryData(&Prop->kind			,	sizeof(Prop->kind		), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left		,	sizeof(Prop->rect.left		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top		,	sizeof(Prop->rect.top		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right		,	sizeof(Prop->rect.right		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom		,	sizeof(Prop->rect.bottom	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size			,	sizeof(Prop->size		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType		,	sizeof(Prop->charType		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle		,	sizeof(Prop->borderStyle	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes		,	sizeof(Prop->attributes		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes		,	sizeof(Prop->auxAttributes	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles			,	sizeof(Prop->styles		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor		,	sizeof(Prop->paintColor		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor		,	sizeof(Prop->textColor		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor		,	sizeof(Prop->borderColor	), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint		,	sizeof(Prop->fontPoint		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle		,	sizeof(Prop->fontStyle		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment		,	sizeof(Prop->alignment		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->offsets			,	sizeof(Prop->offsets		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->keyNames.operator LPCSTR(), Prop->keyNames.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->referenceName.operator LPCSTR(), Prop->referenceName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->whatEdit		,	sizeof(Prop->whatEdit		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->editFormat.operator LPCSTR(), Prop->editFormat.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxValues		,	sizeof(Prop->auxValues		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->flashKeys.operator LPCSTR(), Prop->flashKeys.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint		,	sizeof(Prop->typeHint		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmBUTTON:
			BinaryMngr.AddBinaryData(&Prop->kind		,	sizeof(Prop->kind			), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR()		,	Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left	,	sizeof(Prop->rect.left			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top	,	sizeof(Prop->rect.top			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right	,	sizeof(Prop->rect.right			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom	,	sizeof(Prop->rect.bottom		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes	,	sizeof(Prop->attributes			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles		,	sizeof(Prop->styles			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor	,	sizeof(Prop->paintColor			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor	,	sizeof(Prop->textColor			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint	,	sizeof(Prop->fontPoint			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle	,	sizeof(Prop->fontStyle			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment	,	sizeof(Prop->alignment			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyParam	,	sizeof(Prop->keyParam			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index		,	sizeof(Prop->index			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint	,	sizeof(Prop->typeHint			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmCHECK:
			BinaryMngr.AddBinaryData(&Prop->kind					,	sizeof(Prop->kind			), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR()		,	Prop->name.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left				,	sizeof(Prop->rect.left			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top				,	sizeof(Prop->rect.top			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right				,	sizeof(Prop->rect.right			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom				,	sizeof(Prop->rect.bottom		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType					,	sizeof(Prop->ioType			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes				,	sizeof(Prop->attributes			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles					,	sizeof(Prop->styles			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor				,	sizeof(Prop->paintColor			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor				,	sizeof(Prop->textColor			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR()	,	Prop->fontName.GetLength()		 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint				,	sizeof(Prop->fontPoint			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle				,	sizeof(Prop->fontStyle			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage				,	sizeof(Prop->alignment			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR()		,	Prop->strings.GetLength()		 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index					,	sizeof(Prop->index			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint				,	sizeof(Prop->typeHint			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR()	,	Prop->hintText.GetLength()		 , SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmRADIO:
			BinaryMngr.AddBinaryData(&Prop->kind		,	sizeof(Prop->kind			), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left	,	sizeof(Prop->rect.left			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top	,	sizeof(Prop->rect.top			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right	,	sizeof(Prop->rect.right			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom	,	sizeof(Prop->rect.bottom		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType		,	sizeof(Prop->ioType			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes	,	sizeof(Prop->attributes			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles		,	sizeof(Prop->styles			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor	,	sizeof(Prop->paintColor			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor	,	sizeof(Prop->textColor			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint	,	sizeof(Prop->fontPoint			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle	,	sizeof(Prop->fontStyle			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage	,	sizeof(Prop->alignmentImage		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->keyNames.operator LPCSTR(), Prop->keyNames.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index		,	sizeof(Prop->index			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint	,	sizeof(Prop->typeHint			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmCOMBO:
			BinaryMngr.AddBinaryData(&Prop->kind		,	sizeof(Prop->kind		), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left	,	sizeof(Prop->rect.left		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top	,	sizeof(Prop->rect.top		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right	,	sizeof(Prop->rect.right		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom	,	sizeof(Prop->rect.bottom	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size		,	sizeof(Prop->size		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType	,	sizeof(Prop->charType		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType		,	sizeof(Prop->ioType		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle	,	sizeof(Prop->borderStyle	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes	,	sizeof(Prop->attributes		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes	,	sizeof(Prop->auxAttributes	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles		,	sizeof(Prop->styles		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor	,	sizeof(Prop->paintColor		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor	,	sizeof(Prop->textColor		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint	,	sizeof(Prop->fontPoint		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle	,	sizeof(Prop->fontStyle		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues	,	sizeof(Prop->keyValues		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->displays.operator LPCSTR(), Prop->displays.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->checked		,	sizeof(Prop->checked		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyParam	,	sizeof(Prop->keyParam		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index		,	sizeof(Prop->index		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint	,	sizeof(Prop->typeHint		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			break;
		case fmTABLE:
			if (Prop->cellProperties)
				count = Prop->cellProperties->GetCount();
			else
				count = 0;
			BinaryMngr.AddBinaryData(&Prop->kind		,		sizeof(Prop->kind		), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left	,		sizeof(Prop->rect.left		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top	,		sizeof(Prop->rect.top		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right	,		sizeof(Prop->rect.right		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom	,		sizeof(Prop->rect.bottom	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType		,		sizeof(Prop->ioType		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues	,		sizeof(Prop->keyValues		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&count			,		sizeof(count			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor	,		sizeof(Prop->borderColor	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes	,		sizeof(Prop->attributes		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes	,		sizeof(Prop->auxAttributes	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles		,		sizeof(Prop->styles		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint	,		sizeof(Prop->fontPoint		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle	,		sizeof(Prop->fontStyle		), SEQ_DATA);

			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				BinaryMngr.AddBinaryData((char*)CellProp->name.operator LPCSTR(), CellProp->name.GetLength(), SEQ_CELL);
				BinaryMngr.AddBinaryData(&CellProp->size,		sizeof(CellProp->size		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->width,		sizeof(CellProp->width		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->charType,		sizeof(CellProp->charType	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->attributes,		sizeof(CellProp->attributes	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->auxAttributes,	sizeof(CellProp->auxAttributes	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->styles,		sizeof(CellProp->styles		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->paintColor,		sizeof(CellProp->paintColor	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->textColor,		sizeof(CellProp->textColor	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->headPaintColor,	sizeof(CellProp->headPaintColor	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->headTextColor,	sizeof(CellProp->headTextColor	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentHead,	sizeof(CellProp->alignmentHead	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentText,	sizeof(CellProp->alignmentText	), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->whatEdit,		sizeof(CellProp->whatEdit	), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->editFormat.operator LPCSTR(), CellProp->editFormat.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->auxValues, sizeof(CellProp->auxValues), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->offsets,		sizeof(CellProp->offsets	), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->headText.operator LPCSTR(), CellProp->headText.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->dominoName.operator LPCSTR(), CellProp->dominoName.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->referenceName.operator LPCSTR(), CellProp->referenceName.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->flashKeys.operator LPCSTR(), CellProp->flashKeys.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->typeHint,		sizeof(CellProp->typeHint	), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->hintText.operator LPCSTR(), CellProp->hintText.GetLength(), SEQ_DATA);
			}
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmTREE:
			BinaryMngr.AddBinaryData(&Prop->kind			,		sizeof(Prop->kind				), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left		,		sizeof(Prop->rect.left				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top		,		sizeof(Prop->rect.top				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right		,		sizeof(Prop->rect.right				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom		,		sizeof(Prop->rect.bottom			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size			,		sizeof(Prop->size				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType		,		sizeof(Prop->charType				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType			,		sizeof(Prop->ioType				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes		,		sizeof(Prop->attributes				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles			,		sizeof(Prop->styles				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor		,		sizeof(Prop->paintColor				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor		,		sizeof(Prop->textColor				), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint		,		sizeof(Prop->fontPoint				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle		,		sizeof(Prop->fontStyle				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType			,		sizeof(Prop->ioType				), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			break;

		case fmOBJECT:
			BinaryMngr.AddBinaryData(&Prop->kind					,		sizeof(Prop->kind				), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR()		,		Prop->name.GetLength()				 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left				,		sizeof(Prop->rect.left				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top				,		sizeof(Prop->rect.top				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right				,		sizeof(Prop->rect.right				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom				,		sizeof(Prop->rect.bottom			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType					,		sizeof(Prop->ioType				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType				,		sizeof(Prop->charType				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size					,		sizeof(Prop->size				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fixColumn				,		sizeof(Prop->fixColumn				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fixRow					,		sizeof(Prop->fixRow				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle				,		sizeof(Prop->borderStyle			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes				,		sizeof(Prop->attributes				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes				,		sizeof(Prop->auxAttributes			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles					,		sizeof(Prop->styles				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor				,		sizeof(Prop->paintColor				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor				,		sizeof(Prop->textColor				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor				,		sizeof(Prop->borderColor			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1				,		sizeof(Prop->paintColor1			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor2				,		sizeof(Prop->paintColor2			), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR()	,		Prop->fontName.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint				,		sizeof(Prop->fontPoint				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle				,		sizeof(Prop->fontStyle				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment				,		sizeof(Prop->alignment				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage				,		sizeof(Prop->alignmentImage			), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->offsets					,		sizeof(Prop->offsets				), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->keyNames.operator LPCSTR()	,		Prop->keyNames.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues				,		sizeof(Prop->keyValues				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyParam				,		sizeof(Prop->keyParam				), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->referenceName.operator LPCSTR()	,		Prop->referenceName.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->checked					,		sizeof(Prop->checked				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeCursor				,		sizeof(Prop->typeCursor				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->whatEdit				,		sizeof(Prop->whatEdit				), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->editFormat.operator LPCSTR()	,		Prop->editFormat.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxValues				,		sizeof(Prop->auxValues				), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->displays.operator LPCSTR()	,		Prop->displays.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR()		,		Prop->strings.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR()		,		Prop->images.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->auxImages.operator LPCSTR()	,		Prop->auxImages.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->posImage				,		sizeof(Prop->posImage				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->headHeight				,		sizeof(Prop->headHeight				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rowLines				,		sizeof(Prop->rowLines				), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index					,		sizeof(Prop->index				), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->flashKeys.operator LPCSTR()	,		Prop->flashKeys.GetLength()			 , SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint				,		sizeof(Prop->typeHint				), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR()	,		Prop->hintText.GetLength()			 , SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onCharL, ONCHARCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onKeyL, ONKEYINPUT);
			break;
		case fmFORM:
			BinaryMngr.AddBinaryData(&Prop->kind			,		sizeof(Prop->kind		), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left		,		sizeof(Prop->rect.left		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top		,		sizeof(Prop->rect.top		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right		,		sizeof(Prop->rect.right		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom		,		sizeof(Prop->rect.bottom	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes		,		sizeof(Prop->attributes		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes		,		sizeof(Prop->auxAttributes	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles			,		sizeof(Prop->styles		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index			,		sizeof(Prop->index		), SEQ_DATA);
			break;
		case fmTAB:
			BinaryMngr.AddBinaryData(&Prop->kind			,		sizeof(Prop->kind		), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left		,		sizeof(Prop->rect.left		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top		,		sizeof(Prop->rect.top		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right		,		sizeof(Prop->rect.right		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom		,		sizeof(Prop->rect.bottom	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes		,		sizeof(Prop->attributes		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles			,		sizeof(Prop->styles		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor		,		sizeof(Prop->paintColor		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor		,		sizeof(Prop->textColor		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor2		,		sizeof(Prop->paintColor2	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1		,		sizeof(Prop->paintColor1	), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint		,		sizeof(Prop->fontPoint		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle		,		sizeof(Prop->fontStyle		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment		,		sizeof(Prop->alignment		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->checked			,		sizeof(Prop->checked		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues		,		sizeof(Prop->keyValues		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->displays.operator LPCSTR(), Prop->displays.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint		,		sizeof(Prop->typeHint		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmBROWSER:
			BinaryMngr.AddBinaryData(&Prop->kind			,		sizeof(Prop->kind		), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left		,		sizeof(Prop->rect.left		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top		,		sizeof(Prop->rect.top		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right		,		sizeof(Prop->rect.right		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom		,		sizeof(Prop->rect.bottom	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType			,		sizeof(Prop->ioType		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes		,		sizeof(Prop->attributes		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles			,		sizeof(Prop->styles		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint		,		sizeof(Prop->typeHint		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);
			break;
		case fmMEMO:
			BinaryMngr.AddBinaryData(&Prop->kind			,		sizeof(Prop->kind		), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left		,		sizeof(Prop->rect.left		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top		,		sizeof(Prop->rect.top		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right		,		sizeof(Prop->rect.right		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom		,		sizeof(Prop->rect.bottom	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType		,		sizeof(Prop->charType		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType			,		sizeof(Prop->ioType		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes		,		sizeof(Prop->attributes		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles			,		sizeof(Prop->styles		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle		,		sizeof(Prop->borderStyle	), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor		,		sizeof(Prop->paintColor		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor		,		sizeof(Prop->textColor		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint		,		sizeof(Prop->fontPoint		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle		,		sizeof(Prop->fontStyle		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues		,		sizeof(Prop->keyValues		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index			,		sizeof(Prop->index		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint		,		sizeof(Prop->typeHint		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			break;
		case fmGRID:
		case fmGRIDEX:
			if (Prop->cellProperties)
				count = Prop->cellProperties->GetCount();
			else
				count = 0;
			BinaryMngr.AddBinaryData(&Prop->kind				,		sizeof(Prop->kind	  		), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left			,		sizeof(Prop->rect.left	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top			,		sizeof(Prop->rect.top	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right			,		sizeof(Prop->rect.right	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom			,		sizeof(Prop->rect.bottom  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues			,		sizeof(Prop->keyValues	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size				,		sizeof(Prop->size	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fixColumn			,		sizeof(Prop->fixColumn	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes			,		sizeof(Prop->attributes	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes			,		sizeof(Prop->auxAttributes		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles				,		sizeof(Prop->styles	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor			,		sizeof(Prop->paintColor	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor			,		sizeof(Prop->textColor	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor			,		sizeof(Prop->borderColor  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1			,		sizeof(Prop->paintColor1  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor2			,		sizeof(Prop->paintColor2  		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontPoint			,		sizeof(Prop->fontPoint	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle			,		sizeof(Prop->fontStyle	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->whatEdit			,		sizeof(Prop->whatEdit	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxValues			,		sizeof(Prop->auxValues	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rowLines			,		sizeof(Prop->rowLines	  		), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index				,		sizeof(Prop->index	  		), SEQ_DATA);

			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				BinaryMngr.AddBinaryData((char*)CellProp->name.operator LPCSTR()		,		CellProp->name.GetLength()			, SEQ_CELL);
				BinaryMngr.AddBinaryData(&CellProp->kind		,		sizeof(CellProp->kind			), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->size		,		sizeof(CellProp->size			), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->charType		,		sizeof(CellProp->charType		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->ioType		,		sizeof(CellProp->ioType			), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->width		,		sizeof(CellProp->width			), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->attributes		,		sizeof(CellProp->attributes		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->auxAttributes	,		sizeof(CellProp->auxAttributes		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->styles		,		sizeof(CellProp->styles			), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->paintColor		,		sizeof(CellProp->paintColor		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->textColor		,		sizeof(CellProp->textColor		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->headPaintColor	,		sizeof(CellProp->headPaintColor		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->headTextColor	,		sizeof(CellProp->headTextColor		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentHead	,		sizeof(CellProp->alignmentHead		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentImage	,		sizeof(CellProp->alignmentImage		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentText	,		sizeof(CellProp->alignmentText		), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->whatEdit		,		sizeof(CellProp->whatEdit		), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->editFormat.operator LPCSTR(), CellProp->editFormat.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->auxValues		,		sizeof(CellProp->auxValues		), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->headText.operator LPCSTR(), CellProp->headText.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->images.operator LPCSTR(), CellProp->images.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->entryCombo		,		sizeof(CellProp->entryCombo		), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->displays.operator LPCSTR(), CellProp->displays.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->strings.operator LPCSTR(), CellProp->strings.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->offsets		,		sizeof(CellProp->offsets		), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->dominoName.operator LPCSTR(), CellProp->dominoName.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->referenceName.operator LPCSTR(), CellProp->referenceName.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->keyParam		,		sizeof(CellProp->keyParam		), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->flashKeys.operator LPCSTR(), CellProp->flashKeys.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->typeHint		,		sizeof(CellProp->typeHint		), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->hintText.operator LPCSTR(), CellProp->hintText.GetLength(), SEQ_DATA);
				if (Prop->kind == fmGRIDEX)
					BinaryMngr.AddBinaryData((char*)CellProp->formula.operator LPCSTR()	,		CellProp->formula.GetLength()		, SEQ_DATA);
			}
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onCharL, ONCHARCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onKeyL, ONKEYINPUT);
			break;
		default:
			break;
		}
	}

	file.Write(BinaryMngr.getBuf(), BinaryMngr.getSize());
	file.Close();

	return true;
}

bool CCompile::makeMTSBinaryFile(CString strTmp, mapForm* mapH)
{
	CFile file;
	CString strScript("");
	if (!file.Open(strTmp + ".ax", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		setMessage(-1, 7);
		return false;
	}
	BYTE ScpKind = LUA;

	WORD wdFontHeight = 12; // default 9 point
	CDC* pDC = NULL;
	if (m_pWnd != NULL)
	{
		pDC = m_pWnd->GetDC();
		wdFontHeight = MulDiv(mapH->fPoint, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
	}

	CBinaryMngr BinaryMngr;
	BinaryMngr.AddBinaryData(&mapH->vers			      ,			sizeof(mapH->vers		)	   );
	BinaryMngr.AddBinaryData(&mapH->mapN			      ,			sizeof(mapH->mapN		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->height			      ,			sizeof(mapH->height		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->width			      ,			sizeof(mapH->width		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->controller		      ,			sizeof(mapH->controller		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->fStyle			      ,			sizeof(mapH->fStyle		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&wdFontHeight			     ,			sizeof(wdFontHeight		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->fName			      ,			sizeof(mapH->fName		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->options			      ,			sizeof(mapH->options		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->caption			      ,			sizeof(mapH->caption		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->type			      ,			sizeof(mapH->type		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->helpN			      ,			sizeof(mapH->helpN		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->publicR			      ,			sizeof(mapH->publicR		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->imageK			      ,			sizeof(mapH->imageK		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->bkClr			      ,			sizeof(mapH->bkClr		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->imageV			      ,			sizeof(mapH->imageV		), SEQ_DATA);
	BinaryMngr.AddBinaryData(&ScpKind			      ,			sizeof(ScpKind			), SEQ_DATA);
	BinaryMngr.AddBinaryData(&mapH->alpha			      ,			sizeof(mapH->alpha		), SEQ_DATA);

	BinaryMngr.AddMapScript(mapH->onDeclarationN, ONDECLARE);
	BinaryMngr.AddMapScript(mapH->onInDeclarationN, ONINDECLARE);
	BinaryMngr.AddMapScript(mapH->onStartN, ONSTART);
	BinaryMngr.AddMapScript(mapH->onSendN, ONSEND);
	BinaryMngr.AddMapScript(mapH->onReceiveN, ONRECEIVE);
	BinaryMngr.AddMapScript(mapH->onAlertN, ONALERT);
	BinaryMngr.AddMapScript(mapH->onServiceN, ONSERVICE);
	BinaryMngr.AddMapScript(mapH->onFileN, ONFILE);
	BinaryMngr.AddMapScript(mapH->onSelectN, ONSELECT);
	BinaryMngr.AddMapScript(mapH->onTimerN, ONTIMER);
	BinaryMngr.AddMapScript(mapH->onFocusN, ONFOCUS);
	BinaryMngr.AddMapScript(mapH->onCloseN, ONCLOSE);
	BinaryMngr.AddMapScript(mapH->onApproveN, ONAPPREVE);
	BinaryMngr.AddMapScript(mapH->onKeyN, ONKEY);
	BinaryMngr.AddMapScript(mapH->onDeviceN, ONDEVICE);
	BinaryMngr.AddMapScript(mapH->onChangeLayoutN, ONCHANGELAYOUT);
	BinaryMngr.AddMapScript(mapH->onFlickingN, ONFLICKING);

	BinaryMngr.AddTRlayoutData(mapH->trinfo);
	BinaryMngr.AddRTSData(mapH->rtsinfo);
	//////////////////////////////////////////////////////////////////
	if (mapH->maplayout != NULL)
	{
		CMapStringToString* map = mapH->maplayout;
		CString sIndex, sData, ss;
		struct _Layout* playout = nullptr;

		CObjectLoad*	ObjLoad = NULL;
		CformProp*	Prop = NULL;

		CString strInfo("");
		for (POSITION pos = map->GetStartPosition(); pos;)
		{
			map->GetNextAssoc(pos, sIndex, sData);
			if (_T("Original") == sData)
				continue;
			strInfo = sData;
			strInfo += ",";
			for (int ii = 0; ii < mapH->forms.GetCount(); ii++)
			{
				ObjLoad = (CObjectLoad*)mapH->forms.GetAt(ii);
				Prop = (CformProp*)ObjLoad->getProp();

				// Delete layout...
				for (POSITION pos1 = Prop->layout->GetStartPosition(); pos1;)
				{
					Prop->layout->GetNextAssoc(pos1, ss, (void*&)playout);
					if (sData == ss)
					{
						strInfo.AppendFormat("%s,%d,%d,%d,%d,%d^",
							Prop->name,
							playout->left, playout->top, playout->right - playout->left, playout->bottom - playout->top, (playout->properties & stVISIBLE) ? 1 : 0);
					}
				}
			}
			BinaryMngr.AddScreenLayoutData(strInfo);
		}
	}

	//////////////////////////////////////////////////////////////////
	CformProp* Prop = NULL;
	cellOb* CellProp = NULL;
	int pos = 0;
	int count = 0;
	int fheight = 12; // default 9 point

	for (int ii = 0; mapH->forms.GetCount() > ii; ii++)
	{
		Prop = ((CObjectLoad*)mapH->forms.GetAt(ii))->getProp();
		pos = 0;
		fheight = 12; // default 9 point
		if (m_pWnd != NULL)
		{
			fheight = MulDiv(Prop->fontPoint, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
		}
		switch ((int)Prop->kind)
		{
		case fmLABEL:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->objClass.operator LPCSTR(), Prop->objClass.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle, sizeof(Prop->borderStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor, sizeof(Prop->borderColor), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment, sizeof(Prop->alignment), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage, sizeof(Prop->alignmentImage), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alpha, sizeof(Prop->alpha), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->margin.operator LPCSTR(), Prop->margin.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmBOX:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->objClass.operator LPCSTR(), Prop->objClass.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->size), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->charType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->group), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alpha, sizeof(Prop->alpha), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmGROUP:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->objClass.operator LPCSTR(), Prop->objClass.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size, sizeof(Prop->size), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType, sizeof(Prop->charType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmIMVIEW:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->objClass.operator LPCSTR(), Prop->objClass.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle, sizeof(Prop->borderStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor, sizeof(Prop->borderColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage, sizeof(Prop->alignmentImage), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alpha, sizeof(Prop->alpha), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmEDIT:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->objClass.operator LPCSTR(), Prop->objClass.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size, sizeof(Prop->size), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType, sizeof(Prop->charType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle, sizeof(Prop->borderStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes, sizeof(Prop->auxAttributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1, sizeof(Prop->paintColor1), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor, sizeof(Prop->borderColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxColor, sizeof(Prop->auxColor), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment, sizeof(Prop->alignment), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage, sizeof(Prop->alignmentImage), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->keyNames.operator LPCSTR(), Prop->keyNames.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues, sizeof(Prop->keyValues), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyParam, sizeof(Prop->keyParam), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->offsets, sizeof(Prop->offsets), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->whatEdit, sizeof(Prop->whatEdit), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->editFormat.operator LPCSTR(), Prop->editFormat.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxValues, sizeof(Prop->auxValues), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->flashKeys.operator LPCSTR(), Prop->flashKeys.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alpha, sizeof(Prop->alpha), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->margin.operator LPCSTR(), Prop->margin.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rtsShape, sizeof(Prop->rtsShape), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onCharL, ONCHARCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onKeyL, ONKEYINPUT);
			break;
		case fmOUTPUT:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size, sizeof(Prop->size), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType, sizeof(Prop->charType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle, sizeof(Prop->borderStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes, sizeof(Prop->auxAttributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor, sizeof(Prop->borderColor), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment, sizeof(Prop->alignment), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->offsets, sizeof(Prop->offsets), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->keyNames.operator LPCSTR(), Prop->keyNames.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->referenceName.operator LPCSTR(), Prop->referenceName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->whatEdit, sizeof(Prop->whatEdit), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->editFormat.operator LPCSTR(), Prop->editFormat.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxValues, sizeof(Prop->auxValues), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->flashKeys.operator LPCSTR(), Prop->flashKeys.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmBUTTON:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->objClass.operator LPCSTR(), Prop->objClass.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor2, sizeof(Prop->paintColor2), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1, sizeof(Prop->paintColor1), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment, sizeof(Prop->alignment), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->auxImages.operator LPCSTR(), Prop->auxImages.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyParam, sizeof(Prop->keyParam), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alpha, sizeof(Prop->alpha), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->margin.operator LPCSTR(), Prop->margin.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmCHECK:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->objClass.operator LPCSTR(), Prop->objClass.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1, sizeof(Prop->paintColor1), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment, sizeof(Prop->alignment), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage, sizeof(Prop->alignmentImage), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->auxImages.operator LPCSTR(), Prop->auxImages.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alpha, sizeof(Prop->alpha), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->margin.operator LPCSTR(), Prop->margin.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmRADIO:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage, sizeof(Prop->alignmentImage), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->keyNames.operator LPCSTR(), Prop->keyNames.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmCOMBO:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size, sizeof(Prop->size), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType, sizeof(Prop->charType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle, sizeof(Prop->borderStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes, sizeof(Prop->auxAttributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues, sizeof(Prop->keyValues), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->displays.operator LPCSTR(), Prop->displays.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->checked, sizeof(Prop->checked), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyParam, sizeof(Prop->keyParam), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			break;
		case fmTABLE:
			if (Prop->cellProperties)
				count = Prop->cellProperties->GetCount();
			else
				count = 0;
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues, sizeof(Prop->keyValues), SEQ_DATA);
			BinaryMngr.AddBinaryData(&count, sizeof(count), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor, sizeof(Prop->borderColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes, sizeof(Prop->auxAttributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);

			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				BinaryMngr.AddBinaryData((char*)CellProp->name.operator LPCSTR(), CellProp->name.GetLength(), SEQ_CELL);
				BinaryMngr.AddBinaryData(&CellProp->size, sizeof(CellProp->size), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->width, sizeof(CellProp->width), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->charType, sizeof(CellProp->charType), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->attributes, sizeof(CellProp->attributes), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->auxAttributes, sizeof(CellProp->auxAttributes), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->styles, sizeof(CellProp->styles), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->paintColor, sizeof(CellProp->paintColor), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->textColor, sizeof(CellProp->textColor), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->headPaintColor, sizeof(CellProp->headPaintColor), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->headTextColor, sizeof(CellProp->headTextColor), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentHead, sizeof(CellProp->alignmentHead), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentText, sizeof(CellProp->alignmentText), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->whatEdit, sizeof(CellProp->whatEdit), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->editFormat.operator LPCSTR(), CellProp->editFormat.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->auxValues, sizeof(CellProp->auxValues), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->offsets, sizeof(CellProp->offsets), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->headText.operator LPCSTR(), CellProp->headText.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->dominoName.operator LPCSTR(), CellProp->dominoName.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->referenceName.operator LPCSTR(), CellProp->referenceName.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->flashKeys.operator LPCSTR(), CellProp->flashKeys.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->typeHint, sizeof(CellProp->typeHint), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->hintText.operator LPCSTR(), CellProp->hintText.GetLength(), SEQ_DATA);
			}
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmTREE:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size, sizeof(Prop->size), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType, sizeof(Prop->charType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			break;

		case fmOBJECT:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType, sizeof(Prop->charType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size, sizeof(Prop->size), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fixColumn, sizeof(Prop->fixColumn), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fixRow, sizeof(Prop->fixRow), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle, sizeof(Prop->borderStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes, sizeof(Prop->auxAttributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor, sizeof(Prop->borderColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1, sizeof(Prop->paintColor1), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor2, sizeof(Prop->paintColor2), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment, sizeof(Prop->alignment), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignmentImage, sizeof(Prop->alignmentImage), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->offsets, sizeof(Prop->offsets), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->keyNames.operator LPCSTR(), Prop->keyNames.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues, sizeof(Prop->keyValues), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyParam, sizeof(Prop->keyParam), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->referenceName.operator LPCSTR(), Prop->referenceName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->checked, sizeof(Prop->checked), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeCursor, sizeof(Prop->typeCursor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->whatEdit, sizeof(Prop->whatEdit), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->editFormat.operator LPCSTR(), Prop->editFormat.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxValues, sizeof(Prop->auxValues), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->displays.operator LPCSTR(), Prop->displays.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->auxImages.operator LPCSTR(), Prop->auxImages.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->posImage, sizeof(Prop->posImage), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->headHeight, sizeof(Prop->headHeight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rowLines, sizeof(Prop->rowLines), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->flashKeys.operator LPCSTR(), Prop->flashKeys.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onCharL, ONCHARCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onKeyL, ONKEYINPUT);
			break;
		case fmFORM:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes, sizeof(Prop->auxAttributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);
			break;
		case fmTAB:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor2, sizeof(Prop->paintColor2), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1, sizeof(Prop->paintColor1), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->alignment, sizeof(Prop->alignment), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->checked, sizeof(Prop->checked), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues, sizeof(Prop->keyValues), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->displays.operator LPCSTR(), Prop->displays.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->images.operator LPCSTR(), Prop->images.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			break;
		case fmBROWSER:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);
			break;
		case fmMEMO:
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->charType, sizeof(Prop->charType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->ioType, sizeof(Prop->ioType), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderStyle, sizeof(Prop->borderStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues, sizeof(Prop->keyValues), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->typeHint, sizeof(Prop->typeHint), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->hintText.operator LPCSTR(), Prop->hintText.GetLength(), SEQ_DATA);

			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			break;
		case fmGRID:
		case fmGRIDEX:
			if (Prop->cellProperties)
				count = Prop->cellProperties->GetCount();
			else
				count = 0;
			BinaryMngr.AddBinaryData(&Prop->kind, sizeof(Prop->kind), SEQ_FORMAT);
			BinaryMngr.AddBinaryData((char*)Prop->name.operator LPCSTR(), Prop->name.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.left, sizeof(Prop->rect.left), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.top, sizeof(Prop->rect.top), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.right, sizeof(Prop->rect.right), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rect.bottom, sizeof(Prop->rect.bottom), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->keyValues, sizeof(Prop->keyValues), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->size, sizeof(Prop->size), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fixColumn, sizeof(Prop->fixColumn), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->attributes, sizeof(Prop->attributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxAttributes, sizeof(Prop->auxAttributes), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->styles, sizeof(Prop->styles), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor, sizeof(Prop->paintColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->textColor, sizeof(Prop->textColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->borderColor, sizeof(Prop->borderColor), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor1, sizeof(Prop->paintColor1), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->paintColor2, sizeof(Prop->paintColor2), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->fontName.operator LPCSTR(), Prop->fontName.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&fheight, sizeof(fheight), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->fontStyle, sizeof(Prop->fontStyle), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->whatEdit, sizeof(Prop->whatEdit), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->auxValues, sizeof(Prop->auxValues), SEQ_DATA);
			BinaryMngr.AddBinaryData((char*)Prop->strings.operator LPCSTR(), Prop->strings.GetLength(), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->rowLines, sizeof(Prop->rowLines), SEQ_DATA);
			BinaryMngr.AddBinaryData(&Prop->index, sizeof(Prop->index), SEQ_DATA);

			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				BinaryMngr.AddBinaryData((char*)CellProp->name.operator LPCSTR(), CellProp->name.GetLength(), SEQ_CELL);
				BinaryMngr.AddBinaryData(&CellProp->kind, sizeof(CellProp->kind), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->size, sizeof(CellProp->size), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->charType, sizeof(CellProp->charType), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->ioType, sizeof(CellProp->ioType), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->width, sizeof(CellProp->width), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->attributes, sizeof(CellProp->attributes), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->auxAttributes, sizeof(CellProp->auxAttributes), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->styles, sizeof(CellProp->styles), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->paintColor, sizeof(CellProp->paintColor), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->textColor, sizeof(CellProp->textColor), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->headPaintColor, sizeof(CellProp->headPaintColor), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->headTextColor, sizeof(CellProp->headTextColor), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentHead, sizeof(CellProp->alignmentHead), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentImage, sizeof(CellProp->alignmentImage), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->alignmentText, sizeof(CellProp->alignmentText), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->whatEdit, sizeof(CellProp->whatEdit), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->editFormat.operator LPCSTR(), CellProp->editFormat.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->auxValues, sizeof(CellProp->auxValues), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->headText.operator LPCSTR(), CellProp->headText.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->images.operator LPCSTR(), CellProp->images.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->entryCombo, sizeof(CellProp->entryCombo), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->displays.operator LPCSTR(), CellProp->displays.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->strings.operator LPCSTR(), CellProp->strings.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->offsets, sizeof(CellProp->offsets), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->dominoName.operator LPCSTR(), CellProp->dominoName.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->referenceName.operator LPCSTR(), CellProp->referenceName.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->keyParam, sizeof(CellProp->keyParam), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->flashKeys.operator LPCSTR(), CellProp->flashKeys.GetLength(), SEQ_DATA);
				BinaryMngr.AddBinaryData(&CellProp->typeHint, sizeof(CellProp->typeHint), SEQ_DATA);
				BinaryMngr.AddBinaryData((char*)CellProp->hintText.operator LPCSTR(), CellProp->hintText.GetLength(), SEQ_DATA);
				if (Prop->kind == fmGRIDEX)
					BinaryMngr.AddBinaryData((char*)CellProp->formula.operator LPCSTR(), CellProp->formula.GetLength(), SEQ_DATA);
			}
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onClickL, ONCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onDblClkL, ONDBLCLICK);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onChangeL, ONCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onCharL, ONCHARCHANGE);
			BinaryMngr.AddObjectScript(Prop->scpN, pos, Prop->onKeyL, ONKEYINPUT);
			break;
		default:
			break;
		}
	}

	file.Write(BinaryMngr.getBuf(), BinaryMngr.getSize());
	file.Close();

	if (m_pWnd != NULL)
	{
		m_pWnd->ReleaseDC(pDC);
	}

	return true;
}

void CCompile::SetTempPath(CString& dir, char* fileN, bool source)
{
	dir.Format("%s\\%s", m_curdir, fileN);
	if (!source)
		dir += ".ax";
}

void CCompile::SetDominoForm()
{
	for (int ii = 0; ii < m_fN; ii++)
	{
		if (m_domR[ii].dSym[0] != NULL)
		{
			int	rc = getFormIndex(m_domR[ii].dSym, (int)strlen(m_domR[ii].dSym));
			if (rc < 0)
			{
				setMessage(-1, 10, CString(m_domR[ii].dSym), ii);
				m_formR[ii].keys = NOVALUE;
			}
			else
				m_formR[ii].keys = rc;
		}
		else
		{
			if (m_formR[ii].keys != NOVALUE)
			{
				switch (m_formR[ii].kind)
				{
				case FM_COMBO:
				case FM_TAB:
				case FM_UTAB:
				case FM_GRID:
				case FM_MEMO:
				case FM_TABLE:
				case FM_SHEET:
					break;
				case FM_EDIT:
					if ((m_formR[ii].properties & PR_SPIN) || (m_formR[ii].attr & FA_ENUM))
						break;

					m_formR[ii].keys = NOVALUE;
					break;
				case FM_BUTTON:
					if (m_formR[ii].type == BTN_USER)
						break;
				default:
					m_formR[ii].keys = NOVALUE;
					break;
				}
			}
		}

		if (m_domR[ii].rSym[0] != NULL)
		{
			int	rc = getFormIndex(m_domR[ii].rSym, (int)strlen(m_domR[ii].rSym));
			if (rc < 0)
			{
				setMessage(-1, 10, CString(m_domR[ii].rSym), ii);
				m_formR[ii].vals[2] = NOVALUE;
			}
			else
				m_formR[ii].vals[2] = rc;
		}
	}
}

void CCompile::GetTempName(char* fileN, char* prefix)
{
	_putenv("TMP=NULL");

	char*   tmpN  = _tempnam((char *)m_curdir.operator LPCTSTR(), prefix);
	CString files = CString(tmpN);
	CopyMemory(fileN, (char*)files.operator LPCTSTR(), files.GetLength());
	fileN[files.GetLength()] = '\0';
	free(tmpN);
}

int CCompile::GetFormNode(CNode &pElementNode, int *idx)
{
	return m_XMLMnger->GetFormNode(pElementNode, idx);
}

int CCompile::GetCellNode(CNode &pElementNode, int *idx)
{
	return m_XMLMnger->GetCellNode(pElementNode, idx);
}

void CCompile::GetXmlNode(CNode &pElementNode, CString ElementKind)
{
	if (!m_XMLMnger)
		return;
	if (ElementKind == GLOBAL_DIVISION)
	{
		pElementNode.Copy(m_XMLMnger->m_aryGlobal);
	}
	//else if (ElementKind == FORM_DIVISION)
	//{
	//	pElementNode.Copy(m_XMLMnger->m_aryForm);
	//}
	else if (ElementKind == RESOURCE_DIVISION)
	{
		pElementNode.Copy(m_XMLMnger->m_aryResource);
	}
	//else if (ElementKind == COLINFO)
	//{
	//	pElementNode.Copy(m_XMLMnger->m_aryCell);
	//}
	else if (ElementKind == TRLAYOUT_DIVISION)
	{
		pElementNode.Copy(m_XMLMnger->m_aryTRlayout);
	}
	else if (ElementKind == LAYOUTEDIT_DIVISION)
	{
		pElementNode.Copy(m_XMLMnger->m_aryLayoutEdit);
	}
	else if (ElementKind == RTS_DIVISION)
	{
		pElementNode.Copy(m_XMLMnger->m_aryRTS);
	}
}

void CCompile::initVariables()
{
	m_memForm = NULL;
	m_memDom  = NULL;
	m_memCell  = NULL;

	m_oldPos   = -1;
}

bool CCompile::compareSymbol(CString string)
{
	for (int ii = 0; ii < m_fN; ii++)		// form symbol check
	{
		if (!string.CompareNoCase(&m_strR[m_formR[ii].name]))
			return false;
	}
	return true;
}

bool CCompile::compareColumnSymbol(CString symbol, int start, int current)
{
	int	ii,  rc = 0;

	for (ii = start; ii < current; ii++)		// Column Symbol Check
	{
		if (!symbol.CompareNoCase(&m_strR[m_cellR[ii].name]))
			return false;
	}

	return true;
}

int CCompile::getColumnIndex(char* symbol, int len, int idx)
{
	CString	string;
	int	ii, offs, count;

	string = CString(symbol, len);
	offs = m_formR[idx].vals[0];
	if (m_formR[idx].kind == FM_TABLE)
		count = LOWORD(m_formR[idx].vals[1]);
	else
		count = HIWORD(m_formR[idx].vals[1]);

	for (ii = 0; ii < count; ii++)		// Column Symbol Check
	{
		if (string.CompareNoCase(&m_strR[m_cellR[offs+ii].name]))
			continue;
		else
			return ii;
	}

	return -1;
}

void CCompile::setMessage(int line, int errNo, CString str, int pos, bool isevent)
{
	if (m_status == (CStatus *) NULL)
		return;

	CString	name  = _T("");
	m_err++;
	
	line = (line < 0) ? line : line+1;
	if (pos >= 0 && pos <= m_fN)
	{
		name = CString(&m_strR[m_formR[pos].name]);
		pos  = m_formR[pos].kind;
	}
	else
		pos = -1;

	m_status->SetMessage(m_err, pos, name, _T(""), errNo, str, line);
}
