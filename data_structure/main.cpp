#include <iostream>
#include <cstdio>
#include <cstring>

#include "segment_tree.h"

using namespace std;



const int N = 50009;
int arr[N];
SegmentTree<int> seg;
int main()
{
//    freopen("in","r",stdin);
    int t;
    scanf("%d",&t);
    for(int cot=1;cot<=t;++cot)
    {
        printf("Case %d:\n",cot);
        int n;
        scanf("%d",&n);
        for(int i=0;i<n;++i) scanf("%d",&arr[i]);

        // make_tree(1,1,n);
        seg.build(arr, n);
        char s[100];
        while(scanf("%s",s)==1 && s[0]!='E')
        {
            int a,b,v;
            if(s[0]=='A')
            {
                scanf("%d%d",&a,&v);
                a --;            
                // modify(1,a,v);
                
                seg.modify(a, seg.query(a, a) + v);
            }
            if(s[0]=='S')
            {
                scanf("%d%d",&a,&v);
                a --;
                // modify(1,a,-v);
                seg.modify(a, seg.query(a, a)-v);
            }
            if(s[0]=='Q')
            {
                scanf("%d%d",&a,&b);
                a--;
                b --;
                // printf("%d\n",query(1,a,b));
                printf("%d\n",seg.query(a, b));
            }
        }
    }
    return 0;
}
