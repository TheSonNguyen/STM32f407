
typedef unsigned char     uint8_t;
typedef unsigned long     uint32_t;

/**< This struct have 32 element. Each element control 1 bit of data. */
typedef struct byte {
	uint8_t bit0: 1;	/**< As bit 0 of data. */
	uint8_t bit1: 1;	/**< As bit 1 of data. */
	uint8_t bit2: 1;	/**< As bit 2 of data. */
	uint8_t bit3: 1;	/**< As bit 3 of data. */
	uint8_t bit4: 1;	/**< As bit 4 of data. */
	uint8_t bit5: 1;	/**< As bit 5 of data. */
	uint8_t bit6: 1;	/**< As bit 6 of data. */
	uint8_t bit7: 1;	/**< As bit 7 of data. */
	uint8_t bit8: 1;	/**< As bit 8 of data. */
	uint8_t bit9: 1;	/**< As bit 9 of data. */
	uint8_t bit10: 1;	/**< As bit 10 of data. */
	uint8_t bit11: 1;	/**< As bit 11 of data. */
	uint8_t bit12: 1;	/**< As bit 12 of data. */
	uint8_t bit13: 1;	/**< As bit 13 of data. */
	uint8_t bit14: 1;	/**< As bit 14 of data. */
	uint8_t bit15: 1;	/**< As bit 15 of data. */
	uint8_t bit16: 1;	/**< As bit 16 of data. */
	uint8_t bit17: 1;	/**< As bit 17 of data. */
	uint8_t bit18: 1;	/**< As bit 18 of data. */
	uint8_t bit19: 1;	/**< As bit 19 of data. */
	uint8_t bit20: 1;	/**< As bit 20 of data. */
	uint8_t bit21: 1;	/**< As bit 21 of data. */
	uint8_t bit22: 1;	/**< As bit 22 of data. */
	uint8_t bit23: 1;	/**< As bit 23 of data. */
	uint8_t bit24: 1;	/**< As bit 24 of data. */
	uint8_t bit25: 1;	/**< As bit 25 of data. */
	uint8_t bit26: 1;	/**< As bit 26 of data. */
	uint8_t bit27: 1;	/**< As bit 27 of data. */
	uint8_t bit28: 1;	/**< As bit 28 of data. */
	uint8_t bit29: 1;	/**< As bit 29 of data. */
	uint8_t bit30: 1;	/**< As bit 30 of data. */
	uint8_t bit31: 1;	/**< As bit 31 of data. */

} bit;

typedef union {
	volatile uint32_t fulldata;	
	bit bytedata;			/**< As bit type which can control each bit of data. */
} port;


#define RCC_AHB1ENR      	((port *)(0x40023830))	//dar = adr resger + offset 
#define GPIOG_MODER    		((port *)(0x40021800))
#define GPIOG_OTYPER     	((port *)(0x40021804))	
#define GPIOG_OSPEEDR     ((port *)(0x40021808))	
#define GPIOG_PUPDR     	((port *)(0x4002180C))	
#define GPIOG_IDR    			((port *)(0x40021801))	
#define GPIOG_ODR     		((port *)(0x40021814))	
#define GPIOG_BSRR     		((port *)(0x40021818))  
#define GPIOG_LCKR    		((port *)(0x4002181C))	
#define GPIOG_AFRL     		((port *)(0x40021820))	
#define GPIOG_AFRH     		((port *)(0x40021824))   

#define GPIOA_MODER    		((port *)(0x40020000))
#define GPIOA_OTYPER     	((port *)(0x40020004))	
#define GPIOA_OSPEEDR     ((port *)(0x40020008))	
#define GPIOA_PUPDR     	((port *)(0x4002000C))	
#define GPIOA_IDR    			((port *)(0x40020010))	
#define GPIOA_ODR     		((port *)(0x40020014))	
#define GPIOA_BSRR     		((port *)(0x40020018))  
#define GPIOA_LCKR    		((port *)(0x4002001C))	
#define GPIOA_AFRL     		((port *)(0x40020020))	
#define GPIOA_AFRH     		((port *)(0x40020024))   
#define X 500000
void config_led();
void config_button();
int m;
int main(void)
{

	config_led();
	config_button();
	
	while(1)
	{
		if(GPIOA_IDR->bytedata.bit0 == 1)
			{
					for(int i=0;i<20;i++);
			if(GPIOA_IDR->bytedata.bit0 == 1)
				{
					m++;
					if (m>3) m = 1;
				}
			while(GPIOA_IDR->bytedata.bit0 == 1);
		
			}
		if (m==1)
			{
				GPIOG_ODR->bytedata.bit14=1;
						for(int i=0;i<X;i++);
				GPIOG_ODR->bytedata.bit14=0;
						for(int i=0;i<X;i++);
			}
		if (m==2)	
				{
				GPIOG_ODR->bytedata.bit14=1;
					for(int i=0;i<X/2;i++);
				GPIOG_ODR->bytedata.bit14=0;
					for(int i=0;i<X/2;i++);
				}
		if (m==3)	
				{
				GPIOG_ODR->bytedata.bit14=1;
					for(int i=0;i<X/4;i++);
				GPIOG_ODR->bytedata.bit14=0;
					for(int i=0;i<X/4;i++);
				}
}
}
	
void config_led()
{
		RCC_AHB1ENR->bytedata.bit6=1;  //Enable Clock
//---------------------------------------------	
		GPIOG_MODER->bytedata.bit28=1;  //Config pin 14 of port D
		GPIOG_MODER->bytedata.bit29=0;
	
		GPIOG_OTYPER->bytedata.bit14=0;

		GPIOG_OSPEEDR->bytedata.bit28=0;
		GPIOG_OSPEEDR->bytedata.bit29=1;
}

void config_button()
{
		RCC_AHB1ENR->bytedata.bit0=1;  //Enable Clock
//---------------------------------------------	
		GPIOA_MODER->bytedata.bit1=0;  //Config pin 15 of port D
		GPIOA_MODER->bytedata.bit0=0;
	
		GPIOA_OTYPER->bytedata.bit0=0;

		GPIOA_OSPEEDR->bytedata.bit1=1;
		GPIOA_OSPEEDR->bytedata.bit0=1;
	
		GPIOA_PUPDR->bytedata.bit1=1;
		GPIOA_PUPDR->bytedata.bit0=0;
}

