


struct Ans
{
    int a, b, c, d;
    long long e, f;
};

int a;
int b; int c; int d; int e; int h; int g;

struct Ans f()
{
    struct Ans ans;
    ans.a = 188888;
    ans.b = 288888;
    ans.c = 388888;
    ans.d = 488888;
    ans.e = 588888;
    ans.f = 688888;

    return ans;
}




void ggg()
{

    struct Ans ans;
    ans = f();

    ans.a = 999;
    ans.b = 1999;
    ans.c = 2999;
    ans.d = 3999;
    ans.e = 22222;
    ans.f = 55555555;

}


struct Ans f2()
{
    struct Ans a;
    return a;
}
void dddd()
{
    f();
//    struct Ans ans;
//    ans = f();
    ggg();
    f2();



}
