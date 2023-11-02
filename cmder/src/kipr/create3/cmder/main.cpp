#include <stdlib.h>
#include <kipr/create3/client/Client.hpp>
#include <kipr/create3/client/euler.h>
#include <kipr/create3/client/Vector3.hpp>
#include <kipr/create3/client/Quaternion.hpp>
#include <kipr/create3/client/LedAnimationType.hpp>

#include <clipp.h>
#include <iostream>
#include <string>
#include <thread>
#include <cmath>

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
  CliffIntensity,
  HazardDetection,
  IrIntensity,
  Odometry,
  NavigateTo,
  Rotate,
  DriveStraight,
  LedAnimation,
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

  float navigate_to_x = 0.0;
  float navigate_to_y = 0.0;
  float navigate_to_theta = NAN;

  double navigate_to_max_linear_speed = 0.0;
  double navigate_to_max_angular_speed = 0.0;

  std::string led_animation_type;
  Lightring lightring;
  lightring.led0 = {.r = 0, .g = 0, .b = 0};
  lightring.led1 = {.r = 0, .g = 0, .b = 0};
  lightring.led2 = {.r = 0, .g = 0, .b = 0};
  lightring.led3 = {.r = 0, .g = 0, .b = 0};
  lightring.led4 = {.r = 0, .g = 0, .b = 0};
  lightring.led5 = {.r = 0, .g = 0, .b = 0};
  double led_animation_max_runtime = 0.0;

  double rotate_angle = 0.0;
  double rotate_max_angular_speed = 0.0;

  double drive_straight_distance = 0.0;
  double drive_straight_max_linear_speed = 0.0;

  auto cli = (
    command("help").set(mode, Mode::Help) % "Print the man page" |
    (
      group(),
      option("--address", "-a") & value("host:port") % "The server's address" >> set(address),
      (
        (command("set_velocity") >> set(mode, Mode::SetVelocity), value("linear_x") >> set(linear_x), value("angular_z") >> set(angular_z)) |
        (command("undock") >> set(mode, Mode::Undock)) |
        (command("dock") >> set(mode, Mode::Dock)) |
        (command("hazardDetection") >> set(mode, Mode::HazardDetection)) |
        (command("irIntensity") >> set(mode, Mode::IrIntensity)) |
        (command("cliffIntensity") >> set(mode, Mode::CliffIntensity)) |
        (command("odometry") >> set(mode, Mode::Odometry)) |
        (
          command("led_animation") >> set(mode, Mode::LedAnimation), 
          value("led_animation_type") >> set(led_animation_type),
          value("led0_r") >> set(lightring.led0.r),
          value("led0_g") >> set(lightring.led0.g),
          value("led0_b") >> set(lightring.led0.b),
          value("led1_r") >> set(lightring.led1.r),
          value("led1_g") >> set(lightring.led1.g),
          value("led1_b") >> set(lightring.led1.b),
          value("led2_r") >> set(lightring.led2.r),
          value("led2_g") >> set(lightring.led2.g),
          value("led2_b") >> set(lightring.led2.b),
          value("led3_r") >> set(lightring.led3.r),
          value("led3_g") >> set(lightring.led3.g),
          value("led3_b") >> set(lightring.led3.b),
          value("led4_r") >> set(lightring.led4.r),
          value("led4_g") >> set(lightring.led4.g),
          value("led4_b") >> set(lightring.led4.b),
          value("led5_r") >> set(lightring.led5.r),
          value("led5_g") >> set(lightring.led5.g),
          value("led5_b") >> set(lightring.led5.b),
          value("max_runtime") >> set(led_animation_max_runtime)
        ) |
        (
          command("navigate_to") >> set(mode, Mode::NavigateTo),
          value("x") >> set(navigate_to_x),
          value("y") >> set(navigate_to_y),
          value("max_linear_speed") >> set(navigate_to_max_linear_speed),
          value("max_angular_speed") >> set(navigate_to_max_angular_speed),
          option("theta") >> set(navigate_to_theta)
        ) |
        (command("rotate") >> set(mode, Mode::Rotate), value("angle") >> set(rotate_angle), value("max_angular_speed") >> set(rotate_max_angular_speed)) |
        (command("drive_straight") >> set(mode, Mode::DriveStraight), value("distance") >> set(drive_straight_distance), value("max_linear_speed") >> set(drive_straight_max_linear_speed))
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
    case Mode::DriveStraight:
    {
      auto client = init_client();
      client.driveDistance(
        drive_straight_distance,
        drive_straight_max_linear_speed
      );
      break;
    }
    case Mode::LedAnimation:
    {
      auto client = init_client();
      client.ledAnimation(
        create3_led_animation_type_from_string(led_animation_type.c_str()),
        lightring,
        create3_duration_from_double(led_animation_max_runtime)
      );
      break;
    }
    case Mode::CliffIntensity:
    {
      const auto cliff_intensity_vector = init_client().getCliffIntensityVector();
      std::cout
        << "CliffIntensity:" << std::endl
        << "  size: " << cliff_intensity_vector.size() << std::endl;
      for (std::size_t i = 0; i < cliff_intensity_vector.size(); ++i)
      {
        std::cout 
          << "  " << i << " (" << cliff_intensity_vector[i].frameId << ")  :" << cliff_intensity_vector[i].intensity << std::endl;
      }
      break;
    }
    case Mode::HazardDetection:
    {
      const auto hazard_detection_vector = init_client().getHazardDetectionVector();
      std::cout
        << "HazardDetection:" << std::endl
        << "  size: " << hazard_detection_vector.size() << std::endl;
      for (std::size_t i = 0; i < hazard_detection_vector.size(); ++i)
      {
        std::cout 
          << "  " << i << " (" << hazard_detection_vector[i].frameId << ")  :" << hazard_detection_vector[i].type << std::endl;
      }
    }
    case Mode::IrIntensity:
    {
      const auto ir_intensity_vector = init_client().getIrIntensityVector();
      std::cout
        << "IrIntensity:" << std::endl
        << "  size: " << ir_intensity_vector.size() << std::endl;
      for (std::size_t i = 0; i < ir_intensity_vector.size(); ++i)
      {
        std::cout 
          << "  " << i << " (" << ir_intensity_vector[i].frameId << ")  :" << ir_intensity_vector[i].intensity << std::endl;
      }
      break;
    }

    case Mode::Odometry:
    {
      const auto odometry = init_client().getOdometry();

      const auto euler = create3_euler_from_quaternion(odometry.pose.orientation);

      std::cout
        << "Odometry:" << std::endl
        << "  Position:" << std::endl
        << "    x: " << odometry.pose.position.x << std::endl
        << "    y: " << odometry.pose.position.y << std::endl
        << "    z: " << odometry.pose.position.z << std::endl
        << "  Orientation (Quaternion):" << std::endl
        << "    x: " << odometry.pose.orientation.x << std::endl
        << "    y: " << odometry.pose.orientation.y << std::endl
        << "    z: " << odometry.pose.orientation.z << std::endl
        << "    w: " << odometry.pose.orientation.w << std::endl
        << "  Orientation (Euler):" << std::endl
        << "    x: " << euler.x << std::endl
        << "    y: " << euler.y << std::endl
        << "    z: " << euler.z << std::endl
        << "  Linear Velocity:" << std::endl
        << "    x: " << odometry.velocity.linear_x << std::endl
        << "  Angular Velocity:" << std::endl
        << "    z: " << odometry.velocity.angular_z << std::endl;
      break;
    }
    case Mode::NavigateTo:
    {
      auto client = init_client();

      if (std::isnan(navigate_to_theta))
      {
        client.navigateTo(
          navigate_to_x,
          navigate_to_y,
          navigate_to_max_linear_speed,
          navigate_to_max_angular_speed
        );
      }
      else
      {
        client.navigateTo(
          navigate_to_x,
          navigate_to_y,
          navigate_to_theta,
          navigate_to_max_linear_speed,
          navigate_to_max_angular_speed
        );
      }
      break;
    }
    case Mode::Rotate:
      init_client().rotate(
        rotate_angle, 
        rotate_max_angular_speed
      );
      break;
  }
  
  return EXIT_SUCCESS;
}