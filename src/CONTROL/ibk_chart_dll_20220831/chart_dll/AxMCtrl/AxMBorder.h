#ifndef __AXMBORDER_H__
#define __AXMBORDER_H__

class AFX_EXT_CLASS CAxMBorder
{
public:
	CAxMBorder();
	virtual ~CAxMBorder();

	enum {
		AXB_RAISEDINNER = BDR_RAISEDINNER,
		AXB_SUNKENINNER = BDR_SUNKENINNER,
		AXB_RAISEDOUTER = BDR_RAISEDOUTER,
		AXB_SUNKENOUTER = BDR_SUNKENOUTER,
		AXB_BUMP        = EDGE_BUMP,
		AXB_ETCHED      = EDGE_ETCHED,
		AXB_RAISED	    = EDGE_RAISED,
		AXB_SUNKEN      = EDGE_SUNKEN
	};
public:
	// Draw Edge
	void DrawEdge(CDC* dc, CRect rc, UINT edge, UINT flags = BF_RECT);
	// Draw single Border
	void DrawBorder(CDC* dc, CRect rc, COLORREF borderColor);
};


#endif // __AXMBORDER_H__