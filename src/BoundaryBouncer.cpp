#include "BoundaryBouncer.hpp"

#include "rng.hpp"

BoundaryBouncer::BoundaryBouncer(
    int firstSurfaceIndex,
    int lastSurfaceIndex,
    int initialSurfaceIndex,
    BounceDirection initialDirection
)
    : firstSurfaceIndex(firstSurfaceIndex),
      lastSurfaceIndex(lastSurfaceIndex),
      currentSurfaceIndex(firstSurfaceIndex),
      direction(
          initialDirection != BounceDirection::Random ? initialDirection
          : generate_random_number() % 2 == 0 ? BounceDirection::Backward
                                              : BounceDirection::Forward
      ) {
  if (initialSurfaceIndex == -1) {
    int randomOffset =
        this->lastSurfaceIndex == this->firstSurfaceIndex
            ? 0
            : generate_random_number() %
                  (this->lastSurfaceIndex - this->firstSurfaceIndex);

    this->currentSurfaceIndex = this->firstSurfaceIndex + randomOffset;
  } else if (initialSurfaceIndex != 0)
    this->currentSurfaceIndex = initialSurfaceIndex;
  else
    this->currentSurfaceIndex = this->firstSurfaceIndex;
}

void BoundaryBouncer::update() {
  if (this->currentSurfaceIndex <= this->firstSurfaceIndex)
    this->direction = BounceDirection::Forward;

  if (this->currentSurfaceIndex >= this->lastSurfaceIndex)
    this->direction = BounceDirection::Backward;

  this->currentSurfaceIndex += static_cast<int>(this->direction);

  if (this->firstSurfaceIndex > this->currentSurfaceIndex)
    this->currentSurfaceIndex = this->firstSurfaceIndex;

  if (this->lastSurfaceIndex < this->currentSurfaceIndex)
    this->currentSurfaceIndex = this->lastSurfaceIndex;
}
