

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_CURSORMAP.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_CX_CURSORMAPLib,0x24408BC4,0xDEF4,0x4AF8,0x83,0xDB,0xCB,0x36,0x6E,0x86,0x69,0x24);


MIDL_DEFINE_GUID(IID, DIID__DEXTCTRL,0x5ADC603A,0xF412,0x41B4,0x99,0x64,0xF4,0xBD,0x0A,0x15,0x3C,0x92);


MIDL_DEFINE_GUID(IID, DIID__DEXTCTRLEvents,0xC324A902,0x68A9,0x42BE,0x94,0x2C,0x7E,0x66,0x8F,0x1D,0x32,0x5E);


MIDL_DEFINE_GUID(CLSID, CLSID_CX_CURSORMAP,0xAA0F54A8,0x970E,0x451A,0x97,0x3E,0xB7,0x4D,0x98,0xF9,0xCF,0x16);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



