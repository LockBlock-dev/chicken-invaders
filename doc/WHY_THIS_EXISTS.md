# Why this exists

_Chicken Invaders 1_ was originally released in 1999. The latest official version of the game runs on DirectX 7.0, specifically using the now-obsolete **DirectDraw** component via InterAction Studios' first version of their engine, **UveDX**. Unfortunately, DirectDraw is only software-emulated on modern versions of Windows, which makes the game slow, unstable, and prone to compatibility issues.

As a child, I used to play _Chicken Invaders 1_ — albeit years after its original release — and it stuck with me. The charm and the nostalgia of it all made a lasting impression. Some time ago, I even started working on a cheat for the game, [omelette](https://github.com/LockBlock-dev/omelette), because of the game simplicity. This involved reverse engineering quite a bit of its logic. That process opened the door to a bigger idea: why not go all the way and reverse engineer the entire game?

While doing this, I also came across the [forum thread](https://forum.chickeninvaders.com/t/chicken-invaders-1-remastered-04/26403) by the original developer discussing about the remaster. That inspired me to attempt my own rewrite, not just as a learning experience, but as a way to bring the game into the modern era.

The original game was written in Borland C++ using an object-oriented approach. For this rewrite, I have reimplemented it in C++ 23, and replaced DirectX with SFML to achieve cross-platform support. I initially started with Raylib, but SFML turned out to be a better fit for this project.
