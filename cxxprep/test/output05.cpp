
class [cn C]
{
	class [cn S] { };
	template <{class T}> class [cn Tmpl] { class [cn I] {}; };
};

::[cn C]::[cn Tmpl]<|[cn C]::[cn S]|> $[di foo];

int $[di main]({}) 
{
	i = new (hi) ::[cn C]::[cn Tmpl]<|[cn C]::[cn Tmpl]<|int|>|>::[cn I] [[22.2]];
	i = ::new (this) int;

	laksdjf = delete new int[[22.2]];
	laksdjf = delete[[]] sizeof([cn C]);
	laksdjf = delete[[]] ++sizeof (::new (i--) int <*><&><*>[[i++]][[1.1]](foo,bar)) ;
	laksdjf = delete[[]] ++sizeof (::new (i--) (int (|<*>|)({unsigned, int}))(foo,bar)) ;
}
