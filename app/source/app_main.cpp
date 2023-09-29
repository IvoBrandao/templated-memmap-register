#include "app_main.h"
#include "Systick.h"

void app_main() {

  Systick systick(0, 1000000, 1000);
  systick.Enable();
  systick.Disable();
}