//
// Created by eldar on 05.04.2021.
//

#include "MyX11Window.h"

MyX11Window::MyX11Window() {
    GLint test[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    this->att = test;

    this->dpy = XOpenDisplay(nullptr);
    if (this->dpy == nullptr) {
        throw std::runtime_error("Error 1: Display cannot be opened! Exiting.");
    }

    this->root = DefaultRootWindow(this->dpy);

    this->vi = glXChooseVisual(this->dpy, 0, this->att);

    if (this->vi == nullptr) {
        throw std::runtime_error("Error 2: no appropriate visual found! Exiting.");
    }

    this->colorMap = XCreateColormap(this->dpy, this->root, this->vi->visual, AllocNone);

    this->swa.colormap = this->colorMap;
    this->swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | Button1MotionMask | StructureNotifyMask;

    this->lastMousePos = {-1, -1};
    this->triangle2D = {0, 0, 0, 0, 0, 0};
    this->triangleCreated = false;
}

MyX11Window::~MyX11Window() {
    XFreeColormap(this->dpy, this->colorMap);
    glXMakeCurrent(this->dpy, None, nullptr);
    glXDestroyContext(this->dpy, this->glc);
    XDestroyWindow(this->dpy, this->win);
    XCloseDisplay(this->dpy);
}

void MyX11Window::exec() {

    srandom(time(nullptr));

    this->win = XCreateWindow(this->dpy, this->root, 0, 0, 600, 600, 0, this->vi->depth, InputOutput, this->vi->visual,
                              CWColormap | CWEventMask,
                              &this->swa);

    XMapWindow(this->dpy, this->win);

    wmDeleteWindow = XInternAtom(this->dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(this->dpy, win, &wmDeleteWindow, 1);

    XStoreName(this->dpy, this->win, "X11");

    this->glc = glXCreateContext(this->dpy, this->vi, nullptr, GL_TRUE);
    glXMakeCurrent(this->dpy, this->win, this->glc);

    XGetWindowAttributes(this->dpy, this->win, &this->gwa);
    glViewport(0, 0, this->gwa.width, this->gwa.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1., 1., 1., -1., 0, 20);
    glClearColor(1.0 * 0x25 / 0xff, 1.0 * 0x85 / 0xff, 1.0 * 0x4b / 0xff, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    while (True) {
        XNextEvent(this->dpy, &this->xev);
        switch (this->xev.type) {
            case Expose:
                XGetWindowAttributes(this->dpy, this->win, &this->gwa);
                glViewport(0, 0, this->gwa.width, this->gwa.height);
                glLoadIdentity();
                glClear(GL_COLOR_BUFFER_BIT);
                if (this->triangleCreated) {
                    MyX11Window::DrawTriangle(this->triangle2D);
                }
                glXSwapBuffers(this->dpy, this->win);
                break;

            case ClientMessage:
                if (this->xev.xclient.data.l[0] == this->wmDeleteWindow) {
                    return;
                }
                break;

            case KeyPress:
                return;

            case ButtonPress:
                this->lastMousePos = {this->xev.xbutton.x, this->xev.xbutton.y};
                if (!this->triangleCreated) {
                    this->triangle2D.x[0] = -1. + static_cast<double>(this->xev.xbutton.x) / this->gwa.width * 2.;
                    this->triangle2D.y[0] = 1. - static_cast<double>(this->xev.xbutton.y) / this->gwa.height * 2.;
                    this->triangle2D.x[1] = -1. + static_cast<double>(random()) / RAND_MAX * 2;
                    this->triangle2D.y[1] = -1. + static_cast<double>(random()) / RAND_MAX * 2;
                    this->triangle2D.x[2] = -1. + static_cast<double>(random()) / RAND_MAX * 2;
                    this->triangle2D.y[2] = -1. + static_cast<double>(random()) / RAND_MAX * 2;
                    this->triangleCreated = True;
                }
                glClear(GL_COLOR_BUFFER_BIT);
                if (this->triangleCreated) {
                    MyX11Window::DrawTriangle(this->triangle2D);
                }
                glXSwapBuffers(this->dpy, this->win);
                break;

            case MotionNotify:
                if (abs(this->xev.xmotion.x - this->lastMousePos.x) > this->gwa.width / 50 or
                    abs(this->xev.xmotion.y - this->lastMousePos.y) > this->gwa.height / 50) {
                    this->triangle2D.x[0] +=
                            static_cast<double>(this->xev.xmotion.x - this->lastMousePos.x) / this->gwa.width * 2;
                    this->triangle2D.y[0] -=
                            static_cast<double>(this->xev.xmotion.y - this->lastMousePos.y) / this->gwa.height * 2;
                    this->triangle2D.x[1] +=
                            static_cast<double>(this->xev.xmotion.x - this->lastMousePos.x) / this->gwa.width * 2;
                    this->triangle2D.y[1] -=
                            static_cast<double>(this->xev.xmotion.y - this->lastMousePos.y) / this->gwa.height * 2;
                    this->triangle2D.x[2] +=
                            static_cast<double>(this->xev.xmotion.x - this->lastMousePos.x) / this->gwa.width * 2;
                    this->triangle2D.y[2] -=
                            static_cast<double>(this->xev.xmotion.y - this->lastMousePos.y) / this->gwa.height * 2;
                    this->lastMousePos.x = this->xev.xmotion.x;
                    this->lastMousePos.y = this->xev.xmotion.y;

                    glClear(GL_COLOR_BUFFER_BIT);
                    if (this->triangleCreated) {
                        MyX11Window::DrawTriangle(this->triangle2D);
                    }
                    glXSwapBuffers(this->dpy, this->win);
                }
                break;
        }
    }
}

void MyX11Window::DrawTriangle(Triangle2D triangle2D) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2);

    glBegin(GL_TRIANGLES);
    glVertex2f(triangle2D.x[0], triangle2D.y[0]);
    glVertex2f(triangle2D.x[1], triangle2D.y[1]);
    glVertex2f(triangle2D.x[2], triangle2D.y[2]);
    glEnd();
}