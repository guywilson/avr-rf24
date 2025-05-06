#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "taskdef.h"
#include "heartbeat.h"
#include "rxtxtask.h"
#include "wdttask.h"

#include "led_utils.h"
#include "rtc_atmega328p.h"
#include "serial_atmega328p.h"
#include "wdt_atmega328p.h"

#define NUM_TASKS               3

void setup(void) {
	/*
	 * Disable interrupts...
	 */
	cli();

	setupWDT();

	setupLEDPin();
	setupRTC();
	setupSerial();

	/*
	 * Enable interrupts...
	 */
    sei();
}

int main(void) {
	setup();

	/*
	 * Switch on the LED so we know when the board has reset...
	 */
	turnOn(LED_ONBOARD);

	initScheduler(NUM_TASKS);

	registerTask(TASK_WDT, &wdtTask);
	registerTask(TASK_HEARTBEAT, &HeartbeatTask);
	registerTask(TASK_RXCMD, &RxTask);

	scheduleTask(
			TASK_WDT,
			rtc_val_ms(250),
			true,
			NULL);

	scheduleTask(
			TASK_HEARTBEAT,
			rtc_val_sec(3),
			false,
			NULL);

	/*
	** Start the scheduler...
	*/
	schedule();

	/*
	 * It should never get here...
	 */
	return -1;
}
