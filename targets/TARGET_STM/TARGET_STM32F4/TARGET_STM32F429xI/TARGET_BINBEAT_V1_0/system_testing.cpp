#include "system_testing.h"
#include "mbed.h"

DigitalOut enableDebugPortRX(PB_15, 0); //enable RX on debug port
DigitalOut debugPortForceOff(PD_10, 1); // uses inverse logic!
DigitalOut debugPortForceOn(PD_11, 1);
DigitalOut disableVc(PC_6, 0);  //enable vcontrolled
DigitalOut enable3v3(PG_1, 1);
PwmOut buzzer(PF_7); 
DigitalOut enable5v(PB_1, 1);
InterruptIn irq(PA_0);

void board_powerup()
{
    wait_ms(500);
}


void buzzzz()
{
    buzzer.period(1/(float)2050); //xx seconds period
	buzzer = 0.1; //duty cycle, relative to period
}


void mute()
{
    buzzer = 0;
}


void beep()
{
    buzzzz();
    wait_ms(100);
    mute();
}