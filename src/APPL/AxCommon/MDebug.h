
#pragma once
namespace mdebug
{


struct ETCType {
	ETCType(): str("") {};
	CString str;
};

struct Decimal : public ETCType {
	Decimal(int value, int len, bool fillZero = false);
	Decimal(double value, int len, int decimalLen, bool fillZero = false);
};

struct Today : public ETCType {
	Today(const char* dateFormat = "%Y.%m.%d");
};

struct Now : public ETCType {
	Now(const char* timeFormat = "%H:%M:%S");
}; 

struct FormatString: public ETCType {
	FormatString(const char* format, ...);
};

struct Hexa: public ETCType {
	Hexa(const char* value);
	Hexa(const unsigned char* value, int len);
};

class CDebug {
private:
	CString m_prefix;
	CString m_str;
	bool	m_autoSpace{};
public:
	CDebug(const char* prefix);
	CDebug() {}
	~CDebug();
public:
	const char* Get() const;

	CDebug& operator=(const char* str);
	CDebug& operator<<(const char* value);
	CDebug& operator<<(const int value);
	CDebug& operator<<(const double value);
	CDebug& operator<<(const char value);
	CDebug& operator<<(const unsigned char value);
	CDebug& operator<<(const RECT* rc);
	CDebug& operator<<(const POINT* pt);
	CDebug& operator<<(const ETCType& value);

	void SetAutoSpace(bool newValue);
public:
	static void Out(const char * format, ...);

};

extern const char* endl;

} // namespace mdebug
