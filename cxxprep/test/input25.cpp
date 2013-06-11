namespace ns1
{
    template <class T> struct C
    {
        struct D;
        static int f( int ) 
            { return 0; }
    };
}

namespace ns2
{
    struct E 
    {
        E( int ) {}
    };
}

ns2::E g( ns1::C< ns2::E >::f(1) );
ns2::E h( ns1::C< ns2::E >::D );
