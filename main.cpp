/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include "glew.h"
#include <GLUT/glut.h>
#else
#include <Windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "engine.hpp"

#include <math.h>

Engine *enginePointer;

#define _PI_ 3.14159

void renderScene(){
  enginePointer->renderScene();
}

void changeSize(int w, int h){
  enginePointer->changeSize(w,h);
}

void processKeys(int key, int xx, int yy){
  enginePointer->processKeys(key, xx, yy);
}

void processKeyboard(unsigned char key , int xx, int yy){
  enginePointer->processKeyboard(key, xx, yy);
}

int main(int argc, char **argv) {

  Engine motorApp ( 0.0 , 320.0 , 0.0, 350.0 , 0.0 , 550.0 , 1 );
  enginePointer = &motorApp;
  motorApp.load("fase3.coneEsfera.xml");

  // initialization
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(1920, 1024);
  glutCreateWindow("CG@DI-UM -- Fase 3");

  // registo de funções 
  glutDisplayFunc(renderScene);
  glutIdleFunc(renderScene);
  glutReshapeFunc(changeSize);

  // function association
  glutDisplayFunc(renderScene);
  glutIdleFunc(renderScene);
  glutReshapeFunc(changeSize);
  glutSpecialFunc(processKeys);
  glutKeyboardFunc(processKeyboard);

  // init GLEW
  glewInit();
  motorApp.initGL();

  // entrar no ciclo do GLUT 
  glutMainLoop();
}
