// CodeFilter.cpp : implementation file
//

#include "stdafx.h"
#include "../../h/jmcode.h"

#include "ConfigBar_Pru.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ListInsertItem(CListCtrl* list, CString code, CString name, DWORD data)
{
	name.TrimRight();
	int index = list->InsertItem(list->GetItemCount(), code);
	list->SetItemText(index, 1, name);
	list->SetItemData(index, data);
}


void II_pubcode(CString code, CString name, CListCtrl* list, LPVOID p)
{
	ListInsertItem(list, code, name, (DWORD)p);
}

void II_hjcode(LPVOID node, CListCtrl* list)
{
	struct hjcode* p = (struct hjcode*)node;

	CString code, name;
	code = &p->code[1];
	name = &p->hnam[1];
	name = name.Left(HNameLen-1);

	ListInsertItem(list, code, name, GU_CODE);
}

void II_upcode(LPVOID node, CListCtrl* list)
{
	struct upcode* p = (struct upcode*)node;

	CString code, name;
	
	code.Format("%d%02d", p->jgub, p->ucod);
	name = CString(p->hnam, ARRAY_SIZE(p->hnam));

	ListInsertItem(list, code, name, GU_INDEX);
}

void II_fjcode(LPVOID node, CListCtrl* list)
{
	struct fjcode* p = (struct fjcode*)node;

	CString code, name;
	code = CString(p->cod2, FCodeLen);
	name = CString(p->hnam, FNameLen);

	ListInsertItem(list, code, name, GU_FUTURE);
}

void II_ojcode_c(LPVOID node, CListCtrl* list)
{
	struct ojcode* p = (struct ojcode*)node;

	CString code, name;
	
	for (int i = 0; i < 4; i++) {
		if (p->call[i].yorn == '1') {
			code = CString(p->call[i].cod2, OCodeLen);
			name = CString(p->call[i].hnam, ONameLen);
			
			ListInsertItem(list, code, name, GU_OPTION);
		}
	}
}

void II_ojcode_p(LPVOID node, CListCtrl* list)
{
	struct ojcode* p = (struct ojcode*)node;
	
	CString code, name;
	
	for (int i = 0; i < 4; i++) {
		if (p->put[i].yorn == '1') {
			code = CString(p->put[i].cod2, OCodeLen);
			name = CString(p->put[i].hnam, ONameLen);
			
			ListInsertItem(list, code, name, GU_OPTION);
		}
	}
}

void II_pjcode(LPVOID node, CListCtrl* list)
{
	struct pjcode* p = (struct pjcode*)node;
	
	CString code, name;
	code = CString(p->codx, PCodeLen);
	name = CString(p->hnam, ARRAY_SIZE(p->hnam));

	ListInsertItem(list, code, name, GU_CODE);
}

void II_elwcode(LPVOID node, CListCtrl* list)
{
	struct elwcode* p = (struct elwcode*)node;

	CString code, name;
	code = CString(p->codx, ELWCodeLen);
	name = CString(p->hnam, ELWHNameLen);

	ListInsertItem(list, code, name, GU_CODE);
}

BOOL FP_AllItem(LPVOID node, int kind, int subkind, int index)
{
	struct hjcode* p = (struct hjcode*)node;
	return p->code[0] == 'A';// && (p->kosd == jmKOSPI || p->kosd == jmKOSDAQ); // || p->kosd == jm3RD)	
}


BOOL FP_KospiAll(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->code[1] == '0' && ((struct hjcode*)node)->kosd == jmKOSPI;
	//return ((struct hjcode*)node)->kosd == jmKOSPI;
}

BOOL FP_KospiSize(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && ((struct hjcode*)node)->size == index;
}

BOOL FP_KospiMiddle(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && ((struct hjcode*)node)->ucdm == index;
}

BOOL FP_KospiJejo(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && ((struct hjcode*)node)->jjug == index;
}

BOOL FP_KospiSmall(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && (((struct hjcode*)node)->ucds == index || ((struct hjcode*)node)->ucdm == index);
}

BOOL FP_KospiRange(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && ((struct hjcode*)node)->jsiz == index;
}

BOOL FP_KospiBaedang(LPVOID node, int kind, int subkind, int index)
{
	//return ((struct hjcode*)node)->kosd == jmKOSPI && ((struct hjcode*)node)->jsiz >= (index + 38) && ((struct hjcode*)node)->jsiz <= (index + 41);
	struct hjcode* p = (struct hjcode*)node;
	return p->kosd == jmKOSPI && p->jsiz == index;
}

BOOL FP_KospiWoosoo(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && ((struct hjcode*)node)->wsgb == 1;
}

BOOL FP_Kospi200(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && (((struct hjcode*)node)->kpgb == 1 || ((struct hjcode*)node)->kpgb == 2 || ((struct hjcode*)node)->kpgb == 3);
}

BOOL FP_Kospi100(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && (((struct hjcode*)node)->kpgb == 2 || ((struct hjcode*)node)->kpgb == 3);
}

BOOL FP_Kospi50(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && (((struct hjcode*)node)->kpgb == 3);
}

BOOL FP_KospiIT(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSPI && ((struct hjcode*)node)->itgb == 1;
}


CODEFILTERPROC GetKospiFilterProc(int subkind, int index)
{
	if (index == 1)				return FP_KospiAll;
	else if (index <= 4)		return FP_KospiSize;
	else if (index < 15)		return FP_KospiMiddle;
	else if (index == 27)		return FP_KospiJejo;
	//else if (index < 38)		return FP_KospiSmall;
	else if (index < 41)		return FP_KospiSmall;//return FP_KospiRange;
	else if (index == 41)		return FP_KospiBaedang;
	else if (index == 42)		return FP_KospiWoosoo;
	else {
		switch (index) {
			case ID_KOSPI200:   return FP_Kospi200;
			case ID_KOSPI100:	return FP_Kospi100;
			case ID_KOSPI50:	return FP_Kospi50;
			case ID_KOSPIIT:	return FP_KospiIT;
		}
	}
	
	return NULL;
}

BOOL FP_KosdaqAll(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ;
}

BOOL FP_KosdaqVenture(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->jjug == index;
}

BOOL FP_KosdaqITAllVenture(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->size == index;
}

BOOL FP_KosdaqMiddle(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->ucdm == index;
}

BOOL FP_KosdaqSmall(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->ucds == index;
}

BOOL FP_KosdaqRange(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->jsiz == (index - 37);
}

BOOL FP_Kosdaq50(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->kpgb == 1;
}

BOOL FP_KosdaqIT(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->itgb == 1;
}

BOOL FP_Kostar(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->star == 1;
}

BOOL FP_KosdaqEntertainment(LPVOID node, int kind, int subkind, int index)
{
	return ((struct hjcode*)node)->kosd == jmKOSDAQ && ((struct hjcode*)node)->ucdm == 41;
}


CODEFILTERPROC GetKosdaqFilterProc(int subkind, int index)
{
	if (index == 1)				return FP_KosdaqAll;
	else if (index == 2)		return FP_KosdaqVenture;
	else if (index <= 4)		return FP_KosdaqITAllVenture;
	else if (index <= 14)		return FP_KosdaqMiddle;
	else if (index <= 37)		return FP_KosdaqSmall;
	else if (index < 41)		return FP_KosdaqRange;
	else {
		switch (index) {
			case ID_KOSDAQ50:	return FP_Kosdaq50;
			case ID_KOSDAQIT:	return FP_KosdaqIT;
			case ID_KOSTAR:		return FP_Kostar;
			case ID_ORAKMUNHWA: return FP_KosdaqEntertainment;
		}
	}
	return NULL;
}

BOOL FP_IndexItemSector(LPVOID node, int kind, int subkind, int index)
{
	struct hjcode* p = (struct hjcode*)node;

	unsigned short uStgb = 0x01 << ((index % 100) - 1);
	return (*(unsigned short*)p->stgb & uStgb);
}

BOOL FP_IndexItemFreeboard(LPVOID node, int kind, int subkind, int index)
{
	struct hjcode* p = (struct hjcode*)node;

	if (p->kosd == jm3RD) {
		if (index == 1) return TRUE;
		else if (index == 2 && p->jjug == 3) return TRUE;
		else if (index == 3 && p->ucdm == 3) return TRUE;
		else if (index == 4 && p->ucdm == 4) return TRUE;
	}
	return FALSE;
}

CODEFILTERPROC GetCodeFilterProc(int kind, int subkind, int index)
{
	switch (subkind) {
		case upKOSPI:
				return GetKospiFilterProc(subkind, index);
		case upKOSDAQ:
			return GetKosdaqFilterProc(subkind, index);
		case upSECTOR:
			return FP_IndexItemSector;
		case upFREEBOARD:
			return FP_IndexItemFreeboard;
	}
	return NULL;
}

BOOL FP_ETF(LPVOID node, int kind, int subkind, int index)
{
	return (((struct hjcode*)node)->ssgb == jmETF);
}

BOOL FP_Freeboard(LPVOID node, int kind, int subkind, int index)
{
	return (((struct hjcode*)node)->kosd == jm3RD);
}

BOOL FP_StockOption(LPVOID node, int kind, int subkind, int index)
{
	struct pjcode* p = (struct pjcode*)node;

	if (subkind == CAxTargetGroupPru::groupStockCallOption && p->codx[0] != '2') return FALSE;
	if (subkind == CAxTargetGroupPru::groupStockPutOption && p->codx[0] != '3')  return FALSE;

	return atoi(CString(p->tjgb, 2)) == index;
}

BOOL FP_Index(LPVOID node, int kind, int subkind, int index)
{
	struct upcode* p = (struct upcode*)node;

	switch (index) {
		case CAxTargetGroupPru::groupIndexKospi:
			return p->jgub == upKOSPI || p->jgub == upINTEGRATE;
		case CAxTargetGroupPru::groupIndexKosdaq:
			return p->jgub == upKOSDAQ;
		case CAxTargetGroupPru::groupIndexSector:
			return p->jgub == upSECTOR;
		case CAxTargetGroupPru::groupIndexFreeboard:
			return p->jgub == upFREEBOARD;
	}
	return FALSE;
}

BOOL FP_NoCondition(LPVOID node, int kind, int subkind, int index)
{
	return TRUE;
}

