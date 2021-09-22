/*
 * systick.h
 *
 *  Created on: Aug 22, 2021
 *      Author: Ivo-B
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "Register.h"
#include <type_traits>
#include <new>

namespace HAL
{


//! \brief System tick control bit field representation
enum class SystickControl : std::uint32_t
{
	ENABLE = 0, TICKINT = 1, CLKSOURCE = 2, COUNTFLAG = 16
};

//! \brief System timer Calibration bit field representation
enum class SystickCalib : std::uint32_t
{
	TENMS = 0, SKEW = 30, NOREF = 31
};

class Systick
{

private:
	//!< Base address of System tick Registers
	std::uint32_t base_address = 0xe000e010UL;
	//!< Rate at which the timer will tick
	std::uint32_t tick_rate_hz = 0;
	//!< Reload value of the timer
	std::uint32_t tick_reload = 0;
	//!< timer frequency in hertz
	std::uint32_t tick_freq_hz = 0;
	//!< system clock frequency in hertz
	std::uint32_t sys_clock_hz = 0;

	//!< system tick control and status register
	Register<32, ReadWrite> CSR { base_address + 0UL };
	//!< system tick reload value register
	Register<32, ReadWrite> RVR { base_address + 4UL };
	//!< system tick count value register
	Register<32, ReadWrite> CVR { base_address + 8UL };
	//!< system tick calibration register
	Register<32, ReadWrite> CR { base_address + 12UL };

public:
	Systick(std::uint32_t system_clock_frequency_hz,
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

};

} /* namespace HAL */


#endif /* SYSTICK_H_ */
