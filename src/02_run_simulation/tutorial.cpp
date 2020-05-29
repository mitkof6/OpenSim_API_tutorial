/**
 * @file tutorial.cpp
 *
 * \brief Simulate an OpenSim model (build in 01) of a single-legged hopping
 * mechanism.
 *
 * A controller (brain) generates the muscle excitation signal
 * required to produce a small hop when the mechanism is simulated.
 *
 * @author Dimitar Stanev <jimstanev@gmail.com>
 */
#include <iostream>
#include <OpenSim/OpenSim.h>

using namespace std;
using namespace OpenSim;
using namespace SimTK;

// Used to pause the flow of the program.
#define PAUSE cout << endl << "press a key to continue ..." << endl; getchar();

void simulateModel() {
    // Load the model
    Model model("Dennis.osim");

    // Add ForceReporter and BodyKinematics analyses in order to record the
    // forces and body kinematics, respectively.
#pragma region task_4a
    /*/
    auto forceReporter = new ForceReporter();
    ...
        
    auto bodyKinematics = new BodyKinematics();
    ...
    //*/
#pragma endregion

    // Create a controller to excite the vastus muscle. Provide one of the
    // following functions (PiecewiseConstantFunction, PiecewiseLinearFunction
    // and SimmSpline) to the controller.
#pragma region task_4b
    /*/
    auto brain = new PrescribedController();
    brain->setActuators(model.updActuators());
    double t[3] = {...}, x[3] = {...};
    auto controlFunction = ...
    brain->prescribeControlForActuator("vastus", controlFunction);
    brain->setName("brain");
    model.addController(brain);
    //*/
#pragma endregion

    // Build and initialize model.
    model.setUseVisualizer(true);
    auto& state = model.initSystem();
    model.updMatterSubsystem().setShowDefaultGeometry(true);
    model.updVisualizer().updSimbodyVisualizer().setBackgroundColor(Vec3(1));
    model.updVisualizer().updSimbodyVisualizer().drawFrameNow(state);
    model.equilibrateMuscles(state);

    // Create the manager for the numerical integration.
    Manager manager(model);
    manager.initialize(state);
    manager.integrate(5);

    // Save simulation results.
    model.printControlStorage(model.getName() + "_Controls.sto");
    manager.getStateStorage().print(model.getName() + "_States.sto");
    // forceReporter->printResults(model.getName());
    // bodyKinematics->printResults(model.getName());
}

int main(int argc, char *argv[]) {
    try {
        simulateModel();
    } catch (exception &e) {
        cout << typeid(e).name() << ": " << e.what() << endl;
        PAUSE;
        return -1;
    }
    PAUSE;
    return 0;
}
