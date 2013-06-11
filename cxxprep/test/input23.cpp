
template <class T> struct C {};

struct C<char>
{
	struct D { int i; };
};

typedef char Char;
typedef C<Char> Cchar;

int main()
{
	Cchar::D d;
	return 0;
}


