// UndoRedo.h : CUndoRedo 클래스의 인터페이스
//

#include "h/mapform.h"
#include "amCom/formItem.h"


class CUndoRedo
{
public:
	CUndoRedo(_mapH* mapH, CFormItem* formItem)
	{
		int	nLen;

		CopyMemory(&mx_mapH, mapH, sizeof(struct _mapH));
		if (mapH->trinfo)
		{
			nLen = strlen(mapH->trinfo)+1;
			mx_mapH.trinfo = new char[nLen];
			strcpy_s(mx_mapH.trinfo, nLen, mapH->trinfo);
			mx_mapH.trinfo[nLen-1] = '\0';
		}

		if (mapH->rtsinfo)
		{
			nLen = strlen(mapH->rtsinfo)+1;
			mx_mapH.rtsinfo = new char[nLen];
			strcpy_s(mx_mapH.rtsinfo, nLen, mapH->rtsinfo);
			mx_mapH.rtsinfo[nLen-1] = '\0';
		}

		mx_formR = NULL;
		mx_formN = formItem->GetCount();
		if (mx_formN > 0)
		{
			mx_formR = new char[sizeof(_formR)];
			ZeroMemory(mx_formR, sizeof(_formR));
			 
			struct _formR* formDes = (_formR *) mx_formR;
			struct _formR* formSrc = (_formR *) mapH->formR;
			struct _formR* backDes = NULL;

			for (int ii = 0; ii < mx_formN; ii++)
			{
				if (formDes == NULL)
				{
					formDes = (_formR *) new char[sizeof(_formR)];
					ZeroMemory(formDes, sizeof(_formR));
					backDes->next = formDes;
				}
				CopyMemory(formDes, formSrc, sizeof(_formR));

				if (formSrc->name != NULL)
				{
					nLen = strlen(formSrc->name) + 1;
					formDes->name = new char[nLen];
					strcpy_s(formDes->name, nLen, formSrc->name);
					formDes->name[nLen-1] = '\0';
				}
				if (formSrc->fName != NULL)
				{
					nLen = strlen(formSrc->fName) + 1;
					formDes->fName = new char[nLen];
					strcpy_s(formDes->fName, nLen, formSrc->fName);
				}
				if (formSrc->desc != NULL)
				{
					nLen = strlen(formSrc->desc) + 1;
					formDes->desc = new char[nLen];
					strcpy_s(formDes->desc, nLen, formSrc->desc);
				}
				if (formSrc->auxS != NULL)
				{
					nLen = strlen(formSrc->auxS) + 1;
					formDes->auxS = new char[nLen];
					strcpy_s(formDes->auxS, nLen, formSrc->auxS);
				}
				if (formSrc->editS != NULL)
				{
					nLen = strlen(formSrc->editS) + 1;
					formDes->editS = new char[nLen];
					strcpy_s(formDes->editS, nLen, formSrc->editS);
				}
				if (formSrc->str != NULL)
				{
					nLen = strlen(formSrc->str) + 1;
					formDes->str = new char[nLen];
					strcpy_s(formDes->str, nLen, formSrc->str);
				}
				if (formSrc->str2 != NULL)
				{
					nLen = strlen(formSrc->str2) + 1;
					formDes->str2 = new char[nLen];
					strcpy_s(formDes->str2, nLen, formSrc->str2);
				}
				if (formSrc->dat != NULL)
				{
					nLen = strlen(formSrc->dat) + 1;
					formDes->dat = new char[nLen];
					strcpy_s(formDes->dat, nLen, formSrc->dat);
				}
				if (formSrc->rtssy != NULL)
				{
					nLen = strlen(formSrc->rtssy) + 1;
					formDes->rtssy = new char[nLen];
					strcpy_s(formDes->rtssy, nLen, formSrc->rtssy);
				}
				if (formSrc->scpN != NULL)
				{
					nLen = strlen(formSrc->scpN) + 1;
					formDes->scpN = new char[nLen];
					strcpy_s(formDes->scpN, nLen, formSrc->scpN);
				}
				if (formSrc->optionimg != NULL)
				{
					nLen = strlen(formSrc->optionimg) + 1;
					formDes->optionimg = new char[nLen];
					strcpy_s(formDes->optionimg, nLen, formSrc->optionimg);
				}
				if (formSrc->refS != NULL)
				{
					nLen = strlen(formSrc->refS) + 1;
					formDes->refS = new char[nLen];
					strcpy_s(formDes->refS, nLen, formSrc->refS);
				}
				if (formSrc->help != NULL)
				{
					nLen = strlen(formSrc->help) + 1;
					formDes->help = new char[nLen];
					strcpy_s(formDes->refS, nLen, formSrc->help);
				}
				if (formSrc->optionimg2 != NULL)
				{
					nLen = strlen(formSrc->optionimg2) +1;
					formDes->optionimg2 = new char[nLen];
					strcpy_s(formDes->optionimg2, nLen, formSrc->optionimg2);
				}
				if (formSrc->optionimg3 != NULL)
				{
					nLen = strlen(formSrc->optionimg3) +1;
					formDes->optionimg3 = new char[nLen];
					strcpy_s(formDes->optionimg3, nLen, formSrc->optionimg3);
				}
				if (formSrc->margin && formSrc->margin->GetCount() > 0)
				{
					formDes->margin = new CStringArray;
					formDes->margin->Copy(*formSrc->margin);
				}
				formDes->auxR = NULL;

				// layout
				formDes->layout = new CMapStringToPtr;
				formDes->layout->RemoveAll();
				if (formSrc->layout->GetCount() > 0)
				{
					CString sKey;
					struct _Layout *layout, *layoutTmp;
					for (POSITION pos = formSrc->layout->GetStartPosition(); pos;)
					{
						layout = new _Layout;
						formSrc->layout->GetNextAssoc(pos, sKey, (void*&)layoutTmp);

						layout->bfirst	= false;
						layout->properties = layoutTmp->properties;
						layout->left	= layoutTmp->left;
						layout->right	= layoutTmp->right;
						layout->top	= layoutTmp->top;
						layout->bottom	= layoutTmp->bottom;
												
						formDes->layout->SetAt(sKey, (void*&)layout);
					}
				}

				switch (formSrc->kind)
				{
				case FM_GRID: case FM_GRIDEX: case FM_TABLE:
					if (formSrc->auxR != NULL)
					{
						formDes->auxR = new char[sizeof(_repR)];
						CopyMemory((char*)formDes->auxR, (char*)formSrc->auxR, sizeof(_repR));

						struct _repR* repSrc = (_repR *) formSrc->auxR;
						struct _repR* repDes = (_repR *) formDes->auxR;

						struct _cellR* cellSrc = repSrc->cellR;
						struct _cellR* cellDes = repDes->cellR;
						struct _cellR* cellBak = NULL;

						if (cellSrc == NULL)
							break;

						cellDes = (_cellR*) new char[sizeof(_cellR)]; repDes->cellR = cellDes;
						while (cellSrc != NULL)
						{
							if (cellDes == NULL)
							{
								cellDes = (_cellR*) new char[sizeof(_cellR)];
								ZeroMemory(cellDes, sizeof(_cellR));
								cellBak->next = cellDes;
							}
							CopyMemory(cellDes, cellSrc, sizeof(_cellR));
							if (cellSrc->fName)
							{
								nLen = strlen(cellSrc->fName)+1;
								cellDes->fName = new char[nLen];
								strcpy_s(cellDes->fName, nLen, cellSrc->fName);
							}
							if (cellSrc->margin && cellSrc->margin->GetCount() > 0)
							{
								cellDes->margin = new CStringArray;
								cellDes->margin->Copy(*cellSrc->margin);
							}
							cellSrc = cellSrc->next;
							cellBak = cellDes;
							cellDes = NULL;
						}
					}
					break;
				default:
					break;;
				}

				formSrc = formSrc->next;
				backDes = formDes;
				formDes = NULL;
			}
		}

		mx_fields.RemoveAll();

		void*	value;
		CString	skey;
		for (POSITION pos = formItem->m_fields.GetStartPosition(); pos != NULL; )
		{
			formItem->m_fields.GetNextAssoc(pos, skey, value);
			mx_fields.SetAt(skey, (void *) value);
		}
	}
	~CUndoRedo()
	{
		struct _formR* formR = (_formR *) mx_formR;
		struct _formR* tmpR = NULL;
		CString	tmps;
		struct _Layout* layout;

		for (int ii = 0; ii < mx_formN; ii++)
		{
			if (formR->name != NULL)	delete[] formR->name;
			if (formR->fName != NULL)	delete[] formR->fName;
			if (formR->desc != NULL)	delete[] formR->desc;
			if (formR->auxS != NULL)	delete[] formR->auxS;
			if (formR->editS != NULL)	delete[] formR->editS;
			if (formR->str != NULL)		delete[] formR->str;
			if (formR->str2 != NULL)	delete[] formR->str2;
			if (formR->dat != NULL)		delete[] formR->dat;
			if (formR->rtssy != NULL)	delete[] formR->rtssy;
			if (formR->scpN != NULL)	delete[] formR->scpN;
			if (formR->optionimg != NULL)	delete[] formR->optionimg;
			if (formR->refS != NULL)	delete[] formR->refS;
			if (formR->help != NULL)	delete[] formR->help;
			if (formR->optionimg2 != NULL)	delete[] formR->optionimg2;
			if (formR->optionimg3 != NULL)	delete[] formR->optionimg3;

			if (formR->margin != NULL)
			{
				formR->margin->RemoveAll();
				delete formR->margin;
			}

			for (POSITION pos = formR->layout->GetStartPosition(); pos; )
			{
				formR->layout->GetNextAssoc(pos, tmps, (void*&)layout);
				delete layout;
			}
			formR->layout->RemoveAll();	
			delete formR->layout;

			switch (formR->kind)
			{
			case FM_GRID:
			case FM_GRIDEX:
			case FM_TABLE:
				if (formR->auxR != NULL)
				{
					struct _repR* repR = (_repR*) formR->auxR;
					struct _cellR* cellR = (_cellR *) repR->cellR;
					struct _cellR* tmpCellR = NULL;

					while (cellR != NULL)
					{
						tmpCellR = cellR->next;
						if (cellR->fName)
							delete[] cellR->fName;
						if (cellR->margin)
						{
							cellR->margin->RemoveAll();
							delete cellR->margin;
						}
						delete cellR;
						cellR = tmpCellR;
					}
					delete repR;
				}
				break;
			default:
				break;
			}
			tmpR = formR->next;
			delete formR;
			formR = tmpR;
		}

		if (mx_mapH.trinfo != NULL)
			delete[] mx_mapH.trinfo;
		if (mx_mapH.rtsinfo != NULL)
			delete[] mx_mapH.rtsinfo;
	}

public:
	_mapH	mx_mapH;

	int		mx_formN;
	char*		mx_formR;

	CMapStringToPtr	mx_fields;
};
