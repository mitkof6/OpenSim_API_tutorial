# Overview

1. *01_build_model*: demonstrates how to build a hopper model using the
  OpenSim API.
2. *02_run_simulation*: based on the model built in 1. we setup a
   simulation (forward dynamics or numerical integration).
3. *03_perform_optimization*: based on the model built in 1. we setup
   an optimization that tries to determine the muscle excitation that
   maximize the jump height of the hopper.
4. *04_perturbation_force*: demonstrates how one can create plugins in
   OpenSim, that extend the functionality and use this in Python or
   GUI.
5. *05_eye_fixation_controller*: demonstrates how one can create a
   custom controller as a plugin and use it in the GUI or API to
   control the excitation of the eye's muscles. Based on:
   https://gitlab.com/vvr/upat_eye_model
