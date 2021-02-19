#include "RegisterPlugin.h"

#include "Neuron.h"

#include <OpenSim/Common/Object.h>

using namespace OpenSim;

static dllObjectInstantiator instantiator;

void RegisterPlugin() { Object::RegisterType(Neuron()); }

dllObjectInstantiator::dllObjectInstantiator() { registerDllClasses(); }

void dllObjectInstantiator::registerDllClasses() { RegisterPlugin(); }
