#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include<OpenGl/glu.h>
#include<OpenGl/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "front.h"

#define PI 3.14159265

void display();
void specialKeys();
flock* birds;
//!rotates in y
double rotate_y=0;
//!rotates in x 
double rotate_x=0;
//!tranlates in x
double trans_x =0;
//!tranlates in y
double trans_y = 0;
//!zooms in the screen
double zoom =1;
//!pauses screen 
bool pause = false ;

//! calculates dir_cos based on velocities
double* dir_cos(boid* a){
	double* ans = (double*)malloc(3 * sizeof(double));
	if(pow(a->v_x,2) + pow(a->v_y,2) + pow(a->v_z,2) != 0){
	ans[2] = pow(pow(a->v_x,2) + pow(a->v_y,2) + pow(a->v_z,2)   ,0.5  );
	
	ans[0] = a->v_x /ans[2];
	ans[1] = a->v_y/ans[2];
	ans[2] = a->v_z/ans[2];}
	else{
		ans[0] =1 ;
		ans[1] =0 ;
		ans[2] = 0;
	}
	return ans;

}
//! display function
void display(){
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef( trans_x, trans_y, 0.0 ); 
  glRotatef(rotate_x,1,0,0);
  glRotatef(rotate_y,0,1,0);
  double infi = 200000*zoom ;
  glBegin(GL_LINES);
  glColor3f(1,0,0);
  glVertex3f(0,0,0);
  glVertex3f(infi,0,0);
  glEnd();
  glBegin(GL_LINES);
  glColor3f(0,1,0);
  glVertex3f(0,0,0);
  glVertex3f(0,infi,0);
  glEnd();
  glBegin(GL_LINES);
  glColor3f(0,0,1);
  glVertex3f(0,0,0);
  glVertex3f(0,0,infi);
  glEnd();



  for(int i = 0 ; i< birds -> n ;i++){
  	//glColor3f(0.504,0.768,0.952);
  	double x = birds ->stars[i]->x ;
  	double y = birds ->stars[i]->y ;
  	double z = birds ->stars[i]->z ;
  	double* m = dir_cos(birds ->stars[i]);
    glColor3f(x*x + 0.5,y*y +0.5,z*z + 0.5);
  	
/*
    glTranslatef((x)*zoom , (y)*zoom , (z)*zoom    );
  	glRotatef( acos(m[2])*180/PI,-m[1],+m[0],0 );
 	  glutSolidCone((0.015)*zoom,0.05*zoom,40,40);
  	glRotatef( acos(m[2])*180/PI,+m[1],-m[0],0 );
  	glTranslatef(-(x)*zoom , -(y)*zoom , -(z)*zoom  );
*/
    double flap = (birds -> stars[i])->flap;
    double sc = 0.01 ;
    glTranslatef((x)*zoom , (y)*zoom , (z)*zoom    );
    glRotatef( acos(m[2])*180/PI,-m[1],+m[0],0 );
    glRotatef(-90,0,1,0);
   // beginShape(TRIANGLES);
    glBegin(GL_POLYGON);
    glVertex3f(3*sc,0,0);
    glVertex3f(-3*sc,2*sc,0);
    glVertex3f(-3*sc,-2*sc,0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(3*sc,0,0);
    glVertex3f(-3*sc,2*sc,0);
    glVertex3f(-3*sc,0,2*sc);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(3*sc,0,0);
    glVertex3f(-3*sc,0,2*sc);
    glVertex3f(-3*sc,-2*sc,0);
    glEnd();
    // wings

    //double f =(double) rand();
    //double flap = 7;
    glBegin(GL_POLYGON);
    glVertex3f(2*sc, 0, 0);
    glVertex3f(-1*sc, 0, 0);

    glVertex3f(-1*sc, -8*sc, flap*sc);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(2*sc, 0, 0);
    glVertex3f(-1*sc, 0, 0);
    glVertex3f(-1*sc, 8*sc,flap*sc);
    glEnd();


    glBegin(GL_POLYGON);
    glVertex3f(-3*sc, 0, 2*sc);
    glVertex3f(-3*sc, 2*sc, 0);
    glVertex3f(-3*sc, -2*sc, 0);
    glEnd();
    //
    glRotatef(90,0,1,0);
    glRotatef( acos(m[2])*180/PI,+m[1],-m[0],0 );
    glTranslatef(-(x)*zoom , -(y)*zoom , -(z)*zoom  );

    //box(10);

    if(flap > 7) birds -> stars[i]->flap = 0;
    else birds -> stars[i]->flap = birds -> stars[i]->flap + 1;
  //  std::cout << "hot chick "<< flap << std::endl;
  






  }
  for(int i =0 ; i< birds-> f ;i++){
  glColor3f(1,1,1);
  glTranslatef((birds ->fix[i] ->x)*zoom , (birds ->fix[i] ->y)*zoom , (birds ->fix[i] ->z)*zoom    );
  glutSolidSphere((0.01)*zoom,10,10);
  glTranslatef(-(birds ->fix[i] ->x)*zoom , -(birds ->fix[i] ->y)*zoom , -(birds ->fix[i] ->z)*zoom  );

  }
  
  birds->nextInstant();
  glFlush();
  glutSwapBuffers();
 	
}
 
//! processKeys
void processKeys(unsigned char key , int x , int y){
  if(key == 'w') trans_y += 0.5;
  else if(key == 's') trans_y -=0.5;		
  else if(key == 'd') trans_x +=0.5;		
  else if(key == 'a') trans_x -=0.5;
  else if(key == 'm') zoom = zoom *1.6;
  else if(key == 'n') zoom = zoom *0.625;
  else if(key == 'j'){ 
  	if(birds ->coeff[0] ==0.001) birds ->coeff[0] =0;
  	else birds ->coeff[0] =0.001;}
 else if(key == 'k'){ 
  	if(birds ->coeff[1] ==0.2) birds ->coeff[1] =0;
  	else birds ->coeff[1] =0.2;}
 else if(key == 'l'){ 
  	if(birds ->coeff[2] ==0.1) birds ->coeff[2] =0;
  	else birds ->coeff[2] =0.1; }
  else if(key == 'p'){
  	pause = !pause ;

  }		 	

  glutPostRedisplay();
    
}
//! specialKeys 
void specialKeys( int key, int x, int y ) {
  if (key == GLUT_KEY_RIGHT)
    rotate_y += 5;
  else if (key == GLUT_KEY_LEFT)
    rotate_y -= 5;
  else if (key == GLUT_KEY_UP)
    rotate_x += 5;
  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;
  glutPostRedisplay();
}
//! Timer function
/*
 responsible for updating the frames
*/
void Timer(int value)
{
	glutTimerFunc(30, Timer, 0);
	if(!pause) glutPostRedisplay();
}
 
//! main 
int main(int argc, char* argv[])
{
  int n_root = 4;	
  double* r_sq = (double*) malloc(3*sizeof(double));
  double* coeff = (double*) malloc(3*sizeof(double));
  boid** stars = (boid**) malloc(n_root*n_root*n_root*sizeof(boid*));
  int f_root = 100;
  fixed** fix = (fixed**) malloc(4*f_root*sizeof(fixed*));

  for(int i =0 ; i< f_root ; i++){
  		double m = (double) i ;
  		fix[i] = new fixed( (m/50) -1  ,1.2 ,0 );
  }
  for(int i =f_root ; i< 2*f_root ; i++){
  		double m = (double) i - f_root ;
  		
  		fix[i] = new fixed( (m/50) -1  ,-1.2 ,0 );
  }
  for(int i =2*f_root ; i< 3*f_root ; i++){
  		double m = (double) i - 2*f_root ;
  		
  		fix[i] = new fixed( 1.2,(m/50) -1   ,0 );
  }
  for(int i =3*f_root ; i< 4*f_root ; i++){
  		double m = (double) i - 3*f_root;
  		
  		fix[i] = new fixed( -1.2  ,(m/50) -1 ,0 );
  }

  int q =0 ;
  
  for(int i =0 ; i< n_root ; i++){
  	for(int j =0 ; j<n_root ; j++){
  		for(int k = 0 ; k<n_root ;k++){
  		double m = (double) i ;
  		double n = (double) j ;
  		double l = (double) k ;
  		stars[q] = new boid( m/(n_root) ,n/(n_root),l/(n_root) , 0.0 ,0.0 ,0.00 );
  		q++;
  		}
  	}
  }
  
  r_sq[0] = 0.4;
  r_sq[1] = 0.005;
  r_sq[2] = 0.25;
  coeff[0] = 0.01;
  coeff[1] = 0.7;
  coeff[2] = 0.5;
  
  r_sq[0] = 0.01;
  r_sq[1] = 0.01;
  r_sq[2] = 0.25;
  coeff[0] = 0.01;
  coeff[1] = 0.1;
  coeff[2] = 0.2;
  birds = new flock(n_root*n_root*n_root,0*f_root,fix,stars,r_sq,coeff);


 
  //  Initialize GLUT and process user parameters
  glutInit(&argc,argv);
 
  //  Request double buffered true color window with Z-buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
  // Create window
  glutCreateWindow("thop");
 
  //  Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);
 
  // Callback functions
  glutDisplayFunc(display);
  glutSpecialFunc(specialKeys);
   glutKeyboardFunc(processKeys);
 
  //  Pass control to GLUT for events
  
  Timer(0);
  glutMainLoop();
  //int oldTimeSinceStart = 0;

	
 
  //  Return to OS
  return 0;
 
}