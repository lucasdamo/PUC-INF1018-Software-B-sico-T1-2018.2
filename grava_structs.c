  /*

  Desenvolvido como trabalho de curso para a disciplina INF1308,
  Software Básico, na PUC-Rio. O objetivo desse programa é
  armazenar e ler structs no formato estabelicido por uma main.

  Copyright (C) 2019 Lucas Rebello Damo & Luiza Del Negro Ciuffo Fernandes

  Este programa é um software livre; você pode redistribuí-lo e/ou
  modificá-lo sob os termos da Licença Pública Geral GNU como publicada
  pela Free Software Foundation; na versão 3 da Licença.

  Este programa é distribuído na esperança de que possa ser útil,
  mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO
  a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a
  Licença Pública Geral GNU para mais detalhes.

  Você deve ter recebido uma cópia da Licença Pública Geral GNU junto
  com este programa. Se não, veja <http://www.gnu.org/licenses/>.

   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grava_structs.h"



int grava_structs(int nstructs, void *valores, char *campos, char ord, char *arquivo){
	/* nstructs: o número de elementos do array de structs a ser escrito em arquivo
		valores: endereco do array de structs propriamente dito
		campos: descricao dos campos das structs que compõem o array
		ord: um caractere que indica se os valores inteiros contidos nas estruturas devem ser armazenados no arquivo em ordenacaoo little endian ('L')ou big endian ('B')
		arquivo: o nome do arquivo a ser criado */
	FILE *fp;
	unsigned char primeiroByte;		/* Quantidade de structs */
	unsigned char segundoByte;		/* Little Endian? numero de campos */
	unsigned int nBytes;			/* Numero de bytes necessarios para armazenar */
	unsigned char c;				/* Para escrever byte a byte no arquivo */
	unsigned char *cpvalores;		/* Para utilizadar operacao aritmetica num ponteiro de tipo char e nao void */
	int i,j,k,x;					/* Contadores */
	int maior;						/* Armazena o maior tipo descrito em campos */


	fp = fopen(arquivo, "wb"); 		/* Abre o arquivo para escrever (w) */
	if(fp==NULL) return -1;

	primeiroByte = (unsigned char) nstructs;
	segundoByte = (unsigned char) strlen(campos);

	if(ord == 'L'){
		segundoByte = segundoByte | 0x80; /* Primeio bit = 1 */
	}
	else {
		segundoByte = segundoByte & 0x7F; /* Primeiro bit = 0 */
	}

	fwrite (&primeiroByte,sizeof(char),1,fp);
	fwrite (&segundoByte,sizeof(char),1,fp);

	nBytes = strlen(campos)/4;
	
	for(i=0; i<nBytes; i++){
		c=0;
		for(j=4;j>0;j--){ 
			if(		campos[4*i+(4-j)] == 'c') c = (c<<2);		/* 00 - char 			*/
			else if(campos[4*i+(4-j)] == 's') c = (c<<2)+1;		/* 01 - short int 		*/
			else if(campos[4*i+(4-j)] == 'i') c = (c<<2)+2;		/* 10 - int  			*/
			else if(campos[4*i+(4-j)] == 'l') c = (c<<2)+3;		/* 11 - long int 	    */
		}
		fwrite(&c,sizeof(char),1,fp);
	} /* fez todos os bytes que se completavam, falta o resto */
	c = 0;
	j=0;
	if(strlen(campos)%4 != 0){
		for(i=0; i<4; i++){
			c = c<<2;
			if(j < (strlen(campos)%4) ){
				if(campos[nBytes*4 + i] == 's') c = c + 1; /* 01 - short int 		*/
				if(campos[nBytes*4 + i] == 'i') c = c + 2; /* 10 - int  			*/
				if(campos[nBytes*4 + i] == 'l') c = c + 3; /* 11 - long int 	    */
				j = j + 1;
			}
		}
		fwrite(&c,sizeof(char),1,fp);
	}

	k=0;	/* Armazena a multiplicidade do endereco de valores */
	cpvalores = valores;
	for(i=0; i< nstructs; i++){ 	/* Para cada struct */
		maior = 1;	/* Reseta maior para 1 */
		for(j=0; j< strlen(campos); j++){ 	/* Para cada campo */
			if(campos[j] == 'c'){	/* Se for um char */
				fwrite(cpvalores,sizeof(char),1,fp); 
				cpvalores = cpvalores + 1;
				k = k + 1;
			}
			if(campos[j] == 's'){ /* Se for um short */
				if(k%2!=0){	/* Checa se o endereco e multiplo de 2 */
					cpvalores = cpvalores + (2-(k%2));	/* Caso nao seja multiplo, soma o minimo necessario para que seja (PADDING) */
					k = k + (2-(k%2)); /* Atualiza junto a variavel que armazena a multiplicidade */
				}
				if(ord == 'B'){ /* Se a ordenacao for Big Endian */
					unsigned char *temp;
					for(x=1;x>=0;x--){ /* Para cada byte */
						temp = cpvalores + x; /* Pega de tras para frente para converter */
						fwrite(temp,sizeof(char),1,fp);
					}
				}
				else
					fwrite(cpvalores,sizeof(short),1,fp);
				cpvalores = cpvalores + 2; /* Atualiza o endereco para pular o que ja foi escrito */
				k = k + 2;  /* Atualiza junto a variavel que armazena a multiplicidade */
				if(2>maior) maior = 2;
			}
			if(campos[j] == 'i'){
				if(k%4!=0){
					cpvalores = cpvalores + (4-(k%4));
					k = k + (4-(k%4));
				}
				if(ord == 'B'){
					unsigned char *temp;
					for(x=3;x>=0;x--){
						temp = cpvalores + x;
						fwrite(temp,sizeof(char),1,fp);
					}
				}
				else
					fwrite(cpvalores,sizeof(int),1,fp);
				cpvalores = cpvalores + 4;
				k = k + 4;
				if(4>maior) maior = 4;
			}
			if(campos[j] == 'l'){
				if(k%8!=0){
					cpvalores = cpvalores + (8-(k%8));
					k = k + (8-(k%8));
				}
				if(ord == 'B'){
					unsigned char *temp;
					for(x=7;x>=0;x--){
						temp = cpvalores + x;
						fwrite(temp,sizeof(char),1,fp);
					}
				}
				else
					fwrite(cpvalores,sizeof(long),1,fp);
				cpvalores = cpvalores + 8;
				k = k + 8;
				if(8>maior) maior = 8;
			}
		}
		if(k % maior != 0){ /* Se o valor nao esta em um multiplo do maior tipo da struct existe um padding */
			cpvalores = cpvalores + (maior - (k % maior));
			k = k + (maior - (k%maior));
		}
	}


	fclose(fp);
	return 0;
}

void dump_structs(char *arquivo){
	FILE* fp; 							/* File pointer. Ponteiro para o arquivo necessario em fread() */
	unsigned char c,temp; 				/* Alvo da leitura do arquivo, byte a byte e outro temporario para operacoes */
	char *desc_campos;					/* Vetor contendo a descricao dos campos em ordem. (0 = char, 1 = short, 2 = int, 3 = long) */
	int i,nstructs,ncampos,j,k; 		/* Contadores */



	fp = fopen(arquivo, "rb");					/* Abre o arquivo em modo de leitura binario */
	if(fp==NULL){
		printf("Erro na abertura do arquivo\n");
		return;
	}
	c = 0;										/* Inicializa o alvo de leitura dos bytes */

	fread(&c, sizeof(char),1,fp);				/* Le o primeiro byte */
	nstructs = c;
	

	fread(&c, sizeof(char),1,fp);				/* Le o segundo byte */
	if( (c & 0x80) > 0) printf("L\n");  		/* Se o primeiro byte nao for 0, entao e Little Endian */
	else printf("B\n");							/* Se nao e big endian */
	ncampos = c & 0x7F;							/* Remove o primeiro bit para ter o numero de campos */

	desc_campos = (char*)malloc(ncampos*sizeof(char));	/* Aloca espaco para armazenar os descritores de campos como um vetor */
	if(desc_campos == NULL) printf("erro na alocacao\n");
	k=0; 												/* Indice para armazenar os descritores no vetor */

	for(i = 0; i<(ncampos/4); i++){ /* Para cada byte completo que descreve campos */
		fread(&c, sizeof(char),1,fp);		/* OBS: Como c é um UNSIGNED char, o shift será o shift lógico (preenchendo com 0 a esquerda num shift para direita) */
		for(j=0;j<4;j++){ /* Para cada 2 bits */		
			if(j==0){
				temp = c>>6;				/* Os 2 primeiros bits >> */
			}
			else if (j==1){
				temp = c & 0x30;
				temp = temp >> 4;			/* 3o e 4o bits >> */
			}
			else if (j==2){
				temp = c & 0xC;	
				temp = temp >> 2;			/* 5o e 6o bits >> */
			}
			else if (j==3){
				temp = c & 0x3;				/* 7o e 8o bits >> */
			}
			if 		(temp == 0){ /* 00 char */
				desc_campos[k] = 0;
			}
			else if (temp == 1){ /* 01 short */
				desc_campos[k] = 1;
			} 
			else if (temp == 2){ /* 10 int */
				desc_campos[k] = 2;
			}
			else if (temp == 3){ /* 11 long */
				desc_campos[k] = 3;
			}
			k = k + 1;
		}
	}
	if(ncampos%4 !=0){					/* Como os descritores que completavam bytes foram trabalhados, falta o resto que nao se "encaixou" */
		fread(&c, sizeof(char),1,fp);
		for(i=0;i<(ncampos%4);i++){		/* Para cada 2 bits "deixados de fora" repete a logica do algoritmo anterior */
		//printf("for i = %d   ncampos 4 = %d\n",i,ncampos%4);
			if(i==0){
				temp = c>>6;
			}
			else if (i==1){
				temp = c & 0x30;
				temp = temp >> 4;
			}
			else if (i==2){
				temp = c & 0xC;
				temp = temp >> 2;
			}
			else if (i==3){
				temp = c & 0x3;
			}
			if 		(temp == 0){ /* 00 char */
				desc_campos[k] = 0;
			}
			else if (temp == 1){ /* 01 short */
				desc_campos[k] = 1;
			} 
			else if (temp == 2){ /* 10 int */
				desc_campos[k] = 2;
			}
			else if (temp == 3){ /* 11 long */
				desc_campos[k] = 3;
			}
			k=k+1;
		}
	}
	printf("%d\n",nstructs);
	printf("*\n");
	for(i = 0; i<nstructs; i++){					/* Para cada struct */
		for(j = 0; j<ncampos; j++){ 				/* Para cada campo na struct */
			if(desc_campos[j] == 0){				/* Se for 1 byte (char) */
				fread(&c, sizeof(char),1,fp);
				printf("%02X ",c);
			}
			else if(desc_campos[j] == 1){			/* Se for 2 byte (short) */
				for(k=0;k<2;k++){
					fread(&c, sizeof(char),1,fp);
					printf("%02X ",c);
				}
			}
			else if(desc_campos[j] == 2){			/* Se for 4 byte (int) */
				for(k=0;k<4;k++){
					fread(&c, sizeof(char),1,fp);
					printf("%02X ",c);
				}
			}
			else if(desc_campos[j] == 3){			/* Se for 8 byte (long) */
				for(k=0;k<8;k++){
					fread(&c, sizeof(char),1,fp);
					printf("%02X ",c);
				}
			}
			printf("\n");
		}
		printf("*\n");
	}
	free(desc_campos);
	fclose(fp);
}
