
#include "Systick.h"
#include <iostream>
Systick::Systick(std::uint32_t base_address,
                 std::uint32_t system_clock_frequency_hz,
                 std::uint32_t systick_frequency_hz) {

  // Initialize the registers
  reg_base_address = base_address;
  if (base_address == 0) {

    uintptr_t dummy_addr = reinterpret_cast<uintptr_t>(&dummy_register);
    reg_base_address = static_cast<std::uint32_t>(dummy_addr);
  }

  mmr.CSR.SetAddress(base_address + 0UL);
  mmr.RVR.SetAddress(base_address + 4UL);
  mmr.CVR.SetAddress(base_address + 8UL);
  mmr.CR.SetAddress(base_address + 12UL);
  Init(system_clock_frequency_hz, systick_frequency_hz);
}

void Systick::Init(std::uint32_t system_clock_frequency_hz,
                   std::uint32_t systick_frequency_hz) {

  sys_clock_hz = system_clock_frequency_hz;
  tick_rate_hz = systick_frequency_hz / 1000UL;

  if (tick_rate_hz == 0) {
    tick_rate_hz = 1;
  }

  /* load the reload value */
  tick_reload = ((sys_clock_hz / tick_rate_hz) - 1UL);
  mmr.CSR[EnumToUnderlyingType(SystickControl::CLKSOURCE)] = 1UL;
  mmr.CSR[EnumToUnderlyingType(SystickControl::TICKINT)] = 1UL;
  mmr.RVR = tick_reload;
  mmr.CVR = 0U;
}

//! ------------------------------------------------------------
Systick::~Systick() { this->Disable(); }

//! ------------------------------------------------------------
void Systick::Enable(void) {

  std::cout << "Systick::Enable() - base_address is << " << reg_base_address
            << std::endl;

  mmr.CSR[EnumToUnderlyingType(SystickControl::ENABLE)] = 1UL;
}

//! ------------------------------------------------------------

void Systick::Disable(void) {
  mmr.CSR[EnumToUnderlyingType(SystickControl::ENABLE)] = 0UL;
}

//! ------------------------------------------------------------
void Systick::AcknowledgeIrq(void) { (void)mmr.CSR; }

//! ------------------------------------------------------------
std::uint32_t Systick::GetInterval(void) { return (mmr.RVR + 1UL); }

//! ------------------------------------------------------------
std::uint32_t Systick::GetCount(void) {
  std::uint32_t load = mmr.RVR;
  return (load - mmr.CVR);
}

//! ------------------------------------------------------------
std::uint32_t Systick::GetOverflow(void) { return ((mmr.CSR >> 16UL) & 1U); }