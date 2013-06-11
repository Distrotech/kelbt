


class [cn G];
class [cn H] {};

template <{class T}> class [cn I];
template <{class T}> class [cn J] {};

template <{class}> class [cn I]<|int|> {

};

template <{class}> void $[di foo]({});
template <{class}> void $[di bar]({})
{
}

int $[di main]({})
{
	foo<|int|>(hi);
	bar<|int, (i<1), 1|>(hi);
	kar < new int<*>[[22]];
}
