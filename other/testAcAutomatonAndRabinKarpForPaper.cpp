#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
#include <cstring>
#include <ctime>
#include <unordered_set>
#include <unordered_map>
#include <set>

#define nullptr 0
using namespace std;

template<int N>
struct AcAutomaton
{

    struct Node
    {
        int tail;
        Node *next;

        Node *son[N];
        Node():tail(-1), next(nullptr)
        {
            for(int i = 0; i < N; ++ i)
                son[i] = nullptr;
        }
    };
    Node *root;
    AcAutomaton():root(new Node()) {

    }

    template<typename RandomIterator>
    void insert(RandomIterator first, RandomIterator end, int id)
    {
        RandomIterator s = first;
        int n = end - first;

        Node *top = root;
        for(int i = 0; i < n; ++ i)
        {
            if(top->son[s[i]] == nullptr)
            {
                top->son[s[i]] = new Node();
            }
            top = top->son[s[i]];
        }
        top->tail = id;
    }
    void build() // bfs
    {
        queue<Node*> q;
        root->next = nullptr;
        q.push(root);
        while(!q.empty())
        {
            Node *u = q.front();
            q.pop();
            for(int i = 0; i < N; ++ i)
            {
                if(u->son[i] != nullptr)
                {
                    if(u == root)
                    {
                        u->son[i]->next = root;
                    }
                    else
                    {
                        Node* p;
                        for(p = u->next; p != nullptr; p = p->next)
                        {
                            if(p->son[i] != nullptr)
                            {
                                u->son[i]->next = p->son[i];
                                break;
                            }
                        }
                        if(p == nullptr)
                        {
                            u->son[i]->next = root;
                        }
                    }
                    q.push(u->son[i]);
                }

//                if(u->son[i] == nullptr)
//                {
//                    if(u == root) u->son[i] = root;
//                    else u->son[i] = u->next->son[i];
//                }
//                else
//                {
//                    if(u == root) u->son[i]->next = root;
//                    else u->son[i]->next = u->next->son[i];
//                    q.push(u->son[i]);
//                }
            }
        }
    }
    template<typename RandomIterator>
    tuple<int, int, int> query(RandomIterator first, RandomIterator end)
    {
        RandomIterator s = first;
        int n = end - first;

        Node *u = root;
        tuple<int, int, int> ans(-1, 0, -1);

        for(int i = 0; i < n; ++ i)
        {

            while(u != root && u->son[s[i]] == nullptr) u = u->next;

            u = u->son[s[i]];
            if(u == nullptr) u = root;

            if(u->tail != -1)
            {
                std::get<0>(ans) = u->tail;
                std::get<1>(ans) ++;
                std::get<2>(ans) = i;

            }


        }
        return ans;
    }
    ~AcAutomaton()
    {
    }


};











vector<vector<int> > paternStrings;
vector<int> textString;

// 产生长度为n，每个字符为0到c - 1的整数序列
vector<int> generateIntergeSeq(int n, int c)
{
    vector<int> ans;
    while(n --)
    {
        int x = rand() % c;
        ans.push_back(x);
    }
    return ans;
}

// 每个字符都是0到c-1
void setPaternAndText(int c)
{
    paternStrings.clear();

    set<vector<int> > flag;
    int paternStringNumber = 100;

    int n = 100; // text串长度
    int m = 3; // 每个pattern长度
    for(int i = 0; i < paternStringNumber; ++ i)
    {
        vector<int> cnt;
        while(1)
        {
            cnt = generateIntergeSeq(m, c);
            if(!flag.count(cnt))
            {
                flag.insert(cnt);
                break;
            }
        }

        paternStrings.push_back(cnt);
    }


    int cot = 0;
    while(1)
    {
//        cout << "cot: " << cot ++ << endl;
        textString = generateIntergeSeq(100, c);
        int containNumber = 0; // 大串包含模式串的数目
        for(int i = 0; i < n - m; ++ i)
        {
            vector<int> tmp(textString.begin() + i, textString.begin() + i + m);
            if(flag.count(tmp))
            {
                containNumber ++;
            }
        }
        if(containNumber == 1) break;
    }


}

#define OUT_FUNCTION_NAME cout << string(30, '-') << __FUNCTION__ << endl;

template<int C>
void testAcAutomaton(int runBuildNumber, int runQueryNumber)
{
    OUT_FUNCTION_NAME

    double firstTime, secondTime, thirdTime;


    firstTime = clock();
    AcAutomaton<C> ac;
    while(runBuildNumber -- )
    {
        ac = AcAutomaton<C>();
        for(int i = 0; i < int(paternStrings.size()); ++ i)
        {
            ac.insert(paternStrings[i].begin(), paternStrings[i].end(), i);
        }
        ac.build();
    }


    secondTime = clock();
    int foundNumber;
    int foundId;
    int foundPos;

    while(runQueryNumber --)
    {
        foundId = -1;
        foundNumber = 0;
        foundPos = -1;
        auto ans = ac.query(textString.begin(), textString.end());
        foundId = std::get<0>(ans);
        foundNumber = std::get<1>(ans);
        foundPos = std::get<2>(ans);
    }

    thirdTime = clock();

    cout << "build time: " << (secondTime - firstTime)  << "ms" << endl;
    cout << "query time: " << (thirdTime - secondTime)  << "ms" << endl;

    cout << "foundId: " << foundId << endl;
    cout << "foundNumber: " << foundNumber << endl;
    cout << "foundPos: " << foundPos << endl;

}

template<int d>
void testRabinKarp(int runBuildNumber, int runQueryNumber)
{
    OUT_FUNCTION_NAME

    using HashType = unsigned long long;
    int n = textString.size();
    int m = paternStrings[0].size();// 每个小串的长度


//    unordered_map<HashType, int> hashTable;
    double firstTime, secondTime, thirdTime;

    vector<int> noHashTable;

    firstTime = clock();
    HashType t;
    HashType h;
    while(runBuildNumber -- )
    {
//        hashTable.clear();
        noHashTable.assign(2000, -1);
        for(int i = 0; i < int(paternStrings.size()); ++ i)
        {
            HashType ph = 0;
            for(int x: paternStrings[i])
            {
                ph = ph * d + x;
            }
//            hashTable.insert(make_pair(ph, i));
            noHashTable[ph] = i;
        }

        h = 1;
        for(int i = 1; i < m; ++ i) h = h * d;

    }






    secondTime = clock();
    int foundId, foundNumber, foundPos;
    while(runQueryNumber -- )
    {
        foundId = -1;
        foundPos = -1;
        foundNumber = 0;
        t = 0;
        for(int i = 0; i < m; ++ i)
        {
            t = t * d + textString[i];
        }
        for(int i = 0; i < n - m; ++ i)
        {
            if(noHashTable[t] != -1)
            {
                foundId =  noHashTable[t] ;
                foundNumber ++;
                foundPos = i+ m - 1;
            }

            if(i + m < n)
            {
//                t = t / d * d + textString[i + m];
                t = (t - textString[i] * h) * d + textString[i + m];
            }

//            if(hashTable.count(t))
//            {
//                foundId = hashTable[t];
//                foundNumber ++;
//                foundPos = i;
//                break;
//            }
//            if(i + m < n)
//            {
//                t = (t - textString[i] * h) * d + textString[i + m];
//            }
        }

    }


    thirdTime = clock();






    cout << "build time: " << (secondTime - firstTime)  << "ms" << endl;
    cout << "query time: " << (thirdTime - secondTime)  << "ms" << endl;
    cout << "foundId: " << foundId <<  endl;
    cout << "foundNumber: " << foundNumber <<  endl;
    cout << "foundPos: " << foundPos <<  endl;


}

int main()
{
    constexpr int C = 10;

    srand(time(NULL));

    setPaternAndText(C);



    int runBuildNumber = 10000;
    int runQueryNumber = 100000;
    testAcAutomaton<C>(runBuildNumber, runQueryNumber);

    testRabinKarp<C>(runBuildNumber, runQueryNumber);





    return 0;
}
