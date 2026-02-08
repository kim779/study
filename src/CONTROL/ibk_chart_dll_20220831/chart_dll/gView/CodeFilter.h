#ifndef __CODEFILTER_H__
#define __CODEFILTER_H__

typedef void (*ITEMINSERTPROC)(LPVOID node, CListCtrl* list);

CODEFILTERPROC GetCodeFilterProc(int kind, int subkind, int index);

void II_pubcode(CString code, CString name, CListCtrl* list, LPVOID p  = NULL);
void II_hjcode(LPVOID, CListCtrl*);
void II_upcode(LPVOID, CListCtrl*);
void II_fjcode(LPVOID, CListCtrl*);
void II_ojcode_c(LPVOID, CListCtrl*);
void II_ojcode_p(LPVOID, CListCtrl*);
void II_pjcode(LPVOID, CListCtrl*);
void II_elwcode(LPVOID, CListCtrl*);

BOOL FP_NoCondition(LPVOID node, int kind, int subkind, int index);

BOOL FP_AllItem(LPVOID node, int kind, int subkind, int index);
BOOL FP_Kospi200(LPVOID node, int kind, int subkind, int index);
BOOL FP_Kosdaq50(LPVOID node, int kind, int subkind, int index);
BOOL FP_ETF(LPVOID node, int kind, int subkind, int index);
BOOL FP_Freeboard(LPVOID node, int kind, int subkind, int index);
BOOL FP_StockOption(LPVOID node, int kind, int subkind, int index);
BOOL FP_Index(LPVOID node, int kind, int subkind, int index);
BOOL FP_IndexItem(LPVOID node, int kind, int subkind, int index);




#endif // __CODEFILTER_H__