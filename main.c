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

// Funções do trabalho
Seed proximo(int a, int b, int mt[width][height], Pixel origem[height][width]);
float menor_dist(int x, int y, int xs, int ys, int *c, int *d, float calculo);
float distancia(int x, int y, int xs, int ys);

// Geração de arquivo
FILE *fp;

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

    // Matriz de posição das sementes
    int mt_sem[width][height];
    // Setar r null
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++)
            mt_sem[i][j] = 0;

    // Quantidade total de sementes mestre = 2% da (largura * altura)
    int total_semente = (width*height)*0.03;

    // Seleção de xis e yis
    int xis, yis;

    // Inicializar carga aleatoria
    srand(time(NULL));
    // For de seleção de sementes mestre
    for(int i=0; i<total_semente; i++){
        xis = rand() % width;
        yis = rand() % height;
        if(mt_sem[xis][yis] == 1) {
            i--;
        } else {
            mt_sem[xis][yis] = 1;
        }
    }

    // Abrir arquivo de saida aleatoria
    fp = fopen("./saida_aleatoria.txt","w+");
    // Imprime o tamanho da imagem no console
    fprintf(fp,"%d %d\n", width, height);
    // Seed para preenchimento
    Seed cores;
    // Carrega as cores na segunda imagem
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++) {
            cores = proximo(i, j, mt_sem, in);
            out[j][i] = in[cores.y][cores.x];
        }
    // Fecha o arquivo de saida
    fclose(fp);

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

// Função de calculo por apoximação
Seed proximo(int a, int b, int mt[width][height], Pixel origem[height][width]) {
    // Retorno
    Seed ret;
    // Se for semente
    if(mt[a][b] == 1) {
        ret.x = a;
        ret.y = b;
        // Imprime no arquivo de saida
        fprintf(fp,"%.4f %.4f %d %d %d\n", ((float)a/width), ((float)b/height), origem[b][a].r, origem[b][a].g, origem[b][a].b);
    } else {
        // Contador
        int cont = 1;
        // Valores de distancia
        float calculo;
        calculo = width * height;
        // X e Y de saida
        int c, d;
        // Manipulação
        int meio, k;
        // Executa enquanto o calculo for maior que o contador
        while(cont <= calculo) {
            // Meio
            meio = ((cont/2)+1);

            // Para y (B+CONT) soma x (A-MEIO) => (A+CONT)
            if ((a-meio) >= 0)
                k = a-meio;
            else
                k = 0;
            if ((b+cont) < height)
                for(int l=k; ((l<=(a+cont)) && (l<width)); l++)
                    if(mt[l][b+cont] == 1)
                        calculo = menor_dist(a, b, l, b+cont, &c, &d, calculo);

            // Para x (A-CONT) soma y (B-((CONT/2)+1)) => (B+CONT)
            if ((b-meio) >= 0)
                k = b-meio;
            else
                k = 0;
            if ((a-cont) >= 0)
                for(int m=k; (m<=(b+cont) && (m<height)); m++)
                    if(mt[a-cont][m] == 1)
                        calculo = menor_dist(a, b, a-cont, m, &c, &d, calculo);

            // Para y (B-CONT) subtrai x (A-CONT) => (A+((CONT/2)+1))
            if ((a-cont) >= 0)
                k = a-cont;
            else
                k = 0;
            if ((b-cont) >= 0)
                for(int l=k; ((l<=(a+meio)) && (l<width)); l++)
                    if(mt[l][b-cont] == 1)
                        calculo = menor_dist(a, b, l, b-cont, &c, &d, calculo);

            // Para x (A+CONT) subtrai y (B-CONT) => (B+((CONT/2)+1))
            if ((b-cont) >= 0)
                k = b-cont;
            else
                k = 0;
            if ((a+cont) < width)
                for(int m=k; ((m<=(b+meio)) && (m<height)); m++)
                    if(mt[a+cont][m] == 1)
                        calculo = menor_dist(a, b, a+cont, m, &c, &d, calculo);

            // Incrementa o contador
            cont++;
        }
        // Carrega x e y da menor distancia
        ret.x = c;
        ret.y = d;
    }
    // Retorna a semente mais proxima
    return ret;
}

// Retorna a menor distancia
float menor_dist(int x, int y, int xs, int ys, int *c, int *d, float calculo) {
    float novocalculo;
    // Calcula a distancia
    novocalculo = distancia(x, y, xs, ys);
    // Armazena a menor distancia
    if(novocalculo < calculo) {
        calculo = novocalculo;
        *c = xs;
        *d = ys;
    }
    // Retorna a menor distancia
    return calculo;
}
// Calculando a distancia entre 2 eixos
float distancia(int x, int y, int xs, int ys) {
    return sqrt(pow((x - xs), 2) + pow((y - ys), 2));
}
