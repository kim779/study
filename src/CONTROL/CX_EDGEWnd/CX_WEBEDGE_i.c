

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_WEBEDGE.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_CX_WEBEDGE,0xDBE3823F,0xC5B0,0x467E,0x88,0x26,0x63,0x99,0xB9,0x20,0xE2,0x29);


MIDL_DEFINE_GUID(IID, DIID_IMainWnd,0x1F53C596,0xF91C,0x43E7,0xA3,0xFA,0x98,0xE9,0x2E,0x2B,0x13,0x7D);


MIDL_DEFINE_GUID(IID, DIID_IControlWnd,0x8F1E8118,0xD3F2,0x4D77,0xBA,0xC0,0x21,0x09,0x37,0x1E,0x8A,0xA7);


MIDL_DEFINE_GUID(CLSID, CLSID_ControlWnd,0x7C737475,0x961F,0x45D2,0xAF,0x56,0xA1,0x4E,0x47,0xD6,0x9C,0x89);


MIDL_DEFINE_GUID(CLSID, CLSID_MainWnd,0x45571518,0xC431,0x44C4,0xA2,0xFE,0x47,0xA2,0x8B,0xD2,0xA6,0x72);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



