#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

#include "./BTree.h"

using namespace std;

struct ShareState {
  string symbol;
  string shareName;
  double currentPrice;

  double startPrice;
  double endPrice;
  double percentageChange;

  ShareState(string sy, string s, double sp = NAN) {
    shareName = s;
    symbol = sy;
    startPrice = currentPrice = sp;
    endPrice = NAN;
    percentageChange = 0;
  }
};

class Analyzer {
  BTree *HashTable[26];
  public:
  int firstHash(string s) { return toupper(s[0]) - 'A'; }
  void addTick(Tick t) {
      int n = firstHash(t.symbol);
      string s = t.symbol;
      //make changes if null is returned
    vector<Tick>* v = HashTable[n]->searchEle(&(HashTable[n])->h, s);
    v->push_back(t);
    // HashTable[n];
  }
};

int main() {
  // cout<<"Hello";
  Tick t = {"SHU", double(10.0), 1};
  // string s1 = "Hello";
  // string s2 = "World";
  // cout<<int(s1>s2);
  BTree bt;
  vector<Tick> tv;
  tv.push_back(t);
  tv.push_back(t);
  BTree::EachNode *n = nullptr;
  bool cm = bt.insertEle(&n, &tv);
  if (cm)
    cout << "insertion complete\n";
  else
    cout << "not inserted\n";
}