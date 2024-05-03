/*----------------------------------------------------------------------------
LAB EXERCISE 5.3 - SPI interface
SERIAL COMMUNICATION
 ----------------------------------------
 Interface the LCD display using SPI protocol
	
	GOOD LUCK!
 *----------------------------------------------------------------------------*/
#include "mbed.h"
#include "platform/mbed_thread.h"
#include <string>
#include "NHD_0216HZ.h"
#include "alarm_clock.h"

using namespace std;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() {

    
    
    
    init_spi();
    init_lcd();
    init_lcd();
    
    alarm_clock myClock(25,6,99);

    while(1){
        
        
        myClock.displayTime();

        ThisThread::sleep_for(200);
     }

    


}
