
#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>


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
  EachNode *child = *n1;


  if ((*en)->d2 == nullptr) {
    if (child->d1->key < (*en)->d1->key) {
      
      (*en)->d2 = (*en)->d1;
      (*en)->d1 = child->d1;
      (*en)->r = (*en)->m;
      (*en)->m = child->m;
      (*en)->l = child->l;
    } else {
      // child larger than parent
      (*en)->d2 = child->d1;
      *n1 = child->l;
      (*en)->r = child->m;
    }
    delete child;
    return false;
  }

  
  struct index *left_key, *mid_key, *right_key;
  EachNode *c0, *c1, *c2, *c3;

  int cid  = child->d1->key;
  int d1id = (*en)->d1->key;
  int d2id = (*en)->d2->key;

  if (cid < d1id) {
    //Child is less hence en->l should be null
    left_key  = child->d1;
    mid_key   = (*en)->d1;
    right_key = (*en)->d2;


    c0 = child->l;
    c1 = child->m;
    c2 = (*en)->m;
    c3 = (*en)->r;
  } else if (cid < d2id) {
    // Child is en->m
    left_key  = (*en)->d1;
    mid_key   = child->d1;
    right_key = (*en)->d2;
    c0 = (*en)->l;
    c1 = child->l;
    c2 = child->m;
    c3 = (*en)->r;
  } else {
    // Child is en->r
    left_key  = (*en)->d1;
    mid_key   = (*en)->d2;
    right_key = child->d1;
    c0 = (*en)->l;
    c1 = (*en)->m;
    c2 = child->l;
    c3 = child->m;
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

  delete child;
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


  if ((*en)->d1->key == val->key) {
    for (schema &s : val->data) {
      (*en)->d1->data.push_back(s);
    }

    return false;
  }
  if ((*en)->d2 != nullptr && (*en)->d2->key == val->key) {
    for (schema &s : val->data) {
      (*en)->d2->data.push_back(s);
    }
    
    return false;
  }

  bool promoted = false;

  // leaf node
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

struct index* BTree::searchId(EachNode *const *en, int key) {
  if (en == nullptr || *en == nullptr)
    return nullptr;

  if ((*en)->d1->key == key)
    return (*en)->d1;

  if ((*en)->d2 != nullptr && (*en)->d2->key == key)
    return (*en)->d2;

  if ((*en)->l == nullptr)
    return nullptr;


  if (key < (*en)->d1->key)
    return searchId(&(*en)->l, key);

  else if ((*en)->d2 == nullptr || key < (*en)->d2->key)
    return searchId(&(*en)->m, key);

  else
    return searchId(&(*en)->r, key);
}

