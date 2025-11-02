#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct Tick {
  string symbol;
  double price;
  long time;

  Tick(string s, double p, long t) : symbol(s), price(p), time(t) {}
};

class BTree {
public:

  struct EachNode {
    EachNode *l;
    vector<Tick> *d1;
    EachNode *m;
    vector<Tick> *d2;
    EachNode *r;
    EachNode();
    EachNode(vector<Tick> *, EachNode * = nullptr, EachNode * = nullptr);
  };

  EachNode *h;
  // generalizing splitting
  bool split(EachNode **, EachNode **);

  bool insertEle(EachNode **, vector<Tick> *);
  vector<Tick>* searchEle(EachNode *const *,string&);
};