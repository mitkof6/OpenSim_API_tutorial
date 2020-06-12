#include "Neuron.h"

#include "SimTKcommon/internal/EventHandler.h"

#include <OpenSim/Simulation/Model/Model.h>

using namespace OpenSim;
using namespace SimTK;

Neuron::Neuron() {
    constructProperty_R(1);
    constructProperty_C(0.5);
    constructProperty_v_rest(0.05);
	constructProperty_v_threshold(0.5);
}

void Neuron::computeStateVariableDerivatives(const State& s) const {
    auto v = getStateVariableValue(s, "v");
    auto R = get_R();
    auto C = get_C();
    auto v_rest = get_v_rest();
	auto I = getInputValue<double>(s, "I");
    double dvdt = -(v - v_rest) / (C * R) + I / C;
    setStateVariableDerivativeValue(s, "v", dvdt);
}

void Neuron::extendInitStateFromProperties(State& s) const {
	setStateVariableValue(s, "v", get_v_rest());
}

void Neuron::extendAddToSystem(MultibodySystem& system) const {
    Super::extendAddToSystem(system);
    // add state
    addStateVariable("v", Stage::Position);
    // add event handler
    auto self = const_cast<Neuron*>(this); // hack
    auto event = new EventHandler(self);
    system.addEventHandler(event);
}

Neuron::EventHandler::EventHandler(Neuron* neuron)
	: TriggeredEventHandler(Stage::Position), neuron(neuron) {
    getTriggerInfo().setTriggerOnRisingSignTransition(true);
    getTriggerInfo().setTriggerOnFallingSignTransition(true);
}

Real Neuron::EventHandler::getValue(const State& s) const {
    auto v = neuron->getStateVariableValue(s, "v");
	return v - neuron->get_v_threshold();
}
void Neuron::EventHandler::handleEvent(State& s, Real accuracy,
                                       bool& shouldTerminate) const {
	std::cout << "event " << s.getTime() << std::endl; 
    neuron->setStateVariableValue(s, "v", neuron->get_v_rest());
}
