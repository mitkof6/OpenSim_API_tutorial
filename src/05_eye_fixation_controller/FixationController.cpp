#include "FixationController.h"
#include <OpenSim/Simulation/Model/Model.h>
#include <OpenSim/Simulation/Model/Muscle.h>

using namespace std;
using namespace OpenSim;
using namespace SimTK;

/**
 * Implementation of smooth sigmoid function.
 *
 * Parameters
 * ----------
 * t : time to be evaluated
 * t0 : delay
 * a : magnitude
 * b : slope
 *
 * Returns
 * -------
 * (y, y', y'')
 */
Vec3 sigmoid(double t, double t0, double a, double b) {
    return Vec3(a * (tanh(b * (t - t0)) + 1) / 2,
                -(a * b * (pow(tanh(b * (t - t0)), 2) - 1)) / 2,
                a * pow(b, 2) * tanh(b * (t - t0)) * (pow(tanh(b * (t - t0)), 2) - 1)
    );
}

FixationController::FixationController() : Controller() {
    constructProperties();
}

void FixationController::constructProperties() {
    constructProperty_thetaH(-15);
    constructProperty_thetaV(0);
    constructProperty_kpH(50);
    constructProperty_kdH(1.5);
    constructProperty_kpV(50);
    constructProperty_kdV(1.5);
    constructProperty_kpT(100);
    constructProperty_kdT(0.5);
    constructProperty_saccade_onset(0.5);
    constructProperty_saccade_velocity(600);
}

void FixationController::computeControls(const State& s, Vector& controls) const {
    double t = s.getTime();

    // Get model coordinate
    const auto& yCoord = _model->getCoordinateSet().get("r_eye_add_abd");
    const auto& zCoord = _model->getCoordinateSet().get("r_eye_sup_inf");
    const auto& xCoord = _model->getCoordinateSet().get("r_eye_inc_exc");

    // Get model muscles
    const auto& lat_rect = _model->getMuscles().get("r_Lateral_Rectus");
    const auto& med_rect = _model->getMuscles().get("r_Medial_Rectus");
    const auto& sup_rect = _model->getMuscles().get("r_Superior_Rectus");
    const auto& inf_rect = _model->getMuscles().get("r_Inferior_Rectus");
    const auto& sup_oblq = _model->getMuscles().get("r_Superior_Oblique");
    const auto& inf_oblq = _model->getMuscles().get("r_Inferior_Oblique");

    // Compute the desired position, velocity and acceleration
    auto thetaH = convertDegreesToRadians(get_thetaH());
    auto thetaV = convertDegreesToRadians(get_thetaV());
    auto desSaccadeVelocity = convertDegreesToRadians(get_saccade_velocity());
    double xdes = 0;
    double xdesv = 0;
    double xdesa = 0;

    auto sigmY = sigmoid(t,
                         get_saccade_onset(),
                         thetaH,
                         2 * desSaccadeVelocity / abs(thetaH));
    // auto sigmY = sigmoid(t,
    //                      get_saccade_onset(),
    //                      thetaH,
    //                      2 * desSaccadeVelocity / abs(thetaH))
    //   + sigmoid(t - 1,
    //             get_saccade_onset(),
    //             -thetaH, 2 * desSaccadeVelocity / abs(thetaH));

    double ydes = isnan(sigmY[0]) ? 0 : sigmY[0];
    double ydesv = isnan(sigmY[1]) ? 0 : sigmY[1];
    double ydesa = isnan(sigmY[2]) ? 0 : sigmY[2];

    auto sigmZ = sigmoid(t,
                         get_saccade_onset(),
                         thetaV,
                         2 * desSaccadeVelocity / abs(thetaV));
    // auto sigmZ = sigmoid(t,
    //                      get_saccade_onset(),
    //                      thetaV - thetaV, 2 * desSaccadeVelocity / abs(thetaV))
    //   + sigmoid(t - 2,
    //             get_saccade_onset(),
    //             thetaV, 2 * desSaccadeVelocity / abs(thetaV))
    //   + sigmoid(t - 3,
    //             get_saccade_onset(),
    //             -thetaV, 2 * desSaccadeVelocity / abs(thetaV));

    double zdes = isnan(sigmZ[0]) ? 0 : sigmZ[0];
    double zdesv = isnan(sigmZ[1]) ? 0 : sigmZ[1];
    double zdesa = isnan(sigmZ[2]) ? 0 : sigmZ[2];

    // Get the current position and velocity
    double x = xCoord.getValue(s); double xv = xCoord.getSpeedValue(s);
    double y = yCoord.getValue(s); double yv = yCoord.getSpeedValue(s);
    double z = zCoord.getValue(s); double zv = zCoord.getSpeedValue(s);

    // The sum of errors are used as Excitation levels in the model
    // acceleration is unused because causes problems
    double sumErrX = 0 * xdesa + get_kpT() * (xdes - x) + get_kdT() * (xdesv - xv);
    double sumErrY = 0 * ydesa + get_kpH() * (ydes - y) + get_kdH() * (ydesv - yv);
    double sumErrZ = 0 * zdesa + get_kpV() * (zdes - z) + get_kdV() * (zdesv - zv);

    // If desired force is in direction of one muscle's pull direction, then
    // set that muscle's control based on the position and velocity error,
    // otherwise set the muscle's control to zero
    double leftControl = 0.0, rightControl = 0.0;
    if (sumErrY < 0) {
        leftControl = abs(sumErrY);
        rightControl = 0.0;
    } else if (sumErrY > 0) {
        leftControl = 0.0;
        rightControl = abs(sumErrY);
    }
    double upControl = 0.0, downControl = 0.0;
    if (sumErrZ > 0) {
        upControl = abs(sumErrZ);
        downControl = 0.0;
    } else if (sumErrZ < 0) {
        upControl = 0.0;
        downControl = abs(sumErrZ);
    }
    double upTorControl = 0.0, downTorControl = 0.0;
    if (sumErrX < 0) {
        upTorControl = abs(sumErrX);
        downTorControl = 0.0;
    } else if (sumErrX > 0) {
        upTorControl = 0.0;
        downTorControl = abs(sumErrX);
    }

    // Set the activation inputs to the model Millard muscle has only one
    // control
    Vector muscleControl(1, leftControl); // left control -> Lateral Rectus
    lat_rect.addInControls(muscleControl, controls);

    muscleControl[0] = rightControl; // right control -> Medial Rectus
    med_rect.addInControls(muscleControl, controls);

    muscleControl[0] = upControl; // Up control -> Superior Rectus
    sup_rect.addInControls(muscleControl, controls);

    muscleControl[0] = downControl; // Down control -> Inferior Rectus
    inf_rect.addInControls(muscleControl, controls);

    muscleControl[0] = upTorControl; // Up Torsion Control -> Superior Oblique
    sup_oblq.addInControls(muscleControl, controls);

    muscleControl[0] = downTorControl; // Down Torsion Control -> Inferior Oblique
    inf_oblq.addInControls(muscleControl, controls);
}