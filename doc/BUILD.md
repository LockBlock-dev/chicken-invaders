# Building the game

1. Install the dependencies:

    - [CMake](https://cmake.org/)
    - A [C++23 compatible](https://en.cppreference.com/w/cpp/compiler_support#C.2B.2B23_features) compiler.

2. Download or clone the repository.

3. Acquire the `ChickenInvaders.dat` file:

    1. Download the game (for free) on the [official page](https://www.interactionstudios.com/chickeninvaders.php).

    2. Install `innoextract` ([website](https://constexpr.org/innoextract/)).

    3. Extract the installer content:

    ```sh
    innoextract ChickenInvadersInstaller.exe
    ```

    4. Locate the `ChickenInvaders.dat` file and move it in the [`assets`](../assets/) folder.

4. (Optional) To enable Discord RPC integration, open the [`CMakeLists.txt`](../CMakeLists.txt) file and change the `WITH_DISCORD` option from `OFF` to `ON`:

    ```cmake
    option(WITH_DISCORD "Enable Discord integration" ON)
    ```

5. Create a build directory and navigate into it:

    ```sh
    mkdir build
    cd build
    ```

6. Run CMake to configure the project:

    ```sh
    cmake ..
    ```

7. Build the project:

    ```sh
    cmake --build .
    ```
