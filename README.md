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

This workaround works with Ubuntu 18.04. If you do not have the same OS and use
a different version of Python, then the only way to make OpenSim work is to
build it from source.

For convenience a pre-build version of OpenSim for Ubuntu 18.04 can be
downloaded through the following URL: [pre-build](https://sourceforge.net/projects/dependencies/files/opensim-core/opensim-core-4.1-ubuntu-18.04.tar.xz/download)

Extract and move the opensim-core folder to a convenient location. Then
configure the environmental variables as follows:

```shell
export OPENSIM_HOME=/path-to-filesystem/opensim-core
export OpenSim_DIR=$OPENSIM_HOME/lib/cmake/OpenSim
export LD_LIBRARY_PATH=$OPENSIM_HOME/lib:$LD_LIBRARY_PATH
export PATH=$OPENSIM_HOME/bin:$PATH
export PATH=$OPENSIM_HOME/libexec/simbody:$PATH
```

The Python bindings were build with Python 3.6. To install run:

```shell
cd $OPENSIM_HOME/lib/python3.6/site-packages
python3 setup.py install --user
```
To test the Python bindings try importing OpenSim in python3:

`python3 -c "import opensim"`

### Mac

- Install [OpenSim v4.1](https://simtk.org/frs/index.php?group_id=91)
- Install g++, cmake and cmake-gui
