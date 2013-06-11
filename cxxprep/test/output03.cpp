
class [cn F] {
	struct [cn FH]{};
};

template <{class T}> class [cn TC] {};

class [cn C]
{
	$~[di C]({float $[di f]});
	$[di C]({int $[di i], int $<*>[di j]});

	$[cn C]::~[di C]({});
	$::[cn C]::~[di C]({});
	$::~[di F]({});
//	~F();
	$[di i];
	void $[di func]({}) {
		~[cn C]();
		~[cn F]();
		~G();
		::~[cn C]();
		::~[cn F]();
		::~[cn TC]<|int|>();
		[cn F]::~[cn FH]();
		~G();
		~[cn F];
		~::G;
		::~[cn F];
		~G;
	}
};

void $[di main]({})
{
}

