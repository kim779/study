#pragma once


class CAxMenu : public CMenu
{
public:
	CAxMenu(){}
	virtual ~CAxMenu(){};

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};