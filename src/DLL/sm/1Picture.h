#ifndef __1PICTURE_H__
#define __1PICTURE_H__

#define HIMETRIC_INCH	2540			// 1inch -> 1/100 mm

class CPicture
{
public:
	CPicture(void);
	virtual ~CPicture(void);

protected:
	HBITMAP		m_bitmap;
	IPicture*	m_picture;

public:
	bool	Load(LPCTSTR file);

	bool	GetHandle(HBITMAP& hbmp);
	void	Draw(HDC hdc, CRect rect, int bStyle);
};


#endif // __1PICTURE_H__