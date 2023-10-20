@0xec6fcdd60a9ac7d4;

struct Twist {
  linearX @0 :Float64;
  angularZ @1 :Float64;
}

interface Create3 {
  isConnected @0 () -> (connected :Bool);
  setVelocity @1 (twist :Twist);
}