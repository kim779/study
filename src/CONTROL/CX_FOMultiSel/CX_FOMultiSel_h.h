

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_FOMultiSel.odl:
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


#ifndef __CX_FOMultiSel_h_h__
#define __CX_FOMultiSel_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFOMultiSel_FWD_DEFINED__
#define __IFOMultiSel_FWD_DEFINED__
typedef interface IFOMultiSel IFOMultiSel;

#endif 	/* __IFOMultiSel_FWD_DEFINED__ */


#ifndef __FOMultiSel_FWD_DEFINED__
#define __FOMultiSel_FWD_DEFINED__

#ifdef __cplusplus
typedef class FOMultiSel FOMultiSel;
#else
typedef struct FOMultiSel FOMultiSel;
#endif /* __cplusplus */

#endif 	/* __FOMultiSel_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_FOMultiSel_LIBRARY_DEFINED__
#define __CX_FOMultiSel_LIBRARY_DEFINED__

/* library CX_FOMultiSel */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_FOMultiSel,0xDE8ADC1E,0xDC3F,0x4708,0x97,0x05,0xCC,0x65,0x5E,0x7C,0xC5,0xA1);

#ifndef __IFOMultiSel_DISPINTERFACE_DEFINED__
#define __IFOMultiSel_DISPINTERFACE_DEFINED__

/* dispinterface IFOMultiSel */
/* [uuid] */ 


DEFINE_GUID(DIID_IFOMultiSel,0xDC2EA0B2,0x3E00,0x46F4,0xBB,0x8F,0x53,0x49,0x8C,0xA5,0xDD,0x33);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DC2EA0B2-3E00-46F4-BB8F-53498CA5DD33")
    IFOMultiSel : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IFOMultiSelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFOMultiSel * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFOMultiSel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFOMultiSel * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFOMultiSel * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFOMultiSel * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFOMultiSel * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFOMultiSel * This,
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
    } IFOMultiSelVtbl;

    interface IFOMultiSel
    {
        CONST_VTBL struct IFOMultiSelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFOMultiSel_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFOMultiSel_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFOMultiSel_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFOMultiSel_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFOMultiSel_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFOMultiSel_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFOMultiSel_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IFOMultiSel_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_FOMultiSel,0x26E64617,0x022C,0x47CD,0x89,0x49,0xD4,0xED,0x7C,0xD8,0x16,0x16);

#ifdef __cplusplus

class DECLSPEC_UUID("26E64617-022C-47CD-8949-D4ED7CD81616")
FOMultiSel;
#endif
#endif /* __CX_FOMultiSel_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


