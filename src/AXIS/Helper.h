#ifndef __HELPER_H__
#define __HELPER_H__

#define SZ_FRAME			2
#define SZ_MAINCAPTION		20
#define SZ_CHILDCAPTION		20


class CAxButtonName
{
public:
	CString m_normal, m_hover, m_down;
	bool m_skin;
public:
	CAxButtonName(const char* lpszButtonName)
	{
		m_skin = false;
		if (lpszButtonName && strlen(lpszButtonName) > 0)
			SetName(lpszButtonName);
	}
	CAxButtonName(const char* lpszButtonName,bool bskin)
	{
		m_skin = bskin;

		if (lpszButtonName && strlen(lpszButtonName) > 0)
			SetSkinName(lpszButtonName);
	}

#pragma warning (disable : 6284)
	void SetName(const char* lpszButtonName)
	{
			m_normal.Format("%s\\image\\%s.bmp", Axis::home, lpszButtonName);
			m_hover.Format("%s\\image\\%s_en.bmp", Axis::home, lpszButtonName);
			m_down.Format("%s\\image\\%s_dn.bmp", Axis::home, lpszButtonName);
	}
#pragma warning (default : 6284)

	void SetSkinName(const char* lpszButtonName)
	{
		m_normal.Format("%s", lpszButtonName);
		m_hover.Format("%s_en", lpszButtonName);
		m_down.Format("%s_dn", lpszButtonName);
	}


	inline const char* Normal() {	return (const char*)m_normal; }
	inline const char* Hover()  {	return (const char*)m_hover;  }
	inline const char* Down()   {	return (const char*)m_down;   }
};

//** macro definition
#define DELETE_WND(a) { if (a) { a->DestroyWindow(); delete a; a = NULL; }}
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

#endif // __HELPER_H__