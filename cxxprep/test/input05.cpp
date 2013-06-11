
class C
{
	class S { };
	template <class T> class Tmpl { class I {}; };
};

::C::Tmpl<C::S> foo;

int main() 
{
	i = new (hi) ::C::Tmpl<C::Tmpl<int>>::I [22.2];
	i = ::new (this) int;

	laksdjf = delete new int[22.2];
	laksdjf = delete[] sizeof(C);
	laksdjf = delete[] ++sizeof (::new (i--) int *&*[i++][1.1](foo,bar)) ;
	laksdjf = delete[] ++sizeof (::new (i--) (int (*)(unsigned, int))(foo,bar)) ;
}
