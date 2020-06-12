/**
 * @file Neuron.h
 *
 * \brief Implements a hybrid differential equation (continuous and
 * discrete) of a leaky integrate and fire neuron.
 *
 * @author Dimitar Stanev <jimstanev@gmail.com>
 */
#ifndef NEURON_H
#define NEURON_H

#include "NeuronExports.h"

#include <Common/ComponentOutput.h>
#include <Common/ComponentSocket.h>
#include <OpenSim/Simulation/Model/ModelComponent.h>
#include <Simulation/Model/Model.h>

namespace OpenSim {
/**
 * \brief A leaky integrate and fire neuron. The fire part not implemented.
 */
class Neuron_API Neuron : public OpenSim::ModelComponent {
    OpenSim_DECLARE_CONCRETE_OBJECT(Neuron, OpenSim::ModelComponent);

 public:
    OpenSim_DECLARE_PROPERTY(C, double, "Neuron membrane capacitance.");
    OpenSim_DECLARE_PROPERTY(R, double, "Neuron membrane resistance.");
    OpenSim_DECLARE_PROPERTY(v_rest, double, "Neuron resting membrane potential.");
	OpenSim_DECLARE_PROPERTY(v_threshold, double, "Neuron threshold.");

    OpenSim_DECLARE_OUTPUT_FOR_STATE_VARIABLE(v);

	OpenSim_DECLARE_INPUT(I, double, SimTK::Stage::Time, "Current");

    Neuron();

    // Calculate the state derivative.
    void computeStateVariableDerivatives(const SimTK::State& s) const override;
    // Initializes the state
    void extendInitStateFromProperties(SimTK::State& s) const override;
    // Add state variables to the system.
    void extendAddToSystem(SimTK::MultibodySystem& system) const override;

 private:
    // A class that introduces event detection and handling for hybrid
    // systems.
    class EventHandler : public SimTK::TriggeredEventHandler {
     public:
        EventHandler(Neuron* neuron);
        SimTK::Real getValue(const SimTK::State& s) const override;
        void handleEvent(SimTK::State& s, SimTK::Real accuracy,
                         bool& shouldTerminate) const override;
     private:
		Neuron* neuron;
    };
};
} // namespace OpenSim

#endif
