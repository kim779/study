

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for IBKSConnector.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



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
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_IBKSCONNECTORLib,0x90685967,0x8F5E,0x491A,0x8C,0xCA,0x42,0x20,0x47,0x9A,0xC1,0x89);


MIDL_DEFINE_GUID(IID, DIID__DIBKSConnector,0xC66F0D7B,0x235C,0x409A,0x90,0x18,0x05,0x87,0xD1,0x09,0x4C,0x20);


MIDL_DEFINE_GUID(IID, DIID__DIBKSConnectorEvents,0x55F8C481,0x7091,0x4ACA,0x8E,0x9C,0xA5,0xBA,0x13,0x94,0x43,0x06);


MIDL_DEFINE_GUID(CLSID, CLSID_IBKSConnector,0xCDADD338,0xC7AB,0x4977,0xB6,0x5D,0x8E,0x98,0x8B,0x59,0x58,0xE3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



