/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _LIGHT_H_INCLUDED_
#define _LIGHT_H_INCLUDED_
#define _USE_MATH_DEFINES

#include <string>
#include <iostream>

#include "point.hpp"


#include <stdio.h>

class Light {
  public:
    std::string type;
    float position[4];
    float ambientLight[4];
    float diffuseLight[4];
    float specularLight[4];

    Light ( std::string typeN ) {
      type = typeN;
      position[0] = 0.0f;
      position[1] = 0.0f;
      position[2] = 0.0f;
      position[3] = 0.0f;
      ambientLight[0] = 0.0f;
      ambientLight[1] = 0.0f;
      ambientLight[2] = 0.0f;
      ambientLight[3] = 0.0f;
      diffuseLight[0] = 0.0f;
      diffuseLight[1] = 0.0f;
      diffuseLight[2] = 0.0f;
      diffuseLight[3] = 0.0f;
      specularLight[0] = 0.0f;
      specularLight[1] = 0.0f;
      specularLight[2] = 0.0f;
      specularLight[3] = 0.0f;
    }

    float* getPosition(){
      return position;
    }

    float* getAmbientLight(){
      return ambientLight;
    }

    float* getDiffuseLight(){
      return diffuseLight;
    }

    float* getSpecularLight(){
      return specularLight;
    }

    void setPosition( float s_position[3] ){
      position[0] = s_position[0];
      position[1] = s_position[1];
      position[2] = s_position[2];
      if (strcmp(type.c_str(), "POINT") == 0){
        //point light
        position[3] = 1;
      }
      else {
        //directional
        position[3] = 0;
      }
    }

    void setAmbientLight(float s_ambient[4] ){
      ambientLight[0] = s_ambient[0] ;
      ambientLight[1] = s_ambient[1];
      ambientLight[2] = s_ambient[2];
      ambientLight[3] = s_ambient[3];
    }

    void setDiffuseLight(float s_diffuseLight[4] ){
      diffuseLight[0] = s_diffuseLight[0];
      diffuseLight[1] = s_diffuseLight[1];
      diffuseLight[2] = s_diffuseLight[2];
      diffuseLight[3] = s_diffuseLight[3];

    }

    void setSpecularLight(float s_specularLight[4] ){
      specularLight[0] = s_specularLight[0];
      specularLight[1] = s_specularLight[1];
      specularLight[2] = s_specularLight[2];
      specularLight[3] = s_specularLight[3];
    }

};

#endif /* _LIGHT_H_INCLUDED_ */
