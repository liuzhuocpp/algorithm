
#include <bits/stdc++.h>

#include <lz/debug.h>
#include <lz/sparse_matrix.h>
using namespace lz;
using namespace std;


int main()
{
    SparseMatrix<int> a(100, 100);
    a.setValue(0, 10, 222);
    a.setValue(1, 0, 6666);
    a.setValue(1, 10, 77777);
    a.setValue(99, 99, 999999);

//    a.setValue(2, 10, 5555);
//    a.setValue(2, 10, 33333);
//    a.setValue(2, 10, 7777);
//    a.setValue(2, 11, 8888);
//    a.setValue(3, 10, 999);

//    a.setValue(3, 10, 5555);

    cout << a.getValue(0, 10).first << endl;
    cout << a.getValue(1, 0).first << endl;
    cout << a.getValue(1, 10).first << endl;
    cout << a.getValue(99, 99).first << endl;



//    cout << a.getValue(2, 11).first << endl;
//    cout << a.getValue(3, 11).first << endl;
//    cout << a.getValue(3, 9).first << endl;


	return 0;
}
