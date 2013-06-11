
template <class T> class C {};
void main(int argc, char *argv[])
{
	C< (new C< new int[i>>1.1] > >> i(k--)) > c;
	C< (new C<int>) > c;
	C< (new C<int> > 0.0) > c;
	C< (new C<int> < 0.0) > c;
	C<int> foobar(int i, int *&::*);

	int i = j << 1.1;
	int i = j << 1.1 < 0.0;
	int i = j >> 1.1;
	int i = j >> 1.1 > 0.0;

	int i = ::j + 1.1 > 0.0;
	int i = j ++ + 1.1;
	int i = j +++ 1.1;
	int i = (j ++)+ 1.1 - ::C::getNum( static_cast<int*>(0.0) );
}
