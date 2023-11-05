#include "kipr/create3/client/detail/MostRecentDelayedCallExecutor.hpp"

#include <algorithm>
#include <iostream>

using namespace kipr::create3::client::detail;

void MostRecentDelayedCallExecutor::enqueue(const void *const tag, std::unique_ptr<DelayedCall> &&call)
{
  std::lock_guard<std::mutex> lock(mut_);

  // Find any existing call with the same tag, remove it from the list, then append the new call.
  for (auto it = calls_.begin(); it != calls_.end();)
  {
    if (it->first != tag)
    {
      ++it;
      continue;
    }
    it = calls_.erase(it);
  }

  calls_.emplace_back(tag, std::move(call));
}

bool MostRecentDelayedCallExecutor::empty() const
{
  std::lock_guard<std::mutex> lock(mut_);

  return calls_.empty();
}

void MostRecentDelayedCallExecutor::clear()
{
  std::lock_guard<std::mutex> lock(mut_);

  calls_.clear();
}

void MostRecentDelayedCallExecutor::execute()
{
  std::lock_guard<std::mutex> lock(mut_);
  
  for (const auto &[tag, call] : calls_) (*call)();
  clear();
}