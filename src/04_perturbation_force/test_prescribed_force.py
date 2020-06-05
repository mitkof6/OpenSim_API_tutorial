# This script demonstrates how one can apply a prescribed body force on a body.
#
# author Dimitar Stanev jimstanev@gmail.com
import numpy as np
import opensim

model = opensim.Model('tug_of_war.osim')

# create the prescribed force object
prescribed_force = opensim.PrescribedForce()

# make sure to add the absolute path of the body frame
prescribed_force.setBodyName('/bodyset/block')

# define the point of application in body fixed frame
prescribed_force.setPointFunctions(opensim.Constant(0),
                                   opensim.Constant(0),
                                   opensim.Constant(0))

# construct a spline for the z-component of the force
fz = opensim.SimmSpline()
t = np.linspace(0, 1, 10, endpoint=True)
y = 100 * np.sin(t)
for i in range(t.shape[0]):
    fz.addPoint(t[i], y[i])

prescribed_force.setForceFunctions(opensim.Constant(0), opensim.Constant(0), fz)
model.addForce(prescribed_force)

# simulate
state = model.initSystem()
opensim.simulate(model, state, 1, True)
model.printToXML('pf_model.osim')
