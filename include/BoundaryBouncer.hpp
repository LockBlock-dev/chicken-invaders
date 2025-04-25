#pragma once

enum class BounceDirection : int {
  Backward = -1,
  Random = 0,
  Forward = 1,
};

class BoundaryBouncer {
 public:
  BoundaryBouncer(
      int firstSurfaceIndex,
      int lastSurfaceIndex,
      int initialSurfaceIndex = -1,
      BounceDirection initialDirection = BounceDirection::Random
  );

  void update();

  int firstSurfaceIndex;
  int lastSurfaceIndex;
  int currentSurfaceIndex;

 private:
  BounceDirection direction;
};
