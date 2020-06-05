#include "RegisterPlugin.h"
#include <OpenSim/Common/Object.h>
#include "PerturbationForce.h"

using namespace OpenSim;

static dllObjectInstantiator instantiator;

void RegisterPlugin() {
    Object::RegisterType(PerturbationForce());
}

dllObjectInstantiator::dllObjectInstantiator() {
    registerDllClasses();
}

void dllObjectInstantiator::registerDllClasses() {
    RegisterPlugin();
}
