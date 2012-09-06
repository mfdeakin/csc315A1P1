
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glut.h>

#define VIEWHEIGHT 500.0
#define VIEWWIDTH 500.0
#define OFFHEIGHT 100
#define OFFWIDTH 100

struct pt {
	GLint x;
	GLint y;
	bool valid;
} linestart, lineend, wnddim;

bool ptCompare(struct pt lhs, struct pt rhs)
{
	if(lhs.x != rhs.x || lhs.y != rhs.y)
		return false;
	return true;
}

struct pt dispToCoord(struct pt pos);
void drawView(void);
void drawLine(void);
void display(void);
void resize(GLsizei width, GLsizei height);
void mpress(int btn, int state, int x, int y);

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawView();
	drawLine();
	glFlush();
 	glutSwapBuffers();
}

void drawLine(void)
{
	struct pt bound_bl = dispToCoord((struct pt){0, 0, false}),
		bound_tr = dispToCoord((struct pt){VIEWWIDTH, VIEWHEIGHT, false});
	if(!linestart.valid || !lineend.valid ||
		 ptCompare(linestart, lineend) ||
		 linestart.x < bound_bl.x ||
		 linestart.x > bound_tr.x ||
		 lineend.x < bound_bl.x ||
		 lineend.x > bound_tr.x ||
		 linestart.y < bound_bl.y ||
		 linestart.y > bound_tr.y ||
		 lineend.y < bound_bl.y ||
		 lineend.y > bound_tr.y)
		return;
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	GLint deltax = lineend.x - linestart.x,
		deltay = lineend.y - linestart.y;
	
	GLfloat mag = sqrt(deltax * deltax + deltay * deltay),
		dx = deltax / mag,
		dy = deltay / mag,
		x = 0,
		y = 0;
	while(abs(x) < abs(deltax) || abs(y) < abs(deltay)) {
		x += dx;
		y += dy;
		glVertex2i(linestart.x + x,
							 linestart.y + y);
	}
	glEnd();
	return;
}

struct pt dispToCoord(struct pt pos)
{
	return (struct pt){
		(pos.x + OFFWIDTH) * wnddim.x / (VIEWWIDTH + 2 * OFFWIDTH),
			(pos.y + OFFHEIGHT) * wnddim.y / (VIEWHEIGHT + 2 * OFFHEIGHT),
			true
			};
}

void mpress(int btn, int state, int x, int y)
{
	y = wnddim.y - y;
	struct pt check;
	check.x = 0;
	check.y = 0;
	check.valid = true;
	struct pt checkbl = dispToCoord(check);
	check.x = VIEWWIDTH;
	check.y = VIEWHEIGHT;
	check.valid = true;
	struct pt checktr = dispToCoord(check);
	if(x < checkbl.x || y < checkbl.y ||
		 x > checktr.x || y > checktr.y)
		return;
	if(btn == GLUT_LEFT_BUTTON) {
		linestart.x = x;
		linestart.y = y;
		linestart.valid = true;
		glutPostRedisplay();
	}
	else if(btn == GLUT_RIGHT_BUTTON) {
		lineend.x = x;
		lineend.y = y;
		lineend.valid = true;
		glutPostRedisplay();
	}
}

void drawView(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	struct pt bound_bl = dispToCoord((struct pt){0, 0, false}),
		bound_tr = dispToCoord((struct pt){VIEWWIDTH, VIEWHEIGHT, false}),
		pos = {0, 0, true};
	for(pos.x = bound_bl.x; pos.x < bound_tr.x; pos.x++)
		for(pos.y = bound_bl.y; pos.y < bound_tr.y; pos.y++) {
			glVertex2i(pos.x, pos.y);
		}
	glEnd();
}

void resize(GLsizei width, GLsizei height)
{
	wnddim.x = width;
	wnddim.y = height;
	wnddim.valid = true;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, 0.0,
					height, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void keypress(unsigned char key, int x, int y)
{
	switch(key) {
	case 'q':
	case 'Q':
		exit(0);
	}
}

int gcf(int a, int b)
{
	if(a % b)
		return gcf(b, a % b);
	return b;
}

int main(int argc, char **argv)
{
	memset(&linestart, 0, sizeof(linestart));
	memset(&lineend, 0, sizeof(lineend));
	memset(&wnddim, 0, sizeof(wnddim));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Program 1");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mpress);
	glutKeyboardFunc(keypress);
	glutMainLoop();
  return 0;
}
