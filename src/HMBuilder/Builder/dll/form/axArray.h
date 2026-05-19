#define	MAXs		256

class CAxByteArray
{
public:
	CAxByteArray();
	~CAxByteArray();

public:
	void	InsertAt(int key, BYTE value);
	int	Add(BYTE value);
	void	RemoveAt(int key);
	void	RemoveAll()			{ m_size = 0;		}
	int	GetSize()			{ return m_size;	}
	int	GetUpperBound()			{ return m_size -1;	}
	BYTE	GetAt(int key)			{ return m_array[key];	}
	void	SetAt(int key, BYTE value)	{ m_array[key] = value;	}

private:
	void	reNew();

private:
	int	m_maxs;
	int	m_size;
	BYTE*	m_array;
};

class CAxDWordArray
{
public:
	CAxDWordArray();
	~CAxDWordArray();

public:
	void	InsertAt(int key, DWORD value);
	int	Add(DWORD value);
	void	RemoveAt(int key);
	void	RemoveAll()			{ m_size = 0;		}
	int	GetSize()			{ return m_size;	}
	int	GetUpperBound()			{ return m_size -1;	}
	DWORD	GetAt(int key)			{ return m_array[key];	}
	void	SetAt(int key, DWORD value)	{ m_array[key] = value;	}

private:
	void	reNew();

private:
	int	m_maxs;
	int	m_size;
	DWORD*	m_array;
};

class CAxStringArray
{
public:
	CAxStringArray();
	~CAxStringArray();

public:
	void	InsertAt(int key, CString value);
	int	Add(CString value);
	void	RemoveAt(int key);
	void	RemoveAll();
	int	GetSize()			{ return m_size;	}
	int	GetUpperBound()			{ return m_size -1;	}
	CString	GetAt(int key)			{ return (char *)m_array[key];	}
	void	SetAt(int key, CString value);

private:
	void	reNew();

private:
	int	m_maxs;
	int	m_size;
	DWORD*	m_array;
};
