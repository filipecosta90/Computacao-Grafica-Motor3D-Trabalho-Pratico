/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _GROUP_H_INCLUDED_
#define _GROUP_H_INCLUDED_
#define _USE_MATH_DEFINES

#include <vector>
#include <string>
#include <math.h>
#include <iostream>

#include "point.hpp"
#include "vectors.h"
#include "model.hpp"
#include "matrices.h"
#include "bezier.hpp"

#include <stdio.h>

class Group {
  public:
    std::string groupName;
    std::vector<Model> modelVector;
    Matrix4 matModel;
    std::vector<Point> pointsBezier;
    float animationTime, rotationAnimationTime;
    bool hasAnimation;
    bool hasRotationAnimation;
    Point rotationParameters = Point ( 0.0f , 0.0f , 0.0f ) ;
    float previousRotatedAngle = 0.0f;


    Group ( std::string name ) {
      groupName = name;
      matModel = Matrix4();
      matModel.identity();
      hasAnimation = false;
      hasRotationAnimation = false;
      animationTime = 0.0f;
    }

    Group ( const Group &g1 ) {
      matModel = Matrix4 ( g1.matModel );
      hasAnimation = g1.hasAnimation;
      animationTime = g1.animationTime;
    }

    void addModel( Model m1){
      std::unique_ptr<Model> uniqueModelPointer ( new Model (m1) );
      modelVector.push_back ( *uniqueModelPointer );
    }

    void addBezierPoint( Point p ){
      std::vector<Point>::iterator it1 = pointsBezier.end();
      pointsBezier.insert(it1, p);
    }

    void setAnimationTime( float anTime){
      animationTime = anTime;
      hasAnimation = true;
    }

    std::vector<Model> getModels(){
      return modelVector;
    }

    void groupTranslate2f (  float x, float y, float z){
      matModel.translateCG(x,y,z);
    }

    void groupRotate2f ( float angle, float x, float y, float z){
      matModel.rotate ( angle, x , y , z );
    }

    void groupSetRotate ( float time, float x, float y, float z){
      rotationAnimationTime = time;
      rotationParameters.set ( x , y, z );
      hasRotationAnimation = true;
    }

    void groupRotateAnimate (  int elapsedTimeMs ){
      if( hasRotationAnimation == true ){

        //computes the position on the animation
        float elapsedTimeSeconds = float( elapsedTimeMs ) / 1000.0 ;

        //computes the value of position betwen 0 and animation time
        float positionInAnimation = fmod ( elapsedTimeSeconds , rotationAnimationTime );

        //computes the total rotation
        float rotationAngle = 0.0f;
        rotationAngle = 360.0f / rotationAnimationTime * positionInAnimation;

        //computes the rotation already done
        float realRotate = rotationAngle - previousRotatedAngle;
        groupRotate2f ( realRotate , rotationParameters.x , rotationParameters.y , rotationParameters.z );
        previousRotatedAngle = rotationAngle;
      }
    }

    void groupScale ( float x, float y, float z){
      matModel.scaleCG(x,y,z);
    }

    void groupBezierTranslate (  int elapsedTimeMs ){
      if( hasAnimation == true ){

        //computes the position on the animation
        float elapsedTimeSeconds = float( elapsedTimeMs ) / 1000.0 ;
        float position = fmod ( elapsedTimeSeconds , animationTime );

        //computes the value of position betwen 0 and 1
        float bezierTime = 0.0f;
        bezierTime = ( 1.0f / animationTime ) * position;

        Point bezierPosition = getBezierAtTime( bezierTime , pointsBezier );
        matModel[12] = bezierPosition.x;
        matModel[13] = bezierPosition.y;
        matModel[14] = bezierPosition.z;
      }
    }

    void loadTransformations(float pitchX, float headingY, float roolZ,
        float camPosX, float camPosY, float camPosZ){

      Matrix4 matView, matModelView;

      // set the correct gl Matrix
      glMatrixMode(GL_MODELVIEW);

      // camera (view)
      matView.identity();
      // transform orders:
      // 1: rotate on z-axis
      matView.rotate(-roolZ, 0.0 ,0.0 ,1.0 );
      // 2: rotate on y-axis
      matView.rotate(-headingY, 0.0 ,1.0 ,0.0 );
      // 3: rotate on x-axis
      matView.rotate(-pitchX, 1.0,0.0,0.0);
      // 4: translate along x,y,z
      matView.translate(-camPosX, -camPosY, -camPosZ);

      // build modeview matrix: Mmv = Mv * Mm
      matModelView = matView * matModel;
      // pass it to opengl before draw
      glLoadMatrixf(matModelView.get());
    }
};

#endif /* _MODEL_H_INCLUDED_ */
