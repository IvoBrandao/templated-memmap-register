#include "Mock_Systick.h"
#include "Systick.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>

static MockSystickImpl *p_MockInterfaceSystick = nullptr;

MockSystickImpl::MockSystickImpl() {
  // std::cout << "MockSystickImpl::MockSystickImpl()" << std::endl;
  if (p_MockInterfaceSystick == nullptr) {
    p_MockInterfaceSystick = this;
  }

  mmr_base_address = reinterpret_cast<std::uintptr_t>(&dummy_registers[0]);

  // Initialize register addresses with dummy values
  mmr.CSR.SetAddress(mmr_base_address);
  mmr.RVR.SetAddress(mmr_base_address + 0x04UL);
  mmr.CVR.SetAddress(mmr_base_address + 0x08UL);
  mmr.CR.SetAddress(mmr_base_address + 0x0CUL);
}

MockSystickImpl::~MockSystickImpl() {}
