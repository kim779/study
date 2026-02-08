// CodeHelper.cpp : implementation file
//

#include "stdafx.h"
#include "CodeHelper.h"
#include "ChartHelper.h"

#include "../../h/jmcode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


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
	
	if (mode == modeWrite) {
		TRY {
			CFile::Remove(m_fileName);
		}
		CATCH(CFileException, e) {
		}
		END_CATCH;
	}

	TRY {
		m_file = new CFile(m_fileName, mode == modeRead ? CFile::modeRead : CFile::modeCreate | CFile::modeWrite);

		if (mode == modeRead) {
			LPSTR buffer = new char[m_file->GetLength() + 1];

			m_file->Read(buffer, m_file->GetLength());
			buffer[m_file->GetLength()] = 0x00;
			m_buffer = buffer;
			delete [] buffer;
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
/* CAxTargetGroup class implementation                                  */
/************************************************************************/
CAxTargetGroup::CAxTargetGroup(CWnd* wizard, CWnd* parent, LPCTSTR lpszTabFolder, LPCTSTR lpszUserFolder)
{
	m_wizard = wizard;
	m_parent = parent;
	m_tree = NULL;
	m_tabFolder = lpszTabFolder;
	m_userFolder = lpszUserFolder;
	m_item = NULL;

	m_upcount = 0;
	m_upcode = NULL;
	m_hjcount = 0;
	m_hjcode = NULL;
	m_fjcount = 0;
	m_fjcode = NULL;

	m_ojcode = NULL;
	m_ojcount = 0;
	m_pjcode = NULL;
	m_pjcount = 0;
	m_elwcode = NULL;
	m_elwcount = 0;
}

CAxTargetGroup::~CAxTargetGroup()
{
	if (m_upcode) delete [] m_upcode;
	if (m_hjcode) delete [] m_hjcode;
	if (m_fjcode) delete [] m_fjcode;
	if (m_ojcode) delete [] m_ojcode;
	if (m_pjcode) delete [] m_pjcode;
	if (m_elwcode) delete [] m_elwcode;
}

void CAxTargetGroup::SetTargetGroup(CTreeCtrl* tree)
{
	if (!m_hjcode) {
		LoadJcode();
		LoadUpcode();
		LoadFcode();
		LoadOJcode();
		LoadPJcode();
		LoadELWcode();		
	}
	
	Setting(tree);
}

void CAxTargetGroup::SetTargetItem(CTreeCtrl* tree, CListCtrl* list, KIND kind)
{
	Setting(tree, list, kind);
}

void CAxTargetGroup::LoadCodeMaster(LPCTSTR codeFileName, int& count, void** buffer, int recSize, int headerSize)
{
	if (*buffer) return;
	
	CFile file;
	CString path = m_tabFolder + codeFileName;
	
	if (!file.Open(path, CFile::modeRead | CFile::typeBinary)) {
		return;
	}
	
	int len = file.GetLength();
	count = len / recSize;
	*buffer = new BYTE[len];
	
	if (headerSize)
		file.Read(*buffer, headerSize);
	file.Read(*buffer, len);
	file.Close();
}

/************************************************************************/
/* CAxTargetItem class implementation                                   */
/************************************************************************/
CAxTargetItem::CAxTargetItem(LPCTSTR fileName)
{
	m_fileName = fileName;
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
	
	CAxInformation ax(m_fileName, CAxInformation::modeRead);

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
	
	CAxInformation ax(m_fileName, CAxInformation::modeWrite);
	
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

	for (int i = 0; i < list.GetItemCount(); i++) {
		item = list.GetItemText(i, 0);
		items += item;
		items += ';';
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

