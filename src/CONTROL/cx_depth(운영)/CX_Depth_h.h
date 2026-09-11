

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_Depth.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __CX_Depth_h_h__
#define __CX_Depth_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Idepth_FWD_DEFINED__
#define __Idepth_FWD_DEFINED__
typedef interface Idepth Idepth;

#endif 	/* __Idepth_FWD_DEFINED__ */


#ifndef __depth_FWD_DEFINED__
#define __depth_FWD_DEFINED__

#ifdef __cplusplus
typedef class depth depth;
#else
typedef struct depth depth;
#endif /* __cplusplus */

#endif 	/* __depth_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_Depth_LIBRARY_DEFINED__
#define __CX_Depth_LIBRARY_DEFINED__

/* library CX_Depth */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_Depth,0x5096BF88,0x3C0E,0x40FA,0xB6,0x5E,0xBC,0x16,0x14,0x6A,0x62,0x7C);

#ifndef __Idepth_DISPINTERFACE_DEFINED__
#define __Idepth_DISPINTERFACE_DEFINED__

/* dispinterface Idepth */
/* [uuid] */ 


DEFINE_GUID(DIID_Idepth,0xD31D7272,0x4BBD,0x4056,0xB4,0x8E,0x28,0x10,0x4E,0x17,0xB4,0x58);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D31D7272-4BBD-4056-B48E-28104E17B458")
    Idepth : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IdepthVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Idepth * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Idepth * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Idepth * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Idepth * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Idepth * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Idepth * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Idepth * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IdepthVtbl;

    interface Idepth
    {
        CONST_VTBL struct IdepthVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Idepth_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Idepth_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Idepth_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Idepth_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define Idepth_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define Idepth_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define Idepth_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __Idepth_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_depth,0x62C27CCE,0xB03E,0x47CB,0xB4,0xD2,0x60,0x88,0xD0,0xB5,0x38,0xBE);

#ifdef __cplusplus

class DECLSPEC_UUID("62C27CCE-B03E-47CB-B4D2-6088D0B538BE")
depth;
#endif
#endif /* __CX_Depth_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


