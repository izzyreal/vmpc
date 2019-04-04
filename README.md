# vMPC2000XL
wxWidgets implementation of the Akai MPC2000XL emulator.

# Development Setup (Visual Studio for Windows x64)
Requirements:
* [Python](https://www.python.org/downloads/)
* [Conan](https://docs.conan.io/en/latest/installation.html)

After cloning this repo, enter it and run `python build.py`. You will now have a Visual Studio solution in the `build` directory. Inside VS you can choose to build debug or release.

There are 4 main targets, and 1 test suite target for each of them. Note that these are completely different from Conan's `test_package` directories. The latter are concerned with verifying the inclusion of headers and linked libraries. The test suites of the main targets are for unit and integration testing. So the target list becomes:
* vmpc (application executable) 
* vmpc-tests (tests executable)
* mpc (static library)
* mpc-tests (tests executable)
* ctoot (static library)
* ctoot-tests (tests executable)
* moduru (static library)
* moduru-tests (tests executable)

The `vmpc-workspace` directories [`vmpc`](https://github.com/izzyreal/vmpc), [`mpc`](https://github.com/izzyreal/mpc), [`ctoot`](https://github.com/izzyreal/ctoot) and [`moduru`](https://github.com/izzyreal/moduru) are created by a successful run of `python build.py`. They are pulled from the referred repositories.

#### moduru

Moduru is a messy collection of utilities I made, combined some easy to include 3rd party libraries that I like to use like libsamplerate. It needs a lot of work, if not be removed from the project after coming up with alternatives.

#### ctoot

ctoot is an attempt to bring Steve Taylor's [toot2](https://github.com/toot/toot2) from Java to C++. In many areas only the bare minimum is implemented, so don't expect a full translation. Much of the basics of the digital audio and music problem domain is covered however:

* audio servers
* audio system with mixer (optionally auto-connecting)
* modular configuration of inputs, outputs and other DSP processes
* audio process service discovery (for e.g. effects and synthesizers)
* delay, reverb, EQ, dynamics
* synthesis
* MIDI system (optionally auto-connecting)

#### mpc

`mpc` compiles to a static library that covers most of the [Akai MPC](https://en.wikipedia.org/wiki/Akai_MPC) problem domain. The MPC's core functionalities are:

* sequencing, musical arrangement
* sample record and playback

The library is agnostic to GUI implementation, exposing the contents of its virtual monochrome LCD screen via a 2-dimensional vector of booleans.

#### vmpc

`vmpc` is a runnable GUI implementation of `mpc`. Currently I'm experimenting with a vector graphics UI powered by [cairo](https://www.cairographics.org/) and [sdl](https://www.libsdl.org/).
