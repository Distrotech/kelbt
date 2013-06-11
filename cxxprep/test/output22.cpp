
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

int $[di main]({})
{
	[cn C]<|int,int,int|>::[cn F] $[di foo];
	return 0;
}


