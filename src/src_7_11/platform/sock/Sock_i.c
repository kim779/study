

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Mar 23 15:53:58 2017
 */
/* Compiler settings for Sock.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SOCKLib,0x4240D683,0x6BA4,0x4626,0xB0,0xD9,0x27,0xE1,0x3A,0xCB,0xE9,0x44);


MIDL_DEFINE_GUID(IID, DIID__DSock,0x425B9540,0x5811,0x4FF2,0xB0,0x54,0x41,0x7D,0x84,0x08,0xBA,0xA5);


MIDL_DEFINE_GUID(IID, DIID__DSockEvents,0xDF8B333E,0x3E0B,0x4CCD,0x94,0x0D,0x32,0x8E,0x22,0xD4,0x19,0xF4);


MIDL_DEFINE_GUID(CLSID, CLSID_Sock,0xC8D8A27C,0xB3E4,0x4998,0x97,0xB6,0x2E,0xFF,0x95,0x67,0xB3,0xF9);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



