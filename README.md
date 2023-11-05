# `iRobot® Create® 3 robot ROS2 server and client library`

Holds a ROS2 C++ server for communicating with the iRobot® Create® 3 robot and an associated client library for communicating
with said server. The server can then be packaged into a Docker image, allowing the client to remain free of ROS2 dependencies.

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

For most scenarios:
  - Use `-Dserver=OFF` for the client (eg. dev machine)
  - Use `-Dclient=OFF -Dcmder=OFF` for the server (eg. Docker image)

Use `--parallel <n>` to build with multiple threads in `cmake --build build` step.

### Windows Support

To configure cmake for a windows build, replace `cmake -Bbuild` with:

```
cmake -Bbuild -GNinja -Dserver=OFF
```

The reminaing instructions are the same.


## Examples

Examples are configured to connect to a KIPR Wombat running the docker instance of the server. Users may want to change the IP address in the examples to match their own setup. 

Examples may be run after building by executing:

```bash
cd create3/build
./examples/<example_name>
```

## Tests

To run unit tests, execute:

```bash
cd create3/build
ctest
```
