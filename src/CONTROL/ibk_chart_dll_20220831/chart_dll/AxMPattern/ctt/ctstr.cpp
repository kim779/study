/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cttkit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

short      l_ChrCnt(char *string, char chr)
{
	short      i1 = 0, cnt = 0;
	
	for (i1 = 0; string[i1]; i1++)
		if (string[i1] == chr)
			cnt++;
		return (cnt);
}

short      l_ChrsCnt(char *string, char *chrs)
{
	short      i1 = 0, i2 = 0, cnt = 0;
	
	for (i2 = 0; chrs[i2] != '\0'; i2++)
		for (i1 = 0; string[i1] != '\0'; i1++)
			if (string[i1] == chrs[i2])
				cnt++;
			return (cnt);
}

void     l_Ltrim(char *string)
{
	int      i1;
	
	for (i1 = 0; string[i1] == ' ' && string[i1] != '\0'; i1++);
	memmove(string, string + i1, strlen(string) - i1 + 1);
}

void     l_Rtrim(char *string)
{
	int      i1;
	
	for (i1 = strlen(string) - 1; string[i1] == ' ' && i1 >= 0; i1--)
		string[i1] = '\0';
}

void     l_Trim(char *string)
{
	l_Rtrim(string);
	l_Ltrim(string);
}

short      l_StrCpyChr(char *to, char *from, char chr)
{
	short      i1;
	
	for (i1 = 0; from[i1] != chr && from[i1] != '\0'; i1++)
		to[i1] = from[i1];
	to[i1] = '\0';
	return (i1);
}

short      l_StrNthChr(char *str, char chr, short n)
{
	short      i1, i2;
	
	for (i1 = 0, i2 = 0; i2 < n && str[i1] != '\0'; i1++)
		if (str[i1] == chr)
			i2++;
		return (i1 - 1);
}


int l_strnicmp(const char *s1, const char *s2, size_t n)
{
    int i;
	
	
	
    for (i = 0; (i < (int)n) && (toupper(*s1) == toupper(*s2)); i++, s1++, s2++) {
        if (*s1 == '\0') return (0);
    }
    if (i == (int)n) return (0);
	
    return (toupper(*s1) - toupper(*s2));
}



int l_stricmp(char *s1, char *s2)
{
	int l;
	
	if(strlen(s1) > strlen(s2))
		l = strlen(s1);
	else
		l = strlen(s2);
	return l_strnicmp(s1, s2, l);
}

