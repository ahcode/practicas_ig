#include <stdlib.h> 
#include <GL/glut.h> 

#define WindowsHeight 1000
#define WindowsWidth 1000

#define WorkspaceHeight 500
#define WorkspaceWidth 500

#define RectangleHeight 100
#define RectangleWidth 100

#define Velocity 0.136f

float x, y;
int lasty = 0;
const int ncolors = 5;
const int colors[ncolors][3] = {{1,1,1},{0,1,0},{1,1,0},{1,0,1},{0,1,1}};
int color = 0;
enum Directions { Left, Right, Up, Down };
bool reverse = false;
bool first = true;
Directions dir = Right;

void display(void){
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // DIBUJA SOMBRAS
    if (!reverse){
        if (!first){
            glColor3f(0, 0, 1);
            glRectf (0, 0, WorkspaceWidth, WorkspaceHeight);
        }
        glColor3f(1, 0, 0);
        if (dir == Right){
            glRectf (0, 0, WorkspaceWidth, y);
            glRectf (0, y, x, y+RectangleHeight);
        }else if (dir == Left){
            glRectf (0, 0, WorkspaceWidth, y);
            glRectf (x+RectangleWidth, y, WorkspaceWidth, y+RectangleHeight);
        }else if (dir == Up){
            glRectf (0, 0, WorkspaceWidth, lasty + RectangleHeight);
        }
    }else{
        glColor3f(1, 0, 0);
        glRectf (0, 0, WorkspaceWidth, WorkspaceHeight);
        glColor3f(0, 0, 1);
        if (dir == Right){
            glRectf (0, WorkspaceHeight, WorkspaceWidth, y+RectangleHeight);
            glRectf (0, y, x, y+RectangleHeight);
        }else if (dir == Left){
            glRectf (0, WorkspaceHeight, WorkspaceWidth, y+RectangleHeight);
            glRectf (x+RectangleWidth, y, WorkspaceWidth, y+RectangleHeight);
        }else if (dir == Down){
            glRectf (0, WorkspaceHeight, WorkspaceWidth, lasty);
        }
    }

    // DIBUJA RECTANGULO
    glColor3f(colors[color][0], colors[color][1], colors[color][2]);
    glRectf (x, y ,x+RectangleWidth, y+RectangleHeight);

    glutSwapBuffers();
}


void MueveCuadrado(){
    switch(dir){
        case Right:
            x=x+Velocity;
            if (x + RectangleWidth >= WorkspaceWidth){
                x = WorkspaceWidth - RectangleWidth;
                if (!reverse){
                    if (y + RectangleHeight >= WorkspaceHeight){
                        reverse = true;
                        first = false;
                        dir = Left;
                    }else{
                        lasty = y;
                        dir = Up;
                    }
                }else{
                    if (y <= 0){
                        reverse = false;
                        dir = Left;
                        color = (color+1)%ncolors;
                    }else{
                        lasty = y;
                        dir = Down;
                    }
                }
            }
            break;

        case Left:
            x = x - Velocity;
            if (x <= 0){
                x = 0;
                if (!reverse){
                    if (y + RectangleHeight >= WorkspaceHeight){
                        reverse = true;
                        first = false;
                        dir = Right;
                    }else{
                        lasty = y;
                        dir = Up;
                    }
                }else{
                    if (y <= 0){
                        reverse = false;
                        dir = Right;
                        color = (color+1)%ncolors;
                    }else{
                        lasty = y;
                        dir = Down;
                    }
                }
            }
            break;

        case Up:
            y=y+Velocity;
            if (y >= lasty + RectangleHeight || y + RectangleHeight >= WorkspaceHeight){
                if (y + RectangleHeight >= WorkspaceHeight)
                    y = WorkspaceHeight - RectangleHeight;
                else if (y >= lasty + RectangleHeight)
                    y = lasty + RectangleHeight;
                if (x > 0)
                    dir = Left;
                else
                    dir = Right;
            }
            break;

        case Down:
            y=y-Velocity;
            if (y <= lasty - RectangleHeight || y <= 0){
                if (y <= 0)
                    y = 0;
                else if (y <= lasty - RectangleHeight)
                    y = lasty - RectangleHeight;
                if (x > 0)
                    dir = Left;
                else
                    dir = Right;
            }
            break;
    }

    glutPostRedisplay();
}


static void reshape(int w, int h){
	glViewport(0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0, WorkspaceWidth, 0, WorkspaceHeight, -1.0, 1.0); 
}


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(WindowsWidth, WindowsHeight);
    glutCreateWindow("OpenGL");
    glutReshapeFunc(reshape); 
    glutDisplayFunc(display);
	glutIdleFunc(MueveCuadrado);
    glutMainLoop();
}

