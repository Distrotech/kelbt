
class C
{
	class S { };
	template <class T> class Tmpl { class I {}; };
};

::C::Tmpl<22.2--> foo;
::C::Tmpl<++static_cast<int*>(new C::Tmpl<int>(foo))--> foo;
C::Tmpl<::C::Tmpl<abbacadabb.f(a)->foob(1.1)>> foo;

int main() 
{
}
