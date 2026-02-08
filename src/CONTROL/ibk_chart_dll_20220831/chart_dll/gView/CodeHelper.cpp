// CodeHelper.cpp : implementation file
//

#include "stdafx.h"
#include "axisGView.h"
#include "CodeHelper.h"

#include "../../h/jmcode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL FP_KospiAll(JCODE* p, int index)
{
	return p->Code[0] == '0' && p->kosd == jmKOSPI;
}

BOOL FP_KospiSize(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && p->size == index;
}

BOOL FP_KospiMiddle(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && p->ucdm == index;
}

BOOL FP_KospiJejo(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && p->jjug == index;
}

BOOL FP_KospiSmall(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && (p->ucds == index || p->ucdm == index);
}

BOOL FP_KospiRange(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && p->jsiz == index;
}

BOOL FP_KospiBaedang(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && p->jsiz >= (index + 38) && p->jsiz <= (index + 41);
}

BOOL FP_KospiWoosoo(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && p->wsgb == 1;
}

BOOL FP_Kospi200(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && (p->kpgb == 1 || p->kpgb == 2 || p->kpgb == 3);
}

BOOL FP_Kospi100(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && (p->kpgb == 2 || p->kpgb == 3);
}

BOOL FP_Kospi50(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && (p->kpgb == 3);
}

BOOL FP_KospiIT(JCODE* p, int index)
{
	return p->kosd == jmKOSPI && p->itgb == 1;
}


CODEFILTERPROC GetKospiFilterProc(int subkind, int index)
{
	if (index == 1)				return FP_KospiAll;
	else if (index <= 4)		return FP_KospiSize;
	else if (index < 15)		return FP_KospiMiddle;
	else if (index == 27)		return FP_KospiJejo;
	else if (index < 38)		return FP_KospiSmall;
	else if (index < 41)		return FP_KospiRange;
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

BOOL FP_KosdaqAll(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ;
}

BOOL FP_KosdaqVenture(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->jjug == index;
}

BOOL FP_KosdaqITAllVenture(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->size == index;
}

BOOL FP_KosdaqMiddle(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->ucdm == index;
}

BOOL FP_KosdaqSmall(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->ucds == index;
}

BOOL FP_KosdaqRange(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->jsiz == (index - 37);
}

BOOL FP_Kosdaq50(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->kpgb == 1;
}

BOOL FP_KosdaqIT(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->itgb == 1;
}

BOOL FP_Kostar(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->star == 1;
}

BOOL FP_KosdaqEntertainment(JCODE* p, int index)
{
	return p->kosd == jmKOSDAQ && p->ucdm == 41;
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

CODEFILTERPROC GetCodeFilterProc(int kind, int subkind, int index)
{
	switch (kind) {
		case TRCD_KOSPIUJ:
			return GetKospiFilterProc(subkind, index);
		case TRCD_KOSDAQUJ:
			return GetKosdaqFilterProc(subkind, index);
	}
	return NULL;
}

/************************************************************************/
/* CAxInformation class Implementation                                  */
/************************************************************************/
CAxInformation::CAxInformation(LPCTSTR fileName, BOOL mode)
{
	m_fileName = fileName;

	m_file = NULL;
	m_buffer.Empty();
	m_mode = 0;

	SetMode(mode);
}

CAxInformation::~CAxInformation()
{
	if (m_file)
		delete m_file;
}

void CAxInformation::SetMode(int mode)
{
	if (m_mode == mode) return;
	
	if (m_file) {
		delete m_file;
		m_file = NULL;
	}
	m_buffer.Empty();
	
	if (mode == MODE_WRITE) {
		TRY {
			CFile::Remove(m_fileName);
		}
		CATCH(CFileException, e) {
		}
		END_CATCH;
	}

	TRY {
		m_file = new CFile(m_fileName, mode == MODE_READ ? CFile::modeRead : CFile::modeCreate | CFile::modeWrite);

		if (mode == MODE_READ) {
			LPSTR buffer = new char[m_file->GetLength() + 1];

			m_file->Read(buffer, m_file->GetLength());
			buffer[m_file->GetLength()] = 0x00;
			m_buffer = buffer;
		}
	}
	CATCH (CFileException, e) {
	}
	END_CATCH;
}

CString CAxInformation::GetStartTag(LPCTSTR item)
{
	CString tag;
	tag.Format("<%s>", item);
	return tag;
}
CString CAxInformation::GetEndTag(LPCTSTR item)
{
	CString tag;
	tag.Format("</%s>", item);
	return tag;
}

CString CAxInformation::GetString(LPCTSTR item, LPCTSTR def)
{
	CString value("");;
	if (m_buffer.IsEmpty()) return value;
	
	
	int find1, find2;
	find1 = m_buffer.Find(GetStartTag(item));
	if (find1 < 0) return value;
	find2 = m_buffer.Find(GetEndTag(item), find1);
	if (find2 < 0) return value;

	find1 += GetStartTag(item).GetLength();
	value = m_buffer.Mid(find1, find2 - find1);

	return value;
}

int CAxInformation::GetInt(LPCTSTR item, int def)
{
	CString value = GetString(item, "");
	if (value.IsEmpty())
		return def;
	else
		return atoi(value);
}

void CAxInformation::SetString(LPCTSTR item, LPCTSTR value)
{
	CString str;
	str.Format("%s%s%s", GetStartTag(item), value, GetEndTag(item));

	TRY {
		m_file->Write((LPCTSTR)str, str.GetLength());
	}
	CATCH(CFileException, e) {
	}
	END_CATCH;
}

void CAxInformation::SetInt(LPCTSTR item, int value)
{
	CString s;
	s.Format("%d", value);
	SetString(item, s);
}

void CAxInformation::DeleteAll()
{
	if (m_file) {
		delete m_file;
		m_file = NULL;
	}

	TRY {
		CFile::Remove(m_fileName);
	}
	CATCH(CFileException, e) {
	}
	END_CATCH;

	int mode = m_mode;
	m_mode = 0;
	SetMode(mode);
}

/************************************************************************/
/* CAxTargetItem class implementation                                   */
/************************************************************************/
CAxTargetItem::CAxTargetItem()
{
	CAxisGViewApp* app = (CAxisGViewApp*)AfxGetApp();

	m_fileName.Format("%s\\gex\\selitem_%s.dat", app->GetRoot(), app->GetUID());
}

CAxTargetItem::~CAxTargetItem()
{

}

int CAxTargetItem::KindToKey(KIND& kind)
{
	return MAKELONG(MAKEWORD((BYTE)kind.kind, (BYTE)kind.subkind), kind.index);
}

void CAxTargetItem::KeyToKind(int key, KIND& kind)
{
	kind.kind = LOBYTE(LOWORD(key));
	kind.subkind = HIBYTE(LOWORD(key));
	kind.index = HIWORD(key);
}

void CAxTargetItem::LoadItemFromFile()
{
	m_items.RemoveAll();
	
	CAxInformation ax(m_fileName, MODE_READ);

	int count = ax.GetInt("Count");
	CString s, value;
	CString key, items;

	for (int i = 0; i < count; i++) {
		s.Format("%05d", i);

		value = ax.GetString(s);
		key = value.Left(10);
		items = value.Right(value.GetLength() - 10);

		SetItem(atoi(key), items);
	}
}

void CAxTargetItem::SaveItemToFile()
{
	TRY {
		CFile::Remove(m_fileName);
	}
	CATCH (CFileException, e){
	}
	END_CATCH;

	CAxInformation ax(m_fileName, MODE_WRITE);

	CString s, value, items;
	int key, i = 0;

	ax.SetInt("Count", m_items.GetCount());

	for (POSITION pos = m_items.GetStartPosition(); pos; i++) {
		m_items.GetNextAssoc(pos, key, items);

		s.Format("%05d", i);
		value.Format("%010d%s", key, items);
		ax.SetString(s, value);
	}
}

void CAxTargetItem::SetItem(int key, CString items)
{
	m_items.SetAt(key, items);
}

void CAxTargetItem::SetItem(KIND& kind, CListCtrl& list)
{
	CString item, items(_T(""));
	int find;

	for (int i = 0; i < list.GetItemCount(); i++) {
		item = list.GetItemText(i, 0);

		find = item.Find(SPACE);
		if (find > 0) {
			items += item.Left(find);
			items += ';';
		}
	}

	SetItem(kind, items);
}

void CAxTargetItem::SetItem(KIND& kind, CString items)
{
	int key = KindToKey(kind);
	SetItem(key, items);
}

void CAxTargetItem::GetItem(KIND& kind, CListCtrl& list)
{

}

CString CAxTargetItem::GetItem(KIND& kind) 
{
	CString items;

	int key = KindToKey(kind);
	if (m_items.Lookup(key, items))
		return items;
	else 
		return CString("");
}

