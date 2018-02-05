#include <windows.h>        // For MS Windows
#include <GL/glut.h>        // GLUT, include glu.h and gl.h
#include <bits/stdc++.h>


using namespace std;

int wd, ht, t_mode=0, r_mode=0, xMode=0, yMode=0, zMode=0, ml=1, angle=0;
double my_matrix[16];
double tr_x=0, tr_y=0, tr_z=-10, theta_x=0, theta_y=0, theta_z=0, wheel_speed=0, radius=0;
double xx=0,yy=2,zz=-5;

/* Initialize OpenGL Graphics */
void initGL(){
   glClearColor(224/255.0, 224/255.0, 224/255.0, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void loadIdMatrix(){
    memset(my_matrix,0,sizeof(my_matrix));
    for(int i=0; i<16; i+=5)
        my_matrix[i]=1;
}
void translate(double my_matrix[], double x, double y, double z){
    my_matrix[12]=x;
    my_matrix[13]=y;
    my_matrix[14]=z;
}
void rotateX(double my_matrix[], double theta){
    my_matrix[5]= cos(theta);
    my_matrix[6]= -sin(theta);
    my_matrix[9]= sin(theta);
    my_matrix[10]=cos(theta);
}
void rotateY(double my_matrix[], double theta){
    my_matrix[0]= cos(theta);
    my_matrix[2]= sin(theta);
    my_matrix[8]= -sin(theta);
    my_matrix[10]=cos(theta);
}
void rotateZ(double my_matrix[], double theta){
    my_matrix[0]=cos(theta);
    my_matrix[1]=-sin(theta);
    my_matrix[4]=sin(theta);
    my_matrix[5]=cos(theta);
}
void scale(double my_matrix[], double factorX, double factorY, double factorZ){
    my_matrix[0]=factorX;
    my_matrix[5]=factorY;
    my_matrix[10]=factorZ;
    my_matrix[15]=1;
}

void drawWheel(){
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    double radius = 0.2f; //radius
    double twicePi = 2.0f * 3.1416;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // center of circle
    for(i = 0; i <= triangleAmount; i++){
        glVertex2f((radius * cos(i *  twicePi / triangleAmount)), (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.2f,  0.0f);
    glVertex2f(0.2f,  0.0f);
    glVertex2f(0.0f,  -0.2f);
    glVertex2f(0.0f,  0.2f);
	glEnd();
}
void drawCube(double tr_x, double tr_y,  double bl_x, double bl_y,  double front_z, double back_z){
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    GLfloat vertices[] = {tr_x,  tr_y,  front_z, // front top right corner
                          bl_x,  tr_y,  front_z, // front top left corner
                          bl_x,  bl_y,  front_z, // front bottom left corner
                          tr_x,  bl_y,  front_z, // back bottom right corner
                          tr_x,  tr_y,  back_z,  // back top right corner
                          bl_x,  tr_y,  back_z,  // back top left corner
                          bl_x,  bl_y,  back_z,  // back bottom left corner
                          tr_x,  bl_y,  back_z   // back bottom right corner
                         };
    GLubyte indices[] = {0,1,2,3, // first triangle (bottom left - top left - top right)
                         4,0,3,7,
                         4,5,6,7,
                         1,5,6,2,
                         0,4,5,1,
                         2,6,7,3
                        }; // second triangle (bottom left - top right - bottom right)

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);
}
void drawTable(){
    // table base
    glPushMatrix();
    glColor3f(255/255.0, 178/255.0, 102/255.0);

    loadIdMatrix();
    translate(my_matrix, 0, 0, 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 10, 0.5, 4);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    // table leg 1
    glPushMatrix();
    glColor3f(255/255.0, 204/255.0, 153/255.0);
    loadIdMatrix();
    translate(my_matrix, 9, -3.5, 3.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 3, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();


    // table leg 2
    glPushMatrix();
    glColor3f(255/255.0, 204/255.0, 153/255.0);
    loadIdMatrix();
    translate(my_matrix, -9, -3.5 , 3.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 3, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();


    // table leg 3
    glPushMatrix();
    glColor3f(255/255.0, 204/255.0, 153/255.0);
    loadIdMatrix();
    translate(my_matrix, -9, -3.5 , -3.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 3, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    // table leg 4
    glPushMatrix();
    glColor3f(255/255.0, 204/255.0, 153/255.0);
    loadIdMatrix();
    translate(my_matrix, 9, -3.5 , -3.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 3, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();
}
void drawFloor(){
    //floor
    glPushMatrix();
    glColor3f(192/255.0, 192/255.0, 192/255.0);
    loadIdMatrix();
    translate(my_matrix, 0, -6.6 , 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 20, 0.1, 15);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //wall behind
    glPushMatrix();
    glColor3f(204/255.0, 255/255.0, 255/255.0);
    loadIdMatrix();
    translate(my_matrix, 0, 0, 15);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 20, 6.5, 0.1);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //wall right
    glPushMatrix();
    glColor3f(153/255.0, 255/255.0, 204/255.0);
    loadIdMatrix();
    translate(my_matrix, -20, 0, 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.1, 6.5, 15);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

}
void drawHorse(){
    //body
    glPushMatrix();
    glColor3f(153/255.0, 76/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, 0, 3, 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 2, 0.5, 0.8);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //neck
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, -2, 4, 0);
    rotateZ(my_matrix, 10.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.8, 0.3, 0.8);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //mouth
    glPushMatrix();
    glColor3f(153/255.0, 76/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, -3, 4.2, 0);
    rotateZ(my_matrix, -10.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 0.3, 0.8);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //leg 1
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, -1.7, 1.7, -.6);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.3, 0.85, 0.2);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //leg 2
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, -1.7, 1.7, 0.6);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.3, 0.85, 0.2);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //leg 3
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, 1.7, 1.7, -.6);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.3, 0.85, 0.2);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //leg 4
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, 1.7, 1.7, 0.6);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.3, 0.85, 0.2);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //wheels 1 & 2
    glPushMatrix();
    loadIdMatrix();
    glColor3f(153/255.0, 0/255.0, 0/255.0);
    translate(my_matrix, -1.75, 1.0, 0.85);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 2.5, 2.5, 2.5);
    glMultMatrixd(my_matrix);

    glRotatef(wheel_speed, 0.0f, 0.0f, 1.0f);
    drawWheel();
    for(int i=0; i<80; i++){
        glColor3f(153/255.0, 0/255.0, 0/255.0);
        glTranslatef(0.0, 0.0, -0.001);
        drawWheel();
    }

    glColor3f(153/255.0, 0/255.0, 0/255.0);
    glTranslatef(0.0, 0.0, -0.60);
    drawWheel();
    for(int i=80; i>0; i--){
        glColor3f(153/255.0, 0/255.0, 0/255.0);
        glTranslatef(0.0, 0.0, 0.001);
        drawWheel();
    }
    glPopMatrix();

    //wheels 3 & 4
    glPushMatrix();
    glColor3f(153/255.0, 0/255.0, 0/255.0);
    translate(my_matrix, 1.75, 1.0, 0.85);
    glMultMatrixd(my_matrix);

    glRotatef(wheel_speed, 0.0f, 0.0f, 1.0f);
    drawWheel();
    for(int i=0; i<80; i++){
        glColor3f(153/255.0, 0/255.0, 0/255.0);
        glTranslatef(0.0, 0.0, -0.001);
        drawWheel();
    }

    glColor3f(153/255.0, 0/255.0, 0/255.0);
    glTranslatef(0.0, 0.0, -0.60);
    drawWheel();
    for(int i=80; i>0; i--){
        glColor3f(153/255.0, 0/255.0, 0/255.0);
        glTranslatef(0.0, 0.0, 0.001);
        drawWheel();
    }
    glPopMatrix();
}
void drawObj(){
    glPushMatrix();
    glColor3f(255/255.0, 51/255.0, 51/255.0);
    loadIdMatrix();
    translate(my_matrix, -4, 1, 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 0.5, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();

    loadIdMatrix();
    translate(my_matrix, tr_x, tr_y, tr_z);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    rotateX(my_matrix, theta_x);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    rotateY(my_matrix, theta_y);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    rotateZ(my_matrix, theta_z);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, .3, .3, .3);
    glMultMatrixd(my_matrix);

    gluLookAt(xx,yy,zz, 0,0,0, 0,1,0);

    drawTable();
    drawFloor();
    drawObj();

    glPushMatrix();
    glTranslatef(-4, 1, 0); // 3. Translate to the object's position.
    glRotatef(angle, 0.0, 1.0, 0.0); // 2. Rotate the object.
    glTranslatef(4,-1, 0); // 1. Translate to the origin.

    drawHorse();

    glPopMatrix();

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void timer(int value){
    wheel_speed += 5.0f;
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(150, timer, 0); // next timer call milliseconds later
}

void my_keyboard(unsigned char k, int x, int y){
    switch (k){
//    case 'n':
//        w-=.2;
//        e+=.3;
//        break;
    case ',':
        xx+=.2f;
        break;
    case '.':
        yy+=.2f;
        break;
    case '/':
        zz+=.2f;
        break;
    case '<':
        xx-=.2f;
        break;
    case '>':
        yy-=.2f;
        break;
    case '?':
        zz-=.2f;
        break;

    case 'T':
        if(t_mode==0)
            t_mode=1;
        else
            t_mode=0;
        break;
    case 'R':
        if(r_mode==0)
            r_mode=1;
        else
            r_mode=0;
        break;
    case 'I':
        if(t_mode==1)
            tr_y+=.5f;
        break;
    case 'J':
        if(t_mode==1)
            tr_x-=.5f;
        break;
    case 'K':
        if(t_mode==1)
            tr_y-=.5f;
        break;
    case 'L':
        if(t_mode==1)
            tr_x+=.5f;
        break;
    case 'X':
        if(r_mode==1){
            xMode=1;
            yMode=0;
            zMode=0;
        }
        break;
    case 'Y':
        if(r_mode==1){
            xMode=0;
            yMode=1;
            zMode=0;
        }
        break;
    case 'Z':
        if(r_mode==1){
            xMode=0;
            yMode=0;
            zMode=1;
        }
        break;
    // Rotating clockwise
    case 'c':
        if(r_mode==1){
            if(xMode==1)
                theta_x -= .05;
            else if(yMode==1)
                theta_y -= .05;
            else if(zMode==1)
                theta_z -= .05;
        }
        break;
    //counter counter-clockwise
    case 'C':
        if(r_mode==1){
            if(xMode==1)
                theta_x+= .05;
            else if(yMode==1)
                theta_y += .05;
            else if(zMode==1)
                theta_z += .05;
        }
        break;
    case 'F':
        theta_y+=acos(-1);
        break;

    case 'p':
        //cout<<radius<<" "<<q<<" "<<e<<endl;
        radius=sqrt(xx*xx + zz*zz);

        if((xx+ml*.5)>=radius)
            ml=-1;
        else if((xx+ml*.5)<=-radius)
            ml=1;

        //if((q-radius)<=.5)ml=-1;
        //if((q+radius)<=-.5)ml=1;
        xx=xx+ ml*.5;

        if(xx<=radius && xx>=-radius)
            zz= ml*sqrt(radius*radius-xx*xx);
        break;
    case 'P':
        //cout<<radius<<" "<<q<<" "<<e<<endl;
        radius=sqrt(xx*xx + zz*zz);

        if((xx+ml*.5)>=radius)
            ml=-1;
        else if((xx+ml*.5)<=-radius)
            ml=1;

        //if((q-radius)<=.5)ml=-1;
        //if((q+radius)<=-.5)ml=1;
        xx=xx+ ml*.5;

        if(xx<=radius && xx>=-radius)
            zz= -ml*sqrt(radius*radius-xx*xx);
        break;

    case 'V':
        angle+=5;
        break;
    case 'v':
        angle-=5;
        break;

    case 'b':
        tr_z+=.5f;
        break;
    case 'B':
        tr_z-=.5f;
        break;

    case '0':
        exit(0);			// exit
    }

    glutPostRedisplay();		// redraw the image now
}


/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height){
    // Compute aspect ratio of the new window
    wd=width;
    ht=height;
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv){
    glutInit(&argc, argv);               // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE);    // Enable double buffered mode
    glutInitWindowSize(640, 480);        // Set the window's initial width & height
    glutInitWindowPosition(50, 50);      // Position the window's initial top-left corner
    glutCreateWindow("Horse with wheels");             // Create window with the given title
    glutDisplayFunc(display);            // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);            // Register callback handler for window re-size event
    glutKeyboardFunc(my_keyboard);		// call keyboard() when key is hit
    initGL();                            // Our own OpenGL initialization
    glutTimerFunc(0, timer, 0);          // First timer call immediately [NEW]
    glutMainLoop();                      // Enter the infinite event-processing loop
    return 0;
}
