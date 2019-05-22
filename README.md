Breve descrição:
📚 Um programa que salva e lê arquivos em binário contendo uma ou mais structs.

Data de envio para avaliação: 12 Out 2018, 21:24
Autores: @lucasdamo e Luiza Fernandes

# Executando o programa
Para rodar o programa é necessário criar uma função main. O código fonte contém apenas as funções de gravar e ler structs. Para isso, veja os testes abaixo

# Observações
Após uma pesquisa, descobrimos que void é um tipo incompleto e, portanto, não poderiamos trabalhar com operações aritiméticas em alguns compiladores, então, optamos por transforma-lo, através de um typecast, em unsigned char dentro da função e dessa forma, garantir que poderia ser executada sempre, sem erros. 
Foi acordado escrever byte a byte para evitarmos passar grandes buffers para a fwrite, evitando lidar com memória alocada e outros tipos de erros, tornando o programa mais simples.

# Exemplos
Os testes foram realizados com um modelo de estruct semelhante ao que segue:
```c
typedef struct teste Teste;
struct teste {
	short primeiro;
	long segundo;
	char terceiro;
};
```
Em um exemplo de main, como visto a seguir, mostramos um exemplo de como declaramos os componentes das funções.
```c
int main(void){
	int nstructs = 6;
	char *campos = "slc";
	char ord = 'B';
	char *arquivo = "testandop01";
	Teste array[6] = {{0x26,0x27,0x28}, {0x108,0x11,0x21}, {0x30,0x31,0x32}, {0x26,0x27,0x28}, {0x26,0x27,0x28},{0x26,0x27,0x28}};
	grava_structs(nstructs, array, campos, ord, arquivo);
	dump_structs(arquivo);
	return 0;
}	
```
Para todos os testes realizados, os resultados foram compatíveis, como podemos ver abaixo, no resultado da chamada anterior.
```
B
6
*
00 26 
00 00 00 00 00 00 00 27 
28 
*
01 08 
00 00 00 00 00 00 00 11 
21 
*
00 30 
00 00 00 00 00 00 00 31 
32 
*
00 26 
00 00 00 00 00 00 00 27 
28 
*
00 26 
00 00 00 00 00 00 00 27 
28 
*
00 26 
00 00 00 00 00 00 00 27 
28 
*
```
Agora, segue outro exemplo com little endian:
```c
typedef struct teste Teste;
struct teste {
	short primeiro;
	long segundo;
	char terceiro;
	int quarto;
};
int main(void){
	int nstructs = 5;
	char *campos = "slci";
	char ord = 'L';
	char *arquivo = "testandop01";
	Teste array[5] = {{0x26,0x27,0x28,0x29}, {0x108,0x11,0x21,0x333}, {0x26,0x27,0x28,0x567}, {0x26,0x27,0x28,0x5},{0x26,0x27,0x28,0x77}};
	grava_structs(nstructs, array, campos, ord, arquivo);
	dump_structs(arquivo);
	return 0;
}	
```
```
L
5
*
26 00 
27 00 00 00 00 00 00 00 
28 
29 00 00 00 
*
08 01 
11 00 00 00 00 00 00 00 
21 
33 03 00 00 
*
26 00 
27 00 00 00 00 00 00 00 
28 
67 05 00 00 
*
26 00 
27 00 00 00 00 00 00 00 
28 
05 00 00 00 
*
26 00 
27 00 00 00 00 00 00 00 
28 
77 00 00 00 
*
```
