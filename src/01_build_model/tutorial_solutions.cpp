/**
 * @file tutorial_solutions.cpp
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
#include <OpenSim/OpenSim.h>
#include <iostream>

using namespace std;
using namespace OpenSim;
using namespace SimTK;

// Used to pause the flow of the program.
#define PAUSE                                                                  \
    cout << endl << "press a key to continue ..." << endl;                     \
    getchar();

void buildModel() {
    // Create a new OpenSim model.
    Model model;
    model.setName("Dennis");
    model.setGravity(Vec3(0, -9.80665, 0));

    // Create the pelvis body and attach the pelvis to ground with a vertical
    // slider joint.
#pragma region task_1a
    //*/
    double pelvisMass = 30, pelvisSideLength = 0.2;
    auto pelvisInertia =
            pelvisMass * Inertia::brick(Vec3(pelvisSideLength / 2));
    auto pelvis =
            new OpenSim::Body("pelvis", pelvisMass, Vec3(0), pelvisInertia);
    auto pelvisMesh = new Mesh("cube.obj");
    pelvisMesh->setColor(Vec3(1, 0, 0));
    pelvisMesh->set_scale_factors(Vec3(pelvisSideLength));
    pelvis->attachGeometry(pelvisMesh);
    model.addBody(pelvis);

    Vec3 sliderOrientation(0, 0, SimTK::Pi / 2.);
    auto sliderToGround = new SliderJoint("slider", model.getGround(), Vec3(0),
                                          sliderOrientation, *pelvis, Vec3(0),
                                          sliderOrientation);
    model.addJoint(sliderToGround);
    //*/
#pragma endregion

    // Create the thigh body and define the pelvis as the parent so the
    // reported value is hip flexion using a pin joint.
#pragma region task_1b
    //*/
    double linkMass = 10, linkLength = 0.5, linkRadius = 0.035;
    auto linkInertia =
            linkMass * Inertia::cylinderAlongY(linkRadius, linkLength / 2);
    auto thigh = new OpenSim::Body("thigh", linkMass, Vec3(0), linkInertia);
    auto linkGeometry = new Cylinder(linkRadius, linkLength / 2.);
    linkGeometry->setColor(Vec3(0, 1, 0));
    thigh->attachGeometry(linkGeometry);
    model.addBody(thigh);

    Vec3 linkDistalPoint(0, -linkLength / 2., 0);
    Vec3 linkProximalPoint(0, linkLength / 2., 0);
    auto hip = new PinJoint("hip", *pelvis, Vec3(0), Vec3(0), *thigh,
                            linkProximalPoint, Vec3(0));
    model.addJoint(hip);
    //*/
#pragma endregion

    // Create the shank body and define the shank as the parent so the reported
    // value is knee flexion using a pin joint.
#pragma region task_1c
    //*/
    auto shank = new OpenSim::Body("shank", linkMass, Vec3(0), linkInertia);
    shank->attachGeometry(linkGeometry->clone());
    model.addBody(shank);

    auto knee = new PinJoint("knee", *thigh, linkDistalPoint, Vec3(0), *shank,
                             linkProximalPoint, Vec3(0));
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
    //*/
    auto& sliderCoord = sliderToGround->upd_coordinates(0);
    sliderCoord.setName("y");
    sliderCoord.setDefaultValue(.9);
    sliderCoord.setRangeMin(0);
    sliderCoord.setRangeMax(2);

    auto& hipCoord = hip->upd_coordinates(0);
    hipCoord.setName("hip_flexion");
    hipCoord.setDefaultValue(convertDegreesToRadians(40));
    hipCoord.setRangeMin(convertDegreesToRadians(-90));
    hipCoord.setRangeMax(convertDegreesToRadians(110));

    auto& kneeCoord = knee->upd_coordinates(0);
    kneeCoord.setName("knee_flexion");
    kneeCoord.setDefaultValue(convertDegreesToRadians(-80));
    kneeCoord.setRangeMin(convertDegreesToRadians(-140));
    kneeCoord.setRangeMax(convertDegreesToRadians(-10));
    //*/
#pragma endregion
    // Limit the range of motion for the hip and knee joints.
    //
    // hip - range: [110, -90], stiffness: 20, damping: 5, transition: 10 (in
    // deg) knee - range: [-10, -140], stiffness: 50, damping: 2, transition:
    // 10 (in deg)
#pragma region task_2a
    //*/

    double hipRange[2] = {110, -90};
    double hipStiff[2] = {20, 20}, hipDamping = 5, hipTransition = 10;
    auto hipLimitForce = new CoordinateLimitForce(
            hipCoord.getName(), hipRange[0], hipStiff[0], hipRange[1],
            hipStiff[1], hipDamping, hipTransition);
    hipLimitForce->setName("hip_flexion_limit_force");
    model.addForce(hipLimitForce);

    double kneeRange[2] = {-10, -140};
    double kneeStiff[2] = {50, 50}, kneeDamping = 2, kneeTransition = 10;
    auto kneeLimitForce = new CoordinateLimitForce(
            kneeCoord.getName(), kneeRange[0], kneeStiff[0], kneeRange[1],
            kneeStiff[1], kneeDamping, kneeTransition);
    kneeLimitForce->setName("knee_flexion_limit_force");
    model.addForce(kneeLimitForce);
    //*/
#pragma endregion

    // Create a constraint to keep the foot (distal end of the shank) directly
    // beneath the pelvis (the Y-axis points upwards).
#pragma region task_2b
    //*/
    auto constraint = new PointOnLineConstraint(
            model.getGround(), Vec3(0, 1, 0), Vec3(0), *shank, linkDistalPoint);
    constraint->setName("point_on_line_constraint");
    model.addConstraint(constraint);
    //*/
#pragma endregion

    // Use a contact model to prevent the foot (ContactSphere) from passing
    // through the floor (ContactHalfSpace). The contact parameters
    // (HuntCrossleyForce::ContactParameters) are; stiffness: 1e8, dissipation:
    // 5, friction: {0, 0, 1}.
#pragma region task_2c
    //*/
    auto floor = new ContactHalfSpace(Vec3(0), Vec3(0, 0, -SimTK::Pi / 2.),
                                      model.getGround(), "floor");

    double footRadius = 0.1;
    auto foot = new ContactSphere(footRadius, linkDistalPoint, *shank, "foot");

    double stiffness = 1e8, dissipation = 5, friction[3] = {0.0, 0.0, 1};
    auto contactParams = new OpenSim::HuntCrossleyForce::ContactParameters(
            stiffness, dissipation, friction[0],

            friction[1], friction[2]);
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
    //*/
    double Fmax = 5000, optFibLen = 0.55, tendonSlackLen = 0.25, pennAng = 0;
    auto vastus = new Thelen2003Muscle("vastus", Fmax, optFibLen,
                                       tendonSlackLen, pennAng);
    vastus->addNewPathPoint("origin", *thigh, Vec3(linkRadius, 0.10, 0));
    vastus->addNewPathPoint("insertion", *shank, Vec3(linkRadius, 0.10, 0));
    model.addForce(vastus);
    //*/
#pragma endregion

    // Attach a cylinder (patella) to the distal end of the thigh over which the
    // vastus muscle can wrap.
    //
    // patella: radius: 0.08, length: 2 * radius, quadrant: x
#pragma region task_3b
    //*/
    auto patella = new WrapCylinder();
    patella->setName("patella");
    patella->set_radius(0.08);
    patella->set_length(linkRadius * 2);
    patella->set_quadrant("x");
    patella->set_translation(linkDistalPoint);
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

int main(int argc, char* argv[]) {
    try {
        buildModel();
    } catch (exception& e) {
        cout << typeid(e).name() << ": " << e.what() << endl;
        PAUSE;
        return -1;
    }
    PAUSE;
    return 0;
}
