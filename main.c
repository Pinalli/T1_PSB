#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>		// Para usar strings
#include <time.h> // Para utilizar a hora do sistema

#ifdef WIN32
#include <windows.h>    // Apenas para Windows
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>     // Funções da OpenGL
#include <GL/glu.h>    // Funções da GLU
#include <GL/glut.h>   // Funções da FreeGLUT
#endif

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel Pixel (24 bits)
typedef struct {
    unsigned char r, g, b;
} Pixel;

// Uma imagem Pixel
typedef struct {
    int width, height;
    Pixel* img;
} Img;

// Uma semente de Pixel
typedef struct {
    int x, y;
    Pixel color;
} Seed;

// Protótipos
void load(char* name, Img* pic);
void valida();

// Funções da interface gráfica e OpenGL
void init();
void draw();
void keyboard(unsigned char key, int x, int y);

// Largura e altura da janela
int width, height;

// Identificadores de textura
GLuint tex[2];

// As 2 imagens
Img pic[2];

// Imagem selecionada (0,1)
int sel;

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (Pixel*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char** argv)
{
    if(argc < 1) {
        printf("artistic [im. entrada]\n");
        exit(1);
    }
	glutInit(&argc,argv);

	// Define do modo de operacao da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// pic[0] -> imagem de entrada
	// pic[1] -> imagem de saida

	// Carrega a imagem
    load(argv[1], &pic[0]);

    width = pic[0].width;
    height = pic[0].height;

    // A largura e altura da imagem de saída são iguais às da imagem de entrada (0)
    pic[1].width  = pic[0].width;
    pic[1].height = pic[0].height;
	pic[1].img = calloc(pic[1].width * pic[1].height, 3); // W x H x 3 bytes (Pixel)

	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(width, height);

	// Cria a janela passando como argumento o titulo da mesma
	glutCreateWindow("Arte Computacional");

	// Registra a funcao callback de redesenho da janela de visualizacao
	glutDisplayFunc(draw);

	// Registra a funcao callback para tratamento das teclas ASCII
	glutKeyboardFunc (keyboard);

    // Exibe as dimensões na tela, para conferência
    printf("Origem  : %s %d x %d\n", argv[1], pic[0].width, pic[0].height);
    sel = 0; // entrada

	// Define a janela de visualizacao 2D
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,width,height,0.0);
	glMatrixMode(GL_MODELVIEW);

    // Converte para interpretar como matriz
    Pixel (*in)[width] = (Pixel(*)[width]) pic[0].img;
    Pixel (*out)[width] = (Pixel(*)[width]) pic[1].img;

	// Aplica o algoritmo e gera a saida em out (pic[1].img)
	// ...
	// ...
    // Exemplo: copia apenas o componente vermelho para a saida
    //for(int y=0; y<height; y++)
    //    for(int x=0; x<width; x++)
    //        out[y][x].r = in[y][x].r;

    // ====================================================================
    // Aqui começa as alterações do nosso trabalho na função main

    // Setar r null
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++)
            out[j][i].r = NULL;

    // Quantidade total de sementes mestre = 2% da (largura * altura)
    int total_semente = (width*height)*0.02;
    // Vetor de sementes mestre
    Seed semente[total_semente];

    // Inicializar carga aleatoria
    srand(time(NULL));
    // For de seleção de sementes mestre
    for(int i=0; i<total_semente; i++){
        semente[i].x = rand() % width;
        semente[i].y = rand() % height;
        semente[i].color = in[semente[i].y][semente[i].x];
        // Essa linha coloquei para demonstrar na imagem as sementes mestre
        out[semente[i].y][semente[i].x] = in[semente[i].y][semente[i].x];
    }

    // Aqui vai começar o calculo de qual semente o Pixel esta mais proxima
    // Sugestão: 1) ordenar a Semente por x,y, pois quando percorrer a matriz para procurar a semente,
    // podemos controlar quando o x e y forem muito distantes e parar, lembrando que eles são int e

    // Arquivo desordenado
    FILE *fp;
    fp = fopen("./desord.txt","w+");
    // Imprime o tamanho da imagem no console
    fprintf(fp,"%d %d\n", width, height);
    // Gerar no console as sementes matrizes
    for(int i=0; i<total_semente; i++){
        fprintf(fp,"%.4f %.4f %d %d %d\n", ((float)semente[i].x/width), ((float)semente[i].y/height), semente[i].color.r, semente[i].color.g, semente[i].color.b);
    }
    fclose(fp);

    // QuickXYSort para ordenar a semente
    quickXYSort(semente, 0, total_semente-1);

    // Arquivo ordenado
    fp = fopen("./ord.txt","w+");
    // Imprime o tamanho da imagem no console
    fprintf(fp,"%d %d\n", width, height);
    // Gerar no console as sementes matrizes
    for(int i=0; i<total_semente; i++){
        fprintf(fp,"%.4f %.4f %d %d %d\n", ((float)semente[i].x/width), ((float)semente[i].y/height), semente[i].color.r, semente[i].color.g, semente[i].color.b);
    }
    fclose(fp);

    // Pintar a cor da semente mais proxima
    int pintar;
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++) {
            if(out[j][i].r == NULL) {
                pintar = calcular(i, j, semente, total_semente);
                out[j][i] = semente[pintar].color;
            }
        }

    // Aqui termina nossa alteração da função main
    // =======================================================================

	// Cria texturas em memória a partir dos pixels das imagens
    tex[0] = SOIL_create_OGL_texture((unsigned char*) pic[0].img, width, height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
    tex[1] = SOIL_create_OGL_texture((unsigned char*) pic[1].img, width, height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	// Entra no loop de eventos, não retorna
    glutMainLoop();
}

// Gerencia eventos de teclado
void keyboard(unsigned char key, int x, int y)
{
    if(key==27) {
      // ESC: libera memória e finaliza
      free(pic[0].img);
      free(pic[1].img);
      exit(1);
    }
    if(key >= '1' && key <= '2')
        // 1-2: seleciona a imagem correspondente (origem ou destino)
        sel = key - '1';
    glutPostRedisplay();
}

// Callback de redesenho da tela
void draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Preto
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Para outras cores, veja exemplos em /etc/X11/Pixel.txt

    glColor3ub(255, 255, 255);  // branco

    // Ativa a textura corresponde à imagem desejada
    glBindTexture(GL_TEXTURE_2D, tex[sel]);
    // E desenha um retângulo que ocupa toda a tela
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex2f(0,0);

    glTexCoord2f(1,0);
    glVertex2f(pic[sel].width,0);

    glTexCoord2f(1,1);
    glVertex2f(pic[sel].width, pic[sel].height);

    glTexCoord2f(0,1);
    glVertex2f(0,pic[sel].height);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Exibe a imagem
    glutSwapBuffers();
}

// Metodo quicksort original
void quickXYSort(Seed *matriz, int inicio, int termino) {
    int pivo;
    if(termino > inicio) {
        pivo = particionaXY(matriz, inicio, termino);
        quickXYSort(matriz, inicio, pivo-1);
        quickXYSort(matriz, pivo+1, termino);
    }
}

// Particionar a matriz
int particionaXY(Seed *matriz, int ini, int fim) {
    int esq, dir;
    Seed ponto, aux;
    esq = ini;
    dir = fim;
    ponto = matriz[ini];
    while(esq < dir) {
        while((matriz[esq].x < ponto.x) || ((matriz[esq].x == ponto.x) && (matriz[esq].y <= ponto.y))) {
            esq++;
        }
        while((matriz[dir].x > ponto.x) || ((matriz[dir].x == ponto.x) && (matriz[dir].y > ponto.y))) {
            dir--;
        }
        if(esq < dir) {
            aux = matriz[esq];
            matriz[esq] = matriz[dir];
            matriz[dir] = aux;
        }
    }
    matriz[ini] = matriz[dir];
    matriz[dir] = ponto;
    return dir;
}

// Função para selecionar a semente mais proxima
int calcular(int a, int b, Seed *sem, int contador) {
    int calculo, novocalculo, posicao;
    calculo = width * height;
    for(int k=0; k<contador; k++) {
        novocalculo = distancia(a, b, sem[k].x, sem[k].y);
        if(novocalculo < calculo) {
            calculo = novocalculo;
            posicao = k;
        }
    }
    return posicao;
}

// Calculando a distancia entre 2 eixos
int distancia(int x, int y, int xs, int ys) {
    return sqrt(pow((x - xs), 2) + pow((y - ys), 2));
}
