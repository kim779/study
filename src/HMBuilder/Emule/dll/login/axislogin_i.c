

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Mar 11 10:00:08 2014
 */
/* Compiler settings for axislogin.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_Axislogin,0x808A531C,0xB00D,0x4873,0xB6,0xC5,0x3D,0x68,0x06,0xA1,0xC7,0x60);


MIDL_DEFINE_GUID(IID, DIID_ILogin,0xDEE758DA,0x505A,0x48FF,0x87,0xCD,0x4E,0x4C,0xD8,0x22,0x66,0xA8);


MIDL_DEFINE_GUID(CLSID, CLSID_Login,0x9ED68054,0xA19B,0x4DA7,0x93,0xBC,0x72,0x36,0xC9,0xA9,0x3A,0x82);


MIDL_DEFINE_GUID(IID, DIID_ILedger,0x3BF10F40,0xFDE9,0x4A44,0xBA,0x55,0x56,0xDE,0x5F,0xAE,0xBA,0xC9);


MIDL_DEFINE_GUID(CLSID, CLSID_Ledger,0x11ECD400,0x3667,0x4266,0x9C,0xE7,0xB1,0xB2,0x0A,0xB0,0xA2,0x7D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



