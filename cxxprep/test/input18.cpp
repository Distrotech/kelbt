

class C {
};

template <class T> class D {
};

typedef int I;
typedef int ((*T))(int, char (*)(int));

class C;

C c;

I i;
T t;


namespace NS {
	using C;
	using D;
	using T;
};

NS::T i = unsigned int(77) + NS::T(44) - NS::D<1>(a + b < c);

void f () {
	class G;
}
