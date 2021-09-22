
#include <stdint.h>
#include <memory>
#include <Systick.h>

using namespace HAL;
std::uint32_t count=0;
int main(void)
{

   std::shared_ptr<Systick> systick = std::make_shared<Systick>(4000000,1000);

   systick->Enable();


    /* Loop forever */
	for(;;);
}


extern "C" void SysTick_Handler (void)
{

	count++;
}

extern "C" void WWDG1_IRQHandler()
{

}
