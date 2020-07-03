# Trabalho Prático  - Arte Computacional

## 1 Introdução

A transformação de uma imagem real em um esboço ou algo similar a uma pintura é alvo de pesquisas há bastante tempo. Esse processo pode ser feito de uma forma manual, automática ou semiautomática. O objetivo deste trabalho é a implementação de um algoritmo para a transformação de uma imagem qualquer em uma versão estilizada.

## 2 Funcionamento

Uma imagem é representada por uma matriz de pontos (pixels), onde cada cor é definida por 3 componentes: vermelho (R), verde (G) e azul (B). Cada uma dessas componentes é codificada em um byte, o que produz 3 bytes por pixel (24 bits) - ou seja, 16 milhões de possíveis cores. Em outras palavras, as intensidades (R, G, B) variam de 0 a 255, onde 0 é escuro e 255 é claro.

O algoritmo de estilização que utilizaremos é uma simplificação de um algoritmo geométrico muito conhecido, chamado diagrama de Voronoi. A ideia é a seguinte:
Escolhe-se uma certa quantidade de "sementes", ou seja, posições de influência dentro da imagem. Essa escolha pode ser puramente aleatória ou empregar alguma outra técnica mais sofisticada. Cada semente possui uma posição (xsi,ysi) , e uma cor representativa dessa região (ver abaixo).
Para cada pixel da imagem de entrada (x,y) , descobre-se a semente mais próxima, isto é, aquela cuja distância é a menor até ela. Para calcular a distância, pode-se usar a fórmula de distância Euclideana:
Na imagem de saída, armazena-se então a cor representativa da região no pixel atual.
O processo continua até o algoritmo passar por todos os pixels da imagem original (geralmente, da esquerda para a direita e de cima para baixo).
Esse algoritmo tem dois pontos de interesse:
A escolha das sementes
A escolha da cor representativa
Em um primeiro momento, pode-se simplesmente sortear coordenadas na imagem para definir as sementes, utilizando a cor desse pixel como cor da região.

## 3 Detalhes técnicos
Ao ser iniciado, o programa irá carregar um arquivo de imagem. Para tanto, utilizaremos uma biblioteca simples (integrada no projeto) denominada SOIL.
Após a leitura da imagem, o programa deverá realizar o algoritmo de estilização dos pixels da imagem de entrada, gerando o resultado na imagem de saída, criada com as mesmas dimensões.

## 4 Código base e imagens de teste
O arquivo artistic.zip contém o projeto completo do Code::Blocks para a implementação do trabalho. Esse código já implementa a exibição das 2 imagens na tela gráfica, usando a biblioteca OpenGL (use as teclas 1 e 2 para selecionar a imagem). O projeto pode ser compilado no Linux ou no Windows, bastando selecionar o target desejado no Code::Blocks. No caso de um Linux instalado em máquina pessoal, é necessário o pacote de desenvolvimento da biblioteca freeglut. Para Ubuntu, Mint, Debian e derivados, instale com:

sudo apt-get install freeglut3-dev

Se for utilizado o Linux ou macOS, o projeto também pode ser compilado manualmente pelo terminal usando o comando make (e o Makefile fornecido no projeto):

make -j

Alternativamente, você também pode utilizar o CMake (Cross Platform Make) para compilar pelo terminal. Para tanto, crie um diretório build embaixo do diretório do projeto:

mkdir build

cd build

cmake ..

make -j

## 5 Avaliação
Leia com atenção os critérios de avaliação:
Pontuação:
Algoritmo básico (sementes sorteadas, cor do próprio pixel): 7,5 pontos

Melhorias na escolha das sementes: 1,5 pontos

Melhorias na escolha das cores: 1 ponto

Os trabalhos são em duplas ou individuais.

A pasta do projeto deve ser compactada em um arquivo .zip e este deve ser submetido pelo Moodle até a data e hora especificadas.
Não envie .rar, .7z, .tar.gz - apenas .zip.

O código deve estar identado corretamente (qualquer editor moderno faz isso automaticamente).
A cópia parcial ou completa do trabalho terá como conseqüência a atribuição de nota ZERO ao trabalho dos alunos envolvidos. A verificação de cópias é feita inclusive entre turmas.

A cópia de código ou algoritmos existentes da Internet também não é permitida. Se alguma idéia encontrada na rede for utilizada na implementação, sua descrição e referência deve ser mencionada claramente no código.
