#include "stm32f4xx.h"
#include "stdlib.h"

static int numberofStep	=400; // 
long speed=50; // rpm
long delay;
int step_number=0;
_Bool direction;
void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);
void Step(int steps_to_move);
void StepMotor(int thisStep);

int main(void)
{
	GPIO_Configuration();
	delay=600*1000L*1000L/numberofStep/speed;
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);	//ENA
	GPIO_SetBits(GPIOA,GPIO_Pin_1);	//ENB
	
  while(1)
  {
		Step(400);Delay(500000);  
		Step(-400);Delay(500000);
  }
}

void GPIO_Configuration(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef  GPIO_InitStructure;
  /* Configure PB0 PB1 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure PA0 PA1 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
 
	
void Delay(__IO uint32_t nCount)
{
	while(nCount--){}
}

void Step(int steps_to_move)
{
	int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { direction = 1; }
  if (steps_to_move < 0) { direction = 0; }
	while(steps_left>0)
	{
		Delay(delay);
		if(direction==1){
			step_number++;
			if(step_number==numberofStep){step_number=0;}
		}
		else{
			if(step_number==0){step_number=numberofStep;}
			step_number--;
		}
		steps_left--;
		StepMotor(step_number % 4);
	}
}

void StepMotor(int thisStep)
{
	switch (thisStep) {
      case 0:  // 1010
				GPIO_Write(GPIOB,0x0A);
      break;
      case 1:  // 0110
				GPIO_Write(GPIOB,0x06);
      break;
      case 2:  //0101
				GPIO_Write(GPIOB,0x05);
      break;
      case 3:  //1001
				GPIO_Write(GPIOB,0x09);
      break;
    }
}