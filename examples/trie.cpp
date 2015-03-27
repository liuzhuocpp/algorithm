#include <iostream>
#include <cstdio>
#include <cstring>

#include "trie.h"
// hdu 2222
using namespace std;
using namespace lz;
const int N = 1e6 + 9;
char s[N];
int main()
{
	int T;
	scanf("%d", &T);
	while(T --)
	{
		AcAutomaton<26> ac;
		int n;
		scanf("%d", &n);
		for(int i = 0; i < n; ++ i)
		{
			scanf("%s", s);
			int l = strlen(s);
			for(int i = 0; i < l; ++ i) s[i] = s[i] - 'a';
			ac.insert(s, s + l);
		}
		ac.build();

		scanf("%s", s);
		int l = strlen(s);
		for(int i = 0; i < l; ++ i) s[i] = s[i] - 'a';

		// for(int i = 0; i < l; ++ i)
		// {
		// 	cout << int(s[i]) << endl;
		// }


		
		int ans = ac.query(s, s + l);


		// cout << "FF " << endl;
		printf("%d\n", ans);





	}




	return 0;
}