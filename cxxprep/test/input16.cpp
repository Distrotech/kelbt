
typedef int T1;
T1 i;

class C {
	typedef int T2;
	T2 foo;

	int f()
	{
		typedef int T3;
		T3 foo;
	}
};
