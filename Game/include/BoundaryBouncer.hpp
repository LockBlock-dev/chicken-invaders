#pragma once

enum class BounceDirection : int {
  Backward = -1,
  Random = 0,
  Forward = 1,
};

class BoundaryBouncer {
 public:
  BoundaryBouncer(
      unsigned int firstSurfaceIndex,
      unsigned int lastSurfaceIndex,
      int initialSurfaceIndex = -1,
      BounceDirection initialDirection = BounceDirection::Random
  );

  void update();

  unsigned int firstSurfaceIndex;
  unsigned int lastSurfaceIndex;
  int currentSurfaceIndex;

 private:
  BounceDirection direction;
};
