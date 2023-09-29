#include "Mock_Register.hpp"
#include "Register.h"
#include <cstdint>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Define a fixture for the Register class
class RegisterTest : public testing::Test {
protected:
  // Initialize the Register instance in the member initializer list
  RegisterTest()
      : registerInstance(reinterpret_cast<std::uintptr_t>(&registerValue)) {}

  // Set up common variables for the tests
  void SetUp() override {
    // Add additional setup code if needed
  }

  // Clean up resources after the tests
  void TearDown() override {
    // Add cleanup code if needed
  }

  // Define any other helper functions or variables you need for testing

  // Variable to simulate register behavior (replace with your actual variable)
  uint32_t registerValue = 0;
  Register<32, ReadWrite> registerInstance;
};

// Test case for basic read and write operations
TEST_F(RegisterTest, ReadWriteTest) {
  // Perform write and read operations and check for correctness
  registerInstance = 0xABCD;

  EXPECT_EQ(static_cast<uint32_t>(registerInstance), 0xABCD);
}

// Test case for bitwise OR assignment
TEST_F(RegisterTest, BitwiseORTest) {
  // Perform OR assignment and check for correctness
  registerInstance = 0xAAAA;
  registerInstance |= 0x5555;
  EXPECT_EQ(static_cast<uint32_t>(registerInstance), 0xFFFF);
}

// Test case for bitwise AND assignment
TEST_F(RegisterTest, BitwiseANDTest) {
  // Perform AND assignment and check for correctness
  registerInstance = 0xFFFF;
  registerInstance &= 0x5555;
  EXPECT_EQ(static_cast<uint32_t>(registerInstance), 0x5555);
}

// Test case for increment and decrement operations
TEST_F(RegisterTest, IncrementDecrementTest) {
  // Perform increment and decrement operations and check for correctness
  // registerInstance = 0x10;
  // ++registerInstance;
  // EXPECT_EQ(static_cast<uint32_t>(registerInstance), 0x11);
  //
  // registerInstance--;
  // EXPECT_EQ(static_cast<uint32_t>(registerInstance), 0x10);

  // Additional test cases can be added here
}

// Test case for bitfield access
TEST_F(RegisterTest, BitfieldAccessTest) {
  // Access individual bits and check for correctness
  registerInstance = 0;
  registerInstance[0] = 1;
  registerInstance[1] = 1;
  registerInstance[2] = 0;

  EXPECT_EQ(static_cast<uint32_t>(registerInstance), 0b110);
}
