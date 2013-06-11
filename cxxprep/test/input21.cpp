

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

struct C<int,int,int>
{
	struct G { static int i; };
};

template <class T, class U, class V> int C<T, U, V>::D::i = 0;
template <class T, class U> int C<T, U, int>::E::i = 0;
template <class T> int C<T, int, int>::F::i = 0;
int C<int, int, int>::G::i = 0;

int main()
{
	return 0;
}


