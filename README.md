# Trabalho 1 - 876
### Victor Toon de Araújo - 225231
### Álvaro Marques Macêdo - 212466

Neste readme vamos explicar algumas partes do ccódigo e por onde começar a ler. A ordem recomendada de leitura dos arquivos pra entender como funciona, seria: ler primeiro o operation.h, depois a main.y e a main.i, e por último a operation.c

A ideia aqui é que fizemos uma yacc bem simples, já que implementamos o shift reduce em C usando listas ligadas. Logo abaixo, colocaremos dois adendos aos quais fazemos referencia nos comentários do código

## Adendo 1:
A ideia de ter uma pilha com todas as operações é que podemos depois chamar o nosso algoritmo de shift reduce pra pilha toda. Porém, isso gera um leve problema:

Quando temos, por exemplo, X + (Y + Z) + W, o W tem que guardar a referencia do '(', que é o operador que vem depois do X, como se tivessemos X <- ( <- W, e pra isso, temos que remover o A e o B da pilha, por isso removemos nessa função. A cada inserção dentro do parenteses, removemos a anterior, então quando colocamos B, removemos A, por isso desempilhamos apenas 1 vez pra fazer W -> (.

## Adendo 2:
Essa parte da função é o núcleo do Shift-Reduce. Sabemos que o Shift-reduce tem ordens de prioridade, tomaremos o seguinte exemplo:

X op1 Y op2 Z

Se op1 for + e op 2 for +, podemos fazer X+Y sem problemas
Se op1 for + e op 2 não for +, temos que resolver Y op2 Z pra depois retornar ao op1

E assim por diante, seguindo a seguinte tabela:

![Tabela Prioridade]:(./Tabela-Prioridade.png)

A interpretação dessa tabela é que se oo resultado for 1, op1 pode ser resolvido, e se for 0, temos que resolver op2 primeiro, o que gera essa relação de prioridade entre as operações.

O que fazemos nessa parte da função é justamente analisar op1 e op2, ou, `cabeca->op` e `cabeca->dir->op`, fazer operações com base nisso.

Pra entendermos melhor, vamos imaginar a seguinte operação:
* `1+2*3`
* `cabeca->op` é inicialmente ADD, pq estamos olhando o 1 primeiro
* `cabeca->dir->op` é MULT

Como a prioridade do MULT é maior, chamamos a função pra `cabeca->dir`, pra resolvermos primeiro o MULT
No retorno dessa chamada, poderemos somar 1 com o reduzido de `2*3`.
