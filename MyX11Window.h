//
// Created by eldar on 05.04.2021.
//

#ifndef JB_MYX11WINDOW_H
#define JB_MYX11WINDOW_H

#include <iostream>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<X11/Xatom.h>
#include<GL/gl.h>
#include<GL/glx.h>

struct MousePos {
    int x;
    int y;
};

struct Triangle2D {
    double x[3];
    double y[3];
};

typedef struct MousePos MousePos;
typedef struct Triangle2D Triangle2D;

class MyX11Window {
public:
    MyX11Window();

    ~MyX11Window();

    void exec();

    static void DrawTriangle(Triangle2D);

private:
    Display *dpy;
    Window root;
    GLint *att;
    XSetWindowAttributes swa{};
    Colormap colorMap;
    XVisualInfo *vi;
    Window win{};
    GLXContext glc{};
    XWindowAttributes gwa{};
    XEvent xev{};
    Atom wmDeleteWindow;
    MousePos lastMousePos{};
    bool triangleCreated;
    Triangle2D triangle2D{};
};

#endif //JB_MYX11WINDOW_H