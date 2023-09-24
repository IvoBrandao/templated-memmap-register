#ifndef DRIVER_SYSTICK_H_
#define DRIVER_SYSTICK_H_

#include "mmregister.h"
#include <cstdint>

//! \brief System tick control bit field representation
enum class SystickControl : std::uint32_t {
  ENABLE = 0,
  TICKINT = 1,
  CLKSOURCE = 2,
  COUNTFLAG = 16
};
// //! \brief System timer Calibration bit field representation
enum class SystickCalib : std::uint32_t { TENMS = 0, SKEW = 30, NOREF = 31 };

struct SystickRegisters {
  //!< system tick control and status register
  Register<32, ReadWrite> CSR;
  //!< system tick reload value register
  Register<32, ReadWrite> RVR;
  //!< system tick count value register
  Register<32, ReadWrite> CVR;
  //!< system tick calibration register
  Register<32, ReadWrite> CR;
};

class Systick {

public:
  Systick(std::uint32_t base_address, std::uint32_t system_clock_frequency_hz,
          std::uint32_t systick_frequency_hz);
  //! \brief default constructor
  virtual ~Systick();

  //! \brief Starts the system tick timer
  void Enable(void);

  //! \brief Stops the system tick timer
  void Disable(void);

  //! \brief Acknowledges the system tick timer interrupt
  void AcknowledgeIrq(void);

  //! \brief Returns the interval of the system tick timer
  std::uint32_t GetInterval(void);

  //! \brief Returns the current count of the system tick timer
  std::uint32_t GetCount(void);

  //! \brief Returns the overflow of the system tick timer
  std::uint32_t GetOverflow(void);

private:
  //!< Base address of System tick Registers
  std::uint32_t reg_base_address = 0;
  //!< Rate at which the timer will tick
  std::uint32_t tick_rate_hz = 0;
  //!< Reload value of the timer
  std::uint32_t tick_reload = 0;
  //!< timer frequency in hertz
  std::uint32_t tick_freq_hz = 0;
  //!< system clock frequency in hertz
  std::uint32_t sys_clock_hz = 0;
  // System tick memory mapped registers
  SystickRegisters mmr;

  std::uint32_t dummy_register[10];

  /// \brief Initializes the system tick timer
  /// \param system_clock_frequency_hz system clock frequency in hertz
  /// \param systick_frequency_hz system tick timer frequency in hertz
  void Init(std::uint32_t system_clock_frequency_hz,
            std::uint32_t systick_frequency_hz);
};

#endif // DRIVER_SYSTICK_H_