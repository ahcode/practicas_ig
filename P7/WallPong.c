#include <stdlib.h> 
#include <string>
#include <GL/glut.h> 

#define AltoVentana 1000
#define AnchoVentana 1000

#define AltoAreaTrabajo 1000
#define AnchoAreaTrabajo 1000

#define AltoRaqueta 300
#define AnchoRaqueta 30
#define VelocidadRaqueta 20

#define AnchoMuro 30

#define TamBola 30
#define VelocidadBola 1

float raqueta_y = (float)AltoAreaTrabajo/2 + (float)AltoRaqueta/2;
float bola_x = (float)AnchoAreaTrabajo - AnchoMuro - (float)TamBola/2;
float bola_y = (float)AltoAreaTrabajo/2 + (float)TamBola/2;
enum Directions {Izquierda, Derecha};
Directions dir = Izquierda;
float angulo = 0;

//std::string s = "jajaxd";

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

    //DIBUJA TEXTO
    /*glRasterPos2f(300, AltoAreaTrabajo - 10);
    for (int i=0; i<s.size(); i++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);*/

    glutSwapBuffers();
}


void MueveBola(){
    if (dir == Izquierda){
        if (bola_x - VelocidadBola > AnchoRaqueta){
            bola_x -= VelocidadBola;
        }else{
            bola_x = AnchoRaqueta;
        }
        if (bola_x == AnchoRaqueta){
            if(bola_y - TamBola > raqueta_y || bola_y < raqueta_y - AltoRaqueta){
                bola_x = (float)AnchoAreaTrabajo - AnchoMuro - (float)TamBola/2;
                bola_y = (float)AltoAreaTrabajo/2 + (float)TamBola/2;
                angulo = 0;
            }else{
                dir = Derecha;
                angulo = -((raqueta_y + TamBola - bola_y)/AltoRaqueta - 0.5);
            }
        }
    }else{
        if (bola_x + TamBola + VelocidadBola < AnchoAreaTrabajo - AnchoMuro){
            bola_x += VelocidadBola;
        }else{
            bola_x = AnchoAreaTrabajo - AnchoMuro - TamBola;
        }
        if (bola_x + TamBola == AnchoAreaTrabajo - AnchoMuro){
            dir = Izquierda;
        }
    }

    if (bola_y + VelocidadBola*angulo > AltoAreaTrabajo){
        bola_y = AltoAreaTrabajo;
    }else if (bola_y + VelocidadBola*angulo - TamBola < 0){
        bola_y = TamBola;
    }else{
        bola_y += VelocidadBola*angulo;
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

