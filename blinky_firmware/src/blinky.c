#include <blinky.h>

/*
* Initialize GPIOB's  GPIO_Pin_5 as a push-pull GPIOB pin.
*/
void InitializeLEDs()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ??
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_5;
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &gpioStructure); // configure gpio pin 5
}

/*
* Enable timer and set to interrupt.
*/
void InitializeTimer()
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // ?? 
    
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 4000;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 1000;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &timerInitStructure); // configure timer 4
    TIM_Cmd(TIM4, ENABLE); // enable the counter for timer 4
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // enable interrupt on timer 4
}


/*
* Enable and initialize interrupt on timer 4
*/
void InitializeInterrupt()
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = TIM4_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic); // configure the timer 4 interrupt
}

/*
* Interrupt Callback
*/
void TIM4_IRQHandler()
{
	// If interrupt set, reset interrupt and write the toggled led state
    if (TIM_GetITStatus(TIM4, TIM_IT_Update)!= RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        led_state = 1-led_state;
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, led_state);
    }
}

/*
* Main Function
*/
int main()
{
    InitializeLEDs();
    InitializeTimer();
    InitializeInterrupt();
    while(1);
    return(0);
}

