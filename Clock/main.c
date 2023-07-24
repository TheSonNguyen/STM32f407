#include <stdint.h>
//clock
#define RCC_AHB1ENR 		*((uint32_t*)(0x30+0x40023800))
#define RCC_CR      		*((uint32_t*)(0x00+0x40023800))
#define RCC_CFGR				*((uint32_t*)(0x08+0x40023800)) ////OFFSET RCC 0x40023800

//led portG
#define GPIOG_MODER		  *((uint32_t*)(0x00+0x40021800)) //OFFSET GPIOG 0x40021800
#define GPIOG_OTYPER 		*((uint32_t*)(0x04+0x40021800))
#define GPIOG_OSPEEDR		*((uint32_t*)(0x08+0x40021800))
#define GPIOG_ODR 			*((uint32_t*)(0x14+0x40021800))


//button portA
#define GPIOA_MODER 		*((uint32_t*)(0x00+0x40020000)) //OFFSET GPIOA 0x40020000
#define GPIOA_PUPDR 		*((uint32_t*)(0x0C+0x40020000))
#define GPIOA_IDR 			*((uint32_t*)(0x10+0x40020000))

//INTERRUPT		offset EXTI 0x40013C00
#define RCC_APB2ENR     *((uint32_t*)(0x44+0x40023800))
#define SYSCFG_EXTICR1	*((uint32_t*)(0x08+0x40013800))
#define EXTI_IMR				*((uint32_t*)(0x00+0x40013C00)) //Interrupt mask register
#define EXTI_EMR				*((uint32_t*)(0x04+0x40013C00))	//Event mask register
#define EXTI_RTSR				*((uint32_t*)(0x08+0x40013C00)) //Rising trigger selection register
#define EXTI_FTSR			 	*((uint32_t*)(0x0C+0x40013C00)) //Falling trigger selection register
#define EXTI_SWIER			*((uint32_t*)(0x10+0x40013C00))	//Software interrupt event register
#define EXTI_PR 			 	*((uint32_t*)(0x14+0x40013C00)) //Pending register 

//#define SETBIT(R,P) (R|(1<<P))
//#define CLRBIT(R,P) (R&(~(1<<P)))
#define WBIT(R,P,V) (V>0)?(R|(1<<P)):(R&(~(1<<P)))
#define X 250000
int A;
void delay_ms(int inp);
void config_led();
//void config_button();
void config_clock();
void config_interrupt();

int main(){
		config_led();
  // config_button();
		config_clock();
		config_interrupt();
		 
		while(1)
		{
			if (((GPIOA_IDR)&0x01)==1) 
			{
				for(int i=0;i<10;i++){}; 
			  if (((GPIOA_IDR)&0x01)==1)
					{
						A++;
						if (A>3) A=	0;
					}
					while  (((GPIOA_IDR)&0x01)==1);
			}
			if (A==0)
			{		 
				GPIOG_ODR = 0x2000;
				delay_ms(X);
				GPIOG_ODR = 0x4000;
				delay_ms(X);
			}
			if(A==1)
			{
				GPIOG_ODR = 0x2000;
				delay_ms(X*2);
				GPIOG_ODR = 0x4000;
				delay_ms(X*2);
			}
			if (A==2)
			{
				GPIOG_ODR = 0x2000;
				delay_ms(X*4);
				GPIOG_ODR = 0x4000;
				delay_ms(X*4);
			}
			if (A==3)
			{
				GPIOG_ODR = 0x2000;
				delay_ms(X*8);
				GPIOG_ODR = 0x4000;
				delay_ms(X*8);
			}
		}  
}
void config_interrupt()
{
			RCC_AHB1ENR =  WBIT(RCC_AHB1ENR, 0,1);//CLOCK FOR PORT A
			//SET MODE BUTTON
			GPIOA_MODER = WBIT(GPIOA_MODER, 0,0);  // 00 MEANS INPUT BUTTON   
			GPIOA_MODER = WBIT(GPIOA_MODER, 1,0);
			//SET PULL-UP/PULL-DOW
			GPIOA_PUPDR = WBIT( GPIOA_PUPDR,0,0);		//10 PULL DOW
			GPIOA_PUPDR = WBIT( GPIOA_PUPDR,1,1);
			//CHOOSE PORT A0
			SYSCFG_EXTICR1 = WBIT(SYSCFG_EXTICR1,3,0);
			SYSCFG_EXTICR1 = WBIT(SYSCFG_EXTICR1,2,0);
			SYSCFG_EXTICR1 = WBIT(SYSCFG_EXTICR1,1,0);
			SYSCFG_EXTICR1 = WBIT(SYSCFG_EXTICR1,0,0);
			//CLOCK SYSCFG ENABLE
			RCC_APB2ENR = WBIT(SYSCFG_EXTICR1,14,1);
			EXTI_RTSR		= WBIT(EXTI_RTSR,0,1);
			EXTI_IMR		= WBIT(EXTI_IMR,0,1);		
}

void config_clock()
{			
			//
			RCC_CR	 =	WBIT(RCC_CR,16,1); //BIT 16 HSEON RW
			RCC_CR	 =	WBIT(RCC_CR,17,1); //BIT 17 HSERDY R
	
			//SYSTEM CLOCK SWITCH
			RCC_CFGR =	WBIT(RCC_CFGR,0,1);
			RCC_CFGR =	WBIT(RCC_CFGR,1,0); //01 SELECT HSE
			RCC_CFGR =	WBIT(RCC_CFGR,2,1); //01 SELECT HSE
			RCC_CFGR =	WBIT(RCC_CFGR,3,0);
	
	
			//SET BO CHIA 1000 (2)
			RCC_CFGR =	WBIT(RCC_CFGR,4,0);
			RCC_CFGR =	WBIT(RCC_CFGR,5,0);
			RCC_CFGR =	WBIT(RCC_CFGR,6,0);
			RCC_CFGR =	WBIT(RCC_CFGR,7,1);
}
void config_led()
{
			RCC_AHB1ENR =  WBIT(RCC_AHB1ENR,6,1);  // CLOCK FOR PORTG
			//SET MODE LED
			GPIOG_MODER = WBIT(GPIOG_MODER, 26,1);  // 01 MEANS OUTPUT   
			GPIOG_MODER = WBIT(GPIOG_MODER, 27,0);
			GPIOG_MODER = WBIT(GPIOG_MODER, 28,1);  // 01 MEANS OUTPUT  
			GPIOG_MODER = WBIT(GPIOG_MODER, 29,0);
	
			//SET OUTPUT TYPE
			GPIOG_OTYPER = WBIT( GPIOG_OTYPER,13,0); //  Pull-Push
			GPIOG_OTYPER = WBIT( GPIOG_OTYPER,14,0);
	
	
			//SET SPEED MEDIUM
			GPIOG_OSPEEDR = WBIT(GPIOG_OSPEEDR, 26,1); //1
			GPIOG_OSPEEDR = WBIT(GPIOG_OSPEEDR, 27,0); //0
			GPIOG_OSPEEDR = WBIT(GPIOG_OSPEEDR, 28,1); //1
			GPIOG_OSPEEDR = WBIT(GPIOG_OSPEEDR, 29,0); //0
}

void delay_ms(int inp)
{	
	for (int i=0;i<8000;i++){
  while(inp > 0)
    inp --;
}
}

