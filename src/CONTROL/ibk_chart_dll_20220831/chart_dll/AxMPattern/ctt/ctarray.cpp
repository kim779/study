/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "cttkit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ctArray  ctNOARRAY = {NULL,0,0,0};
ctArray *ctNULLARRAY = &ctNOARRAY;
ctBars   ctNOBARS = {NULL,0,0,0};
ctBars  *ctNULLBARS = &ctNOBARS;

short WINAPI l_AllocArray(ctArray *a1, short size)
{
	a1->size = size;
	a1->width = 1;
	a1->element = 0;
	a1->type = 0;
	if ((a1->data = (float*)calloc(size, sizeof(float))) == NULL)
		return (ctErrMalloc);
	return (0);
}

short WINAPI l_AllocArrayI(ctArrayI *a1, short size)
{
	a1->size = size;
	a1->width = 1;
	a1->element = 0;
	a1->type = 1;
	if ((a1->data = (unsigned short*)calloc(size, sizeof(int))) == NULL)
		return (ctErrMalloc);
	return (0);
}

short WINAPI l_AllocArrayM(ctArray *a1, short width, short size)
{
	a1->size = size;
	a1->width = width;
	a1->element = 0;
	a1->type = 0;
	if ((a1->data = (float*)calloc(size * width, sizeof(float))) == NULL)
		return (ctErrMalloc);
	return (0);
}

short WINAPI l_AllocArrayIM(ctArrayI *a1, short width, short size)
{
	a1->size = size;
	a1->width = width;
	a1->element = 0;
	a1->type = 1;
	if ((a1->data = (unsigned short*)calloc(size * width, sizeof(int))) == NULL)
		return (ctErrMalloc);
	return (0);
}

short WINAPI l_AllocBars(ctBars *b1, short size)
{
	short      i1, i2;
	float   *data[7];
	
	b1->path[0] = '\0';
	b1->name[0] = '\0';
	b1->size = size;
	b1->datasize = size;
	b1->reccnt = 0;
	b1->type = 0;
	if ((unsigned short) size * SIZEOFBARDATA < 65000 && (b1->op.data = (float*)calloc(size, SIZEOFBARDATA)) != NULL)
	{
		b1->op.size = size;
		b1->op.width = 7;
		b1->op.element = 1;
		b1->op.type = 0;
		b1->hi = b1->lo = b1->cl = b1->vol = b1->oi = b1->op;
		b1->hi.element = 2;
		b1->lo.element = 3;
		b1->cl.element = 4;
		b1->vol.element = 5;
		b1->oi.element = 6;
		b1->dt.data = (unsigned short *) b1->op.data;
		b1->dt.size = size;
		b1->dt.width = 14;
		b1->dt.element = 0;
		b1->dt.type = 1;
		b1->ti = b1->dt;
		b1->ti.element = 1;
	}
	else
	{
		for (i1 = 0; i1 < 7; i1++)
		{
			if ((data[i1] = (float*)calloc(size, sizeof(float))) == NULL)
			{
				for (i2 = i1 - 1; i2 >= 0; i2--)
					free(data[i2]);
				b1->datasize = 0;
				return (ctErrMalloc);
			}
		}
		b1->dt.size = size;
		b1->dt.width = 2;
		b1->dt.element = 0;
		b1->dt.type = 1;
		b1->ti = b1->dt;
		b1->ti.element = 1;
		
		b1->op.size = size;
		b1->op.width = 1;
		b1->op.element = 0;
		b1->op.type = 0;
		b1->hi = b1->lo = b1->cl = b1->vol = b1->oi = b1->op;
		
		b1->dt.data = (unsigned short *) data[0];
		b1->ti.data = b1->dt.data;
		b1->op.data = data[1];
		b1->hi.data = data[2];
		b1->lo.data = data[3];
		b1->cl.data = data[4];
		b1->vol.data = data[5];
		b1->oi.data = data[6];
	}
	return (0);
}

long WINAPI l_Decimals(double value, short places)
{
	double   d1;
	return (long)fabs((modf(value, &d1) * pow(10, places)));
}

void WINAPI l_FreeArray(ctArray *a1)
{
	free(a1->data);
	return;
}

void WINAPI l_FreeArrayI(ctArrayI *a1)
{
	free(a1->data);
	return;
}

void WINAPI l_FreeBars(ctBars *b1)
{
	free(b1->dt.data);
	if (b1->op.width != 7)
	{
		free(b1->op.data);
		free(b1->hi.data);
		free(b1->lo.data);
		free(b1->cl.data);
		free(b1->vol.data);
		free(b1->oi.data);
	}
	memset(b1, 0, sizeof(ctBars));
	return;
}

float WINAPI l_GetArrayItem(ctArray *a1, short pos)
{
	if (pos >= a1->size)
		return (0);
	if (a1->type == 0)
		return (ctArrayItemP(a1, pos));
	else 
		return (ctArrayItemP(((ctArrayI *)a1), pos));
}

float WINAPI l_GetArrayItemM(ctArray *a1, short element, short pos)
{
	if (pos >= a1->size)
		return (0);
	if (a1->type == 0)
		return (ctArrayItemPM(a1, element, pos));
	else 
		return (ctArrayItemPM(((ctArrayI *)a1), element, pos));
}

short WINAPI l_IEEEtoMS(float *ieee, float *ms)
{
	unsigned short sign;
	struct ctfloatbytes
	{
		unsigned char bl, bh, al, ah;
	} temp;
	
	if (*ieee == 0)
	{
		memset(ms, 0, 4);
		return(0);
	}
	memcpy(&temp, (char *)ieee, 4);
	sign = temp.ah & 128;
	temp.ah <<= 1;
	temp.ah += ((temp.al & 128) >> 7);
	temp.al = (temp.al & 127) + sign;
	temp.ah += 2;
	memcpy((char *)ms, &temp, 4);
	return(0);
}

short WINAPI l_MStoIEEE(float *ms, float *ieee)
{
	unsigned short sign;
	struct ctfloatbytes
	{
		unsigned char bl, bh, al, ah;
	} temp;
	
	memcpy(&temp, (char *)ms, 4);
	if (temp.ah == 0)
	{
		memset(ieee, 0, 4);
		return (0);
	}
	temp.ah -= 2;
	sign = temp.al & 128;
	temp.al -= sign + ((temp.ah & 1) << 7);
	temp.ah >>= 1;
	temp.ah += sign;
	memcpy((char *)ieee, &temp, 4);
	return (0);
}

short WINAPI l_ReAllocBars(ctBars *b1, short size)
{
	void    *temp;
	
	if (b1->datasize < size)
	{
		if (b1->op.width == 7)
		{
			if ((unsigned short) size * SIZEOFBARDATA >= 65000)
				return (ctErrMalloc);
			temp = b1->op.data;
			if ((b1->op.data = (float*)realloc(b1->op.data, size * SIZEOFBARDATA)) == NULL)
			{
				b1->op.data = (float*)temp;
				return (ctErrMalloc);
			}
		}
		else
		{
			if (size > b1->op.size)
				return (ctErrMalloc);
			b1->dt.data = (unsigned short *) realloc(b1->dt.data, size);
			b1->ti.data = b1->dt.data;
			b1->op.data = (float*)realloc(b1->op.data, size);
			b1->hi.data = (float*)realloc(b1->hi.data, size);
			b1->lo.data = (float*)realloc(b1->lo.data, size);
			b1->cl.data = (float*)realloc(b1->cl.data, size);
			b1->vol.data = (float*)realloc(b1->vol.data, size);
			b1->oi.data = (float*)realloc(b1->oi.data, size);
		}
		b1->datasize = size;
	}
	l_ReSizeBars(b1, size);
	return (0);
}

short WINAPI l_ReSizeBars(ctBars *b1, short size)
{
	if (size > b1->datasize)
		return (ctErrMalloc);
	b1->size = size;
	b1->dt.size = size;
	b1->cl.size = size;
	b1->ti.size = size;
	b1->op.size = size;
	b1->hi.size = size;
	b1->lo.size = size;
	b1->vol.size = size;
	b1->oi.size = size;
	return (0);
}

void WINAPI l_SetArrayItem(ctArray *a1, short pos, float value)
{
	if (pos >= a1->size)
		return;
	if (a1->type == 0)
		ctArrayItemP(a1, pos) = value;
	else
		ctArrayItemP(((ctArrayI *)a1), pos) = (short)value;
	return;
}

void WINAPI l_SetArrayItemM(ctArray *a1, short element, short pos, float value)
{
	if (pos >= a1->size)
		return;
	if (a1->type == 0)
		ctArrayItemPM(a1, element, pos) = value;
	else
		ctArrayItemPM(((ctArrayI *)a1), element, pos) = (short)value;
	return;
}

short WINAPI l_ErrDesc(short err, char *buffer)
{
	static char *errdesc[16] =
	{
		"Sucessfull",
			"Invalid Parm",
			"File Open",
			"Malloc",
			"File Read",
			"File Write",
			"File Seek",
			"File Exists",
			"File Not Found",
			"Record Not Found",
			"No Graphics Found",
			"No Fonts Found",
			"Chart Uninitialized",
			"Max Lines Exceeded",
			"Max Grids Exceeded",
			"Unknown"
	};
	if (err > 0 || err < -14)
		err = -15;
	strcpy_s(buffer, sizeof(buffer), errdesc[-err]);
	return ((short)strlen(buffer));
}

void l_Free(void *buffer)
{
#if !defined(_WINDOWS)
	free(buffer);
#elif defined(_WIN32)
	free(buffer);
#else
	HGLOBAL  hglb;
	hglb = LOWORD(GlobalHandle(SELECTOROF(buffer)));
	GlobalUnlock(hglb);
	GlobalFree(hglb);
#endif
	return;
}

void *l_Malloc(unsigned short size)
{                
#if !defined(_WINDOWS)
	return (malloc(size));
#elif defined(_WIN32)
	return (malloc(size));
#else
	HGLOBAL  hglb;
	void FAR *lpvBuffer;
	hglb = GlobalAlloc(GHND, size);
	if (hglb == 0)
		return (NULL);
	lpvBuffer = GlobalLock(hglb);
	return (lpvBuffer);
#endif
}


/**
#ifdef _WINDOWS

int        fprintf(FILE * stream, const char *format,...)
{
	short      r1;
	char     text[1024];
	va_list  marker;
	
	va_start(marker, format);
	r1 = wvsprintf(text, format, marker);
	va_end(marker);
	fputs(text, stream);
	return (r1);
}

int        sprintf(char *text, const char *format,...)
{
	short      r1;
	va_list  marker;
	
	va_start(marker, format);
	r1 = wvsprintf(text, format, marker);
	va_end(marker);
	return (r1);
}

#endif
**/