namespace ns {
	namespace ns { }
	class c1 {
		class c2 {
		};
		template <class> static int i, ns::k;
		int j;
	};

	c1::c2 id;
	namespace ns2 {
	}
}

namespace ns {
	namespace ns2 { 
		class C {} name; 
		}
}

template <class, class A> class C
{

};

template <class, class A> class D
{
	template <class> static int l;
	class F {};
};

class Arg{};

int ns::c1::i<Arg>;

::C< Arg> foo;
ns::ns2::C D<Arg>::*const *&::C<Arg>::*i, *const v; 
template <class> static int m;
D<Arg>::F fi;

int D<Arg>::l<Arg>;

int *(*foo);
int foo()();

unsigned long int ::C<Arg>::bar;
unsigned long int ::ns::ns2::C::b;

class{int(*sd)()()[];};

template <class C> class F{};

template <class> void foo(int *);
template <class> void doo(int &*const b);

namespace N {

template <class T> class G {};
template <class T> void bar();

}

class N::G<int *const*&(*(*))>
{
	public: int i;
};


class N::D_HiddenInN
{
	public: int i;
};

void f( int (*)(N::D_HiddenInN i, int, char c) );
void f( int (*d)(N::D_HiddenInN i, int, char c) );

int int;

static g;

class F {};
class I {};

class H {
	H (void*, H (*));
	inline H (H H);
	inline H (int H);
	::H (*f);
	inline static void H::H();
	
	const i;
	inline e(H H, const i);
	class G {
		class I {
			class N {};	
			class C {};
		};
	};
};

H::H();

class I {

};

H (*I)()()();

H::H(int);

N::D_HiddenInN d_hidden;

