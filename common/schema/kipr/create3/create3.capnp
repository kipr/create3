@0xec6fcdd60a9ac7d4;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("kipr::create3");

struct Twist {
  linearX @0 :Float64;
  angularZ @1 :Float64;
}

struct AudioNote {
  frequency @0 :UInt16;
  seconds @1 :Float64;
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

  driveStraight @5 (distance :Float32, maxLinearSpeed :Float32);
  driveArc @6 (direction :Int8, radius :Float32, angle :Float32, maxLinearSpeed :Float32);
  rotate @7 (angle :Float32, maxAngularSpeed :Float32);
  navigateTo @8 (pose :Pose, maxLinearSpeed :Float32, maxAngularSpeed :Float32, achieveGoalHeading :Bool);
  followWall @9 (follow :Int8, maxSeconds :Float32);

  getOdometry @10 () -> (pose :Pose, velocity :Twist);
}