#include "BoundaryBouncer.hpp"

#include "rng.hpp"

BoundaryBouncer::BoundaryBouncer(
    unsigned int firstSurfaceIndex,
    unsigned int lastSurfaceIndex,
    int initialSurfaceIndex,
    BounceDirection initialDirection
)
    : firstSurfaceIndex(firstSurfaceIndex),
      lastSurfaceIndex(lastSurfaceIndex),
      currentSurfaceIndex(static_cast<int>(firstSurfaceIndex)),
      direction(
          initialDirection != BounceDirection::Random ? initialDirection
          : random_range(0, 2) == 0 ? BounceDirection::Backward
                                    : BounceDirection::Forward
      ) {
  if (initialSurfaceIndex == -1) {
    unsigned int randomOffset =
        this->lastSurfaceIndex == this->firstSurfaceIndex
            ? 0
            : random_range(
                  0u, (this->lastSurfaceIndex - this->firstSurfaceIndex)
              );

    this->currentSurfaceIndex =
        static_cast<int>(this->firstSurfaceIndex + randomOffset);
  } else if (initialSurfaceIndex != 0)
    this->currentSurfaceIndex = initialSurfaceIndex;
  else
    this->currentSurfaceIndex = static_cast<int>(this->firstSurfaceIndex);
}

void BoundaryBouncer::update() {
  if (this->currentSurfaceIndex <= static_cast<int>(this->firstSurfaceIndex))
    this->direction = BounceDirection::Forward;

  if (this->currentSurfaceIndex >= static_cast<int>(this->lastSurfaceIndex))
    this->direction = BounceDirection::Backward;

  this->currentSurfaceIndex += static_cast<int>(this->direction);

  if (static_cast<int>(this->firstSurfaceIndex) > this->currentSurfaceIndex)
    this->currentSurfaceIndex = static_cast<int>(this->firstSurfaceIndex);

  if (static_cast<int>(this->lastSurfaceIndex) < this->currentSurfaceIndex)
    this->currentSurfaceIndex = static_cast<int>(this->lastSurfaceIndex);
}
