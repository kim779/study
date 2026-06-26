// CDlg_SSib.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_SSib.h"
#include "afxdialogex.h"


// CDlg_SSib 대화 상자

IMPLEMENT_DYNAMIC(CDlg_SSib, CDialogEx)

CDlg_SSib::CDlg_SSib(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLGSSIB, pParent)
{
	AfxMessageBox("dlg contstructor");
}

CDlg_SSib::~CDlg_SSib()
{
	AfxMessageBox("dlg destructor");
}

void CDlg_SSib::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_SSib, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_SSib::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_SSib::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_MUITABLE, &CDlg_SSib::OnBnClickedBtnMuitable)
	ON_BN_CLICKED(IDC_BTN_STATIC, &CDlg_SSib::OnBnClickedBtnStatic)
	ON_BN_CLICKED(IDC_BTN_REF, &CDlg_SSib::OnBnClickedBtnRef)
	ON_BN_CLICKED(IDC_BTN_FRIEND, &CDlg_SSib::OnBnClickedBtnFriend)
	ON_BN_CLICKED(IDC_BTN_OVER, &CDlg_SSib::OnBnClickedBtnOver)
	ON_BN_CLICKED(IDC_BTN_LAMDA, &CDlg_SSib::OnBnClickedBtnLamda)
	ON_BN_CLICKED(IDC_BTN_MANGLING, &CDlg_SSib::OnBnClickedBtnMangling)
	ON_BN_CLICKED(IDC_BTN_STFUNC, &CDlg_SSib::OnBnClickedBtnStfunc)
	ON_BN_CLICKED(IDC_BTN_TEMPLETE, &CDlg_SSib::OnBnClickedBtnTemplete)
	ON_BN_CLICKED(IDC_BTN_TSTATIC, &CDlg_SSib::OnBnClickedBtnTstatic)
	ON_BN_CLICKED(IDC_BTN_BBSORT, &CDlg_SSib::OnBnClickedBtnBbsort)
	ON_BN_CLICKED(IDC_BTN_INHERI, &CDlg_SSib::OnBnClickedBtnInheri)
	ON_BN_CLICKED(IDC_BTN_STDMAP, &CDlg_SSib::OnBnClickedBtnStdmap)
END_MESSAGE_MAP()


// CDlg_SSib 메시지 처리기


void CDlg_SSib::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

//#define DF_TEST_explicit
//#define DF_MUTALBE
class cl_explicit_Test
{
public:
#ifdef DF_MUTALBE
	mutable int m_val;
#else
	int m_val;
#endif
	char* m_pdata;
	CString m_slog;

#ifdef DF_TEST_explicit
		explicit cl_explicit_Test(int ival)
#else
	cl_explicit_Test(int ival)
#endif
	{
		m_slog.Format("ival = [%d]", ival);
		m_val = ival;
		OutputDebugString(m_slog);
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	cl_explicit_Test(const char* pdata)
	{
		m_slog.Format("pdata =[%s]", pdata);
	
		m_pdata = new char[strlen(pdata)];
		memcpy(m_pdata, pdata, strlen(pdata));
		OutputDebugString(m_slog);
	}

	cl_explicit_Test(const cl_explicit_Test& cl_tmp)
	{
		m_slog.Format("cl_explicit_Test ");
		m_val = cl_tmp.m_val;
		OutputDebugString(m_slog);
	}
#ifdef DF_MUTALBE
	void test_mutable(int ival) const
#else
	void test_mutable(int ival) 
#endif
	{
		m_val = ival; 
	}
};

void test_cl_explicit(cl_explicit_Test data)
{
	CString m_slog;
	m_slog.Format("cl_explicit_Test [%s] [%d]", data.m_pdata, data.m_val);
	OutputDebugString(m_slog);
}

void CDlg_SSib::OnBnClickedButton1() //explicit
{//explicit  암시적 변환를 하지 말라는 것이다. 선언된대로 하라!!!
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	test_cl_explicit("test");
	test_cl_explicit(3);  //암시적으로 int형을 받는 생성자가 불러진다.
}


void CDlg_SSib::OnBnClickedBtnMuitable()
{//함수 선언 끝에 const를 붙여두면 그 함수의 정의에서 클래스 멤버변수를 수정할수 없게 한다.
	//이럴때 그 멤버변수를 mutable로 선언해두면 const 함수에서 수정이 가능하게 된다.
	cl_explicit_Test  cs_tss(3);
#ifdef DF_MUTALBE
	cs_tss.test_mutable(3);
#endif
}

class cs_static
{
public:
	 static int m_num;
	 CString m_slog;
	 cs_static() { m_num++;  m_slog.Format("[ssib] %d", m_num); OutputDebugString(m_slog); };
	~cs_static() {};
	
};
int cs_static::m_num = 0;  //중요하다..

void CDlg_SSib::OnBnClickedBtnStatic()
{//클래스 내부에 static 변수를 만들면 모든 클래스 인스턴스에서 그 변수를 공유하게 된다.
	//객체에 종속되는 변수가 아니라 클래스 자체에 종속이라 :: 로 접근한다.
	cs_static cs1;
	cs_static cs2;
	cs_static cs3;
}

class cs_ref
{
public:
	cs_ref(int x)
	{
		m_x = x;
	}
	int m_x;
	int get_x()
	{
		return m_x;
	}

	int& access_x()
	{
		return m_x;
	}

	void showX()
	{
		CString slog;
		slog.Format("%d", m_x);
		OutputDebugString(slog);
	}
};
void CDlg_SSib::OnBnClickedBtnRef()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cs_ref cs1(3);
	cs1.showX();

	int& rval = cs1.access_x();
	rval = 4;
	cs1.showX();

	int val = cs1.access_x();
	val = 5;
	cs1.showX();

	int lval = cs1.get_x();
	lval = 6;
	cs1.showX();

	cs1.access_x() = 7;
	cs1.showX();


	//int& llval = cs1.get_x(); //비const 참조에 대한 초기 값은 lvalue여야 합니다.   int에서 int& 로 변환할수 없습니다.
	//lval = 6;
	//cs1.showX();
}


class cs_c
{
private:
	void c_func() {};
	
};
// cs_a 에서 cs_b는 친구이니 private 를 사용하라는 friend 선언!!
class cs_a
{
private:
	friend class cs_b;
	friend void func() {};  //이거 신기하게도 func은 cs_a의 멤버함수가 아니다.

	 cs_c c;

	void private_func() { AfxMessageBox("a class private func call"); };
};

class cs_b
{
public:

	void  bfunc()
	{
		cs_a a;
		a.private_func();
		//a.c.c_func();
	}
};



void CDlg_SSib::OnBnClickedBtnFriend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cs_b b;
	b.bfunc();
}


class cs_over
{
public:
	cs_over(int num) {
		m_num = num;
	}
	int m_num;
	int operator +(int inum)
	{
		return m_num + inum;
	}

	int operator()(int inum)
	{
		return m_num + inum;
	}


};
//연산자 오버로딩
//(리턴 타입) operator(연산자) (연산자가 받는 인자)
void CDlg_SSib::OnBnClickedBtnOver()
{
	cs_over cs(3);

	int ival = cs + 5;
	ival = cs(10);
	std::cout << "ival =" + ival << std::endl;
}


template <typename T>
void CDlg_SSib::ShowVec(std::vector<T> strvec)
{
//	for(strvec.begin(); strvec.end(); )
	auto begin = strvec.begin();
	auto end = strvec.end();
	OutputDebugString("\r\n ------------------------------------- \r\n");
	CString stmp;

	int isize = strvec.size();
	auto val = strvec[9];
	 
	while (begin != end)
	{
		stmp.Format("\r\n%s\r\n", *begin);
		OutputDebugString(stmp);
		begin++;
	}

	OutputDebugString("\r\n ------------------------------------- \r\n");
	//for (const T& element : strvec)
	//{
	//	stmp.Format("\r\n [%s] \r\n", static_cast<const LPCTSTR>(element));
	//	OutputDebugString(stmp);
	//}
	/*
	 for (const T& element : vec) {
        TRACE(_T("%s\n"), static_cast<const LPCTSTR>(element)); // MFC TRACE 함수를 사용하여 출력합니다.
    }
	*/
}



void CDlg_SSib::OnBnClickedBtnLamda()
{
	CString stmp;

	std::vector<CString> strvec;
	strvec.push_back("일");
	strvec.push_back("이");
	strvec.push_back("삼");
	strvec.push_back("사");
	strvec.push_back("오");
	strvec.push_back("육");
	strvec.push_back("칠");
	strvec.push_back("팔");
	strvec.push_back("구");
	strvec.push_back("십");

	ShowVec(strvec);

	auto lamda = []( CString str) {
		if (str == "구")
			return true;
		else
			return false;
	};

	auto current = strvec.begin();
	auto end = strvec.end();
	while (true)
	{
		current = std::find_if(strvec.begin(), strvec.end(), lamda);
		
		stmp.Format("\r\n %s \r\n", *current);
		OutputDebugString(stmp);
		current++;
		
		//stmp.Format("%s", static_cast<LPCTSTR>(*end));
		stmp.Format("%s", *end);
		if (*current == stmp)
			break;

	}

}

extern "C"
{
	int add(int a, int b) { return a + b; }
	//double add(double a, double b) {};
}

namespace test
{
	void aa(int a, int b) {};
}

void aa(int a, int b)
{
	printf("%x %d %d" , &aa, a, b);
}

void CDlg_SSib::OnBnClickedBtnMangling()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	add(1, 2);
	add(1.1, 2.2);
	test::aa(1, 2);
	::aa(1, 2);
}

void CDlg_SSib::funcpt()
{
	printf("cdlg_ssib::funcpt");
}


struct st_test
{
	int a;
	int b;
	void(*print)(struct st_test*);
	void (*testfunc)();
//	void (*testfunc1)(void*);
};

void stprint(st_test* st)
{
	printf("%d %d", st->a, st->b);

}

void noparam()
{
	TRACE("no param");
}

void CDlg_SSib::OnBnClickedBtnStfunc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	st_test sttest{};
	sttest.a = 1;
	sttest.b = 2;

	st_test sttest1 = { 1,2 };
	sttest1.testfunc = noparam;
	sttest1.testfunc();
	
	//sttest1.testfunc1 = &CDlg_SSib::funcpt;
	//sttest.print(&sttest);  CDlg_SSib::OnBnClickedBtnLamda
	//sttest1.print(&sttest1);
//	sttest1.testfunc(CDlg_SSib::OnBnClickedBtnLamda);
	//sttest1.testfunc1();
	//(CDlg_SSib*(sttest1.testfunc1));
	
}

class cl_test
{
public:
	cl_test() { }
	~cl_test() { }
};


BOOL CDlg_SSib::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	cl_test cl;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#include "Vector.h"
void CDlg_SSib::OnBnClickedBtnTemplete()
{
	Vector<int> int_vec;
	int_vec.push_back(3);
	int_vec.push_back(2);

	m_slog.Format("\r\n%d", int_vec[0]);
	OutputDebugString(m_slog);
	m_slog.Format("\r\n%d", int_vec[1]);
	OutputDebugString(m_slog);
	

	Vector<CString> string_vec;
	string_vec.push_back("test 문자열");

	m_slog.Format("\r\n size = %s", string_vec[string_vec.size()-1]);
	OutputDebugString(m_slog);

	Vector<bool> bool_vec;
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);


	bool_vec.show_vector();

	auto at = tmax(5, 4);
	auto tmp = tmax('c', 4);
	TRACE("test");
}

class cs_sstatic
{
public:
	cs_sstatic() { m_istatic++; };
	static int m_istatic;

	void show_static() const
	{
		printf("%d", m_istatic);
	}
};
int cs_sstatic::m_istatic = 5;
void CDlg_SSib::OnBnClickedBtnTstatic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cs_sstatic css;
	css.show_static();
	cs_sstatic css1;
	css1.show_static();
}




template <typename T>
class _vt_Bsort {
	T* data;
	int capacity;
	int length;

public:
	typedef T value_type;

	//생성자
	_vt_Bsort(int n = 1) : data(new T[n]), capacity(n), length(0) {}

	//맨 뒤에 새로운 원소를 추가한다.
	void push_back(int s)
	{
		if (capacity <= length)
		{
			int* temp = new T[capacity * 2];
			for (int i = 0; i < length; i++)
			{
				temp[i] = data[i];
			}
			delete[] data;
			data = temp;
			capacity *= 2;
		}

		data[length] = s;
		length++;
	}

	//임의의 원소에 접근한다.
	T operator[](int i)
	{
		return data[i];
	}

	//x번째에 위치한 원소를 제거한다.
	void remove(int x)
	{
		for (int ii = x; ii < length; ii++)
			data[ii - 1] = data[ii];

		length--;
	}

	//현재 벡터의 크기를 구한다.
	int size() { return length; }

	//i번째 원소와 j번째 원소의 위치를 바꾼다.
	void swap(int i, int j)
	{
		T temp = data[i];
		data[i] = data[j];
		data[j] = temp;
	}

	~_vt_Bsort()
	{
		if (data)
			delete[] data;
	}


};

template <typename Cont>
void bubble_sort(Cont& cont)
{
	for (int i = 0; i < cont.size(); i++)
	{
		for (int j = i + 1; j < cont.size(); j++)
		{
			if (contp[i] > cont[j])
			{
				cont.swap(i, j);
			}
		}	
	}
}

template <typename Cont, typename Comp>
void bubble_sort(Cont& cont, Comp& comp)
{
	for (int i = 0; i < cont.size(); i++)
	{
		for (int j = i; j < cont.size(); j++)
		{
			if (!comp(cont[i], cont[j]))
			{
				cont.swap(i, j);
			}
		}
	}
}


struct Comp1
{
	bool operator()(int a, int b)
	{
		return a > b;
	}
};

struct Com2
{
	bool operator()(int a, int b)
	{
		return a < b;
	}
};

void CDlg_SSib::OnBnClickedBtnBbsort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vt_Bsort<int> int_vec;
	int_vec.push_back(4);
	int_vec.push_back(3);
	int_vec.push_back(2);
	int_vec.push_back(1);
	int_vec.push_back(5);

	CString stmp;
	for (int i = 0; i < int_vec.size(); i++)
	{
		stmp.Format("\r\n %d", int_vec[i]);
		OutputDebugString(stmp);
	}

	Comp1 comp1;
	bubble_sort(int_vec, comp1);


	for (int i = 0; i < int_vec.size(); i++)
	{
		stmp.Format("\r\n %d", int_vec[i]);
		OutputDebugString(stmp);
	}
}



class cs_pa
{
public:
	cs_pa() {};
	~cs_pa() {};

	//virtual void virfunc() = 0;

	virtual void func()
	{
		OutputDebugString("\r\n-----------------parent!!!");

	}
};

class cs_child : public cs_pa
{
public:
	cs_child() {};
	~cs_child() {};

	

	 void func()
	{
		 cs_pa::func();
		OutputDebugString("\r\n-----------------child!!!");

	}
};


class cs_2child : public cs_pa 
{
public:
	cs_2child() {};
	~cs_2child() {};



	void func()
	{
		cs_pa::func();
		OutputDebugString("\r\n-----------------2child!!!");
	}
};


void CDlg_SSib::OnBnClickedBtnInheri()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cs_2child cs;
	cs.func();
}


struct	_Ralert {
	CString code;
	int	stat{};				// DLL_ALERT.stat
	int	size{};				// data record count 
	std::unique_ptr<char[]> ptr[999]{};	// data record array

	~_Ralert() {
		for (int i = 0; i < 999; i++) {
			ptr[i].reset();
		}
	}
};

void CDlg_SSib::OnBnClickedBtnStdmap()
{
	std::map<CString, std::unique_ptr<struct _Ralert>> _mapRealData;
	_mapRealData.emplace("1234", nullptr);

	_mapRealData.emplace("1234", nullptr);
	_mapRealData.emplace("1234", nullptr);
	_mapRealData.emplace("1234", nullptr);
	_mapRealData.emplace("1234", nullptr);

}
