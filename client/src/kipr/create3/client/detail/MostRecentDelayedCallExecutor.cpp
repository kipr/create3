#include "kipr/create3/client/detail/MostRecentDelayedCallExecutor.hpp"

#include <algorithm>

using namespace kipr::create3::client::detail;

void MostRecentDelayedCallExecutor::enqueue(const void *const tag, std::unique_ptr<DelayedCall> &&call)
{
  // Find any existing call with the same tag, remove it from the list, then append the new call.
  std::remove_if(calls_.begin(), calls_.end(), [tag](const auto &pair) { return pair.first == tag; });
  calls_.emplace_back(tag, std::move(call));
}
  
void MostRecentDelayedCallExecutor::clear()
{
  calls_.clear();
}

void MostRecentDelayedCallExecutor::execute()
{
  for (const auto &[tag, call] : calls_) (*call)();
  clear();
}