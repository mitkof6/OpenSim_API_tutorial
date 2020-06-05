/**
 * @file PerturbationForce.h
 *
 * \brief An implementation of a body force that is intended for perturbation
 * analysis.
 *
 * @author Dimitar Stanev <jimstanev@gmail.com>
 */
#ifndef PERTURBATION_FORCE_H
#define PERTURBATION_FORCE_H

#include "PerturbationForceExports.h"

#include <OpenSim/Simulation/Model/Force.h>

namespace OpenSim {
/**
 * \brief Implements a body force of random direction and constant magnitude.
 */
class PerturbationForce_API PerturbationForce : public OpenSim::Force {
    OpenSim_DECLARE_CONCRETE_OBJECT(PerturbationForce, OpenSim::Force);

 public:
    OpenSim_DECLARE_PROPERTY(body_name, std::string,
                             "Body to apply the force.");
    OpenSim_DECLARE_PROPERTY(offset, SimTK::Vec3,
                             "Point of application in body frame.");
    OpenSim_DECLARE_PROPERTY(magnitude, double,
                             "Magnitude of the perturbation force.");
    PerturbationForce();

 protected:
    void computeForce(const SimTK::State& state,
                      SimTK::Vector_<SimTK::SpatialVec>& bodyForces,
                      SimTK::Vector& generalizedForces) const override;
    void extendConnectToModel(Model& model);
};
} // namespace OpenSim

#endif
