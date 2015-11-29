



struct A
{

    void f()
    {
//        cout << "A" << endl;
    }
};

struct B:
//		virtual
A
{

};

struct C:
//		virtual
		A
{

};

struct D:  B,  C
{
    using B::A::f;

};


void test()
{
	D d;
	d.f();
}











