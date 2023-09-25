#ifndef MOCK_SYSTICK_H_
#define MOCK_SYSTICK_H_

#include "Systick.h"
#include "gmock/gmock.h"
#include <gmock/gmock.h>

class MockInterfaceSystick {

public:
  virtual void Enable() = 0;
  virtual void Disable() = 0;
  virtual void AcknowledgeIrq() = 0;
  virtual std::uint32_t GetInterval() = 0;
  virtual std::uint32_t GetCount() = 0;
  virtual std::uint32_t GetOverflow() = 0;
};
/// @brief Mock implementation of the Systick class
class MockSystickImpl : public MockInterfaceSystick {
protected:
  MockSystickImpl();

public:
  static MockInterfaceSystick *getMock();
  virtual ~MockSystickImpl();
  MOCK_METHOD(void, Enable, ());
  MOCK_METHOD(void, Disable, ());
  MOCK_METHOD(void, AcknowledgeIrq, ());
  MOCK_METHOD(std::uint32_t, GetInterval, ());
  MOCK_METHOD(std::uint32_t, GetCount, ());
  MOCK_METHOD(std::uint32_t, GetOverflow, ());
};

typedef ::testing::NiceMock<MockSystickImpl> Mock_Systick;
typedef ::testing::StrictMock<MockSystickImpl> StrictMock_Systick;

class SystickMockImplNotSetException : public std::exception {
public:
  const char *what() const throw() {
    return "Systick Mock not instanciated in test Fixture";
  }
};

#endif // MOCK_SYSTICK_H_
