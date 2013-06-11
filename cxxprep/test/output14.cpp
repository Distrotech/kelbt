
template <{class T}> class [cn string] {};

template <{class T = [cn string]<|int|>, template <{class S}> class TT = hi}> class [cn Other] 
{
	[cn T] $[di id];
	class [cn FOO] {};
	[cn TT]<|[cn T]|> $<*>[di foobar]({int $[di i], char $[di j], [cn T]<&>});	
};


class [cn C]
{
	
};

[cn Other]<|int|> $[di i] = 22;
[cn C] $[di c] = 33;

[cn Other]<|int|>::[cn FOO] $[di j] = 33;

template <{class T}> class [cn D]
{
	void $[di foo]({int});
} $[di FOO];

template <{class T}> void $[di bar]({int $[di i]})
{
	int $[di j] = 0;
	i = j - 0 - [cn D]<|int|>();
}

int $[di main]({int $[di argc], char $<*>[di argv][[]]})
{
}
