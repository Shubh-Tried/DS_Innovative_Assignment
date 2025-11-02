#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

#include "BTree.h"

BTree::EachNode::EachNode() {
  l = m = r = nullptr;
  d1 = d2 = nullptr;
}

BTree::EachNode::EachNode(vector<Tick> *t, EachNode *l, EachNode *m) {
  d1 = t;
  this->l = l;
  this->m = m;
  this->r = nullptr;
  this->d2 = nullptr;
}

bool BTree::split(EachNode **en, EachNode **n1) {
  EachNode *n2 = new EachNode((*en)->d1, (*en)->l);
  EachNode *n3 = new EachNode((*en)->d2, (*en)->m, (*en)->r);
  // EachNode *l = (*en)->l;
  // EachNode *m = (*en)->m;
  if ((*en)->d2 == nullptr || (*(*en)->d2).empty()) {

    if ((*(*n1)->d1)[0].symbol > (*(*en)->d1)[0].symbol) {
      (*n1)->d2 = (*en)->d1;
      (*n1)->r = (*en)->m;
      delete (*en);
      *en = *n1;
    } else {
      (*en)->d2 = (*n1)->d1;
      (*en)->r = (*n1)->m;
      (*en)->m = (*n1)->l;
    }
    return false;
  }
  delete (*en);
  if ((*(*n1)->d1)[0].symbol > (*n2->d1)[0].symbol) {
    if ((*(*n1)->d1)[0].symbol > (*n3->d1)[0].symbol) {
      n3->l = n2;
      n3->m = *n1;

      *en = n3;
    } else {
      n2->m = (*n1)->l;
      n3->l = (*n1)->l;
      (*n1)->l = n2;
      (*n1)->m = n3;
      *en = *n1;
    }
  } else {
    n2->l = *n1;
    n2->m = n3;
    *en = n2;
  }
  return true;
}

bool BTree::insertEle(EachNode **en, vector<Tick> *t) {
  if (en == nullptr) {
    cout << "no element exists";
    return false;
  }
  if (*en == nullptr) {
    *en = new EachNode(t);
    return true;
  }
  if ((*t).empty())
    return false;
  bool promoted = false;

  // l is null suggests that the node is a leaf node
  if ((*en)->l == nullptr) {
    if ((*(*en)->d2).empty())
      (*en)->d2 = (*(*en)->d1)[0].symbol > (*t)[0].symbol ? (*en)->d1 : t;
    else {
      EachNode *n = new EachNode(t);
      promoted = split(en, &n);
    }
  }
  // the node has children
  else {
    EachNode **n;
    if ((*t)[0].symbol <= (*(*en)->d1)[0].symbol) {
      n = &(*en)->l;
      promoted = insertEle(&(*en)->l, t);
    } else if ((*(*en)->d2).empty() ||
               (*t)[0].symbol <= (*(*en)->d2)[0].symbol) {
      n = &(*en)->m;
      promoted = insertEle(&(*en)->m, t);
    } else {
      n = &(*en)->r;
      promoted = insertEle(&(*en)->r, t);
    }
    if (promoted) {
      promoted = split(en, n);
    }
  }
  return promoted;
}

vector<Tick> *BTree::searchEle(EachNode *const *en, string &str) {
  if (en == nullptr || *en == nullptr)
    return nullptr;

  // If we reached a leaf node
  if ((*en)->l == nullptr) {
    // check d1
    if (!(*(*en)->d1).empty() && (*(*en)->d1)[0].symbol == str)
      return (*en)->d1;
    // check d2
    if ((*en)->d2 && !(*(*en)->d2).empty() && (*(*en)->d2)[0].symbol == str)
      return (*en)->d2;
    return nullptr;
  }

  if ((*(*en)->d1)[0].symbol == str)
    return (*en)->d1;
  if ((*en)->d2 && !(*(*en)->d2).empty() && (*(*en)->d2)[0].symbol == str)
    return (*en)->d2;

  if (str < (*(*en)->d1)[0].symbol)
    return searchEle(&(*en)->l, str);
  else if (!(*en)->d2 || (*(*en)->d2).empty() || str < (*(*en)->d2)[0].symbol)
    return searchEle(&(*en)->m, str);
  else
    return searchEle(&(*en)->r, str);
}