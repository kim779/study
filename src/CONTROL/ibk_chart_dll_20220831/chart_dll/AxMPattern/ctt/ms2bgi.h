/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

/* Borland BGI equivelants for Microsoft graphics functions */

#define _MAXRESMODE     (-3)
#define _MAXCOLORMODE   (-2)
#define _DEFAULTMODE    (-1)
#define _TEXTBW40       0
#define _TEXTC40        1
#define _TEXTBW80       2
#define _TEXTC80        3
#define _MRES4COLOR     4
#define _MRESNOCOLOR    5
#define _HRESBW         6
#define _TEXTMONO       7
#define _HERCMONO       8
#define _MRES16COLOR    13
#define _HRES16COLOR    14
#define _ERESNOCOLOR    15
#define _ERESCOLOR      16
#define _VRES2COLOR     17
#define _VRES16COLOR    18
#define _MRES256COLOR   19
#define _ORESCOLOR      64

#define _GBORDER        2
#define _GFILLINTERIOR  3

struct videoconfig
{
 short numxpixels;
 short numypixels;
 short numtextcols;
 short numtextrows;
 short numcolors;
 short bitsperpixel;
 short numvideopages;
 short mode;
 short adapter;
 short monitor;
 short memory;
};

struct xycoord
{
 short xcoord;
 short ycoord;
};

struct rccoord
{
 short row;
 short col;
};

struct _fontinfo
{
 short   type;
 short   ascent;
 short   pixwidth;
 short   pixheight;
 short   avgwidth;
 char  filename[81];
 char  facename[32];
};

#define _getcolor() getcolor()
#define _outgtext(text) outtext(text)
#define _setlinestyle(mask) setlinestyle(4,mask,1)
#define _setviewport(x1,y1,x2,y2) setviewport(x1,y1,x2,y2,1)

struct xycoord _getcurrentposition(void);
short    _getfontinfo(struct _fontinfo *fontbuffer);
struct videoconfig *_getvideoconfig(struct videoconfig *config);
short    _lineto(short x, short y);
struct xycoord _moveto(short x, short y);
short    _rectangle(short control, short x1, short y1, short x2, short y2);
short    _registerfonts(char *pathname);
short    _setcolor(short color);
void     _setfillmask(unsigned char *mask);
short    _setfont(char *options);
short    _setpixel(short x, short y);
short    _settextcolor(short color);
struct rccoord _settextposition(short row, short col);
short    _setvideomode(short mode);

