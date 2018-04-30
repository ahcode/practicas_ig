#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

#define AltoVentana 1000
#define AnchoVentana 1000

#define AltoAreaTrabajo 1000
#define AnchoAreaTrabajo 1000

#define AltoRaqueta 300
#define AnchoRaqueta 30
#define VelocidadRaqueta 30

#define AnchoMuro 30

#define TamBola 30
#define VelocidadInicial 0.1f

#define RebotesPunto 3
#define PuntosAumentoVelocidad 5
#define PorcentajeAumentoVelocidad 10

#define xPuntuacion 100
#define yPuntuacion 30

#define xVidas 700
#define yVidas 30

float raqueta_y;
float bola_x;
float bola_y;
enum Directions {Izquierda, Derecha};
Directions dir;
float angulo;

float velocidadBola;
int puntuacion;
int vidas;
int rebotes_pala;
char cadenaPuntuacion[20];
char cadenaVidas[10];

void inicializarPosicion(){
    bola_x = (float)AnchoAreaTrabajo - AnchoMuro - (float)TamBola/2;
    bola_y = (float)AltoAreaTrabajo/2 + (float)TamBola/2;
    raqueta_y = (float)AltoAreaTrabajo/2 + (float)AltoRaqueta/2;
    dir = Izquierda;
    angulo = 0;
}

void inicializar(){
    velocidadBola = VelocidadInicial;
    puntuacion = 0;
    vidas = 5;
    rebotes_pala = 0;
    sprintf(cadenaPuntuacion, "Puntuacion: %d", puntuacion);
    sprintf(cadenaVidas, "Vidas: %d", vidas);
    inicializarPosicion();
}

void display(void){
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1, 1, 1);

    // DIBUJA MURO
    glRectf(AnchoAreaTrabajo-AnchoMuro, AltoAreaTrabajo, AnchoAreaTrabajo, 0);

    // DIBUJA RAQUETA
    glRectf(0, raqueta_y, AnchoRaqueta, raqueta_y-AltoRaqueta);
    
    //DIBUJA BOLA
    glRectf(bola_x, bola_y, bola_x+TamBola, bola_y-TamBola);

    //DIBUJA PUNTUACION
    glRasterPos2f(xPuntuacion, yPuntuacion);
    for (int i=0; i<strlen(cadenaPuntuacion); i++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, cadenaPuntuacion[i]);

    //DIBUJA VIDAS
    glRasterPos2f(xVidas, yVidas);
    for (int i=0; i<strlen(cadenaVidas); i++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, cadenaVidas[i]);

    glutSwapBuffers();
}

//Calcula la puntuación y velocidad cada vez que se golpea la pelota
void golpePala(){
    rebotes_pala++;
    if(rebotes_pala == RebotesPunto){
        rebotes_pala = 0;
        puntuacion++;
        sprintf(cadenaPuntuacion, "Puntuacion: %d", puntuacion);
        if(puntuacion % PuntosAumentoVelocidad == 0){
            velocidadBola += ((float)PorcentajeAumentoVelocidad / 100) * velocidadBola;
        }
    }
}

void pierdeBola(){
    if (vidas != 0){
        vidas--;
        inicializarPosicion();
    }else{
        inicializar();
    }
}

void MueveBola(){
    if (dir == Izquierda){
        if (bola_x - velocidadBola > AnchoRaqueta){
            bola_x -= velocidadBola;
        }else{
            bola_x = AnchoRaqueta;
        }
        if (bola_x == AnchoRaqueta){
            if(bola_y - TamBola > raqueta_y || bola_y < raqueta_y - AltoRaqueta){
                pierdeBola();
            }else{
                dir = Derecha;
                angulo = -((raqueta_y + TamBola - bola_y)/AltoRaqueta - 0.5);
                golpePala();
            }
        }
    }else{
        if (bola_x + TamBola + velocidadBola < AnchoAreaTrabajo - AnchoMuro){
            bola_x += velocidadBola;
        }else{
            bola_x = AnchoAreaTrabajo - AnchoMuro - TamBola;
        }
        if (bola_x + TamBola == AnchoAreaTrabajo - AnchoMuro){
            dir = Izquierda;
        }
    }

    if (bola_y + velocidadBola*angulo > AltoAreaTrabajo){
        bola_y = AltoAreaTrabajo;
    }else if (bola_y + velocidadBola*angulo - TamBola < 0){
        bola_y = TamBola;
    }else{
        bola_y += velocidadBola*angulo;
    }

    if (bola_y == AltoAreaTrabajo || bola_y - TamBola == 0){
        angulo = -angulo;
    }

    glutPostRedisplay();
}

void TeclasMovimiento(int key, int x, int y){
    switch (key){
        case GLUT_KEY_UP:
            if(raqueta_y + VelocidadRaqueta < AltoAreaTrabajo)
                raqueta_y += VelocidadRaqueta;
            else
                raqueta_y = AltoAreaTrabajo;
            break;
        case GLUT_KEY_DOWN:
            if(raqueta_y - AltoRaqueta - VelocidadRaqueta > 0)
                raqueta_y -= VelocidadRaqueta;
            else
                raqueta_y = AltoRaqueta;
            break;
    }
}


static void reshape(int w, int h){
	glViewport(0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0, AnchoAreaTrabajo, 0, AltoAreaTrabajo, -1.0, 1.0); 
}


int main(int argc, char **argv){
    inicializar();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(AnchoVentana, AltoVentana);
    glutCreateWindow("OpenGL");
    glutReshapeFunc(reshape); 
    glutDisplayFunc(display);
	glutIdleFunc(MueveBola);
    glutSpecialFunc(TeclasMovimiento);
    glutMainLoop();
}

