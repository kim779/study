#pragma once

class Cmsg : public CObject
{
public:
	Cmsg(void);
	Cmsg(int what, int size);
	~Cmsg(void);

public:
	int	m_what;

public:
	void	SetValue(int index, int val);
	void	SetPtr(int index, void* ptr);
	int	GetValue(int index);
	void*	GetPtr(int index);
	int	GetSize()	{ return m_args.GetSize(); }

protected:
	CPtrArray m_args;
};

Cmsg::Cmsg(void)
{
	m_what = -1;
	m_args.RemoveAll();
}

Cmsg::Cmsg(int what, int size)
{
	m_what = what;
	m_args.SetSize(size);
}

Cmsg::~Cmsg(void)
{
	m_args.RemoveAll();
}

void Cmsg::SetValue(int index, int val)
{
	if (index >= m_args.GetSize())
		return;
	m_args.SetAt(index, (void*)val);
}

void Cmsg::SetPtr(int index, void* ptr)
{
	if (index >= m_args.GetSize())
		return;
	m_args.SetAt(index, ptr);
}

int Cmsg::GetValue(int index)
{
	if (index >= m_args.GetSize())
		return 0;
	return (int)m_args.GetAt(index);
}

void* Cmsg::GetPtr(int index)
{
	if (index >= m_args.GetSize())
		return NULL;
	return m_args.GetAt(index);
}