namespace [nn ns] {
	namespace [nn ns] { }
	class [cn c1] {
		class [cn c2] {
		};
		template <{class}> static int $[di i], $[nn ns]::[di k];
		int $[di j];
	};

	[cn c1]::[cn c2] $[di id];
	namespace [nn ns2] {
	}
}

namespace [nn ns] {
	namespace [nn ns2] { 
		class [cn C] {} $[di name]; 
		}
}

template <{class, class A}> class [cn C]
{

};

template <{class, class A}> class [cn D]
{
	template <{class}> static int $[di l];
	class [cn F] {};
};

class [cn Arg]{};

int $[nn ns]::[cn c1]::[di i]<|[cn Arg]|>;

::[cn C]<| [cn Arg]|> $[di foo];
[nn ns]::[nn ns2]::[cn C] $[cn D]<|[cn Arg]|>::<*>const <*><&>::[cn C]<|[cn Arg]|>::<*>[di i], $<*>const [di v]; 
template <{class}> static int $[di m];
[cn D]<|[cn Arg]|>::[cn F] $[di fi];

int $[cn D]<|[cn Arg]|>::[di l]<|[cn Arg]|>;

int $<*>(|<*>[di foo]|);
int $[di foo]({})({});

unsigned long int $::[cn C]<|[cn Arg]|>::[di bar];
unsigned long int $::[nn ns]::[nn ns2]::[cn C]::[di b];

class{int$(|<*>[di sd]|)({})({})[[]];};

template <{class [cn C]}> class [cn F]{};

template <{class}> void $[di foo]({int <*>});
template <{class}> void $[di doo]({int $<&><*>const [di b]});

namespace [nn N] {

template <{class T}> class [cn G] {};
template <{class T}> void $[di bar]({});

}

class [nn N]::[cn G]<|int <*>const<*><&>(|<*>(|<*>|)|)|>
{
	public:~ int $[di i];
};


class [nn N]::[cn D_HiddenInN]
{
	public:~ int $[di i];
};

void $[di f]({ int (|<*>|)({[nn N]::[cn D_HiddenInN] $[di i], int, char $[di c]}) });
void $[di f]({ int $(|<*>[di d]|)({[nn N]::[cn D_HiddenInN] $[di i], int, char $[di c]}) });

int int;

static $[di g];

class [cn F] {};
class [cn I] {};

class [cn H] {
	$[di H] ({void<*>, [cn H] (|<*>|)});
	inline $[di H] ({[cn H] $[di H]});
	inline $[di H] ({int $[di H]});
	::[cn H] $(|<*>[di f]|);
	inline static void $[cn H]::[di H]({});
	
	const $[di i];
	inline $[di e]({[cn H] $[di H], const $[di i]});
	class [cn G] {
		class [cn I] {
			class [cn N] {};	
			class [cn C] {};
		};
	};
};

$[cn H]::[di H]({});

class [cn I] {

};

[cn H] $(|<*>[di I]|)({})({})({});

$[cn H]::[di H]({int});

[nn N]::[cn D_HiddenInN] $[di d_hidden];

