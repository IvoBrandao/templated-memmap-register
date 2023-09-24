#ifndef MOCK_SYSTICK_H_
#define MOCK_SYSTICK_H_

#include "Systick.h"
#include <gmock/gmock.h>

#if 0
class IMockSystick {

public:
  virtual void Enable() = 0;
  // virtual void Disable() = 0;
  // virtual void AcknowledgeIrq() = 0;
  // virtual std::uint32_t GetInterval() = 0;
  // virtual std::uint32_t GetCount() = 0;
  // virtual std::uint32_t GetOverflow() = 0;
};

class InternalMockSystick : public IMockSystick {
public:
  InternalMockSystick(std::uint32_t base_address,
                      std::uint32_t system_clock_frequency_hz,
                      std::uint32_t systick_frequency_hz);
  virtual ~InternalMockSystick();
  MOCK_METHOD0(Enable, void());
  // MOCK_METHOD(void, Disable, ()) override;
  // MOCK_METHOD(void, AcknowledgeIrq, ()) override;
  // MOCK_METHOD(std::uint32_t, GetInterval, ()) override;
  // MOCK_METHOD(std::uint32_t, GetCount, ()) override;
  // MOCK_METHOD(std::uint32_t, GetOverflow, ()) override;
};

typedef ::testing::NiceMock<InternalMockSystick> Mock_Systick;
typedef ::testing::StrictMock<InternalMockSystick> StrictMock_Systick;
#endif

#endif // MOCK_SYSTICK_H_
