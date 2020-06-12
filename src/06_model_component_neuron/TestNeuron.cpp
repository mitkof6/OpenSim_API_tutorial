/**
 * @file TestNeuron.h
 *
 * \brief Tests the Neuron model.
 *
 * @author Dimitar Stanev <jimstanev@gmail.com>
 */
#include "Neuron.h"
#include <Common/ComponentOutput.h>
#include <Common/Constant.h>
#include <Common/Property.h>
#include <OpenSim/OpenSim.h>
#include <Simulation/Model/ModelComponent.h>

using namespace std;
using namespace OpenSim;

#define PAUSE                                                                  \
    cout << endl << "press a key to continue ..." << endl;                     \
    getchar();

// A constant drive component
class ConstantSource : public ModelComponent {
    OpenSim_DECLARE_CONCRETE_OBJECT(ConstantSource, OpenSim::ModelComponent);
public:
	OpenSim_DECLARE_PROPERTY(value, double, "Constant value.");
	OpenSim_DECLARE_OUTPUT(output, double, getValue, SimTK::Stage::Time);

	ConstantSource(double value) {
		constructProperty_value(value);
	}

	double getValue(const SimTK::State& s) const {
		return get_value();
	}
};

int main(int argc, char* argv[]) {
    try {
        Model model;
        // neuron
		auto neuron = new Neuron();
		neuron->set_C(200e-12);
		neuron->set_R(100e6);
		neuron->set_v_rest(-70e-3);
		neuron->set_v_threshold(-60e-3);
		model.addModelComponent(neuron);

		// current input
		auto constant = new ConstantSource(150e-12);
		model.addModelComponent(constant);
		neuron->connectInput_I(constant->getOutput("output"));

		// print state
		// ConsoleReporter* reporter = new ConsoleReporter();
		// reporter->set_report_time_interval(0.1);
		// reporter->addToReport(neuron->getOutput("v"));
		// model.addComponent(reporter);

        // simulation
        auto state = model.initSystem();
		Manager manager(model);
		manager.setIntegratorMaximumStepSize(0.001);
		// manager.setIntegratorMethod(Manager::IntegratorMethod::SemiExplicitEuler2);
		manager.initialize(state);
		manager.integrate(0.5);
		manager.getStateStorage().print("state.sto");
		model.print("output_model.osim");
    } catch (exception& e) {
        cout << e.what() << endl;
        PAUSE;
        return -1;
    }
	PAUSE;
    return 0;
}
