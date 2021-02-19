/**
 * @file tutorial_solutions.cpp
 *
 * \brief Perform an Optimization of controls using OpenSim model (build in 01)
 * of a single-legged hopping mechanism.
 *
 * @author Dimitar Stanev <jimstanev@gmail.com>
 */
#include <OpenSim/OpenSim.h>
#include <iostream>

using namespace std;
using namespace OpenSim;
using namespace SimTK;

// Used to pause the flow of the program.
#define PAUSE                                                                  \
    cout << endl << "press a key to continue ..." << endl;                     \
    getchar();

class HighJumpOptimization : public OptimizerSystem {
 public:
    HighJumpOptimization(int numParameters, double endTime)
            : OptimizerSystem(numParameters), endTime(endTime) {
        // Setup model.
        model = Model("Dennis.osim");

        // Add force and body kinematics analyses in order to record the forces
        // and body kinematics, respectively.
        forceReporter = new ForceReporter();
        model.addAnalysis(forceReporter);
        bodyKinematics = new BodyKinematics();
        model.addAnalysis(bodyKinematics);

        // Create a controller to excite the vastus muscle.
        controller = new PrescribedController();
        controller->setActuators(model.updActuators());
        controller->setName("brain");
        model.addController(controller);

        // Initialize model and equilibrate muscles.
        state = model.initSystem();
        model.equilibrateMuscles(state);

        // Partition the time uniformly based on the number of parameters and
        // final time.
        for (int i = 0; i < numParameters; i++) {
            timePoints.push_back(endTime / numParameters * i);
        }
    }

    int objectiveFunc(const Vector& newControls, bool new_coefficients,
                      Real& f) const {
        // Initialization
        auto workingState = state;
        forceReporter->updForceStorage().reset(0);
        bodyKinematics->getPositionStorage()->reset(0);
        bodyKinematics->getAccelerationStorage()->reset(0);
        bodyKinematics->getVelocityStorage()->reset(0);

        // Update controller from newControls.
#pragma region task_5a
        //*/
        int N = getNumParameters();
        auto controlFunction = new PiecewiseConstantFunction(N, &timePoints[0],
                                                             &newControls[0]);
        controller->prescribeControlForActuator("vastus", controlFunction);
        //*/
#pragma endregion

        // Create the manager for the simulation and perform the
        // simulation.
#pragma region task_5b
        //*/
        Manager manager(model);
        manager.initialize(workingState);
        manager.integrate(endTime);
        //*/
#pragma endregion

        // Evaluate objective function, which is to maximize the jump height.
        // OptimizerSystem assumese that the objective function is minimized.
#pragma region task_5c
        //*/
        auto bodyPositions = bodyKinematics->getPositionStorage();
        Array<double> CoMY;
        bodyPositions->getDataColumn("center_of_mass_Y", CoMY);

        double maxValue = 0;
        for (int i = 0; i < CoMY.getSize(); i++) {
            if (CoMY[i] > 0 && CoMY[i] > maxValue) { maxValue = CoMY[i]; }
        }
        f = -1 * maxValue;
        //*/
#pragma endregion

        // Use an if statement to only store and print the results of an
        // optimization step if it is better than a previous result.
        if (f < bestSolution) {
            bestSolution = f;
            cout << "objective evaluation: " << stepCount << endl
                 << "controls: " << newControls << endl
                 << "best: " << -f << std::endl;

            model.print(model.getName() + "_Best_Par.osim");
            manager.getStateStorage().print(model.getName() +
                                            "_States_Best_Par.sto");
        }
        stepCount++;
        return 0;
    }

 private:
    mutable Model model;
    mutable ForceReporter* forceReporter;
    mutable BodyKinematics* bodyKinematics;
    mutable PrescribedController* controller;
    State state;
    vector<double> timePoints;
    mutable double bestSolution = Infinity, endTime;
    mutable int stepCount = 0;
};

void performHighJumpOptimization() {
    // Initialize the optimizer system we've defined. Set the upper and lower
    // bounds.
#pragma region task_6a
    //*/
    int N = 5;
    double tf = 1.5;
    HighJumpOptimization optimizationSystem(N, tf);

    Vector lowerBounds(N, 0.01);
    Vector upperBounds(N, 1.0);
    optimizationSystem.setParameterLimits(lowerBounds, upperBounds);
    //*/
#pragma endregion

    // Create an optimizer. Pass in our OptimizerSystem and the name of the
    // optimization algorithm.
#pragma region task_6b
    //*/
    // Optimizer opt(sys, SimTK::LBFGSB);
    Optimizer optimizer(optimizationSystem, SimTK::CMAES);

    // Specify settings for the optimizer.
    optimizer.useNumericalGradient(true);
    optimizer.setMaxIterations(100);

    // Optimize.
    Vector solution(N, 0.01);
    Real f = optimizer.optimize(solution);
    //*/
#pragma endregion

    cout << endl
         << "optimization finished" << endl
         << "maximum CoM = " << -f << " m" << endl
         << solution << endl;
}

int main(int argc, char* argv[]) {
    try {
        performHighJumpOptimization();
    } catch (exception& e) {
        cout << typeid(e).name() << ": " << e.what() << endl;
        PAUSE;
        return -1;
    }
    PAUSE;
    return 0;
}
