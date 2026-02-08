// Line.cpp : Defines the class behaviors for CDashLine
//

#include "stdafx.h"
//#include "DashLine.h"

#include "Line.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// helper

static void MapCourseType(CDC *pDC, int *pType)
{
  for (int i = 0; i < 4; i++)
    pType[i] = MulDiv(pType[i], pDC->GetDeviceCaps(VERTRES), 10 * pDC->GetDeviceCaps(VERTSIZE));
}

/////////////////////////////////////////////////////////////////////////////
// CDashLine

CDashLine::CDashLine(CDC* pDC, COLORREF Color1, COLORREF Color2, int nWidth, int Type[4])
{
  // save device contex and line type
  m_pDC = pDC;
  memcpy(m_Type, Type, 4 * sizeof(int));
  // correct size for printer
  MapCourseType(m_pDC, m_Type);
  // create pen object with the right width
  m_pPen[0] = m_pPen[1] = NULL;
  m_pPen[0] = new CPen; m_pPen[0]->CreatePen(PS_SOLID, nWidth, Color1);
  if (Color1 != Color2)
    { m_pPen[1] = new CPen; m_pPen[1]->CreatePen(PS_SOLID, nWidth, Color2); }
  // reset parameters
  Reset();
}

CDashLine::~CDashLine()
{
  // destroy GDI objects
  for (int c = 0; c < 2; c++)
    if (m_pPen[c] != NULL)
      { m_pPen[c]->DeleteObject(); delete m_pPen[c]; }
}

void CDashLine::Reset()
{
  m_Pos = m_Color = 0;
  memcpy(m_Count, m_Type, 4 * sizeof(int));
  m_pDC->SelectObject(m_pPen[m_Color]);
}

// use linear interpolation to compute next position
void CDashLine::Inter(int x, int y)
{
  int dx = x - m_x;
  int dy = y - m_y;

  int *p1, *p2, *pd1, *pd2;
  if (abs(dx) >= abs(dy))
    { p1 = &m_x; p2 = &m_y; pd1 = &dx; pd2 = &dy; }
  else
    { p1 = &m_y; p2 = &m_x; pd1 = &dy; pd2 = &dx; }

  int max = abs(*pd1);
  int dec = abs(*pd2);
  int s1 = (*pd1 >= 0) ? 1: -1;
  int s2 = (*pd2 >= 0) ? 1: -1;
  int val = max;

  for (int i = 0; i < max; i++)
  {
    val -= dec;
    if (val <= 0) { *p2 += s2; val += max; }
    *p1 += s1;

    m_Count[m_Pos]--;
    if (m_Count[m_Pos] == 0)
    {
      // update position
      m_Pos = (m_Pos + 1) % 4;
      if (m_Pos == 0)
        memcpy(m_Count, m_Type, 4 * sizeof(int));

      // draw new segment
      if (m_pPen[m_Color])
	m_pDC->LineTo(m_x, m_y);
      else
	m_pDC->MoveTo(m_x, m_y);

      // change color if two colors
      m_Color = (m_Color + 1) % 2;
      if (m_pPen[1])
	m_pDC->SelectObject(m_pPen[m_Color]);

      // if last point, return
      if (i == max - 1) return;
    }
  }
  // draw to last point
  if (m_pPen[m_Color])
    m_pDC->LineTo(m_x, m_y);
}

void CDashLine::MoveTo(int x, int y)
{
  // with MoveTo, reset parameters
  Reset();
  // save current position
  m_x = x; m_y = y;
  // move to position
  m_pDC->MoveTo(x, y);
}

void CDashLine::LineTo(int x, int y)
{
  // line type and color
  if (m_Type[0] == 0 && m_pPen[1] == NULL)
  {
    m_pDC->LineTo(x, y);
    m_x = x; m_y = y;
    return;
  }
  // calculate and draw next points
  Inter(x, y);
}
