
#if MBED_CONF_NSAPI_PRESENT

#include "onboard_modem_api.h"
#include "gpio_api.h"
#include "PinNames.h"
#include "platform/mbed_wait_api.h"

gpio_t gpio_GPRS_3V8_EN;
gpio_t gpio_UART_OE;
gpio_t gpio_GPRS_RTS;
gpio_t gpio_GPRS_POWERON;
gpio_t gpio_GPRS_RSTN;
gpio_t gpio_GPRS_I2C;

#if MODEM_ON_BOARD

void onboard_modem_init()
{
    gpio_init_out_ex(&gpio_GPRS_I2C, OE_I2C, 0);         // Disable I2C for GPRS (we don't use it)
    gpio_init_out_ex(&gpio_GPRS_3V8_EN, EN_3V8_GPRS, 0); // Disable power
    gpio_init_out_ex(&gpio_GPRS_RSTN, RSTN_GPRS, 0);     // Reset
    gpio_init_out_ex(&gpio_UART_OE, OE_UART_GRPS, 1);    // Disable level shifter
    gpio_init_out_ex(&gpio_GPRS_RTS, MDMRTS, 0);         // Clear ReadyToSend
    gpio_init_out_ex(&gpio_GPRS_POWERON, PWRON_GPRS, 0); // Control powerup modem
}

void onboard_modem_deinit()
{
}

void onboard_modem_power_up()
{
    gpio_write(&gpio_GPRS_3V8_EN, 1);  // Enable 3V8 PSU
    gpio_write(&gpio_UART_OE, 0);      // Enable UART level shifter
    wait_ms(10);                       // Wait for PSU to settle
    gpio_write(&gpio_GPRS_RTS, 0);     // Clear ReadyToSend
    gpio_write(&gpio_GPRS_POWERON, 1); // Power up modem
    gpio_write(&gpio_GPRS_RSTN, 1);    // Do "reset"
    wait_ms(5);                        // Wait for actions to be processed
}

void onboard_modem_power_down()
{
    gpio_write(&gpio_GPRS_RSTN, 0);    // Prepare for next reset
    gpio_write(&gpio_GPRS_POWERON, 0); // Power down modem
    wait_ms(5);                        // Wait for actions to be processed
    gpio_write(&gpio_UART_OE, 1);      // Disable UART level shifter
    gpio_write(&gpio_GPRS_3V8_EN, 0);  // Disable 3V8 PSU
    wait_ms(10);                       // Wait for PSU to settle
}

#endif //MODEM_ON_BOARD
#endif //MBED_CONF_NSAPI_PRESENT
