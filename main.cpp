#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#define WIDTH 800
#define HEIGHT 600

#define MAX_POINTS 100

typedef struct {
    float x, y;
} Point;

float randFloat(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

float crossProduct(Point A, Point B, Point C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

int compareX(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    return (p1->x - p2->x);
}

int compareY(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    return (p1->y - p2->y);
}


void convexHull(Point points[], int n, Point hull[], int* hullSize) {
    if (n <= 3) {
        for (int i = 0; i < n; i++) {
            hull[i] = points[i];
        }
        *hullSize = n;
        return;
    }

    qsort(points, n, sizeof(Point), compareX);

    Point upperHull[n], lowerHull[n];
    int upperHullSize = 0, lowerHullSize = 0;

    for (int i = 0; i < n; i++) {
        while (upperHullSize >= 2 && crossProduct(upperHull[upperHullSize - 2], upperHull[upperHullSize - 1], points[i]) <= 0) {
            upperHullSize--;
        }
        upperHull[upperHullSize++] = points[i];
    }

    for (int i = n - 1; i >= 0; i--) {
        while (lowerHullSize >= 2 && crossProduct(lowerHull[lowerHullSize - 2], lowerHull[lowerHullSize - 1], points[i]) <= 0) {
            lowerHullSize--;
        }
        lowerHull[lowerHullSize++] = points[i];
    }

    for (int i = 0; i < upperHullSize - 1; i++) {
        hull[i] = upperHull[i];
    }
    for (int i = 0; i < lowerHullSize - 1; i++) {
        hull[i + upperHullSize - 1] = lowerHull[i];
    }
    *hullSize = upperHullSize + lowerHullSize - 2;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(5.0);

    Point points[MAX_POINTS];
    for (int i = 0; i < MAX_POINTS; i++) {
        points[i].x = randFloat(0, WIDTH);
        points[i].y = randFloat(0, HEIGHT);
    }

    Point hull[MAX_POINTS];
    int hullSize;
    convexHull(points, MAX_POINTS, hull, &hullSize);

    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_POINTS; i++) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < hullSize; i++) {
        glVertex2f(hull[i].x, hull[i].y);
    }
    glEnd();

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, WIDTH + 200, -200, HEIGHT + 200);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Convex Hull (Divide and Conquer)");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
