For the mainline VMPC2000XL source code, please refer to https://github.com/izzyreal/vmpc-juce.

The repository you're now looking at is an alternative GUI implementation of VMPC2000XL, based on https://github.com/izzyreal/mpc like the mainline JUCE implementation. Instead of JUCE, this repo uses SDL2, Cairo and PortAudio to display graphics and produce sound.

It is archived and probably won't build out of the box, but if you're interested in doing your own similar thing with `mpc`, let me know.

# Intended Audience

You are interested in

- exploring standalone GUI application writing
- contributing to VMPC2000XL
- C++ build automation and package management with [Conan](https://conan.io/)



# Overview

`vmpc` provides a runnable wrapper for the `mpc` static library -- i.e. it's the application executable of VMPC2000XL. It instantiates and binds GUI code to an `mpc::Mpc` object and other parts of `mpc`. It can be thought of as the graphics, audio, keyboard and mouse engine, though the heavy lifting is performed by 3rd party libraries.

At the moment it is powered by

* [cairo](https://www.cairographics.org/)
* [SDL](https://www.libsdl.org/)

but that could change, as I'm currently in the middle of developing `vmpc`.

<explain Conan add remote and install>
