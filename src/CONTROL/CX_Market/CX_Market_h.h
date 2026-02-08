

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_Market.odl:
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


#ifndef __CX_Market_h_h__
#define __CX_Market_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAxisWnd_FWD_DEFINED__
#define __IAxisWnd_FWD_DEFINED__
typedef interface IAxisWnd IAxisWnd;

#endif 	/* __IAxisWnd_FWD_DEFINED__ */


#ifndef __AxisWnd_FWD_DEFINED__
#define __AxisWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class AxisWnd AxisWnd;
#else
typedef struct AxisWnd AxisWnd;
#endif /* __cplusplus */

#endif 	/* __AxisWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_Market_LIBRARY_DEFINED__
#define __CX_Market_LIBRARY_DEFINED__

/* library CX_Market */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_Market,0xB64CB01F,0x97A8,0x41E2,0xA1,0x98,0x95,0x52,0x33,0xB1,0xFA,0x14);

#ifndef __IAxisWnd_DISPINTERFACE_DEFINED__
#define __IAxisWnd_DISPINTERFACE_DEFINED__

/* dispinterface IAxisWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IAxisWnd,0xD9F419BD,0x6114,0x4ECD,0x94,0x3A,0xB5,0xCA,0xF7,0xC1,0xA6,0x05);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D9F419BD-6114-4ECD-943A-B5CAF7C1A605")
    IAxisWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IAxisWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAxisWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAxisWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAxisWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAxisWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAxisWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAxisWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAxisWnd * This,
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
    } IAxisWndVtbl;

    interface IAxisWnd
    {
        CONST_VTBL struct IAxisWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAxisWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAxisWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAxisWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAxisWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAxisWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAxisWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAxisWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IAxisWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_AxisWnd,0xAE4A6561,0x0182,0x464D,0xB0,0xBE,0x41,0xD3,0xEB,0xB1,0xF9,0xE7);

#ifdef __cplusplus

class DECLSPEC_UUID("AE4A6561-0182-464D-B0BE-41D3EBB1F9E7")
AxisWnd;
#endif
#endif /* __CX_Market_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


