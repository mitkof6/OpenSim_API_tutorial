#include "PerturbationForce.h"

#include <OpenSim/Simulation/Model/Model.h>

using namespace OpenSim;

double randNumber() {
    double min = -1, max = 1;
    return (max - min) * ((double) rand() / (double) RAND_MAX) + min;
}

PerturbationForce::PerturbationForce() {
    constructProperty_body_name("unassigned");
    constructProperty_offset(SimTK::Vec3(0));
    constructProperty_magnitude(0);
}

void PerturbationForce::computeForce(
        const SimTK::State& state,
        SimTK::Vector_<SimTK::SpatialVec>& bodyForces,
        SimTK::Vector& generalizedForces) const {
    auto& body = _model->updBodySet().get(get_body_name());
    auto perturbationForceInG =
            get_magnitude() *
            SimTK::Vec3(randNumber(), randNumber(), randNumber()).normalize();
    applyForceToPoint(state, body, get_offset(), perturbationForceInG,
                      bodyForces);
}

void PerturbationForce::extendConnectToModel(Model& model) {
    Super::extendConnectToModel(model);
    if (!model.updBodySet().contains(get_body_name())) {
        auto errorMessage = "Invalid body_name:" + get_body_name();
        throw Exception(errorMessage.c_str());
    }
}
