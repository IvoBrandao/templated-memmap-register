/*
 * systick.cpp
 *
 *  Created on: Aug 22, 2021
 *      Author: Ivo-B
 */
#include <Systick.h>

namespace HAL {


Systick::Systick(std::uint32_t system_clock_frequency_hz,
		std::uint32_t systick_frequency_hz) {

	this->sys_clock_hz = system_clock_frequency_hz;
	this->tick_rate_hz = systick_frequency_hz / 1000UL;

	if (this->tick_rate_hz == 0)
	{
		this->tick_rate_hz = 1;
	}

	/* load the reload value */
	this->tick_reload = ((this->sys_clock_hz / this->tick_rate_hz) - 1UL);

	this->CSR[EnumToUnderlyingType(SystickControl::CLKSOURCE)] = 1UL;
	this->CSR[EnumToUnderlyingType(SystickControl::TICKINT)]  = 1UL;

	this->RVR = this->tick_reload;
	this->CVR = 0U;
}



//! ------------------------------------------------------------
Systick::~Systick() {

	this->Disable();
}

//! ------------------------------------------------------------
void Systick::Enable(void)
{
	this->CSR[EnumToUnderlyingType(SystickControl::ENABLE)] = 1UL;
}

//! ------------------------------------------------------------

void Systick::Disable(void)
{
	this->CSR[EnumToUnderlyingType(SystickControl::ENABLE)] = 0UL;
}

//! ------------------------------------------------------------
void Systick::AcknowledgeIrq(void) {
	(void) CSR;
}

//! ------------------------------------------------------------
std::uint32_t Systick::GetInterval(void) {
	return (this->RVR + 1UL);
}

//! ------------------------------------------------------------
std::uint32_t Systick::GetCount(void) {
	std::uint32_t load = this->RVR;
	return (load - this->CVR);
}

//! ------------------------------------------------------------
std::uint32_t Systick::GetOverflow(void) {
	return ((this->CSR >> 16UL) & 1U);
}
}
