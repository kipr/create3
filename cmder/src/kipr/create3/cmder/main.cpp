#include <stdlib.h>
#include <kipr/create3/client/Client.hpp>

#include <clipp.h>
#include <iostream>
#include <string_view>
#include <string>

std::pair<std::string_view, std::uint16_t> parseAddress(const std::string &address, const std::uint16_t default_port)
{
  auto sep = address.find(':');
  if(sep == std::string_view::npos)
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
  Help
};



int main(int argc, char *argv[])
{
  using namespace clipp;

  std::string address = "localhost:50051";

  Mode mode = Mode::Help;

  auto cli = (
    command("help").set(mode, Mode::Help) % "Print the man page" |
    (
      group(),
      option("--address", "-a") & value("host:port") % "The server's address" >> set(address),
      (
        (command("set_velocity") >> set(mode, Mode::SetVelocity), value("linear_x"), value("angular_z"))
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
    return kipr::create3::client::Client(host, port);
  };

  switch (mode)
  {
    case Mode::Help:
    {
      std::cout << make_man_page(cli, programName(argv[0])) << std::endl;
      return EXIT_SUCCESS;
    }
    case Mode::SetVelocity:
    {
      init_client().setVelocity(0.5, 0.0);
      return EXIT_SUCCESS;
    }
  }

  
  
  
  
  return EXIT_SUCCESS;
}