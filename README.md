# An OpenSim API introductory tutorial

This is an OpenSim API introductory tutorial. You can find the tutorial files in
the `src/` folder along with the solutions. In the `doc/` folder, you can find a
short presentation introducing C++ basics.

## Pre-requirements

### Windows

For Windows users please make sure to install the following:

- Install [OpenSim v4.1](https://simtk.org/frs/index.php?group_id=91)
- Microsoft Visual Studio (MVS) and *C++ compiler*. Please note that during the
  installation of MVS the C++ compiler will not be installed by default. You
  have to install the Visual C++ build tools. No need to install any optional
  package such as Windows 10 SDK, etc.
- Install [CMake GUI](https://cmake.org/download/)
- Install [Rapid Environmental Editor](https://www.rapidee.com/en/download)
- Optionally, you can install Python 3.x so that we can use OpenSim's API with
  Python. This will not be required for this example, but is very useful.

### Linux

Check the `build_opensim-core` and `BUILD_OPENSIM.md` if you want to build
`opensim-core` from source on Ubuntu or on Linux with some modifications to the
building script.

### Mac

- Install [OpenSim v4.1](https://simtk.org/frs/index.php?group_id=91)
- Install g++, cmake and cmake-gui

# Acknowledge

If you find this useful you can acknowledge it as follows:

```bibtex
@misc{opensim-api-tutorial,
  author = {Stanev, Dimitar},
  title = {An OpenSim API introductory tutorialSim},
  year = {2020},
  publisher = {GitHub},
  journal = {GitHub repository},
  url = {https://github.com/mitkof6/OpenSim_API_tutorial}
}
```

No worries, I do not mind not being acknowledged. Most of the things presented
here are based on things available on SimTK and the user-guide. I hope I will
have more time to improve and provide more examples.
