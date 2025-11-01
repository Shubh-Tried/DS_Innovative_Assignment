#include <vector>
#include <cmath>

using namespace std;

typedef int Tick;

struct EachNode{
    EachNode *l;
    vector<Tick> d1;
    EachNode *m;
    vector<Tick> d2;
    EachNode *r;
    EachNode(vector<Tick> t){
        d1 = t;
        l =m = r=nullptr;
    }
};

void split(EachNode **en, vector<Tick> t){
    EachNode *n1 = new EachNode(t);
    EachNode *n2 = new EachNode((*en)->d1);
    EachNode *n3 = new EachNode((*en)->d2);
}

void insertEle(EachNode **en, vector<Tick> t){
    if(t[0] < (*en)->d1[0] && (*en)->l !=nullptr){
        if(isnan((*en)->d2[0])){
            (*en)->d2 =(*en)->d1;
            (*en)->d1 = t;
        }
        else{
            split(en, t);
        }
    }
}