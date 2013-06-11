

int f( ... );
int f( int a ... );
int f( int a, ... );

int f( int(1) );

class C;

C c(hello + 1);


class D 
{
	void ((*d))(int i = 0, C &);
};

template <class> class F;

int main() 
{
	void (*d)(int i = 0, C &);
}

template< template <class T = F< (1+2) >> class, class> class G;


