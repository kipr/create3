@0xec6fcdd60a9ac7d4;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("kipr::create3");

struct Twist {
  linearX @0 :Float64;
  angularZ @1 :Float64;
}

struct Duration {
  seconds @0 :Int32;
  nanoseconds @1 :UInt32;
}

struct LedColor {
  r @0 :UInt8;
  g @1 :UInt8;
  b @2 :UInt8;
}

struct Lightring {
  led0 @0 :LedColor;
  led1 @1 :LedColor;
  led2 @2 :LedColor;
  led3 @3 :LedColor;
  led4 @4 :LedColor;
  led5 @5 :LedColor;
}

struct HazardDetection {
  type @0 :UInt8;
  frameId @1 :Text;
  timestamp @2 :Float64;
}

struct IrIntensity {
  frameId @0 :Text;
  timestamp @1 :Float64;
  intensity @2 :Int16;
}

struct Quaternion {
  x @0 :Float64;
  y @1 :Float64;
  z @2 :Float64;
  w @3 :Float64;
}

struct Vector3 {
  x @0 :Float64;
  y @1 :Float64;
  z @2 :Float64;
}

struct Pose {
  position @0 :Vector3;
  orientation @1 :Quaternion;
}

interface Create3 {
  isConnected @0 () -> (connected :Bool);
  setVelocity @1 (velocity :Twist);

  dock @2 ();
  undock @3 ();

  driveDistance @4 (distance :Float32, maxLinearSpeed :Float32);
  driveArc @5 (direction :Int8, radius :Float32, angle :Float32, maxLinearSpeed :Float32);
  ledAnimation @6 (animationType: Int8, lightring: Lightring, duration: Duration);
  rotate @7 (angle :Float32, maxAngularSpeed :Float32);
  navigateTo @8 (pose :Pose, maxLinearSpeed :Float32, maxAngularSpeed :Float32, achieveGoalHeading :Bool);
  followWall @9 (follow :Int8, maxSeconds :Float32);

  getCliffIntensityVector @10 () -> (cliffIntensityVector :List(IrIntensity));
  getHazardDetectionVector @11 () -> (hazardDetectionVector :List(HazardDetection));
  getIrIntensityVector @12 () -> (irIntensityVector :List(IrIntensity));
  getOdometry @13 () -> (pose :Pose, velocity :Twist);
}