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
    std::vector<GLfloat> pointsVector;
    std::vector<GLfloat>::iterator it;

    Model ( std::string mName , std::string fName ){
      fileName = fName;
      modelName = mName;

    }

    Model ( const Model &obj ){
      modelName = obj.modelName;
      fileName = obj.fileName;
      pointsVector = obj.pointsVector;
    }

    void addPoint(GLfloat p){
      std::vector<GLfloat>::iterator it1 = pointsVector.end();
      pointsVector.insert(it1, p);
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
        pElem=hRoot.FirstChild( "ponto" ).Element();
        for( ; pElem; pElem=pElem->NextSiblingElement())
        {
          pElem->QueryFloatAttribute("x", &pX);
          pElem->QueryFloatAttribute("y", &pY);
          pElem->QueryFloatAttribute("z", &pZ);
          GLfloat newX ( pX );
          GLfloat newY ( pY );
          GLfloat newZ ( pZ );
          addPoint(newX);
          addPoint(newY);
          addPoint(newZ);
        }
      }
    }
};

#endif /* _MODEL_H_INCLUDED_ */
