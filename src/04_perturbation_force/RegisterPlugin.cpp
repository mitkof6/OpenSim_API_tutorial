#include "RegisterPlugin.h"

#include "PerturbationForce.h"

#include <OpenSim/Common/Object.h>

using namespace OpenSim;

static dllObjectInstantiator instantiator;

void RegisterPlugin() { Object::RegisterType(PerturbationForce()); }

dllObjectInstantiator::dllObjectInstantiator() { registerDllClasses(); }

void dllObjectInstantiator::registerDllClasses() { RegisterPlugin(); }
