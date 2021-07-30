# Picaria

Jogo Picaria desenvolvido em C++ utilizando o framework Qt.

## Programa

O programa possui uma janela com os seguintes componentes:
- Um menu com 3 itens:
  - Jogo: nesse item, você pode selecionar as opções para iniciar um novo jogo ou para sair do jogo.
  - Modo: aqui você seleciona o tipo do tabuleiro: 9 ou 13 buracos.
  - Ajuda: aqui você pode ver algumas informações sobre os desenvolvedores do jogo.
- Um tabuleiro central com 9 ou 13 buracos dependendo do modo de jogo.
- Uma barra de status indicando em qual fase do jogo está e de qual jogador é a vez.

## Jogabilidade

Após definir o tabuleiro inicial, os jogadores vão para a primeira fase do jogo, onde cada jogador deve colocar 3 peças no tabuleiro de forma alternada, começando pelo vermelho.
Depois dos dois jogadores terem colocado as 3 peças, inicia-se a segunda fase do jogo. Nela, os jogadores devem mover as peças para as posições possívels. Para mover, basta clicar
em cima de uma peça que aparecerão os buracos para onde aquela peça pode ir. Ao clicar em um deles, a peça é movimentada para o buraco escolhido. O jogador que conseguir formar uma
sequência de peças de sua cor primeiro, na vertical, na horizontal ou na diagonal, vence a partida.
