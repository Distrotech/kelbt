
struct C {
	
	struct B { int i; };

	struct D : public B { int i;};
};

int main(int argc, char *argv[])
{
	::C::D *p = new C::D;
	p->B::D::FOOBAR::i = 1;
	p.BB::SLDFJ = 3;
	int i = cout << p->B::i << endl;
}
