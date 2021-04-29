#include "stm8s.h"
#include "milis.h"

#include "delay.h"

#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define LED_ON   GPIO_WriteHigh(LED_PORT, LED_PIN);
#define LED_OFF  GPIO_WriteLow(LED_PORT, LED_PIN);
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN);

#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#define BTN_PUSH (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)==RESET)

#define PZ_PORT GPIOD
#define PZ_PIN  GPIO_PIN_4
#define PZ_DOWN   GPIO_WriteHigh(PZ_PORT, PZ_PIN);
#define PZ_UP  GPIO_WriteLow(PZ_PORT, PZ_PIN);
#define PZ_REVERSE GPIO_WriteReverse(PZ_PORT, PZ_PIN);


void delay_ms(uint16_t ms)
{
    uint16_t i;
    for (i = 0; i < ms; ++i) {
        _delay_us(250);
        _delay_us(250);
        _delay_us(250);
        _delay_us(248);
    }
}

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PZ_PORT, PZ_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    init_milis();
}

/*! Toto je ukázka funkce main napsané blokujícím způsobem
 *  ... pořád se jen na něco čeká a nic jiného se dělat nedá.
 */
void main_wrong(void)
{
    init();

    uint16_t i;

    while (1) {
        for (i = 0; i < 20; ++i) {
            PZ_UP;
            delay_ms(1);
            PZ_DOWN;
            delay_ms(1);
        }
        delay_ms(60);
    }
}


void main(void)
{
    uint32_t timeL = 0;
    uint32_t lastTime = 0;
    uint16_t count = 0;
    enum state_of {
        STATE_PAUSE,
        STATE_UP,
        STATE_DOWN
    } state = STATE_UP;
    uint32_t time = 0;

    init();

    while (1) {
        switch (state) {
        case STATE_PAUSE:
            PZ_DOWN;            // v pauze dám repráček do klodového stavu
            if (milis() - lastTime > 777) {
                lastTime = milis();
                state = STATE_UP;
            }
            break;
        case STATE_UP:
            PZ_UP;
            if (milis() - lastTime >= 4) {
                lastTime = milis();
                state = STATE_DOWN;
            }
            break;
        case STATE_DOWN:
            PZ_DOWN;
            if (count >= 10) {
                count = 0;
                lastTime = milis();
                state = STATE_PAUSE;
                break;
            }
            if (milis() - lastTime >= 4) {
                lastTime = milis();
                count++;
                state = STATE_UP;
            }
            break;
        default:
            state = STATE_PAUSE;
        }

        if (milis() - timeL > 555) {
            timeL = milis();
            LED_REVERSE;
        }
    }
}


/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
