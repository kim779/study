// ExResizeRect.h: interface for the CExResizeRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXRESIZERECT_H__F5E166DD_38EB_428D_897D_3194F8053874__INCLUDED_)
#define AFX_EXRESIZERECT_H__F5E166DD_38EB_428D_897D_3194F8053874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef float EX_RESIZE;

struct EX_RESIZERECT
{
	EX_RESIZE left;		// Specifies the x-coordinate of the upper-left corner of a rectangle.
	EX_RESIZE top;		// Specifies the y-coordinate of the upper-left corner of a rectangle.
	EX_RESIZE right;	// Specifies the x-coordinate of the lower-right corner of a rectangle.
	EX_RESIZE bottom;	// Specifies the y-coordinate of the lower-right corner of a rectangle.
};

class CExResizeRect : public EX_RESIZERECT
{
public:
	CExResizeRect();
	CExResizeRect(const RECT& rc);
	CExResizeRect(const EX_RESIZERECT& rrc);
	CExResizeRect(EX_RESIZE l,EX_RESIZE t,EX_RESIZE r,EX_RESIZE b);

	CExResizeRect& operator = (const RECT& rc);
	CExResizeRect& operator = (const EX_RESIZERECT& rrc);
	CExResizeRect& operator += (const EX_RESIZERECT& rrc);
	CExResizeRect operator + (const EX_RESIZERECT& rrc);
	CExResizeRect operator & (const EX_RESIZERECT& rrc);

	bool operator == (const EX_RESIZERECT& rrc);
	bool operator != (const EX_RESIZERECT& rrc);

	operator CRect();

	bool IsNormalized();
	EX_RESIZE Width();
	EX_RESIZE Height();

};

AFX_INLINE CExResizeRect::CExResizeRect(const RECT& rc) {
	(operator =)(rc);
}
AFX_INLINE CExResizeRect::CExResizeRect(const EX_RESIZERECT& rrc) {
	(operator =)(rrc);
}
AFX_INLINE CExResizeRect CExResizeRect::operator + (const EX_RESIZERECT& rrc) { 
	return CExResizeRect(left + rrc.left, top + rrc.top, right + rrc.right, bottom += rrc.bottom); 
}
AFX_INLINE bool CExResizeRect::IsNormalized() {
	return ((left <= right) && (top <= bottom));
}
AFX_INLINE bool CExResizeRect::operator == (const EX_RESIZERECT& rrc) { 
	return left==rrc.left && top==rrc.top && right==rrc.right && bottom==rrc.bottom; 
}
AFX_INLINE bool CExResizeRect::operator != (const EX_RESIZERECT& rrc) { 
	return !operator==(rrc); 
}
AFX_INLINE CExResizeRect::operator CRect() { 
	return CRect((int) left, (int) top, (int) right, (int) bottom); 
}
AFX_INLINE EX_RESIZE CExResizeRect::Width() { 
	return right-left; 
}
AFX_INLINE EX_RESIZE CExResizeRect::Height() { 
	return bottom-top; 
}

#define SZ_RESIZE(x)	CExResizeRect(0,0,x,x)  // Resize.
#define SZ_REPOS(x)		CExResizeRect(x,x,x,x)  // Reposition.
#define SZ_HORRESIZE(x)	CExResizeRect(0,0,x,0)  // Horizontal resize.
#define SZ_HORREPOS(x)	CExResizeRect(x,0,x,0)  // Horizontal reposition.
#define SZ_VERRESIZE(x)	CExResizeRect(0,0,0,x)  // Vertical resize.
#define SZ_VERREPOS(x)	CExResizeRect(0,x,0,x)  // Vertical reposition.

#endif // !defined(AFX_EXRESIZERECT_H__F5E166DD_38EB_428D_897D_3194F8053874__INCLUDED_)
