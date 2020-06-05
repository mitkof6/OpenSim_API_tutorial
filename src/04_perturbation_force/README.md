# Description

Implements a perturbation body force component for OpenSim. 

# Building, testing and using

Make sure that OpenSim v4.0 API is built and installed so that CMake can find it
during building.

Building of the plugin:


```bash
mkdir build
cd build
cmake ../ \
      -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo \
      -DOPENSIM_INSTALL_DIR:PATH=path to OpenSimConfig.cmake \
make

```

Run the test:

`./TestPerturbationForce`

Examine the Python script `test_plugin.py` (copied in the build folder by CMake)
to see how to use the plugin with Python.

The user cannot directly construct a `PerturbationForce` in Python, because we
have not provided Python bindings. However, by loading the dynamic library,
OpenSim can recognize this new type of force. Therefore, the way to use it is by
including the following description in the .osim file of your model:

```xml
<PerturbationForce name="noise">
    <!--Body to apply the force.-->
	<body_name>block</body_name>
	<!--Point of application in body frame.-->
	<offset>0 0 0</offset>
	<!--Magnitude of the perturbation force.-->
	<magnitude>1000</magnitude>
</PerturbationForce>
```
