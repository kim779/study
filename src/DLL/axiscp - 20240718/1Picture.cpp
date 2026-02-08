#include "StdAfx.h"
#include "1picture.h"

CPicture::CPicture(void) : m_picture(NULL), m_bitmap(NULL)
{
}

CPicture::~CPicture(void)
{
	if (m_picture != NULL)
	{
		m_picture->Release();
		m_picture = NULL;
	}

	if (m_bitmap != NULL)
	{
		DeleteObject(m_bitmap);
		m_bitmap = NULL;
	}
}

bool CPicture::Load(LPCTSTR file)
{
	if (m_picture != NULL)
	{
		m_picture->Release();
		m_picture = NULL;
	}

	HANDLE hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	int	size = 0;
	DWORD	read = 0;
	ULONG	realread{};
	char	twb[1024*8];
	IStream* stream;

	CreateStreamOnHGlobal(NULL, TRUE, &stream);
	for (ReadFile(hFile, twb, sizeof(twb), &read, NULL); read != 0;
		read = 0, ReadFile(hFile, twb, sizeof(twb), &read, NULL))
	{						// file -> stream
		size += read;
		stream->Write(twb, read, &realread);
	}
	CloseHandle(hFile);

	LARGE_INTEGER	move;
	move.QuadPart = 0;
	stream->Seek(move, STREAM_SEEK_SET, NULL);	// set stream position

							// create IPicture from stream
	OleLoadPicture(stream, size, FALSE, IID_IPicture, (LPVOID*)&m_picture);
	if (m_picture == NULL)
	{
		stream->Release();
		return false;
	}
	stream->Release();

	GetHandle(m_bitmap);				// create HBITMAP compatible with screen
	return true;
}

bool CPicture::GetHandle(HBITMAP& bitmap)
{
	if (m_picture == NULL)
		return false;

	OLE_XSIZE_HIMETRIC cx;
	OLE_YSIZE_HIMETRIC cy;
							// calulate size from IPicture
	m_picture->get_Width(&cx);
	m_picture->get_Height(&cy);

	HDC memDC = CreateCompatibleDC(NULL);

	const int w = MulDiv(cx, GetDeviceCaps(memDC, LOGPIXELSX), HIMETRIC_INCH);
	const int h = MulDiv(cy, GetDeviceCaps(memDC, LOGPIXELSY), HIMETRIC_INCH);

	DWORD* pBitmapBits{};
	BITMAPINFO bmi;

	ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;

	bitmap = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, (VOID**)&pBitmapBits, NULL, 0 );
	if (bitmap == NULL)
	{
		DeleteDC(memDC);
		return false;
	}
	SetMapMode(memDC, MM_TEXT);
	HBITMAP o_bitmap = (HBITMAP)SelectObject(memDC, bitmap);
							// IPicture -> HBITMAP
	m_picture->Render(memDC, 0, 0, w, h, 0, cy - 1, cx, -cy, CRect(0,0,w,h));

	SelectObject(memDC, o_bitmap);
	DeleteDC(memDC);
	return true;
}

#define AL_TILE		1
#define AL_STRETCH	2
#define AL_CENTER	0

void CPicture::Draw(HDC hDC, CRect rect, int bStyle)
{
	BITMAP	bm{};
	HDC	memDC{};
	HBITMAP o_bitmap{};
	int	ii{}, kk{};
	int	x{}, y{}, cx{}, cy{};

	GetObject(m_bitmap, sizeof(bm), &bm);
	x  = rect.left;
	y  = rect.top;
	cx = bm.bmWidth;
	cy = bm.bmHeight;

	memDC = CreateCompatibleDC(hDC);
	o_bitmap = (HBITMAP)SelectObject(memDC, m_bitmap);

	switch (bStyle)
	{
	case AL_TILE:
		for (ii = 0 ;; ii++, y += bm.bmHeight)
		{
			x  = rect.left;
			cx = bm.bmWidth;
			if (y + bm.bmHeight > rect.bottom)
				cy = rect.bottom - y;
			for (kk = 0 ;; kk++, x += bm.bmWidth)
			{
				if (x + bm.bmWidth > rect.right)
					cx = rect.right - x;
				BitBlt(hDC, x, y, cx, cy, memDC, 0, 0, SRCCOPY);
				if (cx < bm.bmWidth)
					break;
			}
			if (cy < bm.bmHeight)
				break;
		}
		break;

	case AL_STRETCH:
		StretchBlt(hDC, x, y, rect.Width(), rect.Height(), memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		break;

	case AL_CENTER:
	default:
		if (cx >= rect.Width())
		{
			ii = (cx - rect.Width())/2;
			cx = rect.Width();
		}
		else
		{
			ii = 0;
			x += (rect.Width() - cx)/2;
		}

		if (cy >= rect.Height())
		{
			kk = (cy - rect.Height())/2;
			cy = rect.Height();
		}
		else
		{
			kk = 0;
			y += (rect.Height() - cy)/2;
		}
		BitBlt(hDC, x, y, cx, cy, memDC, ii, kk, SRCCOPY);
		break;
	}

	SelectObject(memDC, o_bitmap);
	DeleteDC(memDC);
}

