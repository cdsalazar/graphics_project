/*
 *  Projections
 *
 *  Draw 27 cubes to demonstrate orthogonal & prespective projections
 *
 *  Key bindings:
 *  m          Toggle between perspective and orthogonal
 *  p          Toggle movement on/off
 *  +/-        Changes field of view for perspective
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdarg.h>
 #include <math.h>
 #include "CSCIx229.h"
 //  OpenGL with prototypes for glext
 #define GL_GLEXT_PROTOTYPES
 #ifdef __APPLE__
 #include <GLUT/glut.h>
 #else
 #include <GL/glut.h>
 #endif

int axes=0;       //  Display axes
int mode=1;
int gc_move=0;      //  Projection mode
int ntex=1;       //  Cube faces
// double zh=0;
int th=-250;         //  Azimuth of view angle
int ph=35;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=28.5;   //  Size of world
double idle = 0.0;
int light=1;

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   20;  // Elevation of light
unsigned int texture[7]; // Texture names

float fpnx, fpny, fpnz;   // Camera position
float dirx, diry, dirz; // Camera direction
float fpn_ang, fpn_p; // Rotation angles
float orth_x, orth_z; // Orthogonal angles

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

//Draw a Skyscraper
static void draw_skyscraper(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  glPushMatrix();
  glTranslated(x,y+12.5,z);
  glRotated(90,100,1,0);
  glScaled(10*dx,10*dy,10*dz);
  glColor3f(0.196078,0.6,0.8);
  GLUquadric* qobj;
  qobj = gluNewQuadric();
  gluQuadricNormals(qobj, GLU_SMOOTH);
  gluCylinder(qobj, 0.5, 1, 5, 20000, 16);
  glPopMatrix();
  glPushMatrix();
  glColor3f( 0.6,0.196078,0.8);
  glTranslated(x,y+12.5,z);
  glRotated(90,100,1,0);
  glScaled(4*dx,4*dy,4*dz);
  glutSolidTorus(1.0, 2.0,100,100);
  glPopMatrix();

  glPushMatrix();
  glColor3f( 0.6,0.196078,0.8);
  glTranslated(x,y+14,z);
  glRotated(90,100,1,0);
  glScaled(3*dx,3*dy,3*dz);
  glutSolidTorus(1.0, 2.0,100,100);
  glPopMatrix();

  glPushMatrix();
  glColor3f( 0.6,0.196078,0.8);
  glTranslated(x,y+15,z);
  glRotated(90,100,1,0);
  glScaled(2*dx,2*dy,2*dz);
  glutSolidTorus(1.0, 2.0,100,100);
  glPopMatrix();

  glPushMatrix();
  glColor3f( 0.6,0.196078,0.8);
  glTranslated(x,y+15.75,z);
  glRotated(90,100,1,0);
  glScaled(dx,dy,dz);
  glutSolidTorus(1.0, 2.0,100,100);
  glPopMatrix();



}

//Draw a arch building
static void draw_arch_building(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  glPushMatrix();
  glTranslated(x,y+1.2,z+0.1);
  glRotated(180,0,1,-100);
  glScaled(5*dx,12*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, -1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslated(x,y+2.2,z+1.5);
  glRotated(180,0,1,-100);
  glScaled(5*dx,15*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, 1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x,y+9.2,z+1.4);
  glRotated(180,0,1,-100);
  glScaled(5*dx,15*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslated(x,y+9.25,z-1.3);
  glRotated(180,0,1,-100);
  glScaled(5*dx,15*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  glEnd();

  glPopMatrix();
  glPushMatrix();
  glTranslated(x,y+11.2,z+1.7);
  glRotated(180,0,1,-100);
  glScaled(5*dx,15*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslated(x,y+11.25,z-1.1);
  glRotated(180,0,1,-100);
  glScaled(5*dx,15*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslated(x,y+11.25,z-2.6);
  glRotated(180,0,1,-100);
  glScaled(5*dx,15*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x+0.7,y+2.2,z+2.3);
  glRotated(180,0,1,-100);
  glScaled(2.5*dx,15*dy,2.5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( -1, 0, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslated(x-2.2,y+2.2,z+2.3);
  glRotated(180,0,1,-100);
  glScaled(2.5*dx,15*dy,2.5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( +1, 0, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x,y+2.3,z-5.85);
  glRotated(180,0,1,-100);
  glScaled(5*dx,15*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, -1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslated(x,y+1.3,z-4.4);
  glRotated(180,0,1,-100);
  glScaled(5*dx,12*dy,5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, 1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x+0.7,y+2.3,z-3.6);
  glRotated(180,0,1,-100);
  glScaled(2.5*dx,15*dy,2.5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( -1, 0, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslated(x-2.2,y+2.3,z-3.6);
  glRotated(180,0,1,-100);
  glScaled(2.5*dx,15*dy,2.5*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 1, 0, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x-1.5,y+5.75,z);
  glRotated(180,0,1,-100);
  glScaled(10*dx,3.5*dy,10*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( -1, 0, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslated(x-4.4,y+5.75,z);
  glRotated(180,0,1,-100);
  glScaled(10*dx,3.5*dy,10*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 1, 0, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  glEnd();
  glPopMatrix();


}


//Draw a street lamp
static void draw_lamp(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  int th1, ph1;
  //Lamp post
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(90,100,1,0);
  glScaled(10*dx,10*dy,10*dz);
  glColor3f(0.329412,0.329412,0.329412);
  GLUquadric* qobj;
  qobj = gluNewQuadric();
  gluQuadricNormals(qobj, GLU_SMOOTH);
  gluCylinder(qobj, 0.01, 0.04, 0.7, 20000, 16);
  glPopMatrix();
  //Light source TODO: Make it a source of light
  glPushMatrix();
  glColor3f(0.29, 0.46, 0.43);
  glTranslated(x,y,z);
  glRotated(90,100,1,0);
  glScaled(0.2*dx,0.2*dy,0.2*dz);
  glutSolidTorus(1.0, 2.0,100,100);
  glPopMatrix();

  glPushMatrix();
  glColor3f(1,1,1);
  glTranslated(x,y,z);
  glRotated(90,100,1,0);
  glScaled(0.2*dx,0.2*dy,0.2*dz);
  for (ph1=-90;ph1<90;ph1+=inc)
  {
     glBegin(GL_QUAD_STRIP);
     for (th1=0;th1<=360;th1+=2*inc)
     {
        Vertex(th1,ph1);
        Vertex(th1,ph1+inc);
     }
     glEnd();
   }
  glPopMatrix();
}

//Draw a streetlight
static void draw_streetlights(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  int th1,ph1;
  //First pole
  glPushMatrix();
  if(th == 5) glTranslated(x+th-0.4,y+1,z+th);
  else glTranslated(x+th,y+1,z+th);
  glRotated(90,100,1,0);
  glScaled(10*dx,10*dy,10*dz);
  glColor3f(0.752941, 0.752941, 0.752941);
  GLUquadric* qobj;
  qobj = gluNewQuadric();
  gluQuadricNormals(qobj, GLU_SMOOTH);
  gluCylinder(qobj, 0.02, 0.02, 1, 20000, 16);
  glPopMatrix();
  glPushMatrix();
  if(th == 5) glTranslated(x+th-0.4,y+1,z+th);
  else glTranslated(x+th,y+1,z+th);
  glRotated(90,100,1,0);
  glScaled(0.2*dx,0.2*dy,0.2*dz);
  for (ph1=-90;ph1<90;ph1+=inc)
  {
     glBegin(GL_QUAD_STRIP);
     for (th1=0;th1<=360;th1+=2*inc)
     {
        Vertex(th1,ph1);
        Vertex(th1,ph1+inc);
     }
     glEnd();
  }
  glPopMatrix();

  //Second pole
  glPushMatrix();
  if(th == 5) glTranslated(x+5-0.4,y+1,z);
  else glTranslated(x+5,y+1,z);
  glRotated(90,100,1,0);
  glScaled(10*dx,10*dy,10*dz);
  glColor3f(0.752941, 0.752941, 0.752941);
  gluQuadricNormals(qobj, GLU_SMOOTH);
  gluCylinder(qobj, 0.02, 0.02, 1, 20000, 16);
  glPopMatrix();
  glPushMatrix();
  if(th == 5) glTranslated(x+5-0.4,y+1,z);
  else glTranslated(x+5,y+1,z);
  glRotated(90,100,1,0);
  glScaled(0.2*dx,0.2*dy,0.2*dz);
  for (ph1=-90;ph1<90;ph1+=inc)
  {
     glBegin(GL_QUAD_STRIP);
     for (th1=0;th1<=360;th1+=2*inc)
     {
        Vertex(th1,ph1);
        Vertex(th1,ph1+inc);
     }
     glEnd();
  }
  glPopMatrix();

  //cable
  glPushMatrix();
  if(th == 5) glTranslated(x+5-0.4,y+1,z);
  else glTranslated(x+5,y+1,z);
  glRotated(180,100,1,-100);
  if(th == 5)
    glRotated(180,100,1,-100);
  glScaled(10*dx,10*dy,10*dz);
  glColor3f(0,0,0);
  gluQuadricNormals(qobj, GLU_SMOOTH);
  gluCylinder(qobj, 0.007, 0.007, 1.65, 20000, 16);
  glPopMatrix();

  //draw light 1
  int offset = 3.999999999999999;
  glPushMatrix();
  if(th == 5)  glTranslated(x+1.75+offset,y+0.8,z+offset);
  else glTranslated(x+1.75,y+0.8,z);
  glRotated(180,100,1,-100);
  glScaled(0.25*dx,0.5*dy,0.25*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( -1, 0, 0);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  if(th == 5) glTranslated(x+1.6+offset,y+0.8,z+offset);
  else glTranslated(x+1.6,y+0.8,z);
  glRotated(180,0,1,-100);
  glScaled(0.25*dx,0.5*dy,0.25*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, -1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  if(th == 5) glTranslated(x+1.6+offset,y+0.8,z+offset);
  else glTranslated(x+1.6,y+0.8,z);
  glRotated(180,100,1,-100);
  glScaled(0.25*dx,0.5*dy,0.25*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 1, 0, 0);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  if(th == 5) glTranslated(x+1.6+offset,y+0.8,z-0.15+offset);
  else glTranslated(x+1.6,y+0.8,z-0.15);
  glRotated(180,0,1,-100);
  glScaled(0.25*dx,0.5*dy,0.25*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, 1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  //draw light 2
  offset = 1.9;
  glPushMatrix();
  if(th == 5) glTranslated(x+3.75+offset,y+0.8,z+offset);
  else glTranslated(x+3.65,y+0.8,z);
  glRotated(180,100,1,-100);
  glScaled(0.25*dx,0.5*dy,0.25*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( -1, 0, 0);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  if(th == 5) glTranslated(x+3.6+offset,y+0.8,z+offset);
  else glTranslated(x+3.5,y+0.8,z);
  glRotated(180,0,1,-100);
  glScaled(0.25*dx,0.5*dy,0.25*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, -1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  if(th ==5) glTranslated(x+3.6+offset,y+0.8,z+offset);
  else glTranslated(x+3.5,y+0.8,z);
  glRotated(180,100,1,-100);
  glScaled(0.25*dx,0.5*dy,0.25*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 1, 0, 0);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  if(th == 5) glTranslated(x+3.6+offset,y+0.8,z-0.15+offset);
  else glTranslated(x+3.5,y+0.8,z-0.15);
  glRotated(180,0,1,-100);
  glScaled(0.25*dx,0.5*dy,0.25*dz);
  glColor3f(0.560784,0.560784,0.737255);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, 1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

}


//City frame
static void city_frame(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  //  Draw disc
  int i;
  glPushMatrix();

  glTranslated(x,y-2.4,z);
  glRotated(th,100,1,0);
  glScaled(10*dx,10*dy,10*dz);

  glLineWidth(100);
  glColor3f(0.137255,0.556863,0.137255);
  if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_TRIANGLE_FAN);
  for(i =0; i <= 360; i++){
    int t = i % 2;
    double angle = 2* 3.14159 * i / 360;
    double x = cos(angle)*15;
    double y = sin(angle)*15;
    glNormal3f( x, y, 1);
    glTexCoord2f(t,t); glVertex2d(x,y);
  }
  glDisable(GL_TEXTURE_2D);
  glEnd();
  glPopMatrix();


}

/*
 *  Draw a ground
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void draw_ground(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Draw ground
   glPushMatrix();
    glTranslated(x,y-5,z);
    glRotated(180,100,1,0);
    glScaled(10*dx,10*dy,10*dz);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    glColor3f(0.137255,0.556863,0.137255);
    if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glNormal3f( 0,+1, 0);
    double ground_size = 2.5;
    glTexCoord2f(0,0); glVertex3f(ground_size,-1.0,ground_size);
    glTexCoord2f(1,0); glVertex3f(-ground_size,-1.0,ground_size);
    glTexCoord2f(1,1); glVertex3f(-ground_size,-1.0,-ground_size);
    glTexCoord2f(0,1); glVertex3f(ground_size,-1.0,-ground_size);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnd();

    //Second Block
    glPushMatrix();
     glTranslated(x-14,y-5.3,z);
     glRotated(180,100,1,0);
     glScaled(10*dx,10*dy,10*dz);

     glEnable(GL_TEXTURE_2D);
     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
     glColor3f(0.137255,0.556863,0.137255);
     if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
     glBegin(GL_POLYGON);
     glNormal3f( 0,+1, 0);
     glTexCoord2f(0,0); glVertex3f(ground_size,-1.0,ground_size);
     glTexCoord2f(1,0); glVertex3f(-ground_size,-1.0,ground_size);
     glTexCoord2f(1,1); glVertex3f(-ground_size,-1.0,-ground_size);
     glTexCoord2f(0,1); glVertex3f(ground_size,-1.0,-ground_size);
     glEnd();
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);
     glEnd();

     //Third Block
     glPushMatrix();
      glTranslated(x,y-5,z-14);
      glRotated(180,100,1,0);
      glScaled(10*dx,10*dy,10*dz);

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
      glColor3f(0.137255,0.556863,0.137255);
      if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
      glBegin(GL_POLYGON);
      glNormal3f( 0,+1, 0);
      glTexCoord2f(0,0); glVertex3f(ground_size,-1.0,ground_size);
      glTexCoord2f(1,0); glVertex3f(-ground_size,-1.0,ground_size);
      glTexCoord2f(1,1); glVertex3f(-ground_size,-1.0,-ground_size);
      glTexCoord2f(0,1); glVertex3f(ground_size,-1.0,-ground_size);
      glEnd();
      glPopMatrix();
      glDisable(GL_TEXTURE_2D);
      glEnd();

      //Fourth Block
      glPushMatrix();
       glTranslated(x-14,y-5.3,z-14);
       glRotated(180,100,1,0);
       glScaled(10*dx,10*dy,10*dz);

       glEnable(GL_TEXTURE_2D);
       glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
       glColor3f(0.137255,0.556863,0.137255);
       if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
       glBegin(GL_POLYGON);
       glNormal3f( 0,+1, 0);
       glTexCoord2f(0,0); glVertex3f(ground_size,-1.0,ground_size);
       glTexCoord2f(1,0); glVertex3f(-ground_size,-1.0,ground_size);
       glTexCoord2f(1,1); glVertex3f(-ground_size,-1.0,-ground_size);
       glTexCoord2f(0,1); glVertex3f(ground_size,-1.0,-ground_size);
       glEnd();
       glPopMatrix();
       glDisable(GL_TEXTURE_2D);
       glEnd();


}


static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
  const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode == 1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  First Person Nav
   else{
     dirx = cos(fpn_ang) * cos(fpn_p);
     diry = sin(fpn_p);
     dirz = sin(fpn_ang) * cos(fpn_p);

     orth_x = cos(fpn_ang - M_PI_2);
     orth_z = sin(fpn_ang - M_PI_2);

     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     gluLookAt(fpnx,fpny,fpnz, fpnx+dirx,fpny+diry,fpnz+dirz, 0.0,1.0,0.0);
   }
   //city frame
   city_frame(1,1,1, 0.3,0.3,0.3 , 90);

   //  Draw city foundation
   int stoplight_offset = 0;
   draw_ground(1,1,1, 0.3,0.3,0.3 , 90);
   draw_ground(15,1.3,1, 0.3,0.3,0.3 , 90);
   draw_ground(1,1,15, 0.3,0.3,0.3 , 90);
   draw_ground(15,1.3,15, 0.3,0.3,0.3 , 90);

   //Draw street lights
   draw_streetlights(-2,1,-1.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-2,1,3.75, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-2,1,-10.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-2,1,-15.75, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-2,1,12.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-2,1,17.75, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12,1,-1.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12,1,3.75, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12,1,-10.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12,1,-15.75, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12,1,12.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12,1,17.75, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-16,.5,-1.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-16,.5,3.75, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-16,.5,-10.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-16,.5,-15.75, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-16,.5,12.5, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-16,.5,17.75, 0.3,0.3,0.3 ,stoplight_offset);

   stoplight_offset = 5;
   draw_streetlights(-1.6,1,-1.375, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-6.6,1,-1.375, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-15.6,.5,-1.375, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-20.6,.5,-1.375, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(7.4,1,-1.375, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12.4,1,-1.375, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-1.6,1,12.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-6.6,1,12.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-1.6,1,-15.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-6.6,1,-15.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-15.6,.5,-15.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-20.6,.5,-15.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-15.6,.5,12.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(-20.6,.5,12.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(7.4,1,-15.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12.4,1,-15.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(7.4,1,12.6, 0.3,0.3,0.3 ,stoplight_offset);
   draw_streetlights(12.4,1,12.6, 0.3,0.3,0.3 ,stoplight_offset);

   //Draw Street Lamps
   draw_lamp(7.5,1,12.5, 0.3,0.3,0.3 , 90);
   draw_lamp(7.5,1,17.5, 0.3,0.3,0.3 , 90);
   draw_lamp(7.5,1,-1.25, 0.3,0.3,0.3 , 90);
   draw_lamp(7.5,1,3.5, 0.3,0.3,0.3 , 90);
   draw_lamp(7.5,1,-15.4, 0.3,0.3,0.3 , 90);
   draw_lamp(7.5,1,-10.65, 0.3,0.3,0.3 , 90);
   draw_lamp(-6.5,1,-10.65, 0.3,0.3,0.3 , 90);
   draw_lamp(-6.5,1,-15.4, 0.3,0.3,0.3 , 90);
   draw_lamp(-6.5,1,12.5, 0.3,0.3,0.3 , 90);
   draw_lamp(-6.5,1,17.5, 0.3,0.3,0.3 , 90);
   draw_lamp(-6.5,1,-1.25, 0.3,0.3,0.3 , 90);
   draw_lamp(-6.5,1,3.5, 0.3,0.3,0.3 , 90);

   //Draw Arch building
  draw_arch_building(5,1,1.75, 0.3,0.3,0.3 , 90);
  draw_arch_building(5,1,-4, 0.3,0.3,0.3 , 90);
  draw_arch_building(5,10,-1.25, 0.3,0.3,0.3 , 90);

  //Draw a Skyscraper
  draw_skyscraper(-5.2,1,-5, 0.3,0.3,0.3 , 90);


   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {20*distance*Cos(zh),ylight,20*distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);
   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
     glColor3f(0,0,0);
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   if (mode == 1)
   {
     Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,"Perpective");
   }
   //  First Person
   else if(mode == 0){
     Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,"First Person");
   }
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
  float c_angle = .05;
  float inc = .05;
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0'){
      th = -45;
      ph = -345;
    }
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   else if (ch == 'p' || ch == 'P')
      gc_move = (gc_move+1)%2;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='t' && ambient>0)
      ambient -= 5;
   else if (ch=='T' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='h' && diffuse>0)
      diffuse -= 5;
   else if (ch=='H' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='g' && specular>0)
      specular -= 5;
   else if (ch=='G' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //First Person Nav
   if(mode == 0){
        if(ch == 'w' || ch == 'W') {
           float lx = cos(fpn_ang)*cos(fpn_p);
           float ly = sin(fpn_p);
           float lz = sin(fpn_ang)*cos(fpn_p);
           fpnx = fpnx + inc*lx;
           fpny = fpny + inc*ly;
           fpnz = fpnz + inc*lz;
        }
        else if(ch == 's' || ch == 'S') {
           float c_x = cos(fpn_ang)*cos(fpn_p);
           float c_y = sin(fpn_p);
           float c_z = sin(fpn_ang)*cos(fpn_p);
           fpnx = fpnx - inc*c_x;
           fpny = fpny - inc*c_y;
           fpnz = fpnz - inc*c_z;
        }
        else if(ch == 'a' || ch == 'A'){
           fpn_ang -= c_angle;
        }
        else if(ch == 'd' || ch == 'D'){
           fpn_ang += c_angle;
        }

   }
   //  Reproject
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(45,asp,dim);
}

void idle_function()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Future City");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle_function);
   //  Load textures
   texture[0] = LoadTexBMP("textures/central_block.bmp");
   texture[1] = LoadTexBMP("textures/outide_grass.bmp");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
