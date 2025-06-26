## Project Overview

This project is created as a C-only port of [another project of mine](https://github.com/adachng/ECS-Snake-Game) with the following differences:

- No need for ECS design pattern, as the main point of this project is to explore C.
- Unit testing is still done, but the library has to be different since [`googletest`](https://github.com/adachng/ECS-Snake-Game) is implemented in C++, and the reason for this C port may be platform compatibility.

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

## External Dependencies

This software uses third-party libraries provided 'as-is'. See individual license files for complete terms.

### [SDL](https://github.com/libsdl-org/SDL) ([3.2.16](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.16))

**Copyright:** © 1997-2025 Sam Lantinga<br>
**License:** zlib License (see [license/SDL.txt](license/SDL.txt))<br>
**Usage:** Application window management and input handling.

### [RK Test](https://github.com/Warwolt/rktest) (latest `main` commit)

**License:** Public Domain (see [license/rktest.txt](license/rktest.txt))<br>
**Usage:** Unit testing framework.
