#pragma once

class LinkedListNode {
public:
  virtual ~LinkedListNode() {};
  virtual void update() = 0;

  LinkedListNode *prev = nullptr;
  LinkedListNode *next = nullptr;
};
