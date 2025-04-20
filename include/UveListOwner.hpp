#pragma once

#include "LinkedListNode.hpp"
#include "UveBase.hpp"

namespace UveDX {
class UveListOwner : public UveBase {
public:
  UveListOwner();
  UveListOwner(UveDX *uveDX);
  ~UveListOwner();

  void update() override;
  void add(LinkedListNode *node);
  void emplaceAfter(LinkedListNode *newNode, LinkedListNode *existingNode);
  void remove(LinkedListNode *node);
  void clear();
  bool isEmpty();
  unsigned int size();

  const LinkedListNode *getHead() const;
  LinkedListNode *getHead();

private:
  LinkedListNode *head;
  LinkedListNode *tail;
};
} // namespace UveDX
