/* cosc363 assignment1
   Student name :Heyang Li
   Student ID: 46522791
   starting time: 15-Mar-2017
   task: to make a simple animation like clash royale
   basic requirment:  
				1) one mobile robot(done)
				2) robot needs to perform an action (done)
				3) at three other composite objects(done)
				4) at least two light sources in the scene(done)
				5) at least two objects must have specular refections(done)
				6) move camera(done)
				7) three different textures in the scene(done)
   extra features:
				1) planar shadows (done)
				2) physics model(done)
				3) custom-built model designed using vertex coordinates(done)
				4) collision dection(done)
*/

/*
 * 16-Mar-2017storage
 * finished background
 * finished bridge and river 
 */ 
 
 /*
  * 17-Mar-2017
  * construct castle and cannon
  */ 
  
 /* 2-Apr-2017
  * finished skybox
  * minor modified cannon
  * construct robot and crown
  */
  
 /* 3-Apr-2017
  * construct boundary
  * modified castle
  * fire cannon mode done (need to be modified later)
  * minor bug found during the fire cannon stage
  * lighting done
  */
  
 /* 4-Apr-2017
  * fixed minor bug (camera position when fire)
  * robot move done
  * finish army
  * minor bug on fire and need king near the cannon
  * half way done planar shadows(bug need to be fixed)
  */
				
 /* 5-Apr-2017
  * modified fire cannon
  * fixed the planar shadows bug
  */
  
#include <iostream>
#include <fstream>
#include <climits>
#include <math.h> 
#include <GL/freeglut.h>
#include "loadTGA.h"


//--Globals ---------------------------------------------------------------
float *x, *y, *z;  //vertex coordinate arrays
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles
float ball = 0.0f;
float angle = 90;
float theta = 0.0f;
int fire = 0;
int option = 0;
float r_theta = 20.0f;
float robot_move = 0;
bool arm_army = true;
bool forward_body = true; //the left leg is moving forward
//float eye_x = 0,  eye_y = 60,  eye_z = 100; 	 // a vision to see the cannon ball
float eye_x = 50,  eye_y = 160,  eye_z = 100;    //Initial camera position
float look_x = 12, look_y = 1, look_z = 10;    //"Look-at" point along -z direction
GLuint txId[7];  //Texture ID
int step = 0;
int armMove = 0;
int robotMoveCounter = 0;
int cam_flag = 1;



//-------------------------Fun start here-----------------------------//



void loadTexture()				
{
	glGenTextures(8, txId); 				// Create two Texture objects
	glBindTexture(GL_TEXTURE_2D, txId[0]);		//Use this texture
    loadTGA("river.tga"); //water image in here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
	loadTGA("bridge.tga"); //bridge image in here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[2]);		//Use this texture
    loadTGA("brick.tga"); //tower image in here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	glBindTexture(GL_TEXTURE_2D, txId[3]);		//Use this texture
    loadTGA("wp1.tga"); //wallpaper back
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[4]);		//Use this texture
    loadTGA("wp2.tga"); //wallpaper right
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[5]);		//Use this texture
    loadTGA("wp3.tga"); //wallpaper right
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[6]);		//Use this texture
    loadTGA("wp4.tga"); //wallpaper right
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
	

void drawFloor()
{
	bool flag = false;
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for(int x = -350; x <= 350; x += 50)
	{
		for(int z = -350; z <= -50; z += 50)
		{
			if(flag) glColor3f(0.0 ,1.0 ,0);
			else glColor3f(0.0 ,1 ,0.5 );
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z + 50);
			glVertex3f(x + 50, 0, z + 50);
			glVertex3f(x + 50, 0, z);
			flag = !flag;
		}
	}
	
	for(int x = -350; x <= 350; x += 50)
	{
		for(int z = 38; z <= 350; z += 50)
		{
			if(flag) glColor3f(0.0 ,1.0 ,0);
			else glColor3f(0.0 ,1 ,0.5 );
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z + 50);
			glVertex3f(x + 50, 0, z + 50);
			glVertex3f(x + 50, 0, z);
			flag = !flag;
		}
	}
	glEnd();
}

void drawRiver(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glBegin(GL_QUADS);
	
	glNormal3f(0.0, 1.0, 0.0); // +y top side
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-350.0, 0.1, 40);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(350.0, 0.1, 40);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(350.0, 0.1, -40);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(-350.0, 0.1, -40);
	
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawBridge(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glBegin(GL_QUADS);

//bridge above	
	glNormal3f(0.0, 1.0, 0.0);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 20, -35);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(-25.0, 20, 35);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 20, 35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(25.0, 20, -35);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 0, -50);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(-25.0, 0, -50);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 20, -35);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(25.0, 20, -35);
	
	glNormal3f(0.0, 1.0, .0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 20, 35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(-25.0, 20, 35);
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 0, 50);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(25.0, 0, 50);

// bridge below	
	glNormal3f(0.0, 1.0, .0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 15, -35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(25.0, 0, -45);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 0, -45);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(-25.0, 15, -35);
	
	glNormal3f(0.0, 1.0, .0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 15, 35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(-25.0, 15, 35);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 0, 45);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(25.0, 0, 45);
	
	glNormal3f(0.0, 1.0, .0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 15, -35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(-25.0, 15, -35);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 15, 35);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(25.0, 15, 35);
	
//bridge righs side
	glNormal3f(0.0, .0, 1.0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 0, -45);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(25.0, 0, -50);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(25.0, 20, -35);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(25.0, 15, -35);
	
	glNormal3f(0.0, .0, 1.0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 15, -35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(25.0, 20, -35);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(25.0, 20, 35);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(25.0, 15, 35);
	
	glNormal3f(0.0, .0, 1.0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(25.0, 20, 35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(25.0, 15, 35);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(25.0, 0, 45);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(25.0, 0, 50);
//bridge left side	
	glNormal3f(0.0, .0, 1.0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(-25.0, 0, -45);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(-25.0, 0, -50);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 20, -35);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(-25.0, 15, -35);
	
	glNormal3f(0.0, .0, 1.0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(-25.0, 15, -35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(-25.0, 20, -35);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 20, 35);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(-25.0, 15, 35);
	
	glNormal3f(0.0, .0, 1.0);
	
	glTexCoord2f(1, 0.55);
	glVertex3f(-25.0, 20, 35);
	
	glTexCoord2f(1, 0.1);
	glVertex3f(-25.0, 15, 35);
	
	glTexCoord2f(0, 0.1);
	glVertex3f(-25.0, 0, 45);
	
	glTexCoord2f(0, 0.55);
	glVertex3f(-25.0, 0, 50);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
	}
	
void drawCastle(){
	
		
	glColor3f(.4, 0, 0);
	
	for (int i = -330; i <= 330; i = i + 20){
		glPushMatrix();
		glTranslatef(i, 30, -179);
		glutSolidCube(10);
		glPopMatrix();}
		
	for (int i = -330; i <= 110; i = i + 20){
		glPushMatrix();
		glTranslatef(i, 30, -142);
		glutSolidCube(10);
		glPopMatrix();}
		
	for (int i = 140; i <= 330; i = i + 20){
		glPushMatrix();
		glTranslatef(i, 30, -142);
		glutSolidCube(10);
		glPopMatrix();}
		
		glPushMatrix();
		glTranslatef(-330, 30, -160);
		glutSolidCube(10);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(330, 30, -160);
		glutSolidCube(10);
		glPopMatrix();
		
		
		
		glColor3f(0.3, 0.3, 0.4);		
		glPushMatrix();	
		glTranslatef(0, 15, -160);
		glScalef(14, 0.5, 1);
		glutSolidCube(50);
		glPopMatrix();
		
	}

void loadMeshFile(const char* fname)  
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

	for(int i=0; i < nvrt; i++)                         //read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];

	for(int i=0; i < ntri; i++)                         //read polygon list 
	{
		fp_in >> num >> t1[i] >> t2[i] >> t3[i];
		if(num != 3)
		{
			cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
			exit(1);
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}


void normal(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}

void cannonball(){
	glPushMatrix();
	glTranslatef(37 +  100 * ball, 64 -  
	2 * ball * ball + 40 * ball, -1);
	glutSolidSphere(5, 36, 18);
	glPopMatrix();
	}
	
void Cannon()
{
	glColor3f(0.4, 0.5, 0.4);

    //Construct the object model here using triangles read from OFF file
	glBegin(GL_TRIANGLES);
		for(int tindx = 0; tindx < ntri; tindx++)
		{
		   normal(tindx);
		   glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
		   glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
		   glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
		}
	glEnd();
}

void drawCannon(){
	
	glPushMatrix();
	glTranslatef(-20, 30, 0);
	glRotatef(30, 0, 0, 1);
	glTranslatef(20, -30, 0);
    Cannon();
    glPopMatrix();
    
    
	glColor3ub(0, 70, 255);
    glPushMatrix();
    glTranslatef(-10, 5, 17);
    glScalef(80, 10, 6);
	glutSolidCube(1.0);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-20, 25, 17);
	glScalef(40, 30, 6);
	glutSolidCube(1);
	glPopMatrix();
	
    glPushMatrix();
    glTranslatef(-10, 5, -17);
    glScalef(80, 10, 6);
	glutSolidCube(1.0);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-20, 25, -17);
	glScalef(40, 30, 6);
	glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();
	cannonball();
    glPopMatrix();

	}
	
void drawCannon2(){
	glPushMatrix();
	glTranslatef(0, 27, -140);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.2, 0.2);
	drawCannon();
	glPopMatrix();
	}

void drawCrown(){
	
	glColor3f(0.,0.,0.7);
	for (int i = -1; i < 2; i++){
    glPushMatrix();
	glRotatef(-30 * i, 0., 1., 0.);
	glTranslatef(0, .0, .8);
	glPushMatrix();
		glRotatef(-90,1,0,0);
		glutSolidCone(0.3,1.5,100,100);
	glPopMatrix();
	glPopMatrix();
	}
	
	glColor3f(0,0,0);
	glPushMatrix();
	glTranslatef(0,1.5,.8);
	glutSolidSphere(.1,100,100);
	glPopMatrix();
}

void drawRobot(){
	
	glColor3f(1., 0.78, 0.06);		//Head
	glPushMatrix();
	  glTranslatef(0, 7.7, 0);
	  glutSolidCube(1.4);
	glPopMatrix();
	
	glColor3f(1, 0.2, 0.5); //eyes
	glPushMatrix();
	glTranslatef(0.2, 7.8, .7);
	glutSolidCube(0.1);
	glPopMatrix();
	
	
	
	glColor3f(1, 0.2, 0.5);
	glPushMatrix();
	glTranslatef(-0.2, 7.8, .7);
	glutSolidCube(0.1);
	glPopMatrix();
	
	glColor3f(1, 0.2, 0.5); //mouth
	glPushMatrix();
	glScalef(3,1,1);
	glTranslatef(0, 7.3, .7);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3f(1., 0., 0.);			//Torso
	glPushMatrix();
	  glTranslatef(0, 5.5, 0);
	  glScalef(3, 3, 1.4);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right leg
	glPushMatrix();
	  glTranslatef(-0.8, 2.2, 0);
	  glRotatef(r_theta, -1, 0, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Left leg
	glPushMatrix();
	  glTranslatef(0.8, 4, 0);
	  glRotatef(r_theta, 1, 0, 0);
	  glTranslatef(-0.8, -4, 0);
	  glTranslatef(0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right arm
	glPushMatrix();
	  glTranslatef(-2, 6.5, 0);
	  glRotatef(r_theta, -1, 0, 0);
	  glTranslatef(2, -6.5, 0);
	  glTranslatef(-2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Left arm
	glPushMatrix();
	  glTranslatef(2, 5, 0);
	  glRotatef(r_theta, 1, 0, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();
	}
	
void drawRobot3(){
	glColor3f(1., 0.78, 0.06);		//Head
	glPushMatrix();
	  glTranslatef(0, 7.7, 0);
	  glutSolidCube(1.4);
	glPopMatrix();
	
	glColor3f(1, 0.2, 0.5); //eyes
	glPushMatrix();
	glTranslatef(0.2, 7.8, .7);
	glutSolidCube(0.1);
	glPopMatrix();
	
	glColor3f(1, 0.2, 0.5);
	glPushMatrix();
	glTranslatef(-0.2, 7.8, .7);
	glutSolidCube(0.1);
	glPopMatrix();
	
	glColor3f(1, 0.2, 0.5); //mouth
	glPushMatrix();
	glScalef(3,1,1);
	glTranslatef(0, 7.3, .7);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3f(1., 0., 0.);			//Torso
	glPushMatrix();
	  glTranslatef(0, 5.5, 0);
	  glScalef(3, 3, 1.4);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right leg
	glPushMatrix();
	  glTranslatef(-0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Left leg
	glPushMatrix();
	  glTranslatef(0.8, 4, 0);
	  glTranslatef(-0.8, -4, 0);
	  glTranslatef(0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right arm
	glPushMatrix();
	  glTranslatef(-2, 6.5, armMove);
	  glRotatef(-90,1,0,0);
	  glTranslatef(2, -6.5, 0);
	  glTranslatef(-2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Left arm
	glPushMatrix();
	glTranslatef(0, 6.5, 4 - armMove);
	glRotatef(-90,1,0,0);
	  glTranslatef(2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();
	}
	
void shadowRobot(){
			//Head
	glPushMatrix();
	  glTranslatef(0, 7.7, 0);
	  glutSolidCube(1.4);
	glPopMatrix();
	
	//eyes
	glPushMatrix();
	glTranslatef(0.2, 7.8, .7);
	glutSolidCube(0.1);
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslatef(-0.2, 7.8, .7);
	glutSolidCube(0.1);
	glPopMatrix();
	
	 //mouth
	glPushMatrix();
	glScalef(3,1,1);
	glTranslatef(0, 7.3, .7);
	glutSolidCube(0.1);
	glPopMatrix();

				//Torso
	glPushMatrix();
	  glTranslatef(0, 5.5, 0);
	  glScalef(3, 3, 1.4);
	  glutSolidCube(1);
	glPopMatrix();

				//Right leg
	glPushMatrix();
	  glTranslatef(-0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

				//Left leg
	glPushMatrix();
	  glTranslatef(0.8, 4, 0);
	  glTranslatef(-0.8, -4, 0);
	  glTranslatef(0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

			//Right arm
	glPushMatrix();
	  glTranslatef(-2, 6.5, armMove);
	  glRotatef(-90,1,0,0);
	  glTranslatef(2, -6.5, 0);
	  glTranslatef(-2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();

			//Left arm
	glPushMatrix();
	glTranslatef(0, 6.5, 4 - armMove);
	glRotatef(-90,1,0,0);
	  glTranslatef(2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();
	}
	
void drawRobot2(){
	glPushMatrix();
	glScalef(2, 2, 2);
	glTranslatef(robot_move, 13, -78);
	glRotatef(angle, 0, 1, 0);
	glColor3f(1, 0.5, 0.6); //crown
	glPushMatrix();
	
	glTranslatef(0,8.3,0);
	drawCrown();
	glColor3f(1, 0.5, 0.6); 
	glRotatef(90,1,0,0);
	glScalef(1,1,2);
	glutSolidTorus(0.1,1.07,300,100);
	glPopMatrix();
	drawRobot();
	glPopMatrix();
	}


void drawArmy(){
	glTranslatef(100,0,0);
	glTranslatef(0, 0, -325);
	for (int i = 1; i < 10; i++){
		for (int k = 1; k < 4; k++){
    glPushMatrix();
	glTranslatef(i * 20, 0, k * 20);
	drawRobot3();
	glPopMatrix();
}
}
	
	}
	
void shadowArmy(){
	glTranslatef(100,0,0);
	glTranslatef(0, 0, -325);
	for (int i = 1; i < 10; i++){
		for (int k = 1; k < 4; k++){
    glPushMatrix();
	glTranslatef(i * 20, 0, k * 20);
	shadowRobot();
	glPopMatrix();
}
}
	
	}
void robotTimer(int value){
	
	if (forward_body) {
		if(r_theta > -20 and r_theta <= 20) r_theta--;
		else forward_body = !forward_body;
	} else {
		if(r_theta < 20 and r_theta >= -20) r_theta++;
		else forward_body = !forward_body;
	}
		
	glutPostRedisplay();
	glutTimerFunc(80, robotTimer, 0);
}

	
	
void special(int key, int x, int y)
{
	step = 0;
	if(key == GLUT_KEY_LEFT) theta += 0.1;   //in radians
	else if(key == GLUT_KEY_RIGHT) theta -= 0.1;
	else if(key == GLUT_KEY_DOWN) step = -1;
	else if(key == GLUT_KEY_UP) step = 1;
	glutPostRedisplay();
}





//	Create the skybox
void skybox() {
	//glEnable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, 0.9);
	glPushMatrix();
		//float white[4] = {1.0, 1.0, 1.0, 1.0};
		//float black[4] = {0.0, 0.0, 0.0, 1.0};
		//glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glEnable(GL_TEXTURE_2D);
		// Back
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(-350.0, -0.0, -350.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f( 350.0, -0.0, -350.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f( 350.0,  350.0, -350.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-350.0,  350.0, -350.0);
		glEnd();
		// Right
		glBindTexture(GL_TEXTURE_2D, txId[4]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(350.0, -0.0, -350.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(350.0, -0.0,  350.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(350.0,  350.0,  350.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(350.0,  350.0, -350.0);
		glEnd();
		// Front
		glBindTexture(GL_TEXTURE_2D, txId[5]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f( 350.0, -0.0, 350.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(-350.0, -0.0, 350.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-350.0,  350.0, 350.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f( 350.0,  350.0, 350.0);
		glEnd();
		// Left
		glBindTexture(GL_TEXTURE_2D, txId[4]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(-350.0, -0.0,  350.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(-350.0, -0.0, -350.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-350.0,  350.0, -350.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-350.0,  350.0,  350.0);
		glEnd();
		
		// Top
		glBindTexture(GL_TEXTURE_2D, txId[6]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f( 350.0,  350.0, -350.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f( 350.0,  350.0,  350.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-350.0,  350.0,  350.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-350.0,  350.0, -350.0);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glPopMatrix();
	//glDisable(GL_BLEND);
}

void myTimer(int value){
	if (value < 200)
		{
			if(option == 1){
			ball++;
			glutPostRedisplay();
			value++;}
			
			if(ball == 50)
			{
				ball = 0;
				fire = 0;}
			
			else{
			glutTimerFunc(250, myTimer, value);}
			}
	}

void armyTimer(int value){
	
	if (arm_army) {
		if(armMove > -1 and armMove <= 1) armMove--;
		else arm_army = !arm_army;
	} else {
		if(armMove < 1 and armMove >= -1) armMove++;
		else arm_army = !arm_army;
	}
		
	glutPostRedisplay();
	glutTimerFunc(250, armyTimer, 0);
}

void Keyboard(unsigned char key, int x, int y)
{
	if (robotMoveCounter >= -5 && robotMoveCounter < 5 && key == 'f'){
		glPushMatrix();
		if (angle == 90){
		angle = angle - 90;}
		else if (angle == 270){angle += 90;}
		fire = 1;
		option = 1;
		glPopMatrix();
		glutTimerFunc(20, myTimer,0);
		}
	
	
	switch(key){
	case 'w':
	{
		angle = 90;
		robot_move++;
		robotMoveCounter++;
		break;
		}
	case 's':
	{
		angle = 270;
		robot_move--;
		robotMoveCounter--;
		break;
		}
		
		glutPostRedisplay();
	}
}




void display()  
{
	float lpos[4] = {10., 50., 50., 1.0};  //light's position
	float lpos2[4] = {50., 100., 50., 1.0}; //light's position 2
	float shadowMat[16] = {lpos[1],0,0,0,-lpos[0],0,-lpos[2],-1,0,0,
		lpos[1],0,0,0,0,lpos[1]};
	float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
	float dir_x = -sin(theta), dir_y = 0,  dir_z = -cos(theta);
	

	float s = 3;
	int d = 2;
	
	if (step == 1){
	eye_x = eye_x + s * dir_x;
	eye_z = eye_z + s * dir_z;
	step = 0;}
	
	else if(step == -1){
	eye_x = eye_x - s * dir_x;
	eye_z = eye_z - s * dir_z;	
	step = 0;}


	look_x = eye_x + dir_x * d;	
	look_y = eye_y + dir_y * d;
	look_z = eye_z + dir_z * d;
	
	if(eye_x > 300){
		eye_x = 300;}
	else if(eye_x < -300){
		eye_x = -300;}
	else if(eye_z > 300){
		eye_z = 300;}
	else if(eye_z < -130){
		eye_z = -130;}
		
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position
	glLightfv(GL_LIGHT1, GL_POSITION, lpos2); 
	

	gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);	

	glPushMatrix();
	skybox();
	glPopMatrix();
	
	glPushMatrix();
	drawArmy();
	glPopMatrix();
	
	glPushMatrix();
    drawRobot2();
    glPopMatrix();
    
    glPushMatrix();
    drawCannon2();
    glPopMatrix();
    
	drawFloor();
	
	glPushMatrix();
	drawRiver();
	glPopMatrix();
    
	glPushMatrix();
	drawBridge();
	glPopMatrix();
    
	glPushMatrix();
	drawCastle();
	glPopMatrix();
	
	glDisable(GL_LIGHTING);
	glColor3f(1, 0, 1);
	glPushMatrix();
	glMultMatrixf(shadowMat);
	  glTranslatef(0, 3, 0);
	  glColor3f(1, 0, 1);
	  glColor4f(0.2, 0.2, 0.2, 1);
	  glScalef(1,.1,1);
	  shadowArmy();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	
	lpos[1] = -lpos[1];
	glLightfv(GL_LIGHT0, GL_POSITION, lpos); 
    
	glutSwapBuffers();
	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	loadMeshFile("Cannon.off");	
	float grey[4] = {0.2, 0.2, 0.2, 1.0};
	float white[4]  = {1.0, 1.0, 1.0, 1.0};
	loadTexture(); 

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);  //The camera view volume  
	
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("assignment 1");
   initialize();
   glutTimerFunc(80, robotTimer, 0);
   glutTimerFunc(20, armyTimer, 0);
   glutKeyboardFunc(Keyboard);
   glutDisplayFunc(display);
   glutSpecialFunc(special); 
   glutMainLoop();
   return 0;
}
