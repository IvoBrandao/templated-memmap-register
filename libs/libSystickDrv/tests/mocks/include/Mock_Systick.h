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
  virtual ~MockSystickImpl();
  MOCK_METHOD(void, Enable, (), (override));
  MOCK_METHOD(void, Disable, (), (override));
  MOCK_METHOD(void, AcknowledgeIrq, (), (override));
  MOCK_METHOD(std::uint32_t, GetInterval, (), (override));
  MOCK_METHOD(std::uint32_t, GetCount, (), (override));
  MOCK_METHOD(std::uint32_t, GetOverflow, (), (override));

private:
  SysTickRegisters mmr;
  std::uint32_t dummy_registers[4];
  std::uintptr_t mmr_base_address;
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
