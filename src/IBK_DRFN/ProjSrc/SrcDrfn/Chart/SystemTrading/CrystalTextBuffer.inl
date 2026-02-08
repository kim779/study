////////////////////////////////////////////////////////////////////////////
//	Inline functions of Crystal Edit classes
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#ifndef __CCrystalTextBuffer_INL_INCLUDED
#define __CCrystalTextBuffer_INL_INCLUDED

#include "CrystalTextBuffer.h"

CE_INLINE BOOL CCrystalTextBuffer::IsModified() const
{
	return m_bModified;
}

#endif // __CCrystalTextBuffer_INL_INCLUDED
