
template <{class T}> struct [cn C] {};

struct [cn C]<|char|>
{
	struct [cn D] { int $[di i]; };
};

typedef char $[di Char];
typedef [cn C]<|[cn Char]|> $[di Cchar];

int $[di main]({})
{
	[cn Cchar]::[cn D] $[di d];
	return 0;
}


