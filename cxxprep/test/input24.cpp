class C
{
	struct S;
	void f()
	{
		S w(int (a));
		S x(int ());
		S y((int)a);
		S z = int(a);
	}
};
