
template <class T, class U, class V> struct C
{
	struct D { static int i; };
};

template <class T, class U> struct C<T,U,int>
{
	struct E { static int i; };
};

template <class T> struct C<T,int,int>
{
	struct F { static int i; };
};

int main()
{
	C<int,int,int>::F foo;
	return 0;
}


