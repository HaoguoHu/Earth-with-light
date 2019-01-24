//Code by Haoguo Hu, hghu@umich.edu
//University of Michigan, Ann arbor

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>

using namespace std;

static double backtime=100./120.;             //for time step;
static float ang=2*backtime*360./24./3600.;    // Angle of increation/decreation;
static float angg=ang;                       //for friction;
static float R = 4.;                         //sphere radius;
static float near = 6.;                     // distance from eye to near plane ;      
static float theta = 0.0;                    //longtide;
static float phi=M_PI/2.;                    //initial latitude; 
static int slices=72, stacks =72;                  //for sphere cration; 
                                
static int mouse_x0 = 0;
static int mouse_y0 = 0; 
static int mouse_UPx= 0;
static int mouse_UPy= 0; 

static int autor = 0;                        //auto rotation;
static int mouseN=0;                         //mouse down+up connection;
static int SWIPE=0;                          //spin or not;
static float spinX=0;                        //spin along longitude;  
static float spinY=0;                        //spin along latitude;
static float sunA=23.43*M_PI/180.;
static float syn=1.;

static void glLightfv();

GLuint texID;                               //texture ID;
GLuint LoadTexture( const char * filename ); //load texture fuction;

void myWireSphere(GLfloat R, int slices, int stacks) 
{   
     GLUquadricObj *qObj = gluNewQuadric();
//	 gluQuadricNormals(qObj, GLU_SMOOTH);
	 gluQuadricTexture(qObj, GL_TRUE);
	
	 glRotatef(90.0, 1.0, 0.0, 0.0);                      	
	 gluSphere(qObj, R, slices, stacks);
	
	 glEnable(GL_TEXTURE_2D);
	 glBindTexture(GL_TEXTURE_2D, texID); 	
}

void swipe(int x, int y)
{
     if(abs(x - mouse_UPx) > 2  && mouseN == 2) {
             mouseN =0;
             SWIPE=1;			 
	 } else{
	        mouseN =0; 
	   }
}

void display()   
{
     float scaleR = 2.;	
	 int power=0;
	 int nspin=0;
	 
     glClear(GL_COLOR_BUFFER_BIT);
     glColor3f(1.0, 1.0, 1.0);
     glPushMatrix();
       myWireSphere(R, slices, stacks);  
     glPopMatrix();
     glFlush();
     glutSwapBuffers();  
	 			                      
     if(SWIPE == 1){ 
       nspin += 1;
       angg = angg*0.995;
       if(100*sqrt(spinX*spinX+spinY*spinY)- nspin*0.0001/ang >= 0){     
         if(phi > 0){
           power= int(phi/M_PI); // While phi > 180, logitude rotate reversly.
	       theta += scaleR*spinX * pow(-1., double(power))*angg;   // While phi > 180, logitude rotate reversly.           
         } else {
	          power= int(phi/M_PI); // While phi > 180, logitude rotate reversly.
	          theta += scaleR*spinX * pow(-1., double(power+1))*angg;   // While phi > 180, logitude rotate reversly.
	       }
		
	     phi -=scaleR* angg *spinY*0.; 
       }
	   
//for light rotating with sphere
       GLfloat light_position[] = { 1, sin(sunA), 0, 0 };
       glLightfv   (GL_LIGHT0,GL_POSITION,  light_position);
// end lighting--

     }
	 
     if(autor == 1){
        theta += ang; 
		sunA += syn*ang/10.;	
	    if (sunA >= 23.43*M_PI/180. || sunA <= -23.43*M_PI/180.)
		    syn = -1.*syn;		 		  		
//		 GLfloat light_position[] = { 1, sin(sunA), 0, 0 };		   
//         glLightfv(GL_LIGHT0, GL_POSITION, light_position);

 
	 }	
	 
	 		    
	 
	 glFlush();
     glutSwapBuffers();  	   	     
}
    
void init() {

     glClearColor(0., 0., 0., 1.0);
     glColor3f(1.0, 1.0, 1.0);

     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
   
//     texID = LoadTexture("earth.raw");
	 texID = LoadTexture("etop2.jpg");
//flip the image by x axis  
     glMatrixMode(GL_TEXTURE); 
     glLoadIdentity();
     glScalef(-1.0f, 1.0f, 1.0f);
     glMatrixMode(GL_MODELVIEW);
// end flip the image by x axis  

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(0., 0, 1, 40);
 
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     gluLookAt(5, sin(sunA), 0, 0, 0, 0, 0, 1, 0);
	 

//for lighting 
     GLfloat mat_specular  [] = { 1.0, 1.0, 1.0, 1.0 };
//     GLfloat mat_shininess [] = { 85.0 };
     GLfloat light_position[] = { 1, sin(sunA), 0., 0. };
     glClearColor               (0.0, 0.0, 0.0, 0.0);
     glShadeModel (GL_SMOOTH);

     glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular );
//     glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
     glLightfv   (GL_LIGHT0,GL_POSITION,  light_position);

     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
//end for lighting 

} 

void timer(int v) 
{     
	 float eyeX = (R+near) * cos(theta) * sin(phi);
     float eyeZ = (R+near) * sin(theta) * sin(phi);
     float eyeY = (R+near) * cos(phi); 	
	 float eyeUPx = cos(M_PI+theta) * cos(phi);
	 float eyeUPz = sin(M_PI+theta) * cos(phi);
	 float eyeUPy = sin(phi);
	
	  
	 glLoadIdentity(); 	         
     gluLookAt(eyeX, eyeY ,eyeZ, 0,0,0, eyeUPx,eyeUPy,eyeUPz);	
     glutPostRedisplay();
     glutTimerFunc(backtime, timer, v);	 
}


void mouse(int button, int state, int x, int y) 
{        
	 static int mouse_x1 = 0;
     static int mouse_y1 = 0; 	 
	        float xdis=0;                         //mouse moving distance x while button down;
            float ydis=0;                         //mouse moving distance y while button down;
	 static int  CountClicks=0;                   //for double click;  
	 
     switch (button) {
        case GLUT_LEFT_BUTTON:
          if (state == GLUT_DOWN){		    			 
			 mouse_x0 = x;    //for moving with mouse
			 mouse_y0 = y;	  //for moving with mouse			 
			 mouse_x1 = x;    //for swipe spin
			 mouse_y1 = y;	  //for swipe spin	 
			 		 
			 spinX=0;
             spinY=0; 
			 SWIPE=0;
			 autor = 0;	
			 mouseN=1;		
			 angg=ang;        //restore rotation velocity;
          }
		 
		  if (state == GLUT_UP){  
		     mouseN += 1;		 		 		 		  		  
		     mouse_UPx = x;
			 mouse_UPy = y;
		 	                                 
             xdis += x - mouse_x1;
             ydis += y - mouse_y1;
             		  
             if(xdis != 0 )
               spinX= int(xdis/abs(xdis))*abs(xdis)/sqrt(xdis*xdis+ydis*ydis);
             if(ydis != 0 )
               spinY= int(ydis/abs(ydis))*abs(ydis)/sqrt(xdis*xdis+ydis*ydis);
			      
			      
             if(xdis != 0 && abs(ydis)/abs(xdis) < tan(M_PI*10./180.))
			   spinY=0;                
			 
             if(ydis != 0 && abs(xdis)/abs(ydis) < tan(M_PI*10./180.))             
			   spinX=0;			 					 
            }			
          break;			  		  		  		      
		 
	      case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN){
			    mouse_x0 = x;
			    mouse_y0 = y;
				SWIPE=0;
            if(CountClicks++ == 2){
			    autor = 1;           
                CountClicks = 0; 
			}
			}
           break;	
		 	      
	       case 3: 
		     if(near < 39)        
		          near += 0.1;		
             break;	
	 
	       case 4: 
	         if(near >= 3.0)      
		        near -= 0.1;
		     else if(near >= 2.0) 
		        near=near*0.99;		   	
             break;	
		 	 	      
           default:
               break;		 
      }   
	             
      glutPostRedisplay();
}
 
 
void motion( int x, int y )
{   
     float phiM=1, thetaM=1;
     float xd,yd;
     int power=0;
	 

      xd=abs(x-mouse_x0) ;
      yd=abs(y-mouse_y0) ;
      
      thetaM= xd/sqrt(xd*xd+yd*yd);
      phiM= yd/sqrt(xd*xd+yd*yd) ;   
          
      if(yd/xd < tan(M_PI*10./180.))
          phiM=0;
      if(xd/yd < tan(M_PI*10./180.))
          thetaM=0;
         
      power= int(phi/M_PI); // While phi > 180, logitude rotate reversly
    
      if(phi > 0){   
         if (x > mouse_x0 )
	        theta += pow(-1., double(power))*ang*thetaM;     // While phi > 180, logitude rotate reversly.
	     else if (x < mouse_x0)
	        theta -= pow(-1., double(power))*ang*thetaM;     // While phi > 180, logitude rotate reversly.
	  }else {
	     if (x > mouse_x0 )
	        theta += pow(-1., double(power+1))*ang*thetaM;   // While phi > 180, logitude rotate reversly.
	     else if (x < mouse_x0)
	        theta -= pow(-1., double(power+1))*ang*thetaM;   // While phi > 180, logitude rotate reversly.	 
	  }  


	  if (y > mouse_y0)
	     phi -= ang*phiM;
	  else if (y < mouse_y0)
	     phi += ang*phiM; 
	
	  mouse_x0 = x;
	  mouse_y0 = y;  
	  
//for for light rotating with sphere
      GLfloat light_position[] = { 1, sin(sunA), 0, 0.0 };
      glLightfv   (GL_LIGHT0,GL_POSITION,  light_position);	 	
// end lighting-------------------	   
}


void reshape(GLint w, GLint h) 
{
     glViewport(0, 0, w, h);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(50.0, 1, 1.0, 40.0);
     glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
     switch (key) {
        case 27: // 27 is the code for the ESC key.
           exit(0);
		   break;
		case 113: // 113 is the code for the q key.
           exit(0);
           break;
     }
}

void special(int key, int, int) 
{
     switch (key) {    
        case GLUT_KEY_LEFT:  (theta -= ang   ) ; break;
        case GLUT_KEY_RIGHT: (theta += ang   ) ; break;
        case GLUT_KEY_UP:    (phi   += ang/2.) ; break;
        case GLUT_KEY_DOWN:  (phi   -= ang/2.) ; break;
        default: return;
     }
     glutPostRedisplay();
}


int   main(int argc, char** argv) 
{ 
      glutInit(&argc, argv);
      glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
      glutInitWindowSize(800, 800);  //800*6353/6384);
      glutCreateWindow("Earth");
      init();
      glutDisplayFunc(display);
      glutReshapeFunc(reshape);  
      glutTimerFunc(backtime, timer, 0);  
      glutKeyboardFunc(keyboard);
      glutSpecialFunc(special);
      glutMouseFunc(mouse);
      glutMotionFunc(motion);
      glutPassiveMotionFunc(swipe);
      glutDisplayFunc(display);
//    glEnable(GL_DEPTH_TEST);
      glutMainLoop();
      return 0;
}

GLuint LoadTexture( const char * filename )
{
      GLuint texture;
   
/*//-----for Raw image file------   
      int width, height;
	  int i=0;
      
//    width = 1024;
//    height = 512; 
      ifstream myfile;	
	  myfile.open(filename, ios::in|ios::binary|ios::ate);
	  ifstream::pos_type myfilesize = myfile.tellg();
	  myfile.seekg(0,ios::beg);
	  char * memblock = new char [myfilesize];
	  myfile.read(memblock, myfilesize);
	  myfile.close();
    
	  while(pow(2, ++i) < myfilesize);
	
//    cout << "i= "<< i << "\n";
	  height = pow(2, (i-1)/2-1);
	  width  = pow(2, (i-1)/2); 
	
//	  cout << "width= "<< width << "\n";
//	  cout << "height= "<< height << "\n";

      glGenTextures( 1, &texture );     
      glBindTexture( GL_TEXTURE_2D, texture );
    
      glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 
    
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,     GL_LINEAR );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );    
      gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,  GL_RGB, GL_UNSIGNED_BYTE, memblock );
        
      delete[] memblock;	
*/// ---end for Raw image file --	

  
//   for JPEG image file --use SDL libery
	 SDL_Surface* Surface = IMG_Load(filename);	 
     glGenTextures(1, &texture);
     glBindTexture(GL_TEXTURE_2D, texture);
 
     int Mode = GL_RGB; 
     if(Surface->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
     }
 
     glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels); 
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
//   end for JPEG image file--
	
    return texture;
   
}
