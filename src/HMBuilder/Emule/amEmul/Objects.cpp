// Objects.cpp: implementation of the CObjects class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Objects.h"
#include "Misc.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjects::CObjects(CString root)
{
	m_root = root;
	m_objects.RemoveAll();
}

CObjects::~CObjects()
{
	CString	keys;
	CpubControl* object;

	for (POSITION pos = m_objects.GetStartPosition(); pos; )
	{
		m_objects.GetNextAssoc(pos, keys, (void *&)object);
		delete object->m_target;
		AfxFreeLibrary(object->m_dll);
		delete object;
	}
	m_objects.RemoveAll();
}

bool CObjects::CreateObject(CString name, CCmdTarget*& target)
{
	CpubControl* object;

	name.MakeUpper();
	if (m_objects.Lookup(name, (void *&)object))
	{
		target = object->m_target;
		return true;
	}

	CString	path;
	HINSTANCE hinst;

	path.Format("%s/%s/%s.dll", m_root, DEVDIR, name);
	hinst = AfxLoadLibrary(path);
	if (hinst)
	{
		CCmdTarget* (APIENTRY* axCreate)(void);
		CCmdTarget* (APIENTRY* axCreateEx)(void*);

		axCreate = (CCmdTarget* (APIENTRY*)(void))GetProcAddress(hinst, _T("axCreate"));
		if (axCreate != NULL)
		{
			target = (*axCreate)();
			if (target)
			{
				object = new CpubControl;
				object->m_dll    = hinst;
				object->m_target = target;
				m_objects.SetAt(name, object);
				return true;
			}
		}
		else
		{
			axCreateEx = (CCmdTarget* (APIENTRY*)(void*))GetProcAddress(hinst, _T("axCreateEx"));
			if (axCreateEx != NULL)
			{
				target = (*axCreateEx)((void*)m_root.operator LPCTSTR());
				if (target)
				{
					object = new CpubControl;
					object->m_dll    = hinst;
					object->m_target = target;
					m_objects.SetAt(name, object);
					return true;
				}
			}
		}
		AfxFreeLibrary(hinst);
	}

	target = NULL;
	return false;
}
