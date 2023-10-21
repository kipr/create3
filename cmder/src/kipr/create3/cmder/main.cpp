#include <stdlib.h>
#include <kipr/create3/client/Client.hpp>
#include <kipr/create3/client/Vector3.hpp>
#include <kipr/create3/client/Quaternion.hpp>

#include <clipp.h>
#include <iostream>
#include <string>
#include <thread>

using namespace kipr::create3::client;

std::pair<std::string, std::uint16_t> parseAddress(const std::string &address, const std::uint16_t default_port)
{
  auto sep = address.find(':');
  if(sep == std::string::npos)
  {
    return std::make_pair(address, default_port);
  }
  else
  {
    return std::make_pair(address.substr(0, sep), std::stoi(address.substr(sep + 1)));
  }  
}
  
// Extract program name from argv[0] without path, handling \\ on Windows
std::string programName(const char *const argv0)
{
  std::string program_path(argv0);
  auto last_slash = program_path.find_last_of('/');
  if (last_slash != std::string::npos)
  {
    program_path = program_path.substr(last_slash + 1);
  }
  auto last_backslash = program_path.find_last_of('\\');
  if (last_backslash != std::string::npos)
  {
    program_path = program_path.substr(last_backslash + 1);
  }
  return program_path;
}

enum class Mode
{
  SetVelocity,
  Undock,
  Dock,
  Odometry,
  NavigateTo,
  Help
};

struct AutoWaitClient : public Client
{
  AutoWaitClient(const std::string &host, const std::uint16_t port)
    : Client(host, port)
  {
    while (!isConnected())
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  ~AutoWaitClient()
  {
    wait();
  }
};

int main(int argc, char *argv[])
{
  using namespace clipp;

  std::string address = "localhost:50051";

  Mode mode = Mode::Help;
  double linear_x = 0.0;
  double angular_z = 0.0;

  double navigate_to_position_x = 0.0;
  double navigate_to_position_y = 0.0;
  double navigate_to_position_z = 0.0;

  auto cli = (
    command("help").set(mode, Mode::Help) % "Print the man page" |
    (
      group(),
      option("--address", "-a") & value("host:port") % "The server's address" >> set(address),
      (
        (command("set_velocity") >> set(mode, Mode::SetVelocity), value("linear_x") >> set(linear_x), value("angular_z") >> set(angular_z)) |
        (command("undock") >> set(mode, Mode::Undock)) |
        (command("dock") >> set(mode, Mode::Undock)) |
        (command("odometry") >> set(mode, Mode::Odometry)) |
        (command("navigate_to") >> set(mode, Mode::NavigateTo), value("x") >> set(navigate_to_position_x), value("y") >> set(navigate_to_position_y), value("z") >> set(navigate_to_position_z))
      )
    )
  );

  
  if (!parse(argc, argv, cli))
  {
     std::cout << usage_lines(cli, programName(argv[0])) << std::endl;
     return EXIT_SUCCESS;
  }

  const auto init_client = [&address]() {
    auto [host, port] = parseAddress(address, 50051);
    return AutoWaitClient(host, port);
  };

  switch (mode)
  {
    case Mode::Help:
      std::cout << make_man_page(cli, programName(argv[0])) << std::endl;
      break;
    case Mode::SetVelocity:
      init_client().setVelocity(Twist {
        .linear_x = linear_x,
        .angular_z = angular_z
      });
      break;
    case Mode::Dock:
      init_client().dock();
      break;
    case Mode::Undock:
      init_client().undock();
      break;
    case Mode::Odometry:
    {
      const auto odometry = init_client().getOdometry();

      std::cout << "Odometry:" << std::endl;
      std::cout << "  Position:" << std::endl;
      std::cout << "    x: " << odometry.pose.position.x << std::endl;
      std::cout << "    y: " << odometry.pose.position.y << std::endl;
      std::cout << "    z: " << odometry.pose.position.z << std::endl;
      std::cout << "  Orientation:" << std::endl;
      std::cout << "    x: " << odometry.pose.orientation.x << std::endl;
      std::cout << "    y: " << odometry.pose.orientation.y << std::endl;
      std::cout << "    z: " << odometry.pose.orientation.z << std::endl;
      std::cout << "    w: " << odometry.pose.orientation.w << std::endl;
      std::cout << "  Linear Velocity:" << std::endl;
      std::cout << "    x: " << odometry.velocity.linear_x << std::endl;
      std::cout << "  Angular Velocity:" << std::endl;
      std::cout << "    z: " << odometry.velocity.angular_z << std::endl;

      break;
    }
    case Mode::NavigateTo:
      init_client().navigateTo(Pose {
        .position = Vector3 {
          .x = navigate_to_position_x,
          .y = navigate_to_position_y,
          .z = navigate_to_position_z
        },
      }, 0.5, 0.5, false);
      break;
  }
  
  return EXIT_SUCCESS;
}