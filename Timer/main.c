/*
Use HSI oscillator used as the system clock(RCC) :16MHz
*/
typedef unsigned long      int uint32_t;
typedef unsigned short     int uint16_t;
#define   __IO    volatile             /*!< Defines 'read / write' permissions              */
#define   __I     volatile             /*!< Defines 'read only' permissions	 								*/
#define EXTI ((EXTI_TypeDef *) 0x40013C00)
#define GPIOG    ((GPIO_TypeDef *)(0x40021800))                // register location GPIOG starting at 0x4002 1800
#define GPIOA    ((GPIO_TypeDef *)0x40020000)                //register location GPIOA starting at 0x4002 0000
#define Pin_0   ((uint16_t)0x0001)  											/* Pin 0 selected */
#define RCC_AHB1ENR *((uint32_t *)(0x40023800+0x30))
#define RCC_APB2ENR *((uint32_t *)(0x40023800+0x44))
#define RCC_APB1ENR *((uint32_t *)(0x40023800+0x40))
#define SYSCFG_EXTICR1 *((uint32_t *)(0x40013800+0x08))						//SYSCFG register starting at 0x40013800 + offset EXTICR1=0x08
#define NVIC		((NVIC_Type *)(0xE000E100))											//NVIC register starting at 0xE000E100 								
#define SCB_AIRCR *((uint32_t *)(0xE000E008+0x0C))								// SCB register starting at 0xE000E008 +offset AIRCR=0x0C
#define SYSTICK  ((SysTick_Type *)0xE000E010)
#define TIM6  ((TIM_TypeDef *)0x40001000) 									// TIM6 register starting at 0xE000E008

//#define TIM8  ((TIM_TypeDef *)0x40010400) 									// TIM8 register starting at 0x40010400

#define WRIBIT(R,P,V) (V!=0)?(R = R|1<<P):(R = R &~(1<<P))    //write value V for register R at bit P 

int temp =0;
int count=0;
int temp_Sys=0;
int temp_Timer6=0;
void EXTI0_IRQHandler(void) ;
void loop(unsigned int inp);
uint32_t SysTick_Config(uint32_t ticks);


typedef struct
{
  volatile uint32_t ISER[8];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
        
}  NVIC_Type;
typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;



typedef struct
{
  __IO uint32_t IMR;    /*!< EXTI Interrupt mask register,            Address offset: 0x00 */
  __IO uint32_t EMR;    /*!< EXTI Event mask register,                Address offset: 0x04 */
  __IO uint32_t RTSR;   /*!< EXTI Rising trigger selection register,  Address offset: 0x08 */
  __IO uint32_t FTSR;   /*!< EXTI Falling trigger selection register, Address offset: 0x0C */
  __IO uint32_t SWIER;  /*!< EXTI Software interrupt event register,  Address offset: 0x10 */
  __IO uint32_t PR;     /*!< EXTI Pending register,                   Address offset: 0x14 */
} EXTI_TypeDef;

typedef struct
{
	__IO uint32_t CR1;  		/* control register 1          Address offset: 0x00 */
	__IO uint32_t CR2;  		/* control register 2          Address offset: 0x04 */
			 uint32_t RESERVED1;  /*Reserved,																	Address offset: 0x08 */
	__IO uint32_t DIER;/* DMA Interrupt enable register		Address offset: 0x0C */
	__IO uint32_t SR; /*TIM6 and TIM7 status register Address offset: 0x10 */
	__IO uint32_t EGR; /*TIM6 and TIM7 event generation register Address offset:0x14*/
			 uint32_t RESERVED2[3]; /*Reserved,																	Address offset: 0x18 */
	__IO uint32_t CNT;/*TIM6 and TIM7 counter Address offset: 0x24  */
	__IO uint32_t PSC;/*TIM6 and TIM7 prescaler Address offset: 0x28 */
	__IO uint32_t ARR; /*TIM6 and TIM7 auto-reload register Address offset: 0x2C */
	
}TIM_TypeDef;

/*
Function for delay
*/
   void Delay(int t){
	int i;
	for (i=0; i<t; i++);
}

/**
   Toggles the specified GPIO pins..
  */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

  GPIOx->ODR ^=(1<<GPIO_Pin);
}



/** \brief  Structure type to access the System Timer (SysTick).
 */
typedef struct
{
  __IO uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IO uint32_t LOAD;                    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
  __IO uint32_t VAL;                     /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */
  __I  uint32_t CALIB;                   /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */
} SysTick_Type;


int main()
{

	(RCC_AHB1ENR)|=(1<<6)|(1<<0); 		// clock for PORTG and PORTA


		//	//config for pin PD13(Green), PD14(Red)
	GPIOG->MODER|=(1<<26)|(1<<28); 						// bit for set mode pin 13,14 is mode output
	GPIOG->OTYPER|=(0<<13)|(0<<14);  					// output type for pin 13,14 is push pull
	GPIOG->OSPEEDR|=(1<<26)|(1<<28);      		//  speed is medium
	GPIOG->ODR|=(1<<13);											// set initial value for pinout 13=on,14=off
	GPIOG->ODR&=~(1<<14);
   
	
	/*     config EXTI0*/
	(RCC_APB2ENR)|=(1<<14);										//enable SYSCFGEN clock on RCC APB2 register
	
	GPIOA->MODER&=~(0b11<<0); 										// config for pin PA0 as floating input GPIO_Mode_IN
	GPIOA->PUPDR|=(2<<0); 										// 																			GPIO_PuPd_DOWN
			
	SYSCFG_EXTICR1&=~(0b1111<<0);									// Select the GPIOA pin 0 as input for the interrupt line INTI0
																						// set 0000( PA0 pin) for bit 3-0
	(EXTI->IMR)|=(1<<0); 											// unmask EXTI0

	(EXTI->RTSR)|=(1<<0);											// Rising trigger enabled (for Event and Interrupt) for input line 0
	(EXTI->FTSR)&=~(1<<0);										// Falling trigger disabled (for Event and Interrupt) for input line 0
	
	
	NVIC->ISER[0]|=(1<<6);										// enable interrupt line for EXTI0
	SCB_AIRCR&=~(0b111<<8);												// Select the priority and subpriority of the interrupt to the lowest priority
																						// set (PRIGROUP)bit 8-10 of AIRCR register
	EXTI->PR|=(1<<0);  												// clear pending bit 0

/* Config TIM6*/
	(RCC_APB1ENR)|=(1<<4); 									//enable TIMER 6 clock register on RCC APB1 register
	TIM6->PSC=64999;														//PSC <=0xffff(65535)
																			//f-CL_CNT=(f-CL-PSC)/(PSC+1)=16MHz/(64999+1)=246Hz
																			// clock period =1/246(seconds)
TIM6->ARR=245;												// Period=(ARR+1)*clock period=(245+1)*(1/246)	=1(seconds)		

TIM6->CR1|=(1<<7)|(1<<0);										//at CR1 register enable Auto-reload preload by set  bit 7
																						// 								enable counter						 by set  bit 0						
TIM6->DIER|=(1<<0);                         //Update interrupt enabled.

NVIC->ISER[1]|=(1<<22);											//enable interrup line TIM6 in NVIC
	 

/* Config SysTick*/
int b=SysTick_Config(1000000);
		
   while(1)
   { 

   }
 }

 void EXTI0_IRQHandler(void)  
{
	if((EXTI->PR&Pin_0)!=0)   // edge event arrives on the external interrupt line 0
	{
		Delay(15000);
		count++;
		if(count>=4)
			count=0;
		EXTI->PR|=(1<<0);  // clear pending bit 0
	}
	
}

uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1) >0xFFFFFF)  return (1);      /* Reload value impossible <1677215*/
	SYSTICK->LOAD=ticks -1;												/* set reload register */
	SYSTICK->VAL   =0;														/* Load the SysTick Counter Value */
  SYSTICK->CTRL|=(0x07<<0);										 /* Enable SysTick IRQ and SysTick Timer ,*/
	SYSTICK->CTRL&=~(1<<2);											/*Selects the clock source is AHB/8=16Mhz/8=2MHz*/
	return (0);                                        /* Function successful */

}

void SysTick_Handler(void)
{
	temp_Sys++;
	if(temp_Sys>100)
		temp_Sys=0;
	GPIO_ToggleBits(GPIOG,13);
}

void TIM6_DAC_IRQHandler(void)
{
	temp_Timer6++;
	if(temp_Timer6>100)
		temp_Timer6=0;
	GPIO_ToggleBits(GPIOG,14);
	TIM6->SR&=~(1<<0); 												//clear pending bit to exit interrupt
}
