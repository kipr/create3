#pragma once

#include <tuple>

namespace kipr
{
namespace create3
{
namespace client
{
namespace detail
{
  template<typename Func>
  struct FunctionTraits;

  template<typename Ret, typename... Args>
  struct FunctionTraits<Ret (Args...)>
  {
    using Arguments = ::std::tuple<Args...>;
  };
}
}
}
}