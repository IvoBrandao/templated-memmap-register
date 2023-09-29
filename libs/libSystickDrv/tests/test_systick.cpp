
// definition of the Test Fixture
#include "Mock_Systick.h"
#include "Systick.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

// Test fixture for Systick class
class SystickTest : public ::testing::Test {
public:
  Mock_Systick mock; // Declare the mock as a member variable

protected:
  SystickTest()
      : systick(0, 1000000, 1000) {
  } // Create Systick instance with a dummy base address

  Systick systick; // Systick instance to be tested

  virtual void SetUp() {
    // Set the mock instance for the Systick class to the declared mock.
    ON_CALL(mock, Enable())
        .WillByDefault(::testing::Invoke(&mock, &Mock_Systick::Enable));
    ON_CALL(mock, Disable())
        .WillByDefault(::testing::Invoke(&mock, &Mock_Systick::Disable));
    ON_CALL(mock, AcknowledgeIrq())
        .WillByDefault(::testing::Invoke(&mock, &Mock_Systick::AcknowledgeIrq));
    ON_CALL(mock, GetInterval())
        .WillByDefault(::testing::Invoke(&mock, &Mock_Systick::GetInterval));
    ON_CALL(mock, GetCount())
        .WillByDefault(::testing::Invoke(&mock, &Mock_Systick::GetCount));
    ON_CALL(mock, GetOverflow())
        .WillByDefault(::testing::Invoke(&mock, &Mock_Systick::GetOverflow));
  }
};
// Test case for enabling Systick
TEST_F(SystickTest, EnableSystick) { systick.Enable(); }

// Test case for disabling Systick
TEST_F(SystickTest, DisableSystick) { systick.Disable(); }

// Test case for acknowledging Systick interrupt
TEST_F(SystickTest, AcknowledgeSystickIrq) { systick.AcknowledgeIrq(); }

// Test case for getting Systick interval
TEST_F(SystickTest, GetSystickInterval) {
  std::uint32_t interval = systick.GetInterval();
  EXPECT_EQ(interval, 1000000);
}

// Test case for getting Systick count
TEST_F(SystickTest, GetSystickCount) {
  std::uint32_t count = systick.GetCount();
  EXPECT_EQ(count, 999999);
}

// Test case for getting Systick overflow status
TEST_F(SystickTest, GetSystickOverflow) {
  std::uint32_t overflow = systick.GetOverflow();
  EXPECT_EQ(overflow, 0);
}
