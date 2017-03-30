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
#include <cassert>

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
    AcAutomaton():root(nullptr) {

    }

    AcAutomaton(const AcAutomaton &other) = delete;

    void clear()
    {
        if(root == nullptr) return ;

        // 当前和父亲
        std::queue<Node*> q;
        q.push(root);
        vector<Node*> all;

        while(!q.empty())
        {
            Node* u = q.front();
            all.push_back(u);
            q.pop();
            for(int i = 0; i < N; ++ i)
            {
                if(u->son[i] != nullptr)
                {
                    q.push(u->son[i]);
                }
            }
        }
        for(int i = 0; i < all.size(); ++ i)
        {
            delete all[i];
        }
        root = nullptr;

    }

    void build(vector<vector<int>>::iterator first,  vector<vector<int>>::iterator last)
    {
        clear();
        root = new Node();
        for(int i = 0; i < last - first; ++ i)
        {
            insert(first[i].begin(), first[i].end(), i);
        }
        build();
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
//        cout << "out " << endl;
        clear();


    }


};

// 进制位
template<int C>
struct BruteForce
{

    vector<vector<int>>::iterator patternStringsBegin;
    vector<vector<int>>::iterator patternStringsEnd;

    void clear()
    {

    }


    void build(vector<vector<int>>::iterator first, vector<vector<int>>:: iterator last)
    {
        patternStringsBegin = first;
        patternStringsEnd = last;
    }

    template<typename Iterator>
    auto query(Iterator first, Iterator last)
    {
        tuple<int, int, int> ans(-1, 0, -1);

        int n = last - first;
        int m = patternStringsBegin[0].size();
        for(int i = 0; i <= n - m; ++ i)
        {
            for(int j = 0; j < patternStringsEnd - patternStringsBegin; ++ j)
            {
                if(std::equal(patternStringsBegin[j].begin(), patternStringsBegin[j].end(), first + i))
                {
                    get<0>(ans) = j;
                    get<1>(ans) ++;
                    get<2>(ans) = i + m - 1;
                }
            }
        }
        return ans;
    }
};


// 进制位
template<int d>
struct RabinKarp
{
    vector<int> noHashTable;
    int h;
    int m;

    void clear()
    {

    }

//    void build(vector<vector<int>>& ps)
    void build(vector<vector<int>>::iterator first, vector<vector<int>>:: iterator last)
    {
        m = first[0].size();
        h = 1;
        for(int i = 1; i < m; ++ i) h = h * d;
        noHashTable.assign(2000, -1);

        for(int i = 0; i < last - first; ++ i)
        {
            insert(first[i].begin(), first[i].end(), i);
        }
    }



    template<typename Iterator>
    void insert(Iterator first, Iterator last, int id)
    {
        m = last - first;// 每个小串的长度
        assert(m == (last - first));
        int ph = 0;
        for(int i = 0; i < m; ++ i)
        {
            int x = first[i];
            ph = ph * d + x;
        }
        noHashTable[ph] = id;
    }

    template<typename Iterator>
    tuple<int, int, int> query(Iterator first, Iterator last)
    {
        int n = last - first;
        tuple<int, int, int> ans(-1, 0, -1);
        int t = 0;
        for(int i = 0; i < m; ++ i)
        {
            t = t * d + first[i];
        }
        for(int i = 0; i < n - m; ++ i)
        {
            if(noHashTable[t] != -1)
            {
                std::get<0>(ans) = noHashTable[t];
                std::get<1>(ans)++;
                std::get<2>(ans) = i + m - 1;
            }

            if(i + m < n)
            {
                t = (t - first[i] * h) * d + first[i + m];
            }
        }
        return ans;



    }
};



template<int N>
struct AcAutomatonAndRabinKarp
{
    AcAutomaton<N> ac;
    RabinKarp<N> rabinKarp;

    void clear()
    {
        ac.clear();
        rabinKarp.clear();
    }

    void build(vector<vector<int>>::iterator first, vector<vector<int>>::iterator last)
    {
        int patternsNumber = last - first;
        rabinKarp.build(first, first + patternsNumber / 5 * 4);
        ac.build(first + patternsNumber / 5 * 4, last);

    }
    template<typename Iterator>
    tuple<int, int, int> query(Iterator first, Iterator last)
    {
        auto rabinAns = rabinKarp.query(first, last);
        if(get<0>(rabinAns) != -1)
        {
            return rabinAns;
        }
        return ac.query(first, last);
    }

};



template<int N>
struct MultiKMP
{
    template<typename RandomIterator>
    static vector<int> computePrefix(RandomIterator first, RandomIterator end)
    {
        int n = end - first;
        RandomIterator s = first;
        vector<int> p(n);
        for(int k = p[0] = -1, i = 1; i < n; ++ i)
        {
            while(k != -1 && !(s[k + 1] == s[i]) ) k = p[k];
            if(k != -1) k ++;
            else if(s[0] == s[i]) k = 0;
            p[i] = k;
        }
        return std::move(p);
    }

    template<typename RandomIteratorS, typename RandomIteratorT>
    static RandomIteratorT search(const vector<int> &p, RandomIteratorS sfirst, RandomIteratorS send,
                      RandomIteratorT tfirst, RandomIteratorT tend)
    {
        RandomIteratorS s = sfirst;
        RandomIteratorT t = tfirst;
        int ns = send - sfirst, nt = tend - tfirst;
        for(int i = -1, j = 0; j < nt; ++ j)
        {
            while(i != -1 && !(s[i + 1] == t[j]) ) i = p[i];
            if(i != -1) i ++;
            else if(s[0] == t[j]) i = 0;

            if(i == ns - 1) return t + j - i;
        }
        return tend;
    }


    vector<vector<int>>::iterator patternStringBegin, patternStringEnd;
    vector<vector<int>> nexts;


    void build(vector<vector<int>>::iterator first, vector<vector<int>>::iterator last)
    {
        patternStringBegin = first;
        patternStringEnd = last;

        int n = last - first;
        nexts.assign(n, vector<int>());
        for(int i = 0; i < n; ++ i)
        {
            nexts[i] = computePrefix(first[i].begin(), first[i].end());
        }
    }

    template<typename Iterator>
    tuple<int, int, int> query(Iterator first, Iterator last)
    {
        int n = last - first;
        tuple<int, int, int> ans(-1, 0, -1);

        for(int j = 0; j < nexts.size(); ++ j)
        {
            auto foundPosIt = search(nexts[j], patternStringBegin[j].begin(), patternStringBegin[j].end(), first, last);
            if(foundPosIt != last)
            {
                get<0>(ans) = j;
                get<1>(ans)++;
                get<2>(ans) = foundPosIt - first;
            }
        }


        return ans;
    }

};








vector<vector<int> > patternStrings;
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
    patternStrings.clear();

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

        patternStrings.push_back(cnt);
    }


//    int cot = 0;
    while(1)
    {
        textString = generateIntergeSeq(n, c);
        int containNumber = 0; // 大串包含模式串的数目
        for(int i = 0; i <= n - m; ++ i)
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





template<typename AL>
auto testAL(int runBuildNumber,
        int runQueryNumber,
        vector<vector<int>>& patternStrings,
        vector<int>& textString,
        string info)
{
    cout << string(30, '-') << info << endl;

    double firstTime, secondTime, thirdTime;

    AL al;
    firstTime = clock();
    while(runBuildNumber -- )
    {
        al.build(patternStrings.begin(), patternStrings.end());
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

        auto ans = al.query(textString.begin(), textString.end());
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

    return make_tuple(foundId, foundNumber, foundPos, (secondTime - firstTime), (thirdTime - secondTime));

}







int main()
{
    const int testNumber = 10;

    constexpr int C = 10;

    int runBuildNumber = 10000;
    int runQueryNumber = 10000;
    srand(time(NULL));


    double bruteForceBuildTime = 0;
    double bruteForceQueryTime = 0;

    double multiKMPBuildTime = 0;
    double multiKMPQueryTime = 0;

    double AcAutomatonBuildTime = 0;
    double AcAutomatonQueryTime = 0;


    double AcAutomatonAndRabinKarpBuildTime = 0;
    double AcAutomatonAndRabinKarpQueryTime = 0;





    for(int i = 1; i <= testNumber; ++ i)
    {
        setPaternAndText(C);

        cout << string(100, '*') << "new poll " << i << endl;



        auto bruteForceAns = testAL<BruteForce<C>>(
                runBuildNumber, runQueryNumber, patternStrings, textString, "bruteForce");
        bruteForceBuildTime += get<3>(bruteForceAns);
        bruteForceQueryTime += get<4>(bruteForceAns);

        auto multiKMPAns = testAL<MultiKMP<C>>(
                runBuildNumber, runQueryNumber, patternStrings, textString, "multiKMP");
        multiKMPBuildTime += get<3>(multiKMPAns);
        multiKMPQueryTime += get<4>(multiKMPAns);


        auto AcAutomatonAns = testAL<AcAutomaton<C>>(
            runBuildNumber, runQueryNumber, patternStrings, textString, "AcAutomaton");
        AcAutomatonBuildTime += get<3>(AcAutomatonAns);
        AcAutomatonQueryTime += get<4>(AcAutomatonAns);


        auto AcAutomatonAndRabinKarpAns = testAL<AcAutomatonAndRabinKarp<C>>(
            runBuildNumber, runQueryNumber, patternStrings, textString, "AcAutomatonAndRabinKarp");
        AcAutomatonAndRabinKarpBuildTime += get<3>(AcAutomatonAndRabinKarpAns);
        AcAutomatonAndRabinKarpQueryTime += get<4>(AcAutomatonAndRabinKarpAns);







    }


    cout << "bruteForceBuildTime: " << bruteForceBuildTime / testNumber<< endl;
    cout << "bruteForceQueryTime: " << bruteForceQueryTime / testNumber << endl;
    cout << string(60, '-' ) << endl;


    cout << "multiKMPBuildTime: " << multiKMPBuildTime / testNumber<< endl;
    cout << "multiKMPQueryTime: " << multiKMPQueryTime / testNumber << endl;
    cout << string(60, '-' ) << endl;


    cout << "AcAutomatonBuildTime: " << AcAutomatonBuildTime / testNumber << endl;
    cout << "AcAutomatonQueryTime: " << AcAutomatonQueryTime / testNumber<< endl;
    cout << string(60, '-' ) << endl;


    cout << "AcAutomatonAndRabinKarpBuildTime: " << AcAutomatonAndRabinKarpBuildTime / testNumber << endl;
    cout << "AcAutomatonAndRabinKarpQueryTime: " << AcAutomatonAndRabinKarpQueryTime / testNumber << endl;
    cout << string(60, '-' ) << endl;









    return 0;
}
