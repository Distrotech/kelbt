
class [cn C] {

};

void $[di main]({int $[di argc], char $<*>[di argv][[]]})
{
	[cn C] $(|<*>[di foo]|)({int});
	[cn C](foo, bar)(int(1));
	int $[di i] = sizeof( int() );
	[cn C] $(|<*>[di foo]|);
	int $[di i] = int(1);
	int $[di i] = (int)(1);
	int $[di i] = (int<*>)(1);
	int $[di i] = (int())(1);

	int $[di i] = 1 + 2 + int(1*3) + typeid(int (a)*1*int((*a),1));
}
