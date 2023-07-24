
typedef unsigned long      int uint32_t;
typedef unsigned short     int uint16_t;
#define     __IO    volatile 
#define EXTI ((EXTI_TypeDef *) 0x40013C00)
#define GPIOG    ((GPIO_TypeDef *)(0x40021800))                // register location GPIOG starting at 0x4002 1800
#define GPIOA    ((GPIO_TypeDef *)0x40020000)                //register location GPIOA starting at 0x4002 0000
#define Pin_0   ((uint16_t)0x0001)  											/* Pin 0 selected */
#define RCC_AHB1ENR *((uint32_t *)(0x40023800+0x30))
#define RCC_APB2ENR *((uint32_t *)(0x40023800+0x44))
#define SYSCFG_EXTICR1 *((uint32_t *)(0x40013800+0x08))		   //SYSCFG register starting at 0x40013800 + offset EXTICR1=0x08
#define NVIC		((NVIC_Type *)(0xE000E100))				   //NVIC register starting at 0xE000E100 								
#define SCB_AIRCR *((uint32_t *)(0xE000E008+0x0C))             // SCB register starting at 0xE000E008 +offset AIRCR=0x0C

#define WRIBIT(R,P,V) (V!=0)?(R = R|1<<P):(R = R &~(1<<P))    //write value V for register R at bit P 

int temp =0;
int count=0;
void EXTI0_IRQHandler(void) ;
void loop(unsigned int inp);


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



int main()
{

	(RCC_AHB1ENR)|=(1<<6)|(1<<0); 		// clock for PORTG and PORTA


		//	//config for pin PD13, PD14
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


		
   while(1)
   { 
		  switch (count)
		{
			case 0:
			{
				Delay(500000);
				Delay(500000);
				break;
			}
			case 1:
			{
				Delay(250000);
				Delay(250000);
				break;
			}
			case 2:
			{
				Delay(125000);
				Delay(125000);
				break;
			}
			default:
			{
				Delay(62500);
				Delay(62500);
				break;
			}
		}
		 	GPIO_ToggleBits(GPIOG,13);
			GPIO_ToggleBits(GPIOG,14);
//		 
//		 
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

