# `create3`

Holds a ROS2 C++ server for communicating with the create3 robot and an associated client library for communicating with said server.

The RPC between client and server is handled by Cap'n Proto.

## Building

```bash
mkdir build
cd build
cmake ..
make -j4
```