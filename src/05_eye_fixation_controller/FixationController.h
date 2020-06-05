/**
 * @file FixationController.h
 *
 * \brief A fixation controller for the eye model. The controller accepts the
 * horizontal and vertical target angle and computes the muscle excitation
 * required to track the desired motion.
 *
 * @author Konstantinos Filip <filipconstantinos@gmail.com>
 *         Dimitar Stanev     <jimstanev@gmail.com>
 *
 * @see <a href="https://simtk.org/projects/eye">[SimTK Project]</a>, <a
 * href="https://arxiv.org/abs/1807.07332">[Publication]</a>
 */
#ifndef FIXATION_CONTROLLER_H
#define FIXATION_CONTROLLER_H

#include "FixationControllerExports.h"
#include <OpenSim/Simulation/Control/Controller.h>

namespace OpenSim {
    /**
     * \brief Eye fixation controller.
     */
    class FixationController_API FixationController : public Controller {
        OpenSim_DECLARE_CONCRETE_OBJECT(FixationController, Controller);
    public:
        OpenSim_DECLARE_PROPERTY(thetaH, double, "fixation target angle horizontal axis (in degrees)");
        OpenSim_DECLARE_PROPERTY(thetaV, double, "fixation target angle vertical axis (in degrees)");
        OpenSim_DECLARE_PROPERTY(kpH, double, "horizontal position tracking gain");
        OpenSim_DECLARE_PROPERTY(kdH, double, "horizontal velocity tracking gain");
        OpenSim_DECLARE_PROPERTY(kpV, double, "vertical position tracking gain");
        OpenSim_DECLARE_PROPERTY(kdV, double, "vertical velocity tracking gain");
        OpenSim_DECLARE_PROPERTY(kpT, double, "torsion position tracking gain");
        OpenSim_DECLARE_PROPERTY(kdT, double, "torsion velocity tracking gain");
        OpenSim_DECLARE_PROPERTY(saccade_onset, double, "saccade onset (s)");
        OpenSim_DECLARE_PROPERTY(saccade_velocity, double, "desired saccade velocity (deg / s)");
        /** Constructor. */
        FixationController();
        /** Distractor. */
        ~FixationController() {};
        /** Required to initialize controller properties. */
        void constructProperties();
        /** Controller::computeControls. */
        void computeControls(const SimTK::State& s, SimTK::Vector& controls)
            const override;
    };
}

#endif
