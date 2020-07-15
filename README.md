# Trabalho 1 - 876
### Victor Toon de Araújo - 225231
### Álvaro Marques Macêdo - 212466

Neste readme vamos explicar algumas partes do ccódigo e por onde começar a ler. A ordem recomendada de leitura dos arquivos pra entender como funciona, seria: ler primeiro o operation.h, depois a main.y e a main.l, e por último a operation.c

A ideia aqui é que fizemos uma yacc bem simples, já que implementamos o shift reduce em C usando listas ligadas. Logo abaixo, colocaremos dois adendos aos quais fazemos referencia nos comentários do código

## Estrutura de uma Expressão 

Utilizamos listas duplamete ligadas parar estruturar a expressão recebida, da forma que cada elemento da lista possui um número, um operador, que representa um número da operação e o seu operador subsequente, uma referência ao elemento da direita, da esquerda, e para "cima". Caso tenha um parêntese na expressão, o começo do parêntese vai contar como um elemento que possui o número -1, o operador como o operador subsequente ao fim do parêntese, e uma referência para o ponteiro de "cima", que vai ligar ao primeiro elemento da expressão dentro do parêntese. </br>
No final da expressão (ou final de uma expressão de parêntese) temos último número da expressão e o operador EOE (End of Expression), que indica que a expressão acabou. </br>
Como exemplo temos a expressão 5 + ( 3 + 2 ) ^ 7, da forma que estruturamos teríamos: </br>

![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/EstruturaOperacao.png)

Resolvemos a expressão (e também escrevemos o código assembly) de uma forma recursiva, começando pelo primeiro elemento da operação e indo do sentido esquerda-direita, quando encontramos um parêntese (identificado pelo número `-1`) chamos a mesma função para resolver e escrever o assembly da sua parte de cima, e depois prosseguimos para a direita do elemento do parêntese caso ainda exista expressão. As etapas da resolução da expressão acima seriam: </br>

![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/Res1Exp.png) </br>
![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/Res2Exp.png) </br>
![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/Res3Exp.png) </br>
![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/Res4Exp.png) </br>

## Construção de uma Expressão

Para transforar a string de entrada de uma expressão (como "5 + ( 3 + 2 ) ^ 7") na estrutura apresentada assim utilizamos uma pilha de referências, na qual temos o primeiro elemtno na base dessa pilha e os elementos subsequentes acima dele, não excluímos elementos da pilha a não ser que estejam dentro de parênteses, o porquê disso está explicado no adendo 1. Para casos fora de parêntese, cada elemento colocado na pilha é referenciado como a direita do elemento de baixo e o de baixo é referenciado como a esquerda desse elemento. </br>
As etapas da criação de uma expressão "5 + (3 + 2) ^ 7" seriam: </br>
* Pilha: 
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/pilha1Exp.png) </br>
* Expressão:
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/exp1Exp.png) </br>
* Pilha: 
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/pilha2Exp.png) </br>
* Expressão:
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/exp2Exp.png) </br>
* Pilha: 
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/pilha3Exp.png) </br>
* Expressão:
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/exp3Exp.png) </br>
* Pilha: 
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/pilha4Exp.png) </br>
* Expressão:
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/exp4Exp.png) </br>
* Pilha: 
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/pilha5Exp.png) </br>
* Expressão:
	* ![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/exp5Exp.png) </br>
	 

## Adendo 1:
A ideia de ter uma pilha com todas as operações é que podemos depois chamar o nosso algoritmo de shift reduce pra pilha toda. Porém, isso gera um leve problema:

Quando temos, por exemplo, X + (Y + Z) + W, o W tem que guardar a referencia do '(', que é o operador que vem depois do X, como se tivessemos X <- ( <- W, e pra isso, temos que remover o A e o B da pilha, por isso removemos nessa função. A cada inserção dentro do parenteses, removemos a anterior, então quando colocamos B, removemos A, por isso desempilhamos apenas 1 vez pra fazer W -> (.


## Adendo 2:
Essa parte da função é o núcleo do Shift-Reduce. Sabemos que o Shift-reduce tem ordens de prioridade, tomaremos o seguinte exemplo:

X op1 Y op2 Z

Se op1 for + e op 2 for +, podemos fazer X+Y sem problemas
Se op1 for + e op 2 não for +, temos que resolver Y op2 Z pra depois retornar ao op1

E assim por diante, seguindo a seguinte tabela:

![alt text](https://raw.githubusercontent.com/AlvaroMarques/Trabalho01-EA876/master/img/Tabela-prioridade.png)

A interpretação dessa tabela é que se oo resultado for 1, op1 pode ser resolvido, e se for 0, temos que resolver op2 primeiro, o que gera essa relação de prioridade entre as operações.

O que fazemos nessa parte da função é justamente analisar op1 e op2, ou, `cabeca->op` e `cabeca->dir->op`, fazer operações com base nisso.

Pra entendermos melhor, vamos imaginar a seguinte operação:
* `1+2*3`
* `cabeca->op` é inicialmente ADD, pq estamos olhando o 1 primeiro
* `cabeca->dir->op` é MULT

Como a prioridade do MULT é maior, chamamos a função pra `cabeca->dir`, pra resolvermos primeiro o MULT
No retorno dessa chamada, poderemos somar 1 com o reduzido de `2*3`.

## Agradecimentos 

* D$ LUCKHAOS
* Gay Es Paint (https://jspaint.app/)
* Lucas (Bertoloto) Bertoloto
