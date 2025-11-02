#include <vector>

using namespace std;

typedef int Tick;

class BTree{
    public:
    struct EachNode{
        EachNode *l;
        vector<Tick> d1;
        EachNode *m;
        vector<Tick> d2;
        EachNode *r;
        EachNode();
        EachNode(vector<Tick>, EachNode* = nullptr,EachNode* = nullptr);
    };

    //generalizing splitting
    bool split(EachNode **, EachNode **);
    
    bool insertEle(EachNode **, vector<Tick>);
    
};