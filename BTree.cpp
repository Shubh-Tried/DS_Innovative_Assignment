
#include <cstddef>
#include <iostream>
#include <vector>


#include "BTree.h"

BTree::EachNode::EachNode() {
  l = m = r = nullptr;
  d1 = d2 = nullptr;
}

BTree::EachNode::EachNode(struct index *i, EachNode *l, EachNode *m) {
  d1 = i;
  this->l = l;
  this->m = m;
  this->r = nullptr;
  this->d2 = nullptr;
} 

// bool BTree::split(EachNode **en, EachNode **n1) {
//   EachNode *n2 = new EachNode((*en)->d1, (*en)->l);
//   EachNode *n3 = new EachNode((*en)->d2, (*en)->m, (*en)->r);
//   // EachNode *l = (*en)->l;
//   // EachNode *m = (*en)->m;
//   if ((*en)->d2 == nullptr || (*(*en)->d2).empty()) {

//     if ((*(*n1)->d1)[0].symbol > (*(*en)->d1)[0].symbol) {
//       (*n1)->d2 = (*en)->d1;
//       (*n1)->r = (*en)->m;
//       delete (*en);
//       *en = *n1;
//     } else {
//       (*en)->d2 = (*n1)->d1;
//       (*en)->r = (*n1)->m;
//       (*en)->m = (*n1)->l;
//     }
//     return false;
//   }
//   delete (*en);
//   if ((*(*n1)->d1)[0].symbol > (*n2->d1)[0].symbol) {
//     if ((*(*n1)->d1)[0].symbol > (*n3->d1)[0].symbol) {
//       n3->l = n2;
//       n3->m = *n1;

//       *en = n3;
//     } else {
//       n2->m = (*n1)->l;
//       n3->l = (*n1)->l;
//       (*n1)->l = n2;
//       (*n1)->m = n3;
//       *en = *n1;
//     }
//   } else {
//     n2->l = *n1;
//     n2->m = n3;
//     *en = n2;
//   }
//   return true;
// }

bool BTree::split(EachNode **en, EachNode **n1) {
  EachNode *promoted = *n1;


  if ((*en)->d2 == nullptr) {
    if (promoted->d1->key < (*en)->d1->key) {
      
      (*en)->d2 = (*en)->d1;
      (*en)->d1 = promoted->d1;
      (*en)->r = (*en)->m;
      (*en)->m = promoted->m;
      (*en)->l = promoted->l;
    } else {
      // child larger than parent
      (*en)->d2 = promoted->d1;
      *n1 = promoted->l;
      (*en)->r = promoted->m;
    }
    delete promoted;
    return false;
  }

  
  struct index *left_key, *mid_key, *right_key;
  EachNode *c0, *c1, *c2, *c3;

  int pk  = promoted->d1->key;
  int d1k = (*en)->d1->key;
  int d2k = (*en)->d2->key;

  if (pk < d1k) {
    // Promoted key is smallest (came from left child split)
    left_key  = promoted->d1;
    mid_key   = (*en)->d1;
    right_key = (*en)->d2;


    c0 = promoted->l;
    c1 = promoted->m;
    c2 = (*en)->m;
    c3 = (*en)->r;
  } else if (pk < d2k) {
    // Promoted key is in the middle (came from middle child split)
    left_key  = (*en)->d1;
    mid_key   = promoted->d1;
    right_key = (*en)->d2;
    c0 = (*en)->l;
    c1 = promoted->l;
    c2 = promoted->m;
    c3 = (*en)->r;
  } else {
    // Promoted key is largest (came from right child split, or leaf overflow)
    left_key  = (*en)->d1;
    mid_key   = (*en)->d2;
    right_key = promoted->d1;
    c0 = (*en)->l;
    c1 = (*en)->m;
    c2 = promoted->l;
    c3 = promoted->m;
  }

  // Create new left and right children with properly distributed child pointers
  EachNode *lnode = new EachNode(left_key, c0, c1);
  EachNode *rnode = new EachNode(right_key, c2, c3);

  // Promote middle key up into the current node
  (*en)->d1 = mid_key;
  (*en)->d2 = nullptr;
  (*en)->l  = lnode;
  (*en)->m  = rnode;
  (*en)->r  = nullptr;

  delete promoted;  // container no longer needed
  return true;
}


// bool BTree::insertEle(EachNode **en, vector<Tick> *t) {
//   if (en == nullptr) {
//     cout << "no element exists";
//     return false;
//   }
//   if (*en == nullptr) {
//     *en = new EachNode(t);
//     return true;
//   }
//   if ((*t).empty())
//     return false;
//   bool promoted = false;

//   // l is null suggests that the node is a leaf node
//   if ((*en)->l == nullptr) {
//     if ((*(*en)->d2).empty())
//       (*en)->d2 = (*(*en)->d1)[0].symbol > (*t)[0].symbol ? (*en)->d1 : t;
//     else {
//       EachNode *n = new EachNode(t);
//       promoted = split(en, &n);
//     }
//   }
//   // the node has children
//   else {
//     EachNode **n;
//     if ((*t)[0].symbol <= (*(*en)->d1)[0].symbol) {
//       n = &(*en)->l;
//       promoted = insertEle(&(*en)->l, t);
//     } else if ((*(*en)->d2).empty() ||
//                (*t)[0].symbol <= (*(*en)->d2)[0].symbol) {
//       n = &(*en)->m;
//       promoted = insertEle(&(*en)->m, t);
//     } else {
//       n = &(*en)->r;
//       promoted = insertEle(&(*en)->r, t);
//     }
//     if (promoted) {
//       promoted = split(en, n);
//     }
//   }
//   return promoted;
// }

bool BTree::insertEle(EachNode **en, struct index *val) {
  if (en == nullptr) {
    cout << "No element exists";
    return false;
  }

  if (*en == nullptr) {
    *en = new EachNode(val);
    return true;
  }

  // --- Duplicate key handling: append data to existing index ---
  if ((*en)->d1->key == val->key) {
    for (auto &s : val->data) {
      (*en)->d1->data.push_back(s);
    }
    return false;
  }
  if ((*en)->d2 != nullptr && (*en)->d2->key == val->key) {
    for (auto &s : val->data) {
      (*en)->d2->data.push_back(s);
    }
    return false;
  }

  bool promoted = false;

  // Leaf node (no children)
  if ((*en)->l == nullptr) {
    if ((*en)->d2 == nullptr) {
      // Room in this leaf — just insert
      if (val->key < (*en)->d1->key) {
        (*en)->d2 = (*en)->d1;
        (*en)->d1 = val;
      } else {
        (*en)->d2 = val;
      }
    } else {
      // Leaf is full — split
      EachNode *n = new EachNode(val);
      promoted = split(en, &n);
    }
  }
  // Internal node (has children)
  else {
    EachNode **child;

    if (val->key < (*en)->d1->key) {
      child = &(*en)->l;
    } else if ((*en)->d2 == nullptr || val->key < (*en)->d2->key) {
      child = &(*en)->m;
    } else {
      child = &(*en)->r;
    }

    promoted = insertEle(child, val);

    if (promoted) {
      promoted = split(en, child);
    }
  }
  return promoted;
}

struct index* BTree::searchEle(EachNode *const *en, int key) {
  if (en == nullptr || *en == nullptr)
    return nullptr;

  // Check d1
  if ((*en)->d1->key == key)
    return (*en)->d1;

  // Check d2
  if ((*en)->d2 != nullptr && (*en)->d2->key == key)
    return (*en)->d2;

  // Leaf node — key not found
  if ((*en)->l == nullptr)
    return nullptr;

  // Recurse into the correct child
  if (key < (*en)->d1->key)
    return searchEle(&(*en)->l, key);
  else if ((*en)->d2 == nullptr || key < (*en)->d2->key)
    return searchEle(&(*en)->m, key);
  else
    return searchEle(&(*en)->r, key);
}

