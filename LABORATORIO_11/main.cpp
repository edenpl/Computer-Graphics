#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include<stdio.h>
#include <time.h>
#define random(a,b) (rand()%(b-a)+a)  // genera números aleatorios para controlar la rotación del cubo de rubik
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>



int Cube[6][10];  // cube
int xro[28], yro[28], zro[28];   // capas de cubo
int start_flag = 0, first_in = 1, process_flag = 0, forbid = 0, solved = 0;  // control state flag of rubik's cube
int done = 0, di = 1, rot = 0, yes = 1, k = 0;
float angle = 0.0;  // controlar la rotación del cubo
int rotate_x = -25, rotate_y = 35; // ángulo de visión
int pause = -1, pressed = 0, solving = 0;  // bandera del estado del cubo de rubik
int dir_y, lay_y, dir_x, lay_x, dir_z, lay_z;  // parámetro para controlar la rotación




void rmy(int fr, int le, int ba, int ri, int dir)
{
    /**
     * @brief información del cubo de intercambio a ratote
     */

    int temp;
    if (dir == 1)
    {
        temp = Cube[4][ri];
        Cube[4][ri] = Cube[3][ba];
        Cube[3][ba] = Cube[5][le];
        Cube[5][le] = Cube[1][fr];
        Cube[1][fr] = temp;
    }
    else
    {
        temp = Cube[4][ri];
        Cube[4][ri] = Cube[1][fr];
        Cube[1][fr] = Cube[5][le];
        Cube[5][le] = Cube[3][ba];
        Cube[3][ba] = temp;

    }
}

void rmz(int up, int ri, int dw, int le, int dir)
{
    /**
     * @brief intercambia información de cubo a ratote
     */

    int temp;
    if (dir == 1)
    {
        temp = Cube[4][ri];
        Cube[4][ri] = Cube[2][up];
        Cube[2][up] = Cube[5][le];
        Cube[5][le] = Cube[0][dw];
        Cube[0][dw] = temp;
    }
    else
    {
        temp = Cube[4][ri];
        Cube[4][ri] = Cube[0][dw];
        Cube[0][dw] = Cube[5][le];
        Cube[5][le] = Cube[2][up];
        Cube[2][up] = temp;

    }
}

void fr(int face)
{
    /**
     * @brief intercambiar información del cubo para rotar
     * @param face     cara del cubo
     */

    int temp;

    // la posición par
    temp = Cube[face][2];
    Cube[face][2] = Cube[face][4];
    Cube[face][4] = Cube[face][8];
    Cube[face][8] = Cube[face][6];
    Cube[face][6] = temp;

    // la posición impar
    temp = Cube[face][1];
    Cube[face][1] = Cube[face][7];
    Cube[face][7] = Cube[face][9];
    Cube[face][9] = Cube[face][3];
    Cube[face][3] = temp;
}

void fran(int face)
{
    /**
     * @Brief intercambia la información del cubo a ratote
     * @param cara del cubo
     */

    int temp;

    // la posición par
    temp = Cube[face][2];
    Cube[face][2] = Cube[face][6];
    Cube[face][6] = Cube[face][8];
    Cube[face][8] = Cube[face][4];
    Cube[face][4] = temp;

    // la posición impar
    temp = Cube[face][1];
    Cube[face][1] = Cube[face][3];
    Cube[face][3] = Cube[face][9];
    Cube[face][9] = Cube[face][7];
    Cube[face][7] = temp;
}

void rotation_x_layer(int lay, int dir)
{
    /**
     * @brief control rotation of x axis
     * @param lay    layer number (1, 2, 3)
     * @param dir   direction of rotation (-1, 1)
     */

     // rotar la capa correspondiente de acuerdo a la disposición
    int i, j, temp, temp_1, temp_2, temp_3;
    if (dir == -1)
    {
        temp = lay;
        temp_1 = Cube[0][lay];
        temp_2 = Cube[0][lay + 3];
        temp_3 = Cube[0][lay + 6];
        for (j = 1; j < 4; j++)
            for (i = 0, lay = temp; i < 3; i++, lay += 3)
                Cube[j - 1][lay] = Cube[j][lay];
        lay = temp;
        Cube[3][lay] = temp_1;
        Cube[3][lay + 3] = temp_2;
        Cube[3][lay + 6] = temp_3;
        if (temp == 3)
            fran(4);
        if (temp == 1)
            fran(5);

    }
    else
    {
        temp = lay;
        temp_1 = Cube[3][lay];
        temp_2 = Cube[3][lay + 3];
        temp_3 = Cube[3][lay + 6];
        for (j = 3; j > 0; j--)
            for (i = 0, lay = temp; i < 3; i++, lay += 3)
                Cube[j][lay] = Cube[j - 1][lay];
        lay = temp;
        Cube[0][lay] = temp_1;
        Cube[0][lay + 3] = temp_2;
        Cube[0][lay + 6] = temp_3;
        if (temp == 3)
            fr(4);
        if (temp == 1)
            fr(5);
    }
}

void rotation_y_layer(int lay, int dir)
{
    /**
     * @Breve control de la rotación del eje y
     * @param lay número de capa (1, 2, 3)
     * @param dir dirección de rotación (-1, 1)
     */

     // rotar la capa correspondiente de acuerdo a la disposición
    if (lay == 1)
    {
        rmy(1, 3, 9, 1, dir);
        rmy(2, 2, 8, 2, dir);
        rmy(3, 1, 7, 3, dir);
        if (dir == 1)
            fr(2);
        if (dir == -1)
            fran(2);
    }
    if (lay == 2)
    {
        rmy(4, 6, 6, 4, dir);
        rmy(5, 5, 5, 5, dir);
        rmy(6, 4, 4, 6, dir);
    }
    if (lay == 3)
    {
        rmy(7, 9, 3, 7, dir);
        rmy(8, 8, 2, 8, dir);
        rmy(9, 7, 1, 9, dir);
        if (dir == 1)
            fran(0);
        else
            fr(0);
    }
}

void rotation_z_layer(int lay, int dir)
{
    /**
     * @Breve control de la rotación del eje z
     * @param lay número de capa (1, 2, 3)
     * @param dir dirección de rotación (-1, 1)
     */

     // rotar la capa correspondiente de acuerdo a la disposición
    if (lay == 1)
    {
        rmz(7, 1, 3, 7, dir);
        rmz(8, 4, 2, 4, dir);
        rmz(9, 7, 1, 1, dir);
        if (dir == 1)
            fr(1);
        else
            fran(1);
    }
    if (lay == 2)
    {
        rmz(4, 2, 6, 8, dir);
        rmz(5, 5, 5, 5, dir);
        rmz(6, 8, 4, 2, dir);
    }
    if (lay == 3)
    {
        rmz(1, 3, 9, 9, dir);
        rmz(2, 6, 8, 6, dir);
        rmz(3, 9, 7, 3, dir);
        if (dir == 1)
            fran(3);
        else
            fr(3);
    }
}

void change_point(int w, int h)
{
    /**
     * @brief cambiar el punto de vista
     * @param w parámetro de cambio de la anchura
     * @param h parámetro de cambio de altura
     */
    glViewport(0, 0, w, h);
}
void fin_y(int fa)
{
    /**
     * @brief controlar la rotación de una sola capa del eje y
     * @param fa factor de rotación
     */

     // calcular el resultado según el factor
    dir_y = di = 2 * (fa / 6) - 1;
    lay_y = (fa - 1) % 3 + 1;

    // actualizar el cubo según el resultado
    int i;
    for (i = 1; i < 10; i++)
        yro[(lay_y - 1) * 9 + i] = 1;
    rot = 1;
}

void fin_x(int fa)
{
    /**
     * @brief controlar la rotación de una sola capa del eje x
     * @param fa factor de rotación
     */

     // calcula el resultado según el factor
    dir_x = di = 2 * (fa / 6) - 1;
    lay_x = (fa - 1) % 3 + 1;

    // actualiza el cubo según el resultado
    if (lay_x == 1)
    {
        xro[7] = 1;
        xro[8] = 1;
        xro[9] = 1;
        xro[16] = 1;
        xro[17] = 1;
        xro[18] = 1;
        xro[25] = 1;
        xro[26] = 1;
        xro[27] = 1;
    }
    if (lay_x == 2)
    {
        xro[4] = 1;
        xro[5] = 1;
        xro[6] = 1;
        xro[13] = 1;
        xro[14] = 1;
        xro[15] = 1;
        xro[22] = 1;
        xro[23] = 1;
        xro[24] = 1;
    }
    if (lay_x == 3)
    {
        xro[1] = 1;
        xro[2] = 1;
        xro[3] = 1;
        xro[10] = 1;
        xro[11] = 1;
        xro[12] = 1;
        xro[19] = 1;
        xro[20] = 1;
        xro[21] = 1;
    }

    rot = 2;
}

void fin_z(int fa)
{
    /**
     * @brief controlar la rotación de una capa del eje z
     * @param fa factor de rotación
     */

     // calculate result according to factor
    dir_z = -1 * (di = 2 * (fa / 6) - 1);
    lay_z = (fa - 1) % 3 + 1;

    // actualiza el cubo según el resultado
    if (lay_z == 1)
    {
        zro[9] = 1;
        zro[6] = 1;
        zro[3] = 1;
        zro[18] = 1;
        zro[15] = 1;
        zro[12] = 1;
        zro[21] = 1;
        zro[24] = 1;
        zro[27] = 1;
    }
    if (lay_z == 2)
    {
        zro[2] = 1;
        zro[5] = 1;
        zro[8] = 1;
        zro[11] = 1;
        zro[14] = 1;
        zro[17] = 1;
        zro[20] = 1;
        zro[23] = 1;
        zro[26] = 1;
    }
    if (lay_z == 3)
    {
        zro[1] = 1;
        zro[4] = 1;
        zro[7] = 1;
        zro[10] = 1;
        zro[13] = 1;
        zro[16] = 1;
        zro[19] = 1;
        zro[22] = 1;
        zro[25] = 1;
    }

    rot = 3;
}

void wholr(int ax, int dir)
{
    /**
     * @Brief actualiza el parámetro de rotación en función de ax y dir
     * @param factor ax para calcular el parámetro de rotación, (0, 1, 2)
     * @param dir dirección del parámetro de rotación, (-1, 1)
     */

    int i;

    di = dir;
    for (i = 0; i < 28; i++)
    {
        // judge the axis
        if (ax == 1)
            xro[i] = 1;
        else if (ax == 0)
            yro[i] = 1;
        else
            zro[i] = 1;
    }

    rot = 10 + 5 * ax + dir;  // actualizar el parámetro de rotación
}

void keyboard(unsigned char key, int x, int y)
{
    /**
     * @interacción breve por teclado
     * @param tecla que se ha pulsado
     */

    int i, j;
    pressed = 1;

    if (start_flag == 1)  // válido si el juego comienza
    {

        if (yes && pause != 1 && !solved)  // se resuelve, no puede la rotación

        {
            // interacción según teclas
            switch (key)
            {
                // rotar una sola capa
            case 'x':fin_x(1); yes = 0; break;
            case 'c':fin_x(2); yes = 0; break;
            case 'v':fin_x(3); yes = 0; break;
            case 'w':fin_x(7); yes = 0; break;
            case 'e':fin_x(8); yes = 0; break;
            case 'r':fin_x(9); yes = 0; break;
            case 't':fin_y(1); yes = 0; break;
            case 'g':fin_y(2); yes = 0; break;
            case 'b':fin_y(3); yes = 0; break;
            case 'q':fin_y(7); yes = 0; break;
            case 'a':fin_y(8); yes = 0; break;
            case 'z':fin_y(9); yes = 0; break;
            case 'm':fin_z(1); yes = 0; break;
            case 'j':fin_z(2); yes = 0; break;
            case 'u':fin_z(3); yes = 0; break;
            case 'n':fin_z(7); yes = 0; break;
            case 'h':fin_z(8); yes = 0; break;
            case 'y':fin_z(9); yes = 0; break;

            default:break;
            }
        }

        // lo que sea que resuelva, la tecla de abajo puede usar
        switch (key)
        {
            // rotate whole cube
        case 's':wholr(2, 1); yes = 0; break;
        case 'f':wholr(2, -1); yes = 0; break;


        }
    }

    // en cualquier momento, las teclas a continuación pueden usar
    switch (key)
    {
    case 27:exit(0); break;  // Esc to exit

    case 13:
        // Enter para empezar o terminar

        if (start_flag == 1)  // if start, end
        {
            start_flag = 0;
            pressed = 0;
            solving = 0;
            pause = -1;
            for (i = 0; i < 6; i++)
                for (j = 0; j < 10; j++)
                    Cube[i][j] = i;
            for (i = 0; i < 28; i++)
            {
                xro[i] = 0;
                yro[i] = 0;
                zro[i] = 0;
            }

            // update state flag
            solved = 0;
            process_flag = 0;
            forbid = 0;
        }
        else if (start_flag == 0)  // if end, start
            start_flag = 1;
        break;
    }

}

void special_key(int key, int x, int y)
{
    /**
      * @interacción breve con la tecla especial, para rotar todo el cubo
      * @param tecla que se ha pulsado
      */

    if (start_flag == 1)  //juzgar si el juego se inicia
    {
        if (yes)
        {
            // interacción según clave
            if (key == GLUT_KEY_RIGHT) {
                wholr(0, -1);
            }

            else if (key == GLUT_KEY_LEFT) {
                wholr(0, 1);
            }

            else if (key == GLUT_KEY_UP) {
                wholr(1, 1);
            }

            else if (key == GLUT_KEY_DOWN) {
                wholr(1, -1);
            }
        }
    }
}

void timer(int t)
{

    glutPostRedisplay();
    glutTimerFunc(15, timer, 1);
}

void setcol(int f)
{
    /**
     * @Brief establece el color de cada cara
     * @param l capa que hay que colorear
     */

    switch (f)
    {
        // según f, establece el color
    case 0:glColor3f(1.0, 1.0, 1.0); break;
    case 1:glColor3f(0.0, 0.0, 1.0); break;
    case 2:glColor3f(1.0, 1.0, 0.0); break;
    case 3:glColor3f(0.0, 1.0, 0.0); break;
    case 4:glColor3f(1.0, 0.0, 0.0); break;
    case 5:glColor3f(1.0, 0.5, 0.0); break;
    case 6:glColor3f(0.1, 0.0, 0.0); break;
    default:break;
    }
}
//___________________________________________________ ____(SINGLE) CUBE
void mycube(int pic, float siz, float x, float y, float z, int d, int f, int u, int b, int r, int l)
{
    /**
      * @brief Los cubos pequeños se construyen en base a los parámetros pasados
      */

    siz /= 2;

    glPushMatrix();

    // establecer la rotación
    if (yro[pic])
        glRotatef(angle * di, 0.0, 1.0, 0.0);
    if (xro[pic])
        glRotatef(angle * di, 1.0, 0.0, 0.0);
    if (zro[pic])
        glRotatef(angle * di, 0.0, 0.0, 1.0);

    glTranslatef(x, y, z);

    // modelando 6 caras de un solo cubo
    glBegin(GL_POLYGON);
    setcol(d);  // set color
    glVertex3f(siz, -siz, siz);
    glVertex3f(siz, -siz, -siz);
    glVertex3f(-siz, -siz, -siz);
    glVertex3f(-siz, -siz, siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(b);// set color
    glVertex3f(siz, siz, siz);
    glVertex3f(siz, -siz, siz);
    glVertex3f(-siz, -siz, siz);
    glVertex3f(-siz, siz, siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(u);// set color
    glVertex3f(siz, siz, siz);
    glVertex3f(siz, siz, -siz);
    glVertex3f(-siz, siz, -siz);
    glVertex3f(-siz, siz, siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(f);// set color
    glVertex3f(siz, siz, -siz);
    glVertex3f(siz, -siz, -siz);
    glVertex3f(-siz, -siz, -siz);
    glVertex3f(-siz, siz, -siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(r);// set color
    glVertex3f(siz, siz, siz);
    glVertex3f(siz, -siz, siz);
    glVertex3f(siz, -siz, -siz);
    glVertex3f(siz, siz, -siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(l);// set color
    glVertex3f(-siz, siz, siz);
    glVertex3f(-siz, -siz, siz);
    glVertex3f(-siz, -siz, -siz);
    glVertex3f(-siz, siz, -siz);
    glEnd();

    glPopMatrix();
}

void rubic(float siz, float x)
{
    /**
     * @brief usa 26 cubos pequeños para construir el cubo de Rubik
     */

    GLfloat diffuse[] = { 1.0f, 0.5f, 0.2f, 1.0f, };
    GLfloat ambient[] = { 1.0f, 0.0f, 0.0f, 1.0f, };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f, };

    // la capa superior, 9 cubos
    mycube(1, siz, x, x, x, 6, 6, Cube[2][3], Cube[3][9], Cube[4][3], 6);
    mycube(2, siz, x, x, 0, 6, 6, Cube[2][6], 6, Cube[4][2], 6);
    mycube(3, siz, x, x, -x, 6, Cube[1][3], Cube[2][9], 6, Cube[4][1], 6);

    mycube(4, siz, 0, x, x, 6, 6, Cube[2][2], Cube[3][8], 6, 6);
    mycube(5, siz, 0, x, 0, 6, 6, Cube[2][5], 6, 6, 6);
    mycube(6, siz, 0, x, -x, 6, Cube[1][2], Cube[2][8], 6, 6, 6);

    mycube(7, siz, -x, x, x, 6, 6, Cube[2][1], Cube[3][7], 6, Cube[5][3]);
    mycube(8, siz, -x, x, 0, 6, 6, Cube[2][4], 6, 6, Cube[5][2]);
    mycube(9, siz, -x, x, -x, 6, Cube[1][1], Cube[2][7], 6, 6, Cube[5][1]);

    // la capa intermedia, 8 cubos, el cubo central (14) sin necesidad de modelar
    mycube(10, siz, x, 0, x, 6, 6, 6, Cube[3][6], Cube[4][6], 6);
    mycube(11, siz, x, 0, 0, 6, 6, 6, 6, Cube[4][5], 6);
    mycube(12, siz, x, 0, -x, 6, Cube[1][6], 6, 6, Cube[4][4], 6);

    mycube(13, siz, 0, 0, x, 6, 6, 6, Cube[3][5], 6, 6);
    mycube(15, siz, 0, 0, -x, 6, Cube[1][5], 6, 6, 6, 6);

    mycube(16, siz, -x, 0, x, 6, 6, 6, Cube[3][4], 6, Cube[5][6]);
    mycube(17, siz, -x, 0, 0, 6, 6, 6, 6, 6, Cube[5][5]);
    mycube(18, siz, -x, 0, -x, 6, Cube[1][4], 6, 6, 6, Cube[5][4]);

    // la capa inferior, 9 cubos
    mycube(19, siz, x, -x, x, Cube[0][9], 6, 6, Cube[3][3], Cube[4][9], 6);
    mycube(20, siz, x, -x, 0, Cube[0][6], 6, 6, 6, Cube[4][8], 6);
    mycube(21, siz, x, -x, -x, Cube[0][3], Cube[1][9], 6, 6, Cube[4][7], 6);

    mycube(22, siz, 0, -x, x, Cube[0][8], 6, 6, Cube[3][2], 6, 6);
    mycube(23, siz, 0, -x, 0, Cube[0][5], 6, 6, 6, 6, 6);
    mycube(24, siz, 0, -x, -x, Cube[0][2], Cube[1][8], 6, 6, 6, 6);

    mycube(25, siz, -x, -x, x, Cube[0][7], 6, 6, Cube[3][1], 6, Cube[5][9]);
    mycube(26, siz, -x, -x, 0, Cube[0][4], 6, 6, 6, 6, Cube[5][8]);
    mycube(27, siz, -x, -x, -x, Cube[0][1], Cube[1][7], 6, 6, 6, Cube[5][7]);
}





void draw_play_station(void)
{
    /**
     * @brief dibujar todos los objetos de este proyecto
     */

    int i, j;
    float siz = 0.2, x, sp = .02;  // tamaño del cubo
    x = siz + sp;

    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // establecer el punto de vista
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);

    glTranslatef(0.0, -0.3, 0.0);  // translation

    //controlar la rotación
    if (rot)
    {
        angle += 10;
        if (angle > 90)
        {
            angle = 0;
            for (i = 0; i < 28; i++)
            {
                xro[i] = 0;
                yro[i] = 0;
                zro[i] = 0;
            }

            // según el parámetro de rotación para controlar la rotación del cubo de Rubik
            if (rot == 1)
                rotation_y_layer(lay_y, dir_y);
            if (rot == 2)
                rotation_x_layer(lay_x, dir_x);
            if (rot == 3)
                rotation_z_layer(lay_z, dir_z);
            if (rot == 9)
            {
                rotation_y_layer(1, -1);
                rotation_y_layer(2, -1);
                rotation_y_layer(3, -1);
            }
            if (rot == 11)
            {
                rotation_y_layer(1, 1);
                rotation_y_layer(2, 1);
                rotation_y_layer(3, 1);
            }
            if (rot == 14)
            {
                rotation_x_layer(1, -1);
                rotation_x_layer(2, -1);
                rotation_x_layer(3, -1);
            }
            if (rot == 16)
            {
                rotation_x_layer(1, 1);
                rotation_x_layer(2, 1);
                rotation_x_layer(3, 1);
            }
            if (rot == 19)
            {
                rotation_z_layer(1, 1);
                rotation_z_layer(2, 1);
                rotation_z_layer(3, 1);
            }
            if (rot == 21)
            {
                rotation_z_layer(1, -1);
                rotation_z_layer(2, -1);
                rotation_z_layer(3, -1);
            }

            // actualizar el parámetro de control del proceso
            rot = 0;
            yes = 1;
        }
    }

    rubic(siz, x);  // volver a dibujar el cubo
    glFlush();


    glutSwapBuffers();  // visualización
}






int main(int argc, char** argv)
{
    /**
     * @función principal breve, para el inicio de todo el programa
     */

     // agrega color para cada capa del cubo
    int i, j;
    for (i = 0; i < 6; i++)
        for (j = 0; j < 10; j++)
            Cube[i][j] = i;
    for (i = 0; i < 28; i++)
    {
        xro[i] = 0;
        yro[i] = 0;
        zro[i] = 0;
    }

    // imprimir sentencias en la consola
    printf("\n\n\nInstruction guide:\n");
    printf("\n\n\tStart/End: ENTER\t\\t\tReset: o\n\n\n\n\t\tw\te\tr\n\n\n\tq\t\t\t\tt\t\ty\tu\t\t\t��\n\n\n\ta\t\t\t\tg\t\th\tj\t\ts\t\tf\n\n\n\tz\t\t\t\tb\t\tn\tm\t\t��\t��\t��\n\n\n\t\tx\tc\tv\n\n\n");

    // inicia todo el programa
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);  // tamaño de la ventana
    glutInitWindowPosition(660, 0);  // posición de la ventana
    glutCreateWindow("3D Rubik's cube play station");  // nombre de la ventana
    glClearColor(0.69, 0.878, 0.9, 0.1);  // color de fondo
    glEnable(GL_DEPTH_TEST);  // abre la prueba de profundidad
    // métodos de llamada
    glutDisplayFunc(draw_play_station);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(100, timer, 1);
    glutSpecialFunc(special_key);
    glutReshapeFunc(change_point);
    glutMainLoop();  // bucle para dibujar




    return 0;
}