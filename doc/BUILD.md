# Building the game

1. Install the dependencies:

    - [CMake](https://cmake.org/)
    - A [C++23 compatible](https://en.cppreference.com/w/cpp/compiler_support#C.2B.2B23_features) compiler.

2. Download or clone the repository.

3. Download the game (for free) on the [official page](https://www.interactionstudios.com/chickeninvaders.php).

4. Locate the `ChickenInvaders.dat` file and move it in the [`assets`](../assets/) folder.

5. (Optional) To enable Discord RPC integration, open the [`CMakeLists.txt`](../CMakeLists.txt) file and change the `WITH_DISCORD` option from `OFF` to `ON`:

    ```cmake
    option(WITH_DISCORD "Enable Discord integration" ON)
    ```

6. Create a build directory and navigate into it:

    ```sh
    mkdir build
    cd build
    ```

7. Run CMake to configure the project:

    ```sh
    cmake ..
    ```

8. Build the project:

    ```sh
    cmake --build .
    ```
