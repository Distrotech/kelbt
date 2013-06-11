
template <int i> class C {};

void f()
{
	int i = 1 + C< 1 > ( 1 ) > 2;
	int i = 1 + C< 1 > ( 1 ) > ();
}
