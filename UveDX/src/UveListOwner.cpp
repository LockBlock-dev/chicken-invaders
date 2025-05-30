#include "UveDX/UveListOwner.hpp"

namespace UveDX {
UveListOwner::UveListOwner() : UveBase(), head(nullptr), tail(nullptr) {}

UveListOwner::UveListOwner(UveDX* uveDX)
    : UveBase(uveDX), head(nullptr), tail(nullptr) {}

UveListOwner::~UveListOwner() {
  this->clear();
}

void UveListOwner::update() {
  auto currentHead = this->head;

  while (true) {
    if (!currentHead)
      break;

    UveBase* originalHead = dynamic_cast<UveBase*>(currentHead);
    currentHead = currentHead->next;

    originalHead->update();

    if (originalHead->hasBeenDisposed) {
      this->remove(originalHead);

      delete originalHead;
    }
  }
}

void UveListOwner::add(LinkedListNode* node) {
  if (!node)
    return;

  if (head) {
    this->tail->next = node;
    node->prev = this->tail;
    node->next = nullptr;
    this->tail = node;
  } else {
    this->head = node;
    this->tail = node;

    node->next = nullptr;
    node->prev = nullptr;
  }
}

void UveListOwner::emplaceAfter(
    LinkedListNode* newNode,
    LinkedListNode* existingNode
) {
  newNode->next = existingNode->next;

  newNode->prev = existingNode;

  existingNode->next = newNode;

  newNode->next->prev = newNode;

  if (existingNode == this->tail)
    this->tail = newNode;
}

void UveListOwner::remove(LinkedListNode* node) {
  if (!node)
    return;

  if (node == this->head) {
    LinkedListNode* newHead = this->head->next;
    this->head = newHead;

    if (newHead)
      newHead->prev = nullptr;
    else
      this->tail = nullptr;
  } else if (node == this->tail) {
    LinkedListNode* newTail = this->tail->prev;
    this->tail = newTail;

    if (newTail)
      newTail->next = nullptr;
    else
      this->head = nullptr;
  } else {
    LinkedListNode* previousNode = node->prev;
    LinkedListNode* nextNode = node->next;

    if (previousNode)
      previousNode->next = nextNode;
    if (nextNode)
      nextNode->prev = previousNode;
  }

  node->prev = nullptr;
  node->next = nullptr;
}

void UveListOwner::clear() {
  while (true) {
    auto node = this->head;

    if (!this->head)
      break;

    this->head = node->next;

    delete node;
  }
}

bool UveListOwner::isEmpty() {
  return !this->head;
}

unsigned int UveListOwner::size() {
  unsigned int i;
  auto head = this->head;

  for (i = 0; head != nullptr; head = head->next)
    ++i;

  return i;
}

const LinkedListNode* UveListOwner::getHead() const {
  return this->head;
}

LinkedListNode* UveListOwner::getHead() {
  return this->head;
}
}  // namespace UveDX
