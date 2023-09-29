/*
 * systick.h
 *
 *  Created on: Aug 22, 2021
 *      Author: Ivo-B
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "Register.h"
#include <new>
#include <type_traits>

//! \brief System tick control bit field representation
enum class SystickControl : std::uint32_t {
  ENABLE = 0,
  TICKINT = 1,
  CLKSOURCE = 2,
  COUNTFLAG = 16
};

//! \brief System timer Calibration bit field representation
enum class SystickCalib : std::uint32_t { TENMS = 0, SKEW = 30, NOREF = 31 };

struct SysTickRegisters {
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

private:
  std::uint32_t dummy_registers[4] = {0};
  //!< Base address of System tick Registers
  std::uintptr_t mmr_base_address = 0;
  //!< Rate at which the timer will tick
  std::uint32_t tick_rate_hz = 0;
  //!< Reload value of the timer
  std::uint32_t tick_reload = 0;
  //!< timer frequency in hertz
  std::uint32_t tick_freq_hz = 0;
  //!< system clock frequency in hertz
  std::uint32_t sys_clock_hz = 0;
  //!< pointer to the system tick registers
  SysTickRegisters mmr;

public:
  Systick(std::uintptr_t base_address, std::uint32_t system_clock_frequency_hz,
          std::uint32_t systick_frequency_hz);

  //! \brief default constructor
  virtual ~Systick();

  //! \brief
  void Enable(void);

  //! \brief
  void Disable(void);

  //! \brief
  void AcknowledgeIrq(void);

  //! \brief
  std::uint32_t GetInterval(void);

  //! \brief
  std::uint32_t GetCount(void);

  //! \brief
  std::uint32_t GetOverflow(void);
}; // Systick class

#endif /* SYSTICK_H_ */
