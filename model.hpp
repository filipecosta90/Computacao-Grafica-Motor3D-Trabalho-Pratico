/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _MODEL_H_INCLUDED_
#define _MODEL_H_INCLUDED_
#define _USE_MATH_DEFINES

#include "tinyxml.cpp"
#include "tinyxml.h"
#include "tinyxmlerror.cpp"
#include "tinyxmlparser.cpp"
#include "tinystr.cpp"
#include "tinystr.h"

#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include <memory>    // for std::unique_ptr

#include "point.hpp"

class Model {
  public:
    std::string modelName;
    std::string fileName;
    std::string textureFileName;
    std::vector<GLfloat> pointsVector;
    std::vector<GLfloat> normalVector;
    std::vector<GLfloat> textureVector;
    float diffR, diffG, diffB;


    Model ( std::string mName , std::string fName ){
      fileName = fName;
      modelName = mName;
    }

    Model ( const Model &obj ){
      modelName = obj.modelName;
      fileName = obj.fileName;
      textureFileName = obj.textureFileName;
      pointsVector = obj.pointsVector;
      normalVector = obj.normalVector;
      textureVector = obj.textureVector;
    }

    void addVerticePoint(GLfloat p){
      std::vector<GLfloat>::iterator it1 = pointsVector.end();
      pointsVector.insert(it1, p);
    }

    void addNormalPoint(GLfloat p){
      std::vector<GLfloat>::iterator it1 = normalVector.end();
      pointsVector.insert(it1, p);
    }

    void addTexturePoint(GLfloat p){
      std::vector<GLfloat>::iterator it1 = textureVector.end();
      pointsVector.insert(it1, p);
    }

    void setTexture( std::string textureFilePath ){
      this.textureFileName = textureFilePath;
    }


    void setRGBDiffuse( float n_diffR , float n_diffG, float n_diffB ){
      diffR = n_diffR;
      diffG = n_diffG;
      diffB = n_diffB;
    }

    void load()
    {
      TiXmlDocument doc1( fileName.c_str() );
      if (!doc1.LoadFile()) return;
      TiXmlHandle hDoc(&doc1);
      TiXmlElement* pElem;
      TiXmlHandle hRoot(0);
      pElem=hDoc.FirstChildElement().Element();
      if (!pElem) return;
      hRoot=TiXmlHandle(pElem);
      // block: pontos
      {
        GLfloat pX, pY, pZ;
        pElem=hRoot.FirstChildElement.Element();
        for( ; pElem; pElem=pElem->NextSiblingElement())
        {
          const char *childType=pElem->Value();
          if( strcmp ( childType,  "ponto" ) == 0 ){
            pElem->QueryFloatAttribute("x", &pX);
            pElem->QueryFloatAttribute("y", &pY);
            pElem->QueryFloatAttribute("z", &pZ);
            GLfloat newX ( pX );
            GLfloat newY ( pY );
            GLfloat newZ ( pZ );
            addVerticePoint(newX);
            addVerticePoint(newY);
            addVerticePoint(newZ);
          }
          if( strcmp ( childType,  "normal" ) == 0 ){
            pElem->QueryFloatAttribute("x", &pX);
            pElem->QueryFloatAttribute("y", &pY);
            pElem->QueryFloatAttribute("z", &pZ);
            GLfloat newX ( pX );
            GLfloat newY ( pY );
            GLfloat newZ ( pZ );
            addNormalPoint(newX);
            addNormalPoint(newY);
            addNormalPoint(newZ);
          }
          if( strcmp ( childType,  "textura" ) == 0 ){
            pElem->QueryFloatAttribute("x", &pX);
            pElem->QueryFloatAttribute("y", &pY);
            GLfloat newX ( pX );
            GLfloat newY ( pY );
            addTexturePoint(newX);
            addTexturePoint(newY);
          }
        }
      }
    }
};

#endif /* _MODEL_H_INCLUDED_ */
