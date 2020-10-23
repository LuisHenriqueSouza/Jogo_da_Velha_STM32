/* Projeto: Jogo Da Velha Eletrônico
 *
 * Autores:
 * 		*Gabriel Santos
 * 		*Luís Henrique
 *
 * Turma:4324
 *
 * Disciplina: Sistemas Microprocessados I
 *
 */

#include "stm32f4xx.h"

#define PSC_1us (16000-1)
#define ARR_2ms (2-1)
#define ARR_250ms (50-1)
#define ARR_1s 999

#define col_1 GPIO_ODR_ODR_3
#define col_2 GPIO_ODR_ODR_4
#define col_3 GPIO_ODR_ODR_5

#define lin_3 GPIO_IDR_IDR_2
#define lin_2 GPIO_IDR_IDR_1
#define lin_1 GPIO_IDR_IDR_0

/* matriz botoes
  -----------
   0 | 0 | 0      *0->Posição Vazia
  -----------     *1->Posição Vermelha
   0 | 0 | 0      *2->Posição Azul
  -----------
   0 | 0 | 0
  -----------
*/

int control = 0x01; // variavel de controle do mux botoes
int j = 0;  //variavel de repetição
int k = 0;  //variavel de repetição
int j2 =0;  //variavel de repetição
int k1 =0;  //variavel de repetição
int aux_temp=0;// variavel de contagem de tempo da jogada
int conta_tempo_piscada=0; // variavel de contagem das piscadas
int t=0; // variavel de auxilio para o debouce
int habilita_IA=1;
int a=0;
int8_t botoes [3][3]={0};  // matriz onde são armazenadas as jogadas
int8_t botoes_extra [3][3]={0};// matriz de auxilio das piscadas


int8_t player =1;// variavel de vez do jogador
int8_t jogada_IA = 1;//variavel de controle da "IA"
int8_t numero_da_jogada = 0;

void mux_vermelho(int *linha, int *coluna)
{

/*
	PC6->8 : vermelho coluna
	PC9->11:verde coluna
	PB0->2 linha catodo

	FUNÇÃO LIGA A LINHA E COLUNA DO LED VERMELHO QUE CORRESPONDE A POSIÇÃO NA MATRIZ



*/

	switch(*linha){
	case 0:
	GPIOC->ODR |= GPIO_ODR_ODR_6;
	break;

	case 1:
	GPIOC->ODR |= GPIO_ODR_ODR_7;
	break;

	case 2:
	GPIOC->ODR |= GPIO_ODR_ODR_8;
	break;

	 }


	switch(*coluna)
	{
	   case 0:
			GPIOB->ODR |= GPIO_ODR_ODR_0;
			break;
	   case 1:
			GPIOB->ODR |= GPIO_ODR_ODR_1;
			break;
	   case 2:
			GPIOB->ODR |= GPIO_ODR_ODR_2;
			break;
	}
}

void mux_verde(int *linha2, int *coluna2)
{

/*
	PC6->8 : vermelho coluna
	PC9->11:verde coluna
	PB0->2 linha catodo

	FUNÇÃO LIGA A LINHA E COLUNA DO LED VERDE QUE CORRESPONDE A POSIÇÃO NA MATRIZ


*/

		switch(*linha2){
			case 0:
				GPIOC->ODR |= GPIO_ODR_ODR_9;
				break;

			case 1:
				GPIOC->ODR |= GPIO_ODR_ODR_10;
				break;

			case 2:
				GPIOC->ODR |= GPIO_ODR_ODR_11;
				break;
		 }


		switch(*coluna2)
		{
		   case 0:
				GPIOB->ODR |= GPIO_ODR_ODR_0;
				break;

		   case 1:
				GPIOB->ODR |= GPIO_ODR_ODR_1;
				break;
		   case 2:
				GPIOB->ODR |= GPIO_ODR_ODR_2;
				break;
		}
}

void mux_botoes(void)
{


		switch(control)
		{

		case 1:
			GPIOC->ODR &=~ col_1; // HABILITA COLUNA 1
			GPIOC->ODR |=  col_2; // DESABILITA COLUNA 2
			GPIOC->ODR |=  col_3; // DESABILITA COLUNA 3

			if(!(GPIOC->IDR&lin_1)&&(botoes[0][0]==0)){botoes[0][0] = player;  aux_temp=0;  player^=0x03;}
			if(!(GPIOC->IDR&lin_2)&&(botoes[0][1]==0)){botoes[0][1] = player;  aux_temp=0;  player^=0x03;}
			if(!(GPIOC->IDR&lin_3))
						{
				if(habilita_IA){botoes[0][2]=0;habilita_IA=0;}
					t=1;
					aux_temp=0;
						}



			if((GPIOC->IDR&lin_3)&&(t==1))
			{
				t=0;
				if(botoes[0][2]==0&&a==1){a=0;botoes[0][2] = player; aux_temp=0; player^=0x03;}
				a++;

			}





			control = 0x02;
			break;

		case 2:
			GPIOC->ODR |=   col_1; // DESABILITA COLUNA 1
			GPIOC->ODR &=~  col_2; // HABILITA COLUNA 2
			GPIOC->ODR |=   col_3; // DESABILITA COLUNA 3

			if(!(GPIOC->IDR&lin_1)&&(botoes[1][0]==0)){botoes[1][0] = player;  aux_temp=0;  player^=0x03;}
			if(!(GPIOC->IDR&lin_2)&&(botoes[1][1]==0)){botoes[1][1] = player;  aux_temp=0;  player^=0x03;}
			if(!(GPIOC->IDR&lin_3)&&(botoes[1][2]==0)){botoes[1][2] = player;  aux_temp=0;  player^=0x03;}

			control = 0x03;
			break;

		case 3:
			GPIOC->ODR |=  col_1; // DESABILITA COLUNA 1
			GPIOC->ODR |=  col_2; // DESABILITA COLUNA 2
			GPIOC->ODR &=~ col_3; // HABILITA COLUNA 3

			if(!(GPIOC->IDR&lin_1)&&(botoes[2][0]==0)){botoes[2][0] = player;  aux_temp=0;  player^=0x03;}
			if(!(GPIOC->IDR&lin_2)&&(botoes[2][1]==0)){botoes[2][1] = player;  aux_temp=0;  player^=0x03;}
			if(!(GPIOC->IDR&lin_3)&&(botoes[2][2]==0)){botoes[2][2] = player;  aux_temp=0;  player^=0x03;}

			GPIOC->ODR |=  col_1; // DESABILITA COLUNA 1
			GPIOC->ODR |=  col_2; // DESABILITA COLUNA 2
			GPIOC->ODR |=  col_3; // DESABILITA COLUNA 3
			control = 0x01;
			break;
		}

}

void logica_IA(void)
{

	/*
	 * 00 | 10 | 20    0 | 1 | 2
	 * -------------  -----------
	 * 01 | 11 | 21    3 | 4 | 5
	 * -------------  -----------
	 * 02 | 12 | 22    6 | 7 | 8
	 *
	 */


	switch(jogada_IA)
	{
		case 1:

			if((botoes[0][2]==0)&&(player==0x01)){botoes[0][2]=1.;jogada_IA++;player^=0x03;}
			break;

		case 2:

     		     if((botoes[0][0]==2)&&(player==0x01)){botoes[2][2]=1; jogada_IA++;player^=0x03;numero_da_jogada=1;}
     		else if((botoes[1][0]==2)&&(player==0x01)){botoes[0][0]=1.;jogada_IA++;player^=0x03;numero_da_jogada=2;}
     		else if((botoes[2][0]==2)&&(player==0x01)){botoes[2][2]=1.;jogada_IA++;player^=0x03;numero_da_jogada=3;}

     		else if((botoes[0][1]==2)&&(player==0x01)){botoes[2][2]=1.;jogada_IA++;player^=0x03;numero_da_jogada=4;}
     		else if((botoes[1][1]==2)&&(player==0x01)){botoes[2][2]=1; jogada_IA++;player^=0x03;numero_da_jogada=5;}
     		else if((botoes[2][1]==2)&&(player==0x01)){botoes[2][2]=1; jogada_IA++;player^=0x03;numero_da_jogada=6;}

     		else if((botoes[1][2]==2)&&(player==0x01)){botoes[0][0]=1; jogada_IA++;player^=0x03;numero_da_jogada=7;}
     		else if((botoes[2][2]==2)&&(player==0x01)){botoes[0][0]=1; jogada_IA++;player^=0x03;numero_da_jogada=8;}
			break;

		case 3:

			switch(numero_da_jogada)
			{
				case 1:
					if((player==1)&&(botoes[1][0]==2||botoes[0][1]==2||botoes[1][1]==2||botoes[2][0]==2||botoes[2][1]==2)){botoes[1][2]=1; player^=0x03;}
					else if((player==1)&&(botoes[1][2]==2)){botoes[2][0]=1; player^=0x03;numero_da_jogada=9;}
					break;

				case 2:

					if((player==1)&&(botoes[1][1]==2||botoes[2][0]==2||botoes[2][1]==2||botoes[1][2]==2||botoes[2][2])){botoes[0][1]=1; player^=0x03;}
					else if((player==1)&&(botoes[0][1]==2)){botoes[2][2]=1; player^=0x03;numero_da_jogada=10;}
					break;

				case 3:

					if((player==1)&&(botoes[0][0]==2||botoes[1][0]==2||botoes[0][1]||botoes[1][1]==2||botoes[2][1]==2)){botoes[1][2]=1;player^=0x03;}
					else if((player==1)&&(botoes[1][2]==2)){botoes[0][0]=1;player^=0x03;numero_da_jogada=11;}
					break;
				case 4:

					if((player==1)&&(botoes[0][0]==2||botoes[1][0]==2||botoes[2][0]==2||botoes[1][1]==2||botoes[2][1]==2)){botoes[1][2]=1;player^=0x03;}
					else if((player==1)&&(botoes[1][2]==2)){botoes[1][1]=1;player^=0x03;numero_da_jogada=12;}
					break;

				case 5:
					if((player==1)&&(botoes[0][0]==2||botoes[1][0]==2||botoes[2][0]==2||botoes[0][1]==2||botoes[2][1]==2)){botoes[1][2]=1;}
					else if((player==1)&&(botoes[1][2]==2)){botoes[1][0]=1;numero_da_jogada=13;player^=0x03;}
					break;

				case 6:
					if((player==1)&&(botoes[1][2]==0))botoes[1][2]=1;
					else if((player==1)&&(botoes[1][2]==2)){botoes[1][1]=1;player^=0x03;numero_da_jogada=16;}
					break;

				case 7:
					if((player==1)&&(botoes[0][1]==0))botoes[0][1]=1;
					else if((player==1)&&(botoes[0][1]==2)){botoes[1][1]=1;player^=0x03;numero_da_jogada=17;}
					break;

				case 8:
					if((player==1)&&(botoes[0][1]==0))botoes[0][1]=1;
					else if((player==1)&&(botoes[0][1]==2)){botoes[2][0]=1;player^=0x03;numero_da_jogada=18;}
					break;

				case 9:
					if((player==1)&&(botoes[1][0]==2||botoes[0][1]==2||botoes[1][1]==2)){botoes[2][1]=1;}
					else if((player==1)&&(botoes[2][1]==2))botoes[1][1]=1;
					break;

				case 10:

					if((player==1)&&(botoes[2][0]==2||botoes[1][1]==2||botoes[2][1]==2)){botoes[1][2]=1;}
					else if((player==1)&&(botoes[1][2]==2))botoes[1][1]=1;
					break;

				case 11:
					if((player==1)&&(botoes[1][0]==2||botoes[0][1]==2||botoes[2][1]==2)){botoes[1][1]=1;}
					else if((player==1)&&(botoes[1][1]==2))botoes[0][1]=1;

					break;
				case 12:
					if((player==1)&&(botoes[0][0]==2||botoes[1][0]==2||botoes[2][1]==2)){botoes[2][0]=1;}
					else if((player==1)&&(botoes[2][0]==2))botoes[0][0]=1;
					break;

				case 13:
					if((player==1)&&(botoes[0][1]==2)){botoes[2][1]=1;numero_da_jogada=14;player^=0x03;}
					else if((player==1)&&(botoes[2][1]==2)){botoes[0][1]=1;numero_da_jogada=14;player^=0x03;}
					else if((player==1)&&(botoes[0][0]==2)){botoes[2][0]=1;numero_da_jogada=15;player^=0x03;}
					else if((player==1)&&(botoes[2][0]==2)){botoes[0][0]=1;numero_da_jogada=15;player^=0x03;}
					break;

				case 14:
					if((player==1)&&(botoes[0][0]==2)){botoes[2][0]=1;player^=0x03;}
					else if((player==1)&&(botoes[2][0]==2)){botoes[0][0]=1;player^=0x03;}
					break;

				case 15:
					if((player==1)&&(botoes[2][1]==2)){botoes[0][1]=1;player^=0x03;}
					else if((player==1)&&(botoes[0][1]==2)){botoes[2][1]=1;player^=0x03;}
					break;

				case 16:
					if((player==1)&&(botoes[2][0]==0))botoes[2][0]=1;
					else if((player==1)&&(botoes[0][0]==0))botoes[0][0]=1;
					break;

				case 17:
					if((player==1)&&(botoes[2][0]==0))botoes[2][0]=1;
					else if((player==1)&&(botoes[2][0]=2)){botoes[2][2]=1;botoes[2][0]=1;}
					break;

				case 18:
					if((player==1)&&(botoes[1][0]==0))botoes[1][0]=1;
					else if((player==1)&&(botoes[1][0]==2)){botoes[1][1]=1;}
					break;

			}
			break;
	}






	/*if(botoes[0][2] == 0&&jogada==3&&player==1)
	{
	botoes[0][2]=1;

	} else if (botoes[0][2] == 2&&jogada==3&&player==1)
	{
		if(bt[0][0]==0)bt[0][0]=1;
		else if(bt[2][2]==0)bt[2][2]=1;

	}*/










}


//! Manipulador interupcao TIM10
void TIM1_UP_TIM10_IRQHandler (void)
{
	// Limpa o flag de interrupcao
		TIM10->SR &= ~TIM_SR_UIF; // ZERA A FLAG


		mux_botoes();//função de multiplexação dos botoes

	GPIOC->ODR &=~ ( GPIO_ODR_ODR_6| GPIO_ODR_ODR_7|GPIO_ODR_ODR_8|GPIO_ODR_ODR_9|GPIO_ODR_ODR_10|GPIO_ODR_ODR_11);// APAGA AS LINHAS DOS LEDS
	GPIOB->ODR &=~ ( GPIO_ODR_ODR_0| GPIO_ODR_ODR_1|GPIO_ODR_ODR_2);// APAGA AS COLUNAS DOS LEDS


	if(botoes_extra[j][k]==1||botoes[j][k]== 1)	mux_vermelho(&j, &k); //SE O ELEMENTO DA MATRIZ ==1 LIGA VERMELHO
	else if(botoes[j][k]==2||botoes_extra[j][k]==2) mux_verde(&j, &k);//SE O ELEMENTO DA MATRIZ ==2 LIGA VERDE



   //LAÇO DE REPETIÇÃO
	k++;

	if(k>2)
	{
	k=0;
	j++;
	}

	if(j>2)j=0;
}

int main(void)
{

RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOCEN;

// Configura GPIOA como saida
GPIOA->MODER&=~(GPIO_MODER_MODER5);
GPIOA->MODER|=(GPIO_MODER_MODER5_0);


GPIOC->MODER&=~(GPIO_MODER_MODER3);
GPIOC->MODER|=(GPIO_MODER_MODER3_0);

GPIOC->MODER&=~(GPIO_MODER_MODER4);
GPIOC->MODER|=(GPIO_MODER_MODER4_0);

GPIOC->MODER&=~(GPIO_MODER_MODER5);
GPIOC->MODER|=(GPIO_MODER_MODER5_0);


GPIOC->MODER&=~(GPIO_MODER_MODER0);
GPIOC->MODER&=~(GPIO_MODER_MODER1);
GPIOC->MODER&=~(GPIO_MODER_MODER2);


/*pull up*/
GPIOC->PUPDR &=~ GPIO_PUPDR_PUPDR0;
GPIOC->PUPDR |= GPIO_PUPDR_PUPDR0_0;

GPIOC->PUPDR &=~ GPIO_PUPDR_PUPDR1;
GPIOC->PUPDR |= GPIO_PUPDR_PUPDR1_0;

GPIOC->PUPDR &=~ GPIO_PUPDR_PUPDR2;
GPIOC->PUPDR |= GPIO_PUPDR_PUPDR2_0;
/*---------------------------------*/


GPIOC->MODER&=~(GPIO_MODER_MODER6);//PC6
GPIOC->MODER|=(GPIO_MODER_MODER6_0);

GPIOC->MODER&=~(GPIO_MODER_MODER7);//PC7
GPIOC->MODER|=(GPIO_MODER_MODER7_0);

GPIOC->MODER&=~(GPIO_MODER_MODER8);//PC8
GPIOC->MODER|=(GPIO_MODER_MODER8_0);

GPIOC->MODER&=~(GPIO_MODER_MODER9);//PC9
GPIOC->MODER|=(GPIO_MODER_MODER9_0);

GPIOC->MODER&=~(GPIO_MODER_MODER10);//PC10
GPIOC->MODER|=(GPIO_MODER_MODER10_0);

GPIOC->MODER&=~(GPIO_MODER_MODER11);//PC11
GPIOC->MODER|=(GPIO_MODER_MODER11_0);

GPIOB->MODER&=~(GPIO_MODER_MODER0);//PB0
GPIOB->MODER|=(GPIO_MODER_MODER0_0);

GPIOB->MODER&=~(GPIO_MODER_MODER1);//PB1
GPIOB->MODER|=(GPIO_MODER_MODER1_0);

GPIOB->MODER&=~(GPIO_MODER_MODER2); //PB2
GPIOB->MODER|=(GPIO_MODER_MODER2_0);


RCC->APB2ENR|=RCC_APB2ENR_TIM10EN;  // Habilita clock para timer 10
RCC->APB2ENR|=RCC_APB2ENR_TIM11EN;  // Habilita clock para timer 11
    /*
     * Configurando Timer 10
     */

	TIM10->PSC=PSC_1us;
	TIM10->ARR=ARR_2ms;
	TIM10->CR1=TIM_CR1_ARPE|TIM_CR1_CEN;
	TIM10->DIER|=TIM_DIER_UIE;      // habilita interrupcao por update

	// Habilita interrupcao no controlador
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);//
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 3);//Priority 3

	/* Configura o Timer 11*/
	TIM11->PSC = PSC_1us;
	TIM11->ARR = (199);
	TIM11->CR1 = TIM_CR1_CEN;


//começa com todas as colunas em 1
GPIOC->ODR |= col_1;
GPIOC->ODR |= col_2;
GPIOC->ODR |= col_3;



  while (1) // Laco principal
  {

	  int linha,coluna, jogo[3][3],soma=0,cont,resultado=0;

	  	for(linha=0;linha<3;linha++)
	  	{
	  		for(coluna=0;coluna<3;coluna++){
	  			jogo[linha][coluna]= botoes[linha][coluna];
	  		}
	  	}
	  	//Arrumando os valores
	  	for(linha=0;linha<3;linha++)
	  	{
	  		for(coluna=0;coluna<3;coluna++){
	  			if(jogo[linha][coluna]==2)
	  				jogo[linha][coluna]=4;
	  		}
	  	}


	  	// verifica as linhas
	  		for(cont=0;cont<3;cont++){
	  			soma=0;
	  			for(linha=0;linha<3;linha++){
	  				soma+=jogo[linha][cont];
	  			}
	  			if(soma==3)
	  				resultado=1;
	  					else if(soma==12)
	  							resultado=2;

	  			}

	  		//verifica as colunas
	  		for(cont=0;cont<3;cont++){
	  			soma=0;
	  			for(linha=0;linha<3;linha++){
	  				soma+=jogo[cont][linha];
	  				}
	  			if(soma==3)
	  				resultado=1;
	  				else if(soma==12)
	  					resultado=2;
	  		}

	  		//Diagonal principal
	  		soma=0;
	  		soma=jogo[0][0]+jogo[1][1]+jogo[2][2];

	  		if(soma==3)
	  			resultado=1;
	  			else if(soma==12)
	  				resultado=2;

	  		//Diagonal secundaria
	  		soma=0;
	  		soma=jogo[2][0]+jogo[1][1]+jogo[0][2];
	  		if(soma==3)
	  			resultado=1;
	  			else if(soma==12)
	  				resultado=2;


	  		if(resultado==1)
	  					for ( j2=0;j2<3;j2++)
	  						for (k1=0;k1<3;k1++)
	  							botoes[j2][k1]=1; // se vermelho ganha, todos leds vermelhos
	  			else if (resultado==2)
	  						for (j2=0;j2<3;j2++)
	  							for (k1=0;k1<3;k1++)
	  								botoes[j2][k1]=2;// se verde ganha, todos leds verdes



		if(habilita_IA==1){
				logica_IA();// logica da IA
				}


	  if(TIM11->SR&TIM_SR_UIF)
	  {
		  TIM11->SR &=~TIM_SR_UIF; // ZERA A FLAG DE ESTOURO DO TIMER

		  conta_tempo_piscada++;

			for(j2=0;j2<3;j2++)  // apaga a matriz de piscad
				for(k1=0;k1<3;k1++)
					 botoes_extra[j2][k1]=0;


			if(conta_tempo_piscada>5)// passou 1s? sim...
			{
					conta_tempo_piscada=0;
					aux_temp++;
					if(player==1) // se player for vermelho
					{
						for(j2=0;j2<3;j2++)
							for(k1=0;k1<3;k1++)
								if(botoes[j2][k1]==0)botoes_extra[j2][k1] ^=0x01;// pisca vermelho se a posição n estiver ocupada
					}
					else if(player==2)// se player for verde
					{
						for(j2=0;j2<3;j2++)
								for(int k1=0;k1<3;k1++)
										if(botoes[j2][k1]==0) botoes_extra[j2][k1] ^=0x02;// pisca verde se a posição n estiver ocupada
					}
						if(aux_temp==10)// se passou 10s?sim..
						{
							aux_temp=0;
							if(habilita_IA==0){player^=0x03; GPIOA->ODR |= GPIO_ODR_ODR_5; }
						}else GPIOA->ODR &=~ GPIO_ODR_ODR_5;
	  }//fim do if
    }//fim do if do timer
  }//fim do while
 }// fim main

