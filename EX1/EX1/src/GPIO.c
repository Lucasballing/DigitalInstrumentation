


#init for the Joystick
void initJoystick(){
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE); // Enable clock for GPIO Port C
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE); // Enable clock for GPIO Port B
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // Enable clock for GPIO Port A
     GPIO_InitTypeDef GPIO_InitStructAll; // Define typedef struct for setting pins

     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN; // Set as input
     GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down


     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4; // Set so the configuration is on pin 4
     GPIO_Init(GPIOA, &GPIO_InitStructAll);

     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4; // Set so the configuration is on pin 4
     GPIO_Init(GPIOA, &GPIO_InitStructAll);

     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4; // Set so the configuration is on pin 4
     GPIO_Init(GPIOA, &GPIO_InitStructAll);

     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4;
     GPIO_Init(GPIOA, &GPIO_InitStructAll); // Setup of GPIO with the settings chosen
     // Sets PA9 to output
     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct
     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT; // Set as output
     GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP; // Set as Push-Pull
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_9; // Set so the configuration is on pin 9
     GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz; // Set speed to 2 MHz
     GPIO_Init(GPIOA, &GPIO_InitStructAll); // Setup of GPIO with the settings chosen

}
