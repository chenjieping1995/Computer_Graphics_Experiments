#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
using namespace std;

int v_num = 0; //记录点的数量
int f_num = 0; //记录面的数量
int vn_num = 0;
GLfloat **vArr; //存放点的二维数组
int **fvArr; //存放面顶点的二维数组
int **fnArr;
GLfloat **vnArr;
string s1, s2, s3, s4;
int i1;
GLfloat f2, f3, f4;

// 记录鼠标点击相关事件的全局变量
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraDistanceX;
float cameraDistanceY;
float cameraAngleX;
float cameraAngleY;
float times=1;

void getLineNum(string addrstr) //获取点和面的数量
{
	ifstream infile(addrstr.c_str()); //打开指定文件
	string sline;//每一行
	int i = 0, j = 0;

	while (getline(infile, sline)) //从指定文件逐行读取
	{
		if (sline[0] == 'v')
		{
			if (sline[1] == 'n')
				vn_num++;
			else
				v_num++;
		}
		if (sline[0] == 'f')
		{
			f_num++;
		}
	}
}
int readfile(string addrstr) //将文件内容读到数组中去
{
	//getLineNum(addrstr);
	//创建二维数组
	vArr = new GLfloat*[v_num];
	for (int i = 0; i<v_num; i++)
	{
		vArr[i] = new GLfloat[3];
	}
	vnArr = new GLfloat*[vn_num];
	for (int i = 0; i<vn_num; i++)
	{
		vnArr[i] = new GLfloat[3];
	}
	fvArr = new int*[f_num];
	fnArr = new int*[f_num];
	for (int i = 0; i<f_num; i++)
	{
		fvArr[i] = new int[3];
		fnArr[i] = new int[3];
	}
	ifstream infile(addrstr.c_str());
	string sline;//每一行
	int ii = 0, jj = 0, kk = 0;

	// 按行读取文件信息
	while (getline(infile, sline))
	{
		if (sline[0] == 'v')
		{
			if (sline[1] == 'n')//vn
			{
				istringstream sin(sline);
				sin >> s1 >> f2 >> f3 >> f4;
				vnArr[ii][0] = f2;
				vnArr[ii][1] = f3;
				vnArr[ii][2] = f4;
				ii++;
			}
			else//v
			{
				istringstream sin(sline);
				sin >> s1 >> f2 >> f3 >> f4;
				vArr[jj][0] = f2;
				vArr[jj][1] = f3;
				vArr[jj][2] = f4;
				jj++;
			}
		}
		if (sline[0] == 'f') //存储面
		{
			istringstream in(sline);
			// GLfloat a;
			in >> s1;
			// int i, k;
			for (int i = 0; i<3; i++)
			{
				in >> i1;
				cout << i1 << endl;
				//取出第一个顶点
				fvArr[kk][i] = i1;
			}
			kk++;
		}
	}
	return 0;
}

void init(void)
{
	getLineNum("D:\\tshirtw.obj");
	readfile("D:\\tshirtw.obj");

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	//GLfloat light_position[] = { 1.0, 1.0f, 1.0, 0.0 };
	//GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glTranslatef(0.0, 0.0, 0.0);//平移
	glScalef(times, times, times);//缩放
	//glRotatef(-90, 1, 0, 0);//旋转
	glTranslatef(cameraDistanceX, cameraDistanceY, 0);
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);

	for (int i = 0; i < f_num; i++)
	{
		//glBegin(GL_TRIANGLES);
		//glBegin(GL_TRIANGLE_STRIP);
		glBegin(GL_LINE_LOOP);

		//glNormal3f(vnArr[fnArr[i][0]-1][0], vnArr[fnArr[i][0]-1][1], vnArr[fnArr[i][0]-1][2]);
		glVertex3f(vArr[fvArr[i][0] - 1][0], vArr[fvArr[i][0] - 1][1], vArr[fvArr[i][0] - 1][2]);

		//glNormal3f(vnArr[fnArr[i][1]-1][0], vnArr[fnArr[i][1]-1][1], vnArr[fnArr[i][1]-1][2]);
		glVertex3f(vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1], vArr[fvArr[i][1] - 1][2]);

		//glNormal3f(vnArr[fnArr[i][2]-1][0], vnArr[fnArr[i][2]-1][1], vnArr[fnArr[i][2]-1][2]);
		glVertex3f(vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1], vArr[fvArr[i][2] - 1][2]);

		glEnd();
	}
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5*(GLfloat)w / (GLfloat)h, 1.5*(GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;
	times = 1;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
	/* 按住鼠标中键，上下拖动控制图形缩放
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseMiddleDown = true;
		}
		else if (state == GLUT_UP)
			mouseMiddleDown = false;
	}*/

	/*
	* 鼠标滚轮控制图形缩放
	*/
	else if (state == GLUT_UP && button == GLUT_WHEEL_UP)
	{
		times = 0.008f+1;
		glutPostRedisplay();
	}
	else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
	{
		times = -0.008f+1;
		glutPostRedisplay();
	}
}

void mouseMotionCB(int x, int y)
{
	cameraAngleX = cameraAngleY = cameraDistanceX = cameraDistanceY = 0;
	times = 1;

	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX) * 0.1f;
		cameraAngleX += (y - mouseY) * 0.1f;
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		cameraDistanceX = (x - mouseX) * 0.002f;
		cameraDistanceY = -(y - mouseY) * 0.002f;
		mouseY = y;
		mouseX = x;
	}/* 按住鼠标中键，上下拖动控制图形缩放
	if (mouseMiddleDown) 
	{
		times = -(y - mouseY) * 0.002f+1;
		mouseY = y;
	}*/

	glutPostRedisplay();
}
/* 实现键盘上下键控制图形缩放
void specialKey(GLint key, GLint x, GLint y)
{
	times = 1;
	if (key == GLUT_KEY_UP)
	{
		times += 0.008f;
	}
	if (key == GLUT_KEY_DOWN)
	{
		times -= 0.008f;
	}
	glutPostRedisplay();
}*/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	//glutSpecialFunc(&specialKey);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}