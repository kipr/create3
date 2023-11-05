#pragma once

#include "FunctionTraits.hpp"

#include <memory>
#include <functional>

namespace kipr
{
namespace create3
{
namespace client
{
namespace detail
{
  struct DelayedCall
  {
    virtual void operator ()() = 0;
  };

  template<typename... Args>
  struct DelayedCallWrapper : public DelayedCall
  {
    DelayedCallWrapper(const std::function<void (Args...)> &func, const std::tuple<Args...> &args)
      : func_(func)
      , args_(args)
    {
    }

    virtual void operator ()() override
    {
      std::apply(func_, args_);
    }
  
  private:
    std::function<void (Args...)> func_;
    std::tuple<Args...> args_;
  };

  struct DelayedCallExecutor
  {
  public:
    virtual void enqueue(const void *const tag, std::unique_ptr<DelayedCall> &&call) = 0;
  };

  template<typename... Args>
  struct DelayedCallProxy
  {
    DelayedCallProxy(
      const std::function<void (Args...)> &func,
      const std::shared_ptr<DelayedCallExecutor> &executor
    ) : func_(func)
      , executor_(executor)
    {
    }

    void operator ()(Args... args) const
    {
      executor_->enqueue(
        reinterpret_cast<const void *>(this),
        std::make_unique<DelayedCallWrapper<Args...>>(func_, std::make_tuple(args...))
      );
    }
  private:
    std::function<void (Args...)> func_;
    std::shared_ptr<DelayedCallExecutor> executor_;
  };
}
}
}
}