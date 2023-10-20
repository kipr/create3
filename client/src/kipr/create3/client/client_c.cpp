#include "kipr/create3/client/Client.hpp"
#include "kipr/create3/client/client.h"

#include <mutex>
#include <thread>
#include <iostream>

namespace kipr
{
namespace create3
{
namespace client
{
  static std::unique_ptr<Client> global_client;
  static std::mutex global_client_mut;
}
}
}

using namespace kipr::create3::client;

int create3_connect()
{
  std::lock_guard<std::mutex> lock(global_client_mut);
  if (global_client)
  {
    std::cerr << __func__ << ": already connected" << std::endl;
    return true;
  }

  global_client = std::make_unique<Client>();

  std::cout << __func__ << ": Waiting for the Create 3 to connect..." << std::endl;

  while (!global_client->isConnected())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << __func__ << ": Create 3 connected!" << std::endl;

  return true;
}

int create3_connect_manual(const char *const host, const unsigned short port)
{
  std::lock_guard<std::mutex> lock(global_client_mut);

  if (global_client)
  {
    std::cerr << __func__ << ": already connected" << std::endl;
    return false;
  }

  global_client = std::make_unique<Client>(host, port);

  std::cout << __func__ << ": Waiting for the Create 3 to connect..." << std::endl;

  while (!global_client->isConnected())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << __func__ << ": Create 3 connected!" << std::endl;

  return true;
}

int create3_is_connected()
{
  std::lock_guard<std::mutex> lock(global_client_mut);

  return global_client->isConnected();
}

void create3_set_velocity(const double linear_x, const double angular_z)
{
  std::lock_guard<std::mutex> lock(global_client_mut);

  global_client->setVelocity(linear_x, angular_z);
}