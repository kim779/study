#ifndef __MBONG_LIB_H__
#define __MBONG_LIB_H__

/**************************************************************************************/
// Template Information - Function
/**************************************************************************************/
//#include <afxtempl.h>
#include <math.h>

/**************************************************************************************/
// class
/**************************************************************************************/

class CBongData
{
public:
	CBongData()	{ Reset(); }
	~CBongData()	{ Reset(); }
	void Reset()
	{
		m_date = _T("");
		m_siga = 0.0;
		m_koga = 0.0;
		m_jega = 0.0;
		m_jgga = 0.0;
		m_gvol = 0;
	}

	void SetData(struct _Candle* data)
	{
		CString	tmpstr;
		
		m_date.Format("%.*s", sizeof(data->date), data->date);

		tmpstr.Format("%.*s", sizeof(data->siga), data->siga);
		m_siga = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->koga), data->koga);
		m_koga = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->jega), data->jega);
		m_jega = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->jgga), data->jgga);
		m_jgga = atof(tmpstr);

		tmpstr.Format("%.*s", sizeof(data->gvol), data->gvol);
		m_gvol = atof(tmpstr);
	}
/*
	void SetEmptyData(double val)
	{
		m_date = _T("00000000");
		m_siga = m_koga = m_jega = m_jgga = m_gvol = val;
	}
*/
	CString		m_date;
	double		m_siga;
	double		m_koga;
	double		m_jega;
	double		m_jgga;
	double		m_gvol;
};
#endif

		