#include "RegisterPlugin.h"
/*
 * Strange incomplete type error
 *
 * I was getting a strange incomplete type error when including
 * FixationController.h here. The fixation controller includes the Controller.h
 * file which makes a forward declaration of type Actuator that is passed to the
 * Set<> as a const Actuator. However, the Set object performs some operations
 * that violate the constness. Not sure why this triggered an error that is
 * located in the opensim-core library. However, by including Actuator.h before
 * the FixationController.h seems to solve the problem. This is not necessary
 * when using the fixation controller elsewhere (e.g., see Simulation.cpp).
 */
#include <OpenSim/Simulation/Model/Actuator.h>
#include "FixationController.h"
#include <OpenSim/Simulation/Model/ExpressionBasedCoordinateForce.h>

using namespace OpenSim;

// OpenSim bug: when <OpenSim/OpenSim.h> is included then instantiator is
// redefined so make sure not to include <OpenSim/OpenSim.h> in your
// implementation
static dllObjectInstantiator instantiator;

void RegisterPlugin() {
    // fix OpenSim bug: ExpressionBasedCoordinateForce is not registered, thus
    // it can't be used in the .osim model.
    Object::RegisterType(ExpressionBasedCoordinateForce());
    Object::RegisterType(FixationController());
}

dllObjectInstantiator::dllObjectInstantiator() {
    registerDllClasses();
}

void dllObjectInstantiator::registerDllClasses() {
    RegisterPlugin();
}
