// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information Inc.
//
//  Components   : form.h	AXIS FORM object
//  Rev. History :
//  		  Ver	 Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#pragma once

#include "fmProperties.h"

#define	NOVALUE		0xffffffff
#define	L_NAMEs		16		// etc name length
#define	L_FILED		4		// FA_FILE data length

#define	basPRGB		200
#define	basINPUT	192

#define	gaAUTOSIZE	0x80000000	// only workshop auto size

#pragma	pack(1)


struct	_Layout {
	int	left;
	int	top;
	int	right;
	int	bottom;

	bool	bfirst;
	DWORD	properties;

};
class cellOb : public CcellProperties
{
public:
	cellOb()
	{
	}
	void SetCopy(cellOb* srcCellProp)
	{
		kind			= srcCellProp->kind			;
		headKind		= srcCellProp->headKind			;
		name			= srcCellProp->name			;
		size			= srcCellProp->size			;
		charType		= srcCellProp->charType			;
		ioType			= srcCellProp->ioType			;
		width			= srcCellProp->width			;
		entryCombo		= srcCellProp->entryCombo		;
		alignmentHead		= srcCellProp->alignmentHead		;
		alignmentText		= srcCellProp->alignmentText		;
		alignmentImage		= srcCellProp->alignmentImage		;
		textColor		= srcCellProp->textColor		;
		paintColor		= srcCellProp->paintColor		;
		headText		= srcCellProp->headText			;
		headTextColor		= srcCellProp->headTextColor		;
		headPaintColor		= srcCellProp->headPaintColor		;
		fontStyle		= srcCellProp->fontStyle		;
		fontPoint		= srcCellProp->fontPoint		;
		fontName		= srcCellProp->fontName			;
		attributes		= srcCellProp->attributes		;
		auxAttributes		= srcCellProp->auxAttributes		;
		flashKeys		= srcCellProp->flashKeys		;
		whatEdit		= srcCellProp->whatEdit			;
		auxValues		= srcCellProp->auxValues		;
		editFormat		= srcCellProp->editFormat		;
		styles			= srcCellProp->styles			;
		offsets			= srcCellProp->offsets			;
		keyParam		= srcCellProp->keyParam			;
		dominoName		= srcCellProp->dominoName		;
		displays		= srcCellProp->displays			;
		strings			= srcCellProp->strings			;
		images			= srcCellProp->images			;
		auxImages		= srcCellProp->auxImages		;
		merges			= srcCellProp->merges			;
		typeCursor		= srcCellProp->typeCursor		;
		typeCursorH		= srcCellProp->typeCursorH		;
		typeHint		= srcCellProp->typeHint			;
		hintText		= srcCellProp->hintText			;
		referenceName		= srcCellProp->referenceName		;
		lineN			= srcCellProp->lineN			;
		block			= srcCellProp->block			;
		formula			= srcCellProp->formula			;
	}
public:
	BYTE	lineN;			// line number
	bool	block;			// multi selection
};

class CformProp : public CfmProperties
{
public:
	CformProp()
	{
		fontName = _T("±¼¸²");
		fontPoint = 9;
		styles = stENABLE | stVISIBLE;
		//////////////////////////////////
		group = NULL;
		onClick = -1;
		onClickL = 0;
		onDblClk = -1;
		onDblClkL = 0;
		onChange = -1;
		onChangeL = 0;
		onChar = -1;
		onCharL = 0;
		onKey = -1;
		onKeyL = 0;
		iAutoSize = 20;
		block = false;
		bFixedSize = false;
		CellSelected = -1;
		groupRc.SetRectEmpty();
		/////////////////////////////////////////
		layout = new CMapStringToPtr;
		layout->RemoveAll();
		iCtmIdx = 0;
		objClass = _T("");
		alpha = 0;
		fHeight = 0;
		margin = _T("");
		rtsShape = NULL;
		auxColor = 0;
	}
	~CformProp()
	{
		if (cellProperties)
		{
			for (size_t ii = 0; ii < (size_t)cellProperties->GetCount(); ii++)
			{
				cellOb* CellProp = (cellOb*)cellProperties->GetAt(ii);
				delete CellProp;
			}
			cellProperties->RemoveAll();
			delete cellProperties;
		}

		if (layout != NULL)
		{
			CString sKey;
			struct _Layout *layoutTmp;
			for (POSITION pos = layout->GetStartPosition(); pos;)
			{
				layout->GetNextAssoc(pos, sKey, (void*&)layoutTmp);
				delete layoutTmp;
			}
			layout->RemoveAll();
			delete layout;
		}
	}
	void SetCopy(CformProp* srcProp)
	{
		kind					= srcProp->kind;			
		name					= srcProp->name;		
		size					= srcProp->size;
		charType				= srcProp->charType;
		ioType					= srcProp->ioType;
		keyValues				= srcProp->keyValues;
		rect.CopyRect(srcProp->rect);
		alignment				= srcProp->alignment;
		alignmentImage				= srcProp->alignmentImage;
		borderStyle				= srcProp->borderStyle;
		index					= srcProp->index;
		iorder					= srcProp->iorder;
		fontName				= srcProp->fontName;
		fontPoint				= srcProp->fontPoint;
		fontStyle				= srcProp->fontStyle;
		textColor				= srcProp->textColor;
		paintColor				= srcProp->paintColor;
		borderColor				= srcProp->borderColor;
		paintColor1				= srcProp->paintColor1;
		paintColor2				= srcProp->paintColor2;
		attributes				= srcProp->attributes;
		auxAttributes				= srcProp->auxAttributes;
		styles					= srcProp->styles;
		checked					= srcProp->checked;
		typeCursor				= srcProp->typeCursor;
		typeHint				= srcProp->typeHint;
		hintText				= srcProp->hintText;
		keyNames				= srcProp->keyNames;
		whatEdit				= srcProp->whatEdit;
		auxValues				= srcProp->auxValues;
		editFormat				= srcProp->editFormat;
		offsets					= srcProp->offsets;
		strings					= srcProp->strings;
		displays				= srcProp->displays;
		images					= srcProp->images;
		auxImages				= srcProp->auxImages;
		posImage				= srcProp->posImage;
		if (srcProp->cellProperties)
		{
			cellProperties = new CObArray();
			for (size_t ii = 0; ii < (size_t)srcProp->cellProperties->GetCount(); ii++)
			{
				cellOb* dstCellProp = new cellOb();
				cellOb* srcCellProp = (cellOb*)srcProp->cellProperties->GetAt(ii);
				dstCellProp->SetCopy(srcCellProp);
				cellProperties->Add(dstCellProp);
			}
		}
		fixRow					= srcProp->fixRow;			
		fixColumn				= srcProp->fixColumn;		
		headHeight				= srcProp->headHeight;	
		rowLines				= srcProp->rowLines;
		flashKeys				= srcProp->flashKeys;
		referenceName				= srcProp->referenceName;
		keyParam				= srcProp->keyParam;
		group					= srcProp->group;
		onClick					= srcProp->onClick;	
		onClickL				= srcProp->onClickL;
		onDblClk				= srcProp->onDblClk;
		onDblClkL				= srcProp->onDblClkL;
		onChange				= srcProp->onChange;
		onChangeL				= srcProp->onChangeL;
		onChar					= srcProp->onChar;
		onCharL					= srcProp->onCharL;
		onKey					= srcProp->onKey;
		onKeyL					= srcProp->onKeyL;
		iAutoSize				= srcProp->iAutoSize;
		block					= srcProp->block;
		bFixedSize				= srcProp->bFixedSize;
		CellSelected				= srcProp->CellSelected;
		iCtmIdx					= srcProp->iCtmIdx;
		groupRc.CopyRect(srcProp->groupRc);

		// layout
		if (srcProp->layout != NULL)
		{
			CString sKey;
			struct _Layout *playout, *layoutTmp;
			if (layout = NULL)
				layout = new CMapStringToPtr;
			srcProp->layout->RemoveAll();
			for (POSITION pos = srcProp->layout->GetStartPosition(); pos;)
			{
				playout = new _Layout;
				srcProp->layout->GetNextAssoc(pos, sKey, (void*&)layoutTmp);

				playout->bfirst = false;
				playout->properties = layoutTmp->properties;
				playout->left = layoutTmp->left;
				playout->right = layoutTmp->right;
				playout->top = layoutTmp->top;
				playout->bottom = layoutTmp->bottom;

				layout->SetAt(sKey, (void*&)layout);
			}
		}

		objClass = srcProp->objClass;
		alpha = srcProp->alpha;
		fHeight = srcProp->fHeight;
		margin = srcProp->margin;
		rtsShape = srcProp->rtsShape;
		auxColor = srcProp->auxColor;
	}

	void Init(char _kind)
	{
		rect.left	= 0;
		rect.top	= 0;
		switch (_kind)
		{
		case fmLABEL:
			kind = fmLABEL;
			paintColor = 200;
			textColor = 232;
			strings = "Label";
			alignment = atLEFT;
			rect.right = 30;
			rect.bottom = 12;
			break;
		case fmBOX:
			kind = fmBOX;
			size = 1;
			paintColor = 232;
			textColor = 232;
			rect.right = 120;
			rect.bottom = 50;
			break;
		case fmGROUP:
			kind = fmGROUP;
			size = 1;
			paintColor = 232;
			textColor = 232;
			strings = "Group";
			rect.right = 120;
			rect.bottom = 50;
			break;
		case fmIMVIEW:
			kind = fmIMVIEW;
			size = 1;
			charType = bxBORDER;
			borderStyle = bsRAISED;
			paintColor = 200;
			textColor = 232;
			rect.right = 120;
			rect.bottom = 50;
			break;
		case fmBUTTON:
			kind = fmBUTTON;
			styles |= stCHECKED;
			paintColor = 200;
			textColor = 232;
			rect.right = 80;
			rect.bottom = 40;
			break;
		case fmRADIO:
			kind = fmRADIO;
			styles |= stCHECKED;
			paintColor = 200;
			textColor = 232;
			strings = "Radio";
			rect.right = 100;
			rect.bottom = 12;
			break;
		case fmCHECK:
			kind = fmCHECK;
			styles |= stCHECKED;
			paintColor = 200;
			textColor = 232;
			strings = "Check";
			rect.right = 100;
			rect.bottom = 12;
			break;
		case fmEDIT:
			kind = fmEDIT;
			//styles |= stEDIT;
			paintColor = 192;
			textColor = 232;
			attributes = faWRAP;
			charType = ctNUMERIC;
			ioType = ioINPUT;
			rect.right = 120;
			rect.bottom = 20;
			break;
		case fmCOMBO:
			kind = fmCOMBO;
			paintColor = 192;
			textColor = 232;
			ioType = ioINPUT;
			charType = ctNUMERIC;
			rect.right = 120;
			rect.bottom = 20;
			break;
		case fmOUTPUT:
			kind = fmOUTPUT;
			//styles |= stSUM;
			paintColor = 192;
			textColor = 232;
			charType = ctNUMERIC;
			ioType = bxBORDER;
			rect.right = 30;
			rect.bottom = 15;
			break;
		case fmMEMO:
			kind = fmMEMO;
			paintColor = 192;
			textColor = 232;
			charType = ctMIXED;
			ioType = ioINPUT;
			rect.right = 150;
			rect.bottom = 70;
			break;
		case fmGRID:
			kind = fmGRID;
			paintColor = 200;
			textColor = 232;
			size = 10;
			keyValues = MAKELONG(20, 0);
			rect.right = 120;
			rect.bottom = 120;
			styles |= stCHECKED;
			auxAttributes |= gaHEADER;
			rowLines = 1;
			break;
		case fmGRIDEX:
			kind = fmGRIDEX;
			paintColor = 200;
			textColor = 232;
			size = 10;
			keyValues = MAKELONG(20, 0);
			rect.right = 120;
			rect.bottom = 120;
			styles |= stCHECKED;
			auxAttributes |= gaHEADER;
			break;
		case fmTABLE:
			kind = fmTABLE;
			borderColor = 232;
			auxAttributes |= faDROP | gaAUTOSIZE;
			rect.right = 120;
			rect.bottom = 120;
			ioType |= ioOUTPUT;
			break;
		case fmTREE:
			kind = fmTREE;
			paintColor = 192;
			textColor = 232;
			rect.right = 120;
			rect.bottom = 50;
			attributes |= faSEND;
			styles |= stCHECKED;
			break;
		case fmFORM:
			kind = fmFORM;
			paintColor = 242;
			rect.right = 105;
			rect.bottom = 105;
			break;
		case fmTAB:
			kind = fmTAB;
			paintColor = 200;
			textColor = 232;
			paintColor1 = 232;
			paintColor2 = 200;
			strings = _T("1:0");
			strings += _T('\0');
			displays = _T("tab0");
			displays += _T('\0');
			alignment = atTOP;
			keyValues = 1;
			rect.right = 100;
			rect.bottom = 25;
			styles |= stCHECKED;
			break;
		case fmBROWSER:
			kind = fmBROWSER;
			rect.right = 105;
			rect.bottom = 105;
			styles |= stCHECKED;
			break;
		case fmOBJECT:
			kind = fmOBJECT;
			paintColor = 200;
			textColor = 232;
			rect.right = 30;
			rect.bottom = 12;
			break;
		}
	}
public:
	CString scpN;			// temporary script file name
	int	onClick;		// Click information file offset (if -1, not defined)
	int	onClickL;		// Click information data length
	int	onDblClk;		// DblClk information file offset
	int	onDblClkL;		// DblClk information data length
	int	onChange;		// Edit Change information file offset (if -1, not defined)
	int	onChangeL;		// Edit Change information data length
	int	onChar;			// Edit char information file offset (if -1, not defined)
					// alpha/numeric FORM only
	int	onCharL;		// Edit char information data length
	int	onKey;			// Grid Key Change information file offset (if -1, not defined)
	int	onKeyL;			// Grid Key Change information data length

	// Layout
	CMapStringToPtr*  layout;
	//CStringArray*	margin;		// margin

	bool	initC;			// if true, form has initial coursor

	bool	block;			// form in block
	bool	drawn;			//
	bool	bCommon;		// properties common
	bool	bFixedSize;		// Object size fix

	int	scrollPos;		// scroll pos
	int	iAutoSize;
	int	iCtmIdx;
	
	CformProp* group;			// Group
	//////////////////////////////////////////////////////////////
	CRect	groupRc;
	int	CellSelected;			// grid cell selected;
	/////////////////////////////mobile//////////////////////////////////////
	CString objClass;
	int	alpha;
	int	fHeight;
	CString margin;
	DWORD auxColor;
	char rtsShape;
};

#pragma	pack()
