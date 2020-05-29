/**
 * @file tutorial.cpp
 *
 * \brief Build an OpenSim model of a single-legged hopping mechanism.
 *
 * The mechanism consists of a brick (pelvis) and two cylinders (thigh and
 * shank segments), all pin-connected, and is constrained to remain upright. A
 * contact sphere (foot) prevents the end of the lower cylinder from passing
 * through the floor. A vastus muscle wraps around a cylinder (patella) at the
 * knee.
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

void buildModel() {
    // Create a new OpenSim model.
    Model model;
    model.setName("Dennis");
    model.setGravity(Vec3(0, -9.80665, 0));

    // Create the pelvis body and attach the pelvis to ground with a vertical
    // slider joint.
#pragma region task_1a
    /*/
    double pelvisMass = 30, pelvisSideLength = 0.2;
    auto pelvisInertia = pelvisMass * Inertia::brick(Vec3(pelvisSideLength / 2));
    auto pelvis = new OpenSim::Body(...);
    auto pelvisMesh = new Mesh("cube.obj");
    pelvisMesh->setColor(Vec3(1, 0, 0));
    pelvisMesh->set_scale_factors(Vec3(pelvisSideLength));
    pelvis->attachGeometry(pelvisMesh);
    model.addBody(pelvis);

    Vec3 sliderOrientation(0, 0, SimTK::Pi / 2.);
    auto sliderToGround = new SliderJoint(...);
    model.addJoint(sliderToGround);
    //*/
#pragma endregion

    // Create the thigh body and define the pelvis as the parent so the
    // reported value is hip flexion using a pin joint.
#pragma region task_1b
    /*/
    double linkMass = 10, linkLength = 0.5, linkRadius = 0.035;
    auto linkInertia = linkMass * Inertia::cylinderAlongY(linkRadius, linkLength / 2);
    auto thigh = new OpenSim::Body("thigh", linkMass, Vec3(0), linkInertia);
    auto linkGeometry = new Cylinder(linkRadius, linkLength/2.);
    linkGeometry->setColor(Vec3(0, 1, 0));
    thigh->attachGeometry(linkGeometry);
    model.addBody(thigh);

    Vec3 linkDistalPoint(0, -linkLength / 2., 0);
    Vec3 linkProximalPoint(0, linkLength / 2., 0);
    auto hip = new PinJoint(...);
    model.addJoint(hip);
    //*/
#pragma endregion

    // Create the shank body and define the shank as the parent so the reported
    // value is knee flexion using a pin joint.
#pragma region task_1c
    /*/
    auto shank = new OpenSim::Body("shank", linkMass, Vec3(0), linkInertia);
    shank->attachGeometry(linkGeometry->clone());
    model.addBody(shank);

    auto knee = new PinJoint(...);
    model.addJoint(knee);
    //*/
#pragma endregion

    // Set the coordinate names, default values and range. Note that we need
    // "auto&" here so that we get a reference to the Coordinate rather than a
    // copy. Make use of convertDegreesToRadians().
    //
    // y - default: .9, range_min: 0, range_max: 2
    // hip_flexion - default: 40, range_min: -90, range_max: 110 (in rad)
    // knee_flexion - default: -80, range_min: -140, range_max: -10 (in rad)
#pragma region task_1d
    /*/
    auto& sliderCoord = sliderToGround->upd_coordinates(0);
    sliderCoord.setName("y");
    sliderCoord.setDefaultValue(.9);
    sliderCoord.setRangeMin(0);
    sliderCoord.setRangeMax(2);

    auto& hipCoord = hip->upd_coordinates(0);
    hipCoord.setName("hip_flexion");
    ...

    auto& kneeCoord = knee->upd_coordinates(0);
    kneeCoord.setName("knee_flexion");
    ...
    //*/
#pragma endregion

    // Limit the range of motion for the hip and knee joints.
    //
    // hip - range: [110, -90], stiffness: 20, damping: 5, transition: 10 (in
    // deg) knee - range: [-10, -140], stiffness: 50, damping: 2, transition:
    // 10 (in deg)
#pragma region task_2a
    /*/
    double hipRange[2] = {110, -90};
    double hipStiff[2] = {20, 20}, hipDamping = 5, hipTransition = 10;
    auto hipLimitForce = new CoordinateLimitForce(hipCoord.getName(), hipRange[0],
                                                  hipStiff[0], hipRange[1],
                                                  hipStiff[1], hipDamping,
                                                  hipTransition);
    hipLimitForce->setName("hip_flexion_limit_force");
    model.addForce(hipLimitForce);

    auto kneeLimitForce = new CoordinateLimitForce(...);
    ...
    //*/
#pragma endregion

    // Create a constraint to keep the foot (distal end of the shank) directly
    // beneath the pelvis (the Y-axis points upwards).
#pragma region task_2b
    /*/
    auto constraint = new PointOnLineConstraint(...);
    constraint->setName("point_on_line_constraint");
    model.addConstraint(constraint);
    //*/
#pragma endregion

    // Use a contact model to prevent the foot (ContactSphere) from passing
    // through the floor (ContactHalfSpace). The contact parameters
    // (HuntCrossleyForce::ContactParameters) are; stiffness: 1e8, dissipation:
    // 5, friction: {0, 0, 1}.
#pragma region task_2c
    /*/
    auto floor = new ContactHalfSpace(...);

    double footRadius = 0.1;
    auto foot = new ContactSphere(...);

    auto contactParams = new OpenSim::HuntCrossleyForce::ContactParameters(...);
    contactParams->addGeometry("floor");
    contactParams->addGeometry("foot");
    auto contactForce = new OpenSim::HuntCrossleyForce(contactParams);
    contactForce->setName("foot_floor_force");

    // Add the contact-related components to the model.
    model.addContactGeometry(floor);
    model.addContactGeometry(foot);
    model.addForce(contactForce);
    //*/
#pragma endregion

    // Create the vastus muscle and set its origin and insertion points.
    //
    // vastus - Fmax: 5000, optimal_fiber_length: 0.55, tendon_slack_length:
    // 0.25, pennation_angle: 0, origin_point: thigh, [linkRadius, 0.1, 0],
    // insertion_point: shank, [linkRadius, 0.1, 0]
#pragma region task_3a
    /*/
    auto vastus = new Thelen2003Muscle(...);
    vastus->addNewPathPoint(...);
    vastus->addNewPathPoint(...);
    model.addForce(vastus);
    //*/
#pragma endregion

    // Attach a cylinder (patella) to the distal end of the thigh over which the
    // vastus muscle can wrap.
    //
    // patella: radius: 0.08, length: 2 * radius, quadrant: x
#pragma region task_3b
    /*/
    auto patella = new WrapCylinder();
    patella->setName("patella");
    ...

    thigh->addWrapObject(patella);

    // Configure the vastus muscle to wrap over the patella.
    vastus->updGeometryPath().addPathWrap(*patella);
    //*/
#pragma endregion

    // Export and visualize model.
    model.setUseVisualizer(true);
    auto& state = model.initSystem();
    model.updMatterSubsystem().setShowDefaultGeometry(true);
    model.updVisualizer().updSimbodyVisualizer().setBackgroundColor(Vec3(1));
    model.updVisualizer().updSimbodyVisualizer().drawFrameNow(state);
    model.print(model.getName() + ".osim");
}

int main(int argc, char *argv[]) {
    try {
        buildModel();
        // simulateModel();
        // performHighJumpOptimization();
    } catch (exception &e) {
        cout << typeid(e).name() << ": " << e.what() << endl;
        PAUSE;
        return -1;
    }
    PAUSE;
    return 0;
}
