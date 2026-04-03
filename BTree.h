#pragma once
#include <iostream>
#include <vector>

using namespace std;

// struct Tick {
//   string symbol;
//   double price;
//   long time;

//   Tick(string s, double p, long t) : symbol(s), price(p), time(t) {}
// };

struct schema{
  int dep_id;
  string emp_name;
  double emp_salary;
  int building_number;
};

struct index{
  int key;
  vector<schema> data;  
};

class BTree {
public:

//   struct EachNode {
//     EachNode *l;
//     vector<Tick> *d1;
//     EachNode *m;
//     vector<Tick> *d2;
//     EachNode *r;
//     EachNode();
//     EachNode(vector<Tick> *, EachNode * = nullptr, EachNode * = nullptr);
//   };

struct EachNode {
    EachNode *l;
    struct index *d1;
    EachNode *m;
    struct index *d2;
    EachNode *r;
    EachNode();
    EachNode(struct index *,EachNode * = nullptr, EachNode * =nullptr);
  };

  EachNode *h;
  // generalizing splitting
  bool split(EachNode **, EachNode **);

  bool insertEle(EachNode **, struct index *);

  struct index* searchEle(EachNode *const *, int);
};