/*
 * TXL is able to parse the first function call correctly because f < int
 * fails as a relational expression, however f < C succeeds as a relational
 * expression and so the entire expression gets incorrectly parsed as the
 * relational expression f < C > ( 1 ). To parse the statement correctly the
 * parser needs to know that f is a template id and that we should immediately
 * be trying to parse a template argument list.
 */

class C
{
};

template <class T> void f(int)
{
}

int main()
{
	f<int>(1);
	f<C>(1);
	return 0;
}
