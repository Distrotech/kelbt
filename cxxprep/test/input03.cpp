
class F {
	struct FH{};
};

template <class T> class TC {};

class C
{
	~C(float f);
	C(int i, int *j);

	C::~C();
	::C::~C();
//	::~F();
//	~F();
	i;
	void func() {
		~C();
		~F();
		~G();
		::~C();
		::~F();
		::~TC<int>();
		F::~FH();
		~G();
		~F;
		~::G;
		::~F;
		~G;
	}
};

void main()
{
}

