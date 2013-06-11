

template <{class T, class U, class V}> struct [cn C]
{
	struct [cn D] { static int $[di i]; };
};

template <{class T, class U}> struct [cn C]<|[cn T],[cn U],int|>
{
	struct [cn E] { static int $[di i]; };
};

template <{class T}> struct [cn C]<|[cn T],int,int|>
{
	struct [cn F] { static int $[di i]; };
};

struct [cn C]<|int,int,int|>
{
	struct [cn G] { static int $[di i]; };
};

template <{class T, class U, class V}> int $[cn C]<|[cn T], [cn U], [cn V]|>::[cn D]::[di i] = 0;
template <{class T, class U}> int $[cn C]<|[cn T], [cn U], int|>::[cn E]::[di i] = 0;
template <{class T}> int $[cn C]<|[cn T], int, int|>::[cn F]::[di i] = 0;
int $[cn C]<|int, int, int|>::[cn G]::[di i] = 0;

int $[di main]({})
{
	return 0;
}


