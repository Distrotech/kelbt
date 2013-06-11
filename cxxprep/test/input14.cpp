
template <class T> class string {};

template <class T = string<int>, template <class S> class TT = hi> class Other 
{
	T id;
	class FOO {};
	TT<T> *foobar(int i, char j, T&);	
};


class C
{
	
};

Other<int> i = 22;
C c = 33;

Other<int>::FOO j = 33;

template <class T> class D
{
	void foo(int);
} FOO;

template <class T> void bar(int i)
{
	int j = 0;
	i = j - 0 - D<int>();
}

int main(int argc, char *argv[])
{
}
