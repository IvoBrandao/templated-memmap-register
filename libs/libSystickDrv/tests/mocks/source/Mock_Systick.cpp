#include "Mock_Systick.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>

#if 0
class SystickMockImplNotSetException : public std::exception {
public:
  const char *what() const throw() {
    return "Systick Mock Implementation not set";
  }
};

static InternalMockSystick *p_IMockSystick = nullptr;

//
InternalMockSystick::InternalMockSystick(
    std::uint32_t base_address, std::uint32_t system_clock_frequency_hz,
    std::uint32_t systick_frequency_hz) {}
//
InternalMockSystick::~InternalMockSystick() {}

IMockSystick *getTestDouble() {
  IMockSystick *p_selected = p_IMockSystick;

  if (p_IMockSystick == nullptr) {
    std::cerr << "ERROR: No test double selected for IMock_Systick"
              << std::endl;
    throw SystickMockImplNotSetException();
  }

  return p_selected;
}

void resetTestDouble() { p_IMockSystick = nullptr; }

void Enable() { p_IMockSystick->Enable(); }

#if 0
void Disable() { getTestDouble()->Disable(); }

void AcknowledgeIrq() { getTestDouble()->AcknowledgeIrq(); }

std::uint32_t GetInterval() { return getTestDouble()->GetInterval(); }

std::uint32_t GetCount() { return getTestDouble()->GetCount(); }

std::uint32_t GetOverflow() { return getTestDouble()->GetOverflow(); }
#endif

#endif