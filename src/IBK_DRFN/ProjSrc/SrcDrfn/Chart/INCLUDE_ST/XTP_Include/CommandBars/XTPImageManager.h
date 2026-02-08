// XTPImageManager.h : interface for the CXTPImageManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©2004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#if !defined(__XTPIMAGEMANAGER_H__)
#define __XTPIMAGEMANAGER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// Summary: Image state enumerators.
enum XTPImageState 
{
	xtpImageNormal,		// Normal image.
	xtpImageDisabled,	// Disabled image.
	xtpImageHot			// Hot image.
};

// Summary: Icons holder class.
class _XTP_EXT_CLASS CXTPImageManagerIcon
{
	CXTPImageManagerIcon* Scale(int nWidth);
	
public:
	// Input:   nID - Icon identifier.
	//          nWidth - Initial width for the icon.
	//          nHeight - Initial height for the icon.
	// Summary: Constructs a CXTPImageManagerIcon object.
	CXTPImageManagerIcon(UINT nID, int nWidth, int nHeight = 15);

	// Summary: Destroys a CXTPImageManagerIcon object, handles cleanup and de-allocation.
	~CXTPImageManagerIcon();

	// Returns: Identifier of the image
	// Summary: Retrieves the image identifier.
	UINT GetID();

	// Returns: TRUE if the image has alpha channel; otherwise returns FALSE
	// Summary: Checks if the image has alpha channel.
	BOOL IsAlpha();

	// Returns: Icon handle.
	// Summary: Retrieves the icon handle.
	HICON GetIcon();
	
	// Returns: Icon handle.
	// Summary: Retrieves the faded icon.
	HICON GetFadedIcon();
	
	// Returns: Icon handle.
	// Summary: Retrieves the shadow for the icon.
	HICON GetShadowIcon();
	
	// Returns: Icon handle.
	// Summary: Retrieves hot icon.
	HICON GetHotIcon();
	
	// Returns: Icon handle.
	// Summary: Retrieves the disabled icon.
	HICON GetDisabledIcon();

	// Input:	hIcon - Icon handle to set.
	// Summary: Sets the icon.
	void SetIcon(HICON hIcon);
	
	// Input:	hIcon - Icon handle to set.
	// Summary: Sets the disabled icon.
	void SetDisabledIcon(HICON hIcon);
	
	// Input:	hIcon - Icon handle to set.
	// Summary: Sets the hot icon.
	void SetHotIcon(HICON hIcon);
	
	// Input:	hIcon - Icon handle to set.
	// Summary: Sets the icon.
	void SetNormalIcon(HICON hIcon);

	// Returns: Size of the icon.
	// Summary: Retrieves the size of the icon.
	CSize GetExtent();
	
	// Input:	hIcon - Icon handle.
	// Returns: Size of the icon.
	// Summary: Retrieves the size of the icon.
	static CSize GetExtent(HICON hIcon);
	
	// Returns: Height of the icon.
	// Summary: Retrieves the icon height.
	int GetHeight();
	
	// Returns: Width of the icon.
	// Summary: Retrieves the icon width.
	int GetWidth();

	// Input:	archive - A CArchive object to serialize to or from. 
	// Summary: Reads or writes this object from or to an archive.
	void Serialize(CArchive& ar);
	
	// Input:	hIcon - Icon to be serialized
	// Input:	archive - A CArchive object to serialize to or from. 
	// Summary: Reads or writes this object from or to an archive.
	void SerializeIcon(HICON& hIcon, CArchive& ar);

public:	
	static HICON ScaleToFit(HICON hIcon, CSize szExtent, int nWidth);	
	
private:
	void InitBimaps(CImageList& imageList, int nIndex);
	void CreateFadedIcon();
	void CreateShadowIcon();
	void CreateDisabledIcon();
	
	void Clear(BOOL bIcon = FALSE);
	void Refresh();
	COLORREF LightenColor(COLORREF clr, double factor);

	BOOL GetDIBBitmap(HBITMAP hBitmap, PBYTE& pBits, PBITMAPINFO& pbmi);
	HBITMAP ReadDIBBitmap(CArchive& ar);


private:
	const UINT m_nID;
	const UINT m_nWidth;
	UINT m_nHeight;
	HICON m_hIcon;
	HICON m_hFaded;
	HICON m_hShadow;
	HICON m_hHot;
	HICON m_hDisabled;	
	HICON m_hDisabledAuto;	
	BOOL m_bAlpha;
	BOOL m_bScaled;
	
	friend class CXTPImageManager;
	friend class CXTPImageManagerIconSet;
};


// Summary: CXTPImageManagerIconSet represents the collection of icons for specified identifier.
class _XTP_EXT_CLASS CXTPImageManagerIconSet
{
public:
	typedef CMap<UINT, UINT, CXTPImageManagerIcon*, CXTPImageManagerIcon*> CIconSetMap;	// Icons collection.

	// Input:	nID - Icon set identifier.
	// Summary: Constructs a CXTPImageManagerIconSet object.
	CXTPImageManagerIconSet(UINT nID);
	
	// Summary: Destroys a CXTPImageManagerIconSet object, handles cleanup and de-allocation.
	~CXTPImageManagerIconSet();
	
	// Input:	nWidth - Width of the icon to retrieve.
	//			bScaled - TRUE to scale icon.
	// Returns: A pointer to a CXTPImageManagerIcon object
	// Summary: Call this member to get the icon with the specified width
	CXTPImageManagerIcon* GetIcon(UINT nWidth, BOOL bScaled = TRUE);
	
	// Returns: A pointer to a CIconSetMap object
	// Summary: Retrieves the collection of icons
	CIconSetMap* GetIcons();
	
	// Returns: Identifier of icons.
	// Summary: Retrieves the identifier of the collection set.
	UINT GetID();

private:
	CXTPImageManagerIcon* CreateIcon(UINT nWidth);
	void SetIcon(HICON hIcon, UINT nWidth, BOOL bAlpha = FALSE);
	void SetDisabledIcon(HICON hIcon, UINT nWidth);
	void SetHotIcon(HICON hIcon, UINT nWidth);	
	
	void RemoveAll();
	void RefreshAll(void);
	
private:
	
	CIconSetMap m_mapIcons;
	const UINT m_nID;

	friend class CXTPImageManager;
};


// Summary: CXTPImageManager is a stand alone class. It is used to manipulate a command bar's icons.
class _XTP_EXT_CLASS CXTPImageManager : public CCmdTarget
{
public:

	// Summary: Constructs a CXTPImageManager object.
	CXTPImageManager(void);
	
	// Summary: Destroys a CXTPImageManager object, handles cleanup and de-allocation.
	~CXTPImageManager(void);
public:

	// Input:   nIDResourceToolBar - Toolbar resource identifier
	//			imageState - Images state.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Adds the icons.
	BOOL SetIcons(UINT nIDResourceToolBar, XTPImageState imageState = xtpImageNormal);
	
	// Input:	nIDResourceBitmap - Bitmap resource identifier
	//			pCommands - Pointer to an array of IDs. 
	//			nCount - Number of elements in the array pointed to by lpIDArray.
	//			szIcon - Size of the icons.
	//			imageState - Images state.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Adds the icons.
	BOOL SetIcons(UINT nIDResourceBitmap, UINT* pCommands, int nCount, CSize szIcon, XTPImageState imageState = xtpImageNormal);
	
	// Input:	nIDResourceToolBar - Toolbars identifiers resource.
	//			nIDResourceBitmap - Bitmap resource.
	//			imageState - Images state.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Adds the icons.
	BOOL SetIcons(UINT nIDResourceToolBar, UINT nIDResourceBitmap, XTPImageState imageState = xtpImageNormal);
	
	// Input:	bmpIcons - Bitmap containing icons to be added.
	//			pCommands - Pointer to an array of IDs. 
	//			nCount - Number of elements in the array pointed to by lpIDArray.
	//			szIcon - Size of the icons.
	//			imageState - Images state.
	//			bAlpha - TRUE if bitmap has alpha channel.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Adds the icons.
	BOOL SetIcons(CBitmap& bmpIcons, UINT* pCommands, int nCount, CSize szIcon, XTPImageState imageState = xtpImageNormal, BOOL bAlpha = FALSE);
	
	// Input:	imlIcons - Image list containing the icons to be added.
	//			pCommands - Pointer to an array of IDs. 
	//			nCount - Number of elements in the array pointed to by lpIDArray.
	//			szIcon - Size of the icons.
	//			imageState - Images state.
	//			bAlpha - TRUE if bitmap has alpha channel.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Adds the icons.
	BOOL SetIcons(CImageList& imlIcons, UINT* pCommands, int nCount, CSize szIcon, XTPImageState imageState = xtpImageNormal, BOOL bAlpha = FALSE);
	
	// Input:	nIDResourceToolBar - Toolbar's identifiers resource.
	//			bmpIcons - Bitmap containing the icons to be added.
	//			imageState - Images state.
	//			bAlpha - TRUE if bitmap has alpha channel.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Adds the icons.
	BOOL SetIcons(UINT nIDResourceToolBar, CBitmap& bmpIcons, XTPImageState imageState = xtpImageNormal, BOOL bAlpha = FALSE);
	
	// Input:	nIDResourceToolBar - Toolbar's identifiers resource.
	//			imlIcons - Image list containing the icons to be added.
	//			imageState - Images state.
	//			bAlpha - TRUE if bitmap has alpha channel.
	// Summary: Adds the icons.
	BOOL SetIcons(UINT nIDResourceToolBar, CImageList& imlIcons, XTPImageState imageState = xtpImageNormal, BOOL bAlpha = FALSE);

	// Input:	nIDResourceIcon - Toolbar's identifiers resource.
	//			imageState - Images state.
	//			bAlpha - TRUE if bitmap has alpha channel.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Adds the icons.
	BOOL SetIcon(UINT nIDResourceIcon, XTPImageState imageState = xtpImageNormal, BOOL bAlpha = FALSE);
	
	// Input:	hIcon - Icon handle to add
	//			nIDCommand - Icon identifier.
	//			szIcon - Size of the icons.
	//			imageState - Images state.
	//			bAlpha - TRUE if icon has alpha channel.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Adds the icon.
	BOOL SetIcon(HICON hIcon, UINT nIDCommand, CSize szIcon = 0, XTPImageState imageState = xtpImageNormal, BOOL bAlpha = FALSE);

	// Input:	clrMask - Mask color to be set.
	// Returns: Previous color mask.
	// Summary: Call this method to set the color mask of the bitmaps that will be added.
	COLORREF SetMaskColor(COLORREF clrMask);
	
	// Returns: Current color mask.
	// Summary: Retrieves color mask.
	COLORREF GetMaskColor();
	
	// Summary: Removes all icons.
	void RemoveAll(void);
	
	// Summary: Redraws all icons.
	void RefreshAll(void);

	// Input:	nCommand - ID of the icon to be retrieved
	//			nWidth - Width of the icon to be retrieved
	// Returns: A pointer to a CXTPImageManagerIcon
	// Summary: Call this member to get the icons with the specified Id and width.
	CXTPImageManagerIcon* GetImage(UINT nCommand, int nWidth = 16);
	
	// Input:	nCommand - ID of the icon set to be retrieved
	// Returns: A pointer to a CXTPImageManagerIconSet object
	// Summary: Call this member to get the icons set with the specified Id.
	CXTPImageManagerIconSet* GetIconSet(UINT nCommand);
	
	// Returns: A CMap object
	// Summary: Retrieves a collection of images.
	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* GetImages();

	// Input:	nCommand - Identifier needed to check.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Checks if icons set exists with the specified ID.
	BOOL Lookup(UINT nCommand);

public:

	// Input:	nCommand - Specified command to serialize.
	//          ar - A CArchive object to serialize to or from. 
	// Summary: Reads or writes icons from or to an archive.
	void Serialize(UINT nCommand, CArchive& ar);
	
	// Input:	ar - A CArchive object to serialize to or from. 
	// Summary: Reads or writes all icons from or to an archive.
	void Serialize(CArchive& ar);


public:	

	// Input:	nIDResourceBitmap - Resource identifier of the bitmap to check.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Checks if bitmap with specified resource has alpha channel.
	static BOOL AFX_CDECL IsAlphaBitmapResource(UINT nIDResourceBitmap);
	
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Determines if the OS supports alpha icons.
	static BOOL AFX_CDECL IsAlphaIconsSupported();

	CLIPFORMAT m_nClipFormat;					// Clipboard format of icons.

public:
	static double m_dDisabledBrightnessFactor;	// Brightness factor of disabled icons.
	static double m_dDisabledAlphaFactor;		// Alpha factor of disabled icons.
	
private:
	BOOL LoadToolbar(UINT nIDResourceToolBar, UINT*& pItems, int& nCount, CSize& szIcon);
	
	UINT AddCustomIcon(HICON hIcon);
	UINT PasteCustomImage(COleDataObject& data);
	void CopyImage(UINT nCommand);

private:
	COLORREF m_clrMask;
	static CXTPImageManager s_managerInstance;
	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*> m_mapImages;

	UINT m_nCustomID;

private:	
	friend CXTPImageManager* XTPImageManager();
	friend class CXTPCustomizeSheet;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE UINT CXTPImageManagerIconSet::GetID() { 
	return m_nID; 
}
AFX_INLINE UINT CXTPImageManagerIcon::GetID() { 
	return m_nID; 
}
AFX_INLINE int CXTPImageManagerIcon::GetHeight() {
	return m_nHeight;
}
AFX_INLINE int CXTPImageManagerIcon::GetWidth() {
	return m_nWidth;
}
AFX_INLINE int CXTPImageManagerIcon::IsAlpha() {
	return m_bAlpha;
}

AFX_INLINE CXTPImageManager* XTPImageManager() { 
	return &CXTPImageManager::s_managerInstance; 
}

AFX_INLINE CXTPImageManagerIconSet::CIconSetMap* CXTPImageManagerIconSet::GetIcons() {
	return &m_mapIcons;
}
AFX_INLINE CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* CXTPImageManager::GetImages() {
	return  &m_mapImages;
}

#endif //#if !defined(__XTPIMAGEMANAGER_H__)