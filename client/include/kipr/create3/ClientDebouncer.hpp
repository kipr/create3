#pragma once

namespace kipr
{
namespace create3
{
namespace client
{
  struct Call
  {
    virtual void operator ()() = 0;
  };

  template<typename Ret, typename... Args>
  struct CallWithArgs : public Call
  {
    virtual Ret operator ()(Args... args) = 0;
  };

  class ClientDebouncer
  {
  };
}
}
}