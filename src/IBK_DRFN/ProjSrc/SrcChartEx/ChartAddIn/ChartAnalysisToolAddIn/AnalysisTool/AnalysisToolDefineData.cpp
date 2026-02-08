// AnalysisToolDefineData.cpp: implementation of the CAnalysisToolDefineData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnalysisToolDefineData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CSaveDefineData

const char* CSaveDefineData::COLUMN_START = "<Column>\r\n";
const char* CSaveDefineData::COLUMN_END = "<\\Column>\r\n";
const char* CSaveDefineData::ROW_START = "<Row>\r\n";
const char* CSaveDefineData::ROW_END = "<\\Row>\r\n";
const char* CSaveDefineData::ELEMENT_START = "<Element>\r\n";
const char* CSaveDefineData::ELEMENT_END = "<\\Element>\r\n";
const char* CSaveDefineData::INDEX_START = "<Index>";
const char* CSaveDefineData::INDEX_END = "<\\Index>\r\n";
const char* CSaveDefineData::TOOLTYPE_START = "<ToolType>";
const char* CSaveDefineData::TOOLTYPE_END = "<\\ToolType>\r\n";
const char* CSaveDefineData::POINTS_START = "<Points>\r\n";
const char* CSaveDefineData::POINTS_END = "<\\Points>\r\n";
const char* CSaveDefineData::POINT_START = "<Point>";
const char* CSaveDefineData::POINT_END = "<\\Point>\r\n";
const char* CSaveDefineData::COMENV_START = "<CommonEnvironment>\r\n";
const char* CSaveDefineData::COMENV_END = "<\\CommonEnvironment>\r\n";
const char* CSaveDefineData::AUTO_START = "<AutoPirce>";
const char* CSaveDefineData::AUTO_END = "<\\AutoPirce>\r\n";
const char* CSaveDefineData::PEN_START = "<Pen>\r\n";
const char* CSaveDefineData::PEN_END = "<\\Pen>\r\n";
const char* CSaveDefineData::COLOR_START = "<Color>";
const char* CSaveDefineData::COLOR_END = "<\\Color>\r\n";
const char* CSaveDefineData::WIDTH_START = "<Width>";
const char* CSaveDefineData::WIDTH_END = "<\\Width>\r\n";
const char* CSaveDefineData::STYLE_START = "<Style>";
const char* CSaveDefineData::STYLE_END = "<\\Style>\r\n";
const char* CSaveDefineData::DATATYPE_START = "<DataType>";
const char* CSaveDefineData::DATATYPE_END = "<\\DataType>\r\n";
const char* CSaveDefineData::COORDINATE_START = "<Coordinate>";
const char* CSaveDefineData::COORDINATE_END = "<\\Coordinate>\r\n";
const char* CSaveDefineData::VRET_START = "<Vert>";
const char* CSaveDefineData::VRET_END = "<\\Vert>\r\n";
const char* CSaveDefineData::HORZ_START = "<Horz>";
const char* CSaveDefineData::HORZ_END = "<\\Horz>\r\n";
const char* CSaveDefineData::PARTENV_START = "<PartEnvironment>";
const char* CSaveDefineData::PARTENV_END = "<\\PartEnvironment>\r\n";
const char* CSaveDefineData::TEXTENV_START = "<TextEnvironment>";
const char* CSaveDefineData::TEXTENV_END = "<\\TextEnvironment>\r\n";
const char* CSaveDefineData::TEXTPOS_START = "<TextPosition>";
const char* CSaveDefineData::TEXTPOS_END = "<\\TextPosition>\r\n";
const char* CSaveDefineData::SHOW_START = "<Show>";
const char* CSaveDefineData::SHOW_END = "<\\Show>\r\n";
const char* CSaveDefineData::LEFT_START = "<Left>";
const char* CSaveDefineData::LEFT_END = "<\\Left>\r\n";
const char* CSaveDefineData::RIGHT_START = "<Right>";
const char* CSaveDefineData::RIGHT_END = "<\\Right>\r\n";
const char* CSaveDefineData::TOP_START = "<Top>";
const char* CSaveDefineData::TOP_END = "<\\Top>\r\n";
const char* CSaveDefineData::BOTTOM_START = "<Bottom>";
const char* CSaveDefineData::BOTTOM_END = "<\\Bottom>\r\n";
const char* CSaveDefineData::HORZVERT_START = "<HorzVert>";
const char* CSaveDefineData::HORZVERT_END = "<\\HorzVert>\r\n";
