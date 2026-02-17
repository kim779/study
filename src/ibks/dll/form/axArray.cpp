#include "stdafx.h"
#include "AxArray.h"

CAxByteArray::CAxByteArray()
{
	m_size  = 0;
	m_maxs  = MAXs;
	m_array = new BYTE[m_maxs];
	RemoveAll();
}

CAxByteArray::~CAxByteArray()
{
	RemoveAll();
	delete [] m_array;
}

bool CAxByteArray::InsertAt(int key, BYTE value)
{
	reNew();

	if (key >= m_size)
		return false;

	MoveMemory(&m_array[key+1], &m_array[key], sizeof(BYTE) * (m_size-key));
	m_array[key] = value;
	m_size++;
	return true;
}

int CAxByteArray::Add(BYTE value)
{
	reNew();

	m_array[m_size++] = value;
	return m_size;
}

void CAxByteArray::RemoveAt(int key)
{
	m_size--;
	if (key < m_size)
		CopyMemory(&m_array[key], &m_array[key+1], sizeof(BYTE) * (m_size-key));
}

void CAxByteArray::reNew()
{
	if (m_size >= m_maxs -1)
	{
		BYTE*	array = m_array;
		m_maxs += MAXs;

		try
		{
			m_array = new BYTE[m_maxs];
			ZeroMemory(m_array, sizeof(BYTE) * m_maxs);
			CopyMemory(m_array, array, sizeof(BYTE) * m_size);
			delete [] array;
		}
		catch(CMemoryException* ec)
		{
			ec->ReportError();
			ec->Delete();
		}
	}
}

CAxDWordArray::CAxDWordArray()
{
	m_size  = 0;
	m_maxs  = MAXs;
	m_array = new DWORD[m_maxs];
	RemoveAll();
}

CAxDWordArray::~CAxDWordArray()
{
	RemoveAll();
	delete [] m_array;
}

bool CAxDWordArray::InsertAt(int key, DWORD value)
{
	reNew();

	if (key >= m_size)
		return false;

	MoveMemory(&m_array[key+1], &m_array[key], sizeof(DWORD) * (m_size-key));
	m_array[key] = value;
	m_size++;
	return true;
}

int CAxDWordArray::Add(DWORD value)
{
	reNew();
	m_array[m_size++] = value;
	return m_size;
}

void CAxDWordArray::RemoveAt(int key)
{
	m_size--;
	if (key < m_size)
		CopyMemory(&m_array[key], &m_array[key+1], sizeof(DWORD) * (m_size-key));
}

void CAxDWordArray::reNew()
{
	if (m_size >= m_maxs -1)
	{
		DWORD*	array = m_array;

		m_maxs += MAXs;
		m_array = new DWORD[m_maxs];
		CopyMemory(m_array, array, sizeof(DWORD) * m_size);
		delete [] array;
	}
}

CAxStringArray::CAxStringArray()
{
	m_size  = 0;
	m_maxs  = MAXs;
	m_array = new DWORD[m_maxs];
	RemoveAll();
}

CAxStringArray::~CAxStringArray()
{
	RemoveAll();
	delete [] m_array;
}

bool CAxStringArray::InsertAt(int key, CString value)
{
	reNew();

	if (key >= m_size)
		return false;

	MoveMemory(&m_array[key+1], &m_array[key], sizeof(DWORD) * (m_size-key));
	m_array[key] = (DWORD)new char[value.GetLength()+1];
	strcpy_s((char *)m_array[key], value.GetLength()+1, value.GetString());
	m_size++;

	return true;
}

int CAxStringArray::Add(CString value)
{
	reNew();
	m_array[m_size] = (DWORD)new char[value.GetLength()+1];
	strcpy_s((char *)m_array[m_size], value.GetLength()+1, value);
	m_size++;
	return m_size;
}

void CAxStringArray::SetAt(int key, CString value)
{
	delete [] (char *)m_array[key];

	m_array[key] = (DWORD)new char[value.GetLength()+1];
	strcpy_s((char *)m_array[key], value.GetLength()+1, value);
}

void CAxStringArray::RemoveAt(int key)
{
	delete [] (char *)m_array[key];

	m_size--;
	if (key < m_size)
		CopyMemory(&m_array[key], &m_array[key+1], sizeof(DWORD) * (m_size-key));
}

void CAxStringArray::RemoveAll()
{
	for (int ii = 0; ii < m_size; ii++)
		delete [] (char *)m_array[ii];
	m_size = 0;
}

void CAxStringArray::reNew()
{
	if (m_size >= m_maxs -1)
	{
		DWORD*	array = m_array;

		m_maxs += MAXs;
		m_array = new DWORD[m_maxs];
		CopyMemory(m_array, array, sizeof(DWORD) * m_size);
		delete [] array;
	}
}
