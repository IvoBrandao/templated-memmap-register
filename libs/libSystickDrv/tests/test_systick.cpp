
// definition of the Test Fixture
#include "Mock_Systick.h"
#include "Systick.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

// definition of the Test Fixture
class SystickTestFixture : public ::testing::Test {

public:
  std::shared_ptr<Systick> systick =
      std::make_shared<Systick>(0, 4000000, 1000);

  void SetUp() {}
  void TearDown() {}
};

TEST_F(SystickTestFixture, Enable) { systick->Enable(); }
