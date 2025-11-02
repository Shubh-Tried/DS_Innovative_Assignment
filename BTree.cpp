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
    EachNode(){
        l = m = r = nullptr;
    }
    EachNode(vector<Tick> t, EachNode *l = nullptr,EachNode *m = nullptr){
        d1 = t;
        this->l = l;
        this->m = m;
        this->r = nullptr;
    }
};


//generalizing splitting
bool split(EachNode **en, EachNode **n1){
    EachNode *n2 = new EachNode((*en)->d1, (*en)->l);
    EachNode *n3 = new EachNode((*en)->d2, (*en)->m, (*en)->r);
    // EachNode *l = (*en)->l;
    // EachNode *m = (*en)->m;
    if((*en)->d2.empty()){
        if((*n1)->d1[0] > (*en)->d1[0]){
            (*n1)->d2 = (*en)->d1;
            (*n1)->r = (*en)->m;
            delete(*en);
            *en = *n1;
        }
        else{
            (*en)->d2 = (*n1)->d1;
            (*en)->r = (*n1)->m;
            (*en)->m = (*n1)->l;
        }
        return false;
    }
    delete(*en);
    if((*n1)->d1[0] > n2->d1[0]){
        if((*n1)->d1[0] > n3->d1[0]) {
            n3->l = n2;
            n3->m = *n1;
            
            *en = n3;
        }
        else{
            n2->m = (*n1)->l;
            n3->l = (*n1)->l;
            (*n1)->l = n2;
            (*n1)->m = n3;
            *en = *n1;
        }
    }
    else{
        n2->l = *n1;
        n2->m = n3;
        *en = n2;
    }
    return true;
}

bool insertEle(EachNode **en, vector<Tick> t){
    bool promoted = false;
    if(t[0] <= (*en)->d1[0] && (*en)->l ==nullptr){
        if(isnan((*en)->d2[0])){
            (*en)->d2 =(*en)->d1;
            (*en)->r = (*en)->m;
            (*en)->m = nullptr;
            (*en)->d1 = t;
        }
        else{
            EachNode *n = new EachNode(t);
            promoted = split(en, &n);
        }
    }
    else if(t[0]<=(*en)->d1[0]){
        promoted = insertEle(&(*en)->l, t); 
        
        //if the child node does not have a second data then we will do splitting
        if(promoted){
            promoted = split(en, &(*en)->l);
        }
    }
    //checking for null pointer to check if the node is leaf node or not
    else if(t[0]<=(*en)->d2[0] && (*en)->m == nullptr){
        EachNode *n = new EachNode(t);
        promoted = split(en, &n); 
    }

//l is null suggests that the node is a leaf node
    if((*en)->l == nullptr){
        if(isnan((*en)->d2[0]))
            (*en)->d2 = (*en)->d1[0]>t[0]?(*en)->d1:t;
        else
        {
            EachNode *n = new EachNode(t); 
            promoted = split(en, &n);
        }
    }
    else{
        if(t[0]<=(*en)->d1[0]){
            promoted = insertEle(&(*en)->l, t);

            //if the child node does not have a second data then we will do splitting
            if(promoted){
                promoted = split(en, &(*en)->l);
            }
        }
        if(isnan((*en)->d2[0])){
            if((*en)->d1[0]>t[0]){
                (*en)->d2 = (*en)->d1;
                (*en)->d1 = t;
                (*en)->r = (*en)->m;
                (*en)->m = (*en)->l;
                (*en)->l = nullptr;
            }
            else{
                (*en)->d2 = t;
            }
        }
    }
    return promoted;
}

int main(){
    return 0;
}