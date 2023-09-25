#include "Mock_Systick.h"
#include "Systick.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>

static MockSystickImpl *p_MockInterfaceSystick = nullptr;

MockSystickImpl::MockSystickImpl() {
  if (p_MockInterfaceSystick == nullptr) {
    p_MockInterfaceSystick = this;
  }
}

MockSystickImpl::~MockSystickImpl() {
  if (p_MockInterfaceSystick != nullptr) {
    delete p_MockInterfaceSystick;
    p_MockInterfaceSystick = nullptr;
  }
}

MockInterfaceSystick *MockSystickImpl::getMock() {
  MockInterfaceSystick *p_interface = p_MockInterfaceSystick;

  if (p_interface == nullptr) {
    std::cerr << "ERROR: No test mock instance for Systick" << std::endl;
    throw SystickMockImplNotSetException();
  }
  return p_interface;
}

/// -----------------------------------------------------------------
// Implementation of the Systick class
Systick::Systick(std::uint32_t base_address,
                 std::uint32_t system_clock_frequency_hz,
                 std::uint32_t systick_frequency_hz) {}

Systick::~Systick() {}

void Systick::Enable() { MockSystickImpl::getMock()->Enable(); }

void Systick::Disable() { MockSystickImpl::getMock()->Disable(); }

void Systick::AcknowledgeIrq() { MockSystickImpl::getMock()->AcknowledgeIrq(); }

std::uint32_t Systick::GetInterval() {
  return MockSystickImpl::getMock()->GetInterval();
}

std::uint32_t Systick::GetCount() {
  return MockSystickImpl::getMock()->GetCount();
}

std::uint32_t Systick::GetOverflow() {
  return MockSystickImpl::getMock()->GetOverflow();
}
