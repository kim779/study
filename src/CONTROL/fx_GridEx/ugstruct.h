/*************************************************************************
	Header file : ugstruct.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

	UGID - Ultimate Grid global ID - GUID
*************************************************************************/
#pragma once


typedef struct
{
    unsigned long	Data1;
    unsigned short	Data2;
    unsigned short	Data3;
    unsigned char	Data4[8];
} UGID;


