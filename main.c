
#include <math.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glut.h>

#define VIEWHEIGHT 700.0
#define VIEWWIDTH 700.0
#define OFFHEIGHT 100
#define OFFWIDTH 100

struct pt {
	GLint x;
	GLint y;
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
	if(!ptCompare(linestart, lineend))
		drawLine();
	glFlush();
 	glutSwapBuffers();
}

void drawLine(void)
{
	float dydx = ((GLfloat)linestart.y - lineend.y) /
		((GLfloat)linestart.x - lineend.x);
	GLint x = linestart.x;
	float y = linestart.y;
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	printf("dy/dx: %6f\n"
				 "linestart: x: %4d y: %4d\n"
				 "lineend:   x: %4d y: %4d\n",
				 dydx,
				 linestart.x,
				 linestart.y,
				 lineend.x,
				 lineend.y);
	while(x <= lineend.x) {
		struct pt coord = dispToCoord((struct pt){x, (GLint) y});
		glVertex2i(coord.x, coord.y);
		x++;
		y += dydx;
	}
	printf("middle:    x: %4d y: %4d\n", x, (GLint)y);
	while(x >= lineend.x) {
		struct pt coord = dispToCoord((struct pt){x, (GLint) y});
		glVertex2i(coord.x, coord.y);
		x--;
		y -= dydx;
	}
	printf("final:     x: %4d y: %4d\n\n", x, (GLint)y);
	glEnd();
}

struct pt dispToCoord(struct pt pos)
{
	if(pos.x > 0 && pos.x < VIEWWIDTH &&
		 pos.y > 0 && pos.y < VIEWHEIGHT)
		return (struct pt){
			pos.x + OFFWIDTH,
			pos.y + OFFHEIGHT
	  };
	return (struct pt){
		0,
		0
	};
}

void mpress(int btn, int state, int x, int y)
{
	if(x > OFFWIDTH && x < VIEWWIDTH + OFFWIDTH &&
		 y > OFFHEIGHT && y < VIEWWIDTH + OFFHEIGHT) {
		x -= OFFWIDTH;
		y = VIEWHEIGHT - OFFHEIGHT - y;
		x = (int)((float)x * (VIEWWIDTH + OFFWIDTH) / wnddim.x);
		y = (int)((float)y * (VIEWHEIGHT + OFFHEIGHT) / wnddim.y);
		if(btn == GLUT_LEFT_BUTTON) {
			linestart = dispToCoord((struct pt){x, y});
			glutPostRedisplay();
		}
		else if(btn == GLUT_RIGHT_BUTTON) {
			lineend = dispToCoord((struct pt){x, y});
			glutPostRedisplay();
		}
	}
}

void drawView(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	struct pt pos;
	for(pos.x = 0; pos.x < VIEWWIDTH; pos.x++)
		for(pos.y = 0; pos.y < VIEWHEIGHT; pos.y++) {
			struct pt coord = dispToCoord(pos);
			glVertex2i(coord.x, coord.y);
		}
	glEnd();
}

void resize(GLsizei width, GLsizei height)
{
	wnddim.x = width;
	wnddim.y = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, VIEWWIDTH + 2 * OFFWIDTH, 0.0,
					VIEWHEIGHT + 2 * OFFHEIGHT, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	linestart.x = 0;
	linestart.y = 0;
	lineend.x = 0;
	lineend.y = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Program 1");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mpress);
	glutMainLoop();
  return 0;
}
