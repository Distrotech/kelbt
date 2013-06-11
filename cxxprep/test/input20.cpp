
struct C
{
	char doit() { return 'c'; }
};

template <typename T, T (C::*f)()> T g(C*c)
{
	return (c->*f)();
}

int main()
{
	C c;
	g<char, &C::doit>(&c);
	return 0;
}
