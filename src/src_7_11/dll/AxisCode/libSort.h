#if !defined(AFX_LIBSORT_H__DC455747_81A8_4D38_87A7_7592A629C17A__INCLUDED_)
#define AFX_LIBSORT_H__DC455747_81A8_4D38_87A7_7592A629C17A__INCLUDED_

struct _sortStr
{
	CString code;
	CString name;
};

int _sortCode(const void *arg1, const void *arg2);
int _sortName(const void *arg1, const void *arg2);

#endif // !defined(AFX_LIBSORT_H__DC455747_81A8_4D38_87A7_7592A629C17A__INCLUDED_)

