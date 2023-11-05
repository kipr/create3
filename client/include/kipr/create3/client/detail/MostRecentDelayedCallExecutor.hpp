#pragma once

#include "DelayedCall.hpp"

#include <list>
#include <mutex>

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
    

    bool empty() const;
    void clear();

    void execute();

  private:
    mutable std::mutex mut_;
    std::list<std::pair<const void *, std::unique_ptr<DelayedCall>>> calls_;
  };
}
}
}
}