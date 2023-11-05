#pragma once

#include "DelayedCall.hpp"

#include <list>

namespace kipr
{
namespace create3
{
namespace client
{
namespace detail
{
  struct MostRecentDelayedCallExecutor : public DelayedCallExecutor
  {
  public:
    virtual void enqueue(const void *const tag, std::unique_ptr<DelayedCall> &&call) override;
    
    void clear();

    void execute();

  private:
    std::list<std::pair<const void *, std::unique_ptr<DelayedCall>>> calls_;
  };
}
}
}
}