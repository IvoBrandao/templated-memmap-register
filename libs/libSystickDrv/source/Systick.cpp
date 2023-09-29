/*
 * systick.cpp
 *
 *  Created on: Aug 22, 2021
 *      Author: Ivo-B
 */
#include <Systick.h>

Systick::Systick(std::uintptr_t base_address,
                 std::uint32_t system_clock_frequency_hz,
                 std::uint32_t systick_frequency_hz) {

  sys_clock_hz = system_clock_frequency_hz;
  tick_rate_hz = systick_frequency_hz / 1000UL;

  if (tick_rate_hz == 0) {
    tick_rate_hz = 1;
  }

  /* load the reload value */
  tick_reload = ((sys_clock_hz / tick_rate_hz) - 1UL);

  if (tick_reload > 0x00FFFFFFUL) {
    tick_reload = 0x00FFFFFFUL;
  }

  if (tick_reload < 1UL) {
    tick_reload = 1UL;
  }
  mmr_base_address = base_address;
  if (mmr_base_address == 0UL) {
    mmr_base_address = reinterpret_cast<std::uintptr_t>(&dummy_registers[0]);
  }
  // Initialized the registes
  mmr.CSR.SetAddress(mmr_base_address);
  mmr.RVR.SetAddress(mmr_base_address + 0x04UL);
  mmr.CVR.SetAddress(mmr_base_address + 0x08UL);
  mmr.CR.SetAddress(mmr_base_address + 0x0CUL);

  mmr.CSR[EnumToUnderlyingType(SystickControl::CLKSOURCE)] = 1UL;
  mmr.CSR[EnumToUnderlyingType(SystickControl::TICKINT)] = 1UL;
  mmr.RVR = this->tick_reload;
  mmr.CVR = 0U;
}

//! ------------------------------------------------------------
Systick::~Systick() { this->Disable(); }

//! ------------------------------------------------------------
void Systick::Enable(void) {
  mmr.CSR[EnumToUnderlyingType(SystickControl::ENABLE)] = 1UL;
}

//! ------------------------------------------------------------

void Systick::Disable(void) {
  mmr.CSR[EnumToUnderlyingType(SystickControl::ENABLE)] = 0UL;
}

//! ------------------------------------------------------------
void Systick::AcknowledgeIrq(void) {
  // Ignore ack if no interrupt is pending
  (void)mmr.CSR;
}

//! ------------------------------------------------------------
std::uint32_t Systick::GetInterval(void) {
  std::uint32_t value = (mmr.RVR + 1UL);
  return value;
}

//! ------------------------------------------------------------
std::uint32_t Systick::GetCount(void) {
  std::uint32_t load = mmr.RVR;
  std::uint32_t count = mmr.CVR;
  std::uint32_t value = (load - count);
  return value;
}

//! ------------------------------------------------------------
std::uint32_t Systick::GetOverflow(void) {
  std::uint32_t value = ((mmr.CSR >> 16UL) & 1U);
  return value;
}
