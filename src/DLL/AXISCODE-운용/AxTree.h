
// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2019
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information.
//  
//  작성자        :  김정식
//  요약          :  Tree를 쉽게 제어하는 클래스 
//  Components   :   AxObjTree.h, AxObjTree.cpp	
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2019-01	Initial version
// *****************************************************************************

#pragma once

 
template <typename T>
class xx
{
public:
	static_assert(std::is_assignable<T&, std::nullptr_t>::value, "T cannot be assigned nullptr.");

	template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
	constexpr xx(U&& u) : ptr_(std::forward<U>(u))
	{
	}

	template <typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
	constexpr xx(T u) : ptr_(u)
	{
	}

	template <typename U>
	constexpr xx(U& other, bool flag) : ptr_(other , flag)
	{
	}

	template <typename U>
	constexpr xx(U& other) : ptr_(other)
	{
	}

	T& get()
	{
		return ptr_;
	}

	constexpr operator T() const { return get(); }
	constexpr T operator->() const { return get(); }
	constexpr decltype(auto) operator*() const { return *get(); }

	xx(const xx& other) = default;
	xx(xx&& other) = default;	
	
	// prevents compilation when someone attempts to assign a null pointer constant
	xx(std::nullptr_t) = delete;
	xx& operator=(const xx& other) = delete;

	// unwanted operators...pointers only point to single objects!
	xx& operator++() = delete;
	xx& operator--() = delete;
	xx operator++(int) = delete;
	xx operator--(int) = delete;
	xx& operator+=(std::ptrdiff_t) = delete;
	xx& operator-=(std::ptrdiff_t) = delete;

	template <typename U>
	xx& operator<<(U  u)
	{
		ptr_ = u;
		return *this;
	}

	template <typename U>
	xx& operator=(U  u)
	{
		ptr_ = u;
		return *this;
	}
	
	template <typename U>
	constexpr xx operator[](U u)
	{
		return std::move(xx<T>(ptr_.CreateNode(u)));
	}
private:
	T ptr_;
};


enum imgTYPE { imgNONE = -2 };
class AxNode
{
	friend class AxTree;
public:
	static void setTree(AxTree* ptree) { _pTree = ptree; }

	AxNode(const HTREEITEM hitem);
	AxNode(const CString, bool bCreate = true, HTREEITEM hItem = (HTREEITEM)4294901760U);
	AxNode(const AxNode& node);
	AxNode(const AxNode&& node) noexcept;
	
	void operator=(const AxNode&& rhs);
	AxNode& operator=(CString sValue);
	AxNode& operator=(int value);
	AxNode& operator<<(CString sValue);
	AxNode& operator<<(int value);

	inline HTREEITEM getItem() noexcept { return _htree;  }
	inline int       getImg()  noexcept { return _iImg;   }
	inline CString   getText() noexcept { return _sNode; }
	inline CString   getData() noexcept { return _sData;  }

	AxNode  CreateNode(CString s, bool bflag = true);
	bool    Expand(UINT style = TVE_EXPAND);
	AxNode& Bold(bool bflag = true);
	HTREEITEM getParent();
	bool	Delete();
	bool	ReName(CString name);
	bool    Select();
	BOOL    SelectItem();
	void    setData(CString sData);
	void    setData(int val);

protected:
	bool getNode(HTREEITEM item);

private:
	static gsl::renter<AxTree*> _pTree;

	HTREEITEM _htree;
	CString   _sNode;
	CString   _sData;
	int	  _iImg;
	int       _sImg;
	
};

using xnode = ::xx<AxNode>;
class AxTree : public CTreeCtrl
{
	friend class AxNode;
	DECLARE_DYNAMIC(AxTree)
public:
	AxTree() noexcept;
//	bool	 SetAt(CString sKey, HTREEITEM item);
//	bool	 Lookup(CString sKey, HTREEITEM& item);
	
	BOOL	 clear();
	void     ExpandAll(UINT style = TVE_EXPAND);
	

	void	 initTree(int sort = 0);
	bool     search(CString sTitle);

	int	  getSelectedItemCount();
	HTREEITEM getFirstSelectedItem();
	HTREEITEM getNextSelectedItem(HTREEITEM hitem);

	map <HTREEITEM, CString>  _dataMap;
	imgTYPE _imType;
	
protected:
	std::shared_ptr<CImageList> createDragImage(CPoint& point);

private:
	
	map<CString, HTREEITEM>		_treemap;
	std::shared_ptr<CImageList>	_dragImg;
	void	 removeKey(CString sItem);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
