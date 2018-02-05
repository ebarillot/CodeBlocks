
#include "BTree.h"

struct _BTree
{
  int     key;
  struct _BTree* left;
  struct _BTree* right;
};


BTree* BTree_add
