#include "RegisterPlugin.h"
#include <OpenSim/Common/Object.h>
#include "Neuron.h"

using namespace OpenSim;

static dllObjectInstantiator instantiator;

void RegisterPlugin() {
    Object::RegisterType(Neuron());
}

dllObjectInstantiator::dllObjectInstantiator() {
    registerDllClasses();
}

void dllObjectInstantiator::registerDllClasses() {
    RegisterPlugin();
}
