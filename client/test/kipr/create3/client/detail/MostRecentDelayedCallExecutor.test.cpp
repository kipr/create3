#include <gtest/gtest.h>

#include "kipr/create3/client/detail/MostRecentDelayedCallExecutor.hpp"

using namespace kipr::create3::client::detail;



TEST(MostRecentDelayedCallExecutor, simple) {
  auto executor = std::make_shared<MostRecentDelayedCallExecutor>();

  std::string value;
  auto func = [&value](const std::string &next_value) {
    value = next_value;
  };

  DelayedCallProxy<const std::string &> proxy(func, executor);
  
  proxy("hello");

  EXPECT_EQ(value, "");

  executor->execute();

  EXPECT_EQ(value, "hello");
}

TEST(MostRecentDelayedCallExecutor, complex) {
  auto executor = std::make_shared<MostRecentDelayedCallExecutor>();

  std::string value1;

  auto func1 = [&value1](const std::string &next_value) {
    value1 = next_value;
  };

  std::string value2;
  auto func2 = [&value2](const std::string &next_value) {
    value2 = next_value;
  };

  DelayedCallProxy<const std::string &> proxy1(func1, executor);
  DelayedCallProxy<const std::string &> proxy2(func2, executor);
  
  proxy2("goodbye");
  proxy1("hello");
  proxy1("hello world");
  proxy2("goodbye world");
  proxy2("goodbye world x2");

  EXPECT_EQ(value1, "");
  EXPECT_EQ(value2, "");

  executor->execute();

  EXPECT_EQ(value1, "hello world");
  EXPECT_EQ(value2, "goodbye world x2");
}

TEST(MostRecentDelayedCallExecutor, clear) {
  auto executor = std::make_shared<MostRecentDelayedCallExecutor>();

  std::string value1;

  auto func1 = [&value1](const std::string &next_value) {
    value1 = next_value;
  };

  std::string value2;
  auto func2 = [&value2](const std::string &next_value) {
    value2 = next_value;
  };

  DelayedCallProxy<const std::string &> proxy1(func1, executor);
  DelayedCallProxy<const std::string &> proxy2(func2, executor);
  
  proxy2("goodbye");
  proxy1("hello");
  proxy1("hello world");
  proxy2("goodbye world");
  proxy2("goodbye world x2");

  EXPECT_EQ(value1, "");
  EXPECT_EQ(value2, "");

  executor->clear();

  executor->execute();

  EXPECT_EQ(value1, "");
  EXPECT_EQ(value2, "");
}