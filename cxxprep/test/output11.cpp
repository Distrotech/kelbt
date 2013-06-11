
struct [cn C] {
	
	struct [cn B] { int $[di i]; };

	struct [cn D] :+ public [cn B] { int $[di i];};
};

int $[di main]({int $[di argc], char $<*>[di argv][[]]})
{
	::[cn C]::[cn D] $<*>[di p] = new [cn C]::[cn D];
	p->[cn B]::[cn D]::[cn FOOBAR]::i = 1;
	p.[cn BB]::SLDFJ = 3;
	int $[di i] = cout << p->[cn B]::i << endl;
}
