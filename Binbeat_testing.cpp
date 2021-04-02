#include "Binbeat_testing.h"
#include "mbed.h"

DigitalOut enableDebugPortRX(EN_RS232, 0);       //enable RX on debug port
DigitalOut debugPortForceOff(FORCEOFF_RS232, 1); // uses inverse logic!
DigitalOut debugPortForceOn(FORCEON_RS232, 1);

DigitalOut disableVc(PC_6, 0); //enable vcontrolled

DigitalOut enable5v(EN_5V_LOGIC, 1);
DigitalOut enable3v3(EN_3V3_LOGIC, 1);

PwmOut buzzer(PF_7);

void board_powerup()
{
    ThisThread::sleep_for(500);
}

void buzzzz()
{
    buzzer.period(1 / (float)2050); //xx seconds period
    buzzer = 0.1;                   //duty cycle, relative to period
}
void mute()
{
    buzzer = 0;
}

void beep()
{
    buzzzz();
    ThisThread::sleep_for(100);
    mute();
}