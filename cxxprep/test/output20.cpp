
struct [cn C]
{
	char $[di doit]({}) { return 'c'; }
};

template <{typename T, [cn T] $(|[cn C]::<*>[di f]|)({})}> [cn T] $[di g]({[cn C]$<*>[di c]})
{
	return (c->*f)();
}

int $[di main]({})
{
	[cn C] $[di c];
	g<|char, &[cn C]::doit|>(&c);
	return 0;
}
