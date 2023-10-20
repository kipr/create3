# `create3`

Holds a ROS2 C++ server for communicating with the create3 robot and an associated client library for communicating
with said server. The server can then be packaged into a docker image, allowing the client to remain free of ROS2 dependencies.

The RPC between client and server is handled by Cap'n Proto.

## Dependencies

### Common
  - C++ Compiler with C++17 support
  - CMake >3.5

### Server
  - ROS 2 

## Building

```bash
cd create3
cmake -Bbuild
cmake --build build
```

### Build Options

  - `-Dserver=ON|OFF` (Default: `ON`) - Controls whether the server is built.
  - `-Dclient=ON|OFF` (Default: `ON`) - Controls whether the client library is built.
  - `-Dcmder=ON|OFF` (Default: `ON`) - Controls whether the create3 CLI is built.
  - `-Dtests=ON|OFF` (Default: `ON`) - Controls whether tests are generated that can be run with `ctest`.

### Windows Support

To configure cmake for a windows build, replace `cmake -Bbuild` with:

```
cmake -Bbuild -GNinja -Dserver=OFF
```

The reminaing instructions are the same.

## Tests

To run unit tests, execute:

```
cd create3/build
ctest
```
