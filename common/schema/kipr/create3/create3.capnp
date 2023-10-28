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

struct AudioNote {
  frequency @0 :UInt16;
  duration @1 :Duration;
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
  playAudio @2 (notes :List(AudioNote), overwrite :Bool);

  dock @3 ();
  undock @4 ();

  driveDistance @5 (distance :Float32, maxLinearSpeed :Float32);
  driveArc @6 (direction :Int8, radius :Float32, angle :Float32, maxLinearSpeed :Float32);
  ledAnimation @7 (animationType: Int8, lightring: Lightring, duration: Duration);
  rotate @8 (angle :Float32, maxAngularSpeed :Float32);
  navigateTo @9 (pose :Pose, maxLinearSpeed :Float32, maxAngularSpeed :Float32, achieveGoalHeading :Bool);
  followWall @10 (follow :Int8, maxSeconds :Float32);

  getOdometry @11 () -> (pose :Pose, velocity :Twist);
}