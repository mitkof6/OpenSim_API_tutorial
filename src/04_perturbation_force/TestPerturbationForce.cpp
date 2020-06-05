/**
 * @file TestPerturbationForce.h
 *
 * \brief Tests the PerturbationForce plugin on the tug of war model.
 *
 * @author Dimitar Stanev <jimstanev@gmail.com>
 */
#include "PerturbationForce.h"

#include <OpenSim/OpenSim.h>
#include <iostream>

using namespace std;
using namespace OpenSim;

#define PAUSE                                                                  \
    cout << endl << "press a key to continue ..." << endl;                     \
    getchar();

int main(int argc, char* argv[]) {
    try {
        Model model("tug_of_war.osim");
        model.setUseVisualizer(true);
        // force
        auto perturbationForce = new PerturbationForce();
        perturbationForce->setName("noise");
        perturbationForce->set_body_name("block");
        perturbationForce->set_offset(SimTK::Vec3(0, 0, 0));
        perturbationForce->set_magnitude(1000);
        model.addForce(perturbationForce);
        // simulation
        auto state = model.initSystem();
        simulate(model, state, 1, true);
        // output
        model.print("output_model.osim");
    } catch (exception& e) {
        cout << e.what() << endl;
        PAUSE;
        return -1;
    }
	PAUSE;
    return 0;
}
