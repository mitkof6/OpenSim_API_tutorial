# Demonstrates the use of PerturbationForce plugin with Python.
#
# author: Dimitar Stanev jimstanev@gmail.com
import opensim

# The plugin must be loaded so that the new force is recognized by
# OpenSim. However, for the force to be used other than .xml a swig wrapper
# should be implemented.
lib = opensim.LoadOpenSimLibraryExact('libPerturbationForce.so')

# Make sure to run TestPerturbationForce so that the output_model.osim is
# generate. You can find the PerturbationForce xml description in the .osim file
# under the ForceSet.
model = opensim.Model('output_model.osim')
# model.setUseVisualizer(True)

state = model.initSystem()
opensim.simulate(model, state, 1, True)
