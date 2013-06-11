
class [cn C]
{
	class [cn S] { };
	template <{class T}> class [cn Tmpl] { class [cn I] {}; };
};

::[cn C]::[cn Tmpl]<|22.2--|> $[di foo];
::[cn C]::[cn Tmpl]<|++static_cast<|int<*>|>(new [cn C]::[cn Tmpl]<|int|>(foo))--|> $[di foo];
[cn C]::[cn Tmpl]<|::[cn C]::[cn Tmpl]<|abbacadabb.f(a)->foob(1.1)|>|> $[di foo];

int $[di main]({}) 
{
}
