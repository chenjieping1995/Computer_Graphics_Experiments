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

float normal[426][3] = { 0 };      //顶点法向量信息，为单位法向量
float angle[426] = { 0 };        //用来计算法向量的角度信息的辅助数组

float viewpoint_x = 0;         //记录视点位置
float viewpoint_y = 0;
float viewpoint_z = 2;
float light_x = 0;             //记录光源位置
float light_y = 0;
float light_z = 0;
float rotate_x = 0;            //记录旋转角度
float rotate_y = 0;
float rotate_z = 0;
bool l = false;                //当前模式为光源移动
bool v = false;                //当前模式为视点移动
bool r = false;                //当前模式为旋转  

// 用于计算顶点法向量的函数
void calc_n_vector()
{
	float n1[3], n2[3];        //临时向量
	float n[3];                //法向量
	float a;                   //顶点角度
	float l1, l2;              //临时向量长度
	int i, j;
	float x, y, z, r;             //顶点法向量坐标及长度
								  //法向量计算公式为n=(n1*a1+n2*a2+...)/(a1+a2+...)
								  //ai为顶点所在三角形的顶点角度（弧度），ni为顶点所在三角形的法向量
	for (i = 0; i < 604; i++)
	{
		n1[0] = vArr[fvArr[i][1] - 1][0] - vArr[fvArr[i][0] - 1][0];
		n1[1] = vArr[fvArr[i][1] - 1][1] - vArr[fvArr[i][0] - 1][1];
		n1[2] = vArr[fvArr[i][1] - 1][2] - vArr[fvArr[i][0] - 1][2];
		n2[0] = vArr[fvArr[i][2] - 1][0] - vArr[fvArr[i][1] - 1][0];
		n2[1] = vArr[fvArr[i][2] - 1][1] - vArr[fvArr[i][1] - 1][1];
		n2[2] = vArr[fvArr[i][2] - 1][2] - vArr[fvArr[i][1] - 1][2];
		//由于坐标很小，导致法向量很小，为防止后面计算精度不够，先统一扩大
		n[0] = 100 * (n1[1] * n2[2] - n2[1] * n1[2]);
		n[1] = 100 * (n1[2] * n2[0] - n2[2] * n1[0]);
		n[2] = 100 * (n1[0] * n2[1] - n2[0] * n1[1]);
		//ai的计算公式为a=arccos((pa*pb)/|pa||pb|)，此处*为点积
		for (j = 0; j < 3; j++)
		{
			n1[0] = vArr[fvArr[i][(j + 2) % 3] - 1][0] - vArr[fvArr[i][j] - 1][0];
			n1[1] = vArr[fvArr[i][(j + 2) % 3] - 1][1] - vArr[fvArr[i][j] - 1][1];
			n1[2] = vArr[fvArr[i][(j + 2) % 3] - 1][2] - vArr[fvArr[i][j] - 1][2];
			l1 = sqrtf(n1[0] * n1[0] + n1[1] * n1[1] + n1[2] * n1[2]);
			n2[0] = vArr[fvArr[i][(j + 1) % 3] - 1][0] - vArr[fvArr[i][j] - 1][0];
			n2[1] = vArr[fvArr[i][(j + 1) % 3] - 1][1] - vArr[fvArr[i][j] - 1][1];
			n2[2] = vArr[fvArr[i][(j + 1) % 3] - 1][2] - vArr[fvArr[i][j] - 1][2];
			l2 = sqrtf(n2[0] * n2[0] + n2[1] * n2[1] + n2[2] * n2[2]);
			a = acosf((n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2]) / (l1*l2));
			normal[fvArr[i][j] - 1][0] += n[0] * a;
			normal[fvArr[i][j] - 1][1] += n[1] * a;
			normal[fvArr[i][j] - 1][2] += n[2] * a;
			angle[fvArr[i][j] - 1] += a;
		}
	}
	//计算顶点法向量最终结果，并变为单位向量
	for (i = 0; i < 426; i++)
	{
		x = normal[i][0] / angle[i];
		y = normal[i][1] / angle[i];
		z = normal[i][2] / angle[i];
		r = sqrtf(x*x + y*y + z*z);
		normal[i][0] = x / r;
		normal[i][1] = y / r;
		normal[i][2] = z / r;
	}
}


// 新增的光源设置函数，设定光源，位置可通过键盘改变
void setlight()
{
	float light_position[4] = { light_x,light_y,light_z,1.0 };     //光源位置
	float light_ambient[4] = { 0,0,0,1.0 };                       //光源环境光，黑色，即无
	float light_diffuse[4] = { 1.0,1.0,1.0,1.0 };                 //光源漫反射光，白色
	float light_specular[4] = { 1.0,1.0,1.0,1.0 };                //光源反射光，白色
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);                                          //开启光源
	glEnable(GL_LIGHTING);
}



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
	GLfloat light_position[] = { 1.0, 1.0f, 1.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
}

// 模型绘制函数
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);  //增加深度检测，体现遮挡效果
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//设置视点，视点位置可通过键盘改变
	glLoadIdentity();
	gluPerspective(60.0f, 1.0f, 1.0f, 10.0f);
	gluLookAt(viewpoint_x, viewpoint_y, viewpoint_z, 0, 0, 0, 0, 1, 0);

	setlight();

	glRotatef(rotate_x, 1, 0, 0);
	glRotatef(rotate_y, 0, 1, 0);
	glRotatef(rotate_z, 0, 0, 1);

	float ts_ambient[4] = { 1,1,0,1.0 };       //物体环境光，黄色
	float ts_diffuse[4] = { 1,1,1,1.0 };       //物体漫反射光，白色
	float ts_specular[4] = { 1,1,1,1.0 };      //物体镜面反射光，白色
	float ts_emission[4] = { 0,0,0,1.0 };      //物体辐射光，黑色，即不发光
	float ts_shininess = 30.0;                 //设置物体镜面属性

	//设置物体材料属性
	glMaterialfv(GL_FRONT, GL_AMBIENT, ts_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ts_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ts_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, ts_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, ts_shininess);

	for (int i = 0; i < f_num; i++)
	{
		// glBegin(GL_TRIANGLES);
		glBegin(GL_TRIANGLE_STRIP);
		// glBegin(GL_LINE_LOOP);

		//glNormal3f(vnArr[fnArr[i][0]-1][0], vnArr[fnArr[i][0]-1][1], vnArr[fnArr[i][0]-1][2]);
		glVertex3f(vArr[fvArr[i][0] - 1][0], vArr[fvArr[i][0] - 1][1], vArr[fvArr[i][0] - 1][2]);

		//glNormal3f(vnArr[fnArr[i][1]-1][0], vnArr[fnArr[i][1]-1][1], vnArr[fnArr[i][1]-1][2]);
		glVertex3f(vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1], vArr[fvArr[i][1] - 1][2]);

		//glNormal3f(vnArr[fnArr[i][2]-1][0], vnArr[fnArr[i][2]-1][1], vnArr[fnArr[i][2]-1][2]);
		glVertex3f(vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1], vArr[fvArr[i][2] - 1][2]);

		for (int j = 0; j < 3; j++)
		{
			glNormal3fv(normal[fvArr[i][j] - 1]);
			glVertex3fv(vArr[fvArr[i][j] - 1]);
		}
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

/* 键盘响应函数 */
void mykeyboard(unsigned char key, int x, int y)        
{
	/* 响应键盘输入
	 *
	 * L(light)：光源移动模式
	 * V(view)：视点移动模式
	 * R(rotate)：旋转模型
	 *
	 * Q：向x轴正方向移动
	 * A：向x轴负方向移动
	 * W：向y轴正方向移动
	 * S：向y轴负方向移动
	 * E：向z轴正方向移动
	 * D：向z轴负方向移动
	 */
	switch (key)
	{
	case 'l':l = true; v = false; r = false; break;
	case 'v':l = false; v = true; r = false; break;
	case 'r':l = false; v = false; r = true; break;
	case 'e':if (l)light_z += 0.2; 
		if (v)viewpoint_z += 0.2;
		if (r)rotate_z += 1;
		break;
	case 'd':if (l)light_z -= 0.2;
		if (v)viewpoint_z -= 0.2;
		if (r)rotate_z -= 1;
		break;
	case 'w':if (l)light_y += 0.2;
		if (v)viewpoint_y += 0.2;
		if (r)rotate_y += 1;
		break;
	case 's':if (l)light_y -= 0.2;
		if (v)viewpoint_y -= 0.2;
		if (r)rotate_y -= 1;
		break;
	case 'q':if (l)light_x += 0.2;
		if (v)viewpoint_x += 0.2;
		if (r)rotate_x += 1;
		break;
	case 'a':if (l)light_x -= 0.2;
		if (v)viewpoint_x -= 0.2;
		if (r)rotate_x -= 1;
		break;
	default:break;
	}
	glutPostRedisplay();//重新绘制
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	calc_n_vector();
	glutKeyboardFunc(mykeyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
