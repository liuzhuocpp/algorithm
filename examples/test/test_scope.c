



int a1, a2, a3, a4, a5;

int f(int i)
{
    int a = 22222;
    int haha = 33333;
    while(i > 0)
    {
        int a = 555555;
        haha += a;
    }
    a += haha;
    return a;
}


int g(int i)
{
   return f(i);
}
