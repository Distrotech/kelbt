
template <{class T}> class [cn C] {};
void $[di main]({int $[di argc], char $<*>[di argv][[]]})
{
	[cn C]<| (new [cn C]<| new int[[i>>1.1]] |> >> i(k--)) |> $[di c];
	[cn C]<| (new [cn C]<|int|>) |> $[di c];
	[cn C]<| (new [cn C]<|int|> > 0.0) |> $[di c];
	[cn C]<| (new [cn C]<|int|> < 0.0) |> $[di c];
	[cn C]<|int|> $[di foobar]({int $[di i], int <*><&>::<*>});

	int $[di i] = j << 1.1;
	int $[di i] = j << 1.1 < 0.0;
	int $[di i] = j >> 1.1;
	int $[di i] = j >> 1.1 > 0.0;

	int $[di i] = ::j + 1.1 > 0.0;
	int $[di i] = j ++ + 1.1;
	int $[di i] = j +++ 1.1;
	int $[di i] = (j ++)+ 1.1 - ::[cn C]::getNum( static_cast<|int<*>|>(0.0) );
}
