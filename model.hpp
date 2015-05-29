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
    float mat_diff[4];
    float mat_spec[4];
    float mat_amb[4];
	float mat_emi[4];
	GLfloat mat_shi;

    bool normalVectorDefined;
    bool textureVectorDefined;
    bool textureVectorEnabled;
    bool materialDefined;



    Model ( std::string mName , std::string fName ){
      fileName = fName;
      modelName = mName;
      normalVectorDefined = false;
      textureVectorDefined = false;
      textureVectorEnabled = false;
      materialDefined = false;
      //standard material values
      mat_diff[0] = mat_diff[1] = mat_diff[2] = 0.8f;
      mat_amb[0] = mat_amb[1] = mat_amb[2] = 0.2f;
      mat_spec[0] = mat_spec[1] = mat_spec[2] = 0.0f;
	  mat_emi[0] = mat_emi[1] = mat_emi[2] = 0.0f;
      mat_diff[3] = mat_spec[3] = mat_amb[3] = mat_emi[3]= 1.0f;
	  mat_shi = 0.0f;

    }

    Model ( const Model &obj ){
      modelName = obj.modelName;
      fileName = obj.fileName;
      textureFileName = obj.textureFileName;
      pointsVector = obj.pointsVector;
      normalVector = obj.normalVector;
      textureVector = obj.textureVector;
      normalVectorDefined = obj.normalVectorDefined;
      textureVectorDefined = obj.textureVectorDefined;
      textureVectorEnabled = obj.textureVectorEnabled;
      materialDefined = obj.materialDefined;
	  mat_diff[0] = obj.mat_diff[0];
	  mat_diff[1] = obj.mat_diff[1];
	  mat_diff[2] = obj.mat_diff[2];
	  mat_diff[3] = obj.mat_diff[3];
	  mat_amb[0] = obj.mat_amb[0];
	  mat_amb[1] = obj.mat_amb[1];
	  mat_amb[2] = obj.mat_amb[2];
	  mat_amb[3] = obj.mat_amb[3];
	  mat_spec[0] = obj.mat_spec[0];
	  mat_spec[1] = obj.mat_spec[1];
	  mat_spec[2] = obj.mat_spec[2];
	  mat_spec[3] = obj.mat_spec[3];
		mat_emi[0] = obj.mat_emi[0];
		mat_emi[1] = obj.mat_emi[1];
		mat_emi[2] = obj.mat_emi[2];
		mat_emi[3] = obj.mat_emi[3];
		mat_shi = obj.mat_shi;
    }

    void addVerticePoint(GLfloat p){
      std::vector<GLfloat>::iterator it1 = pointsVector.end();
      pointsVector.insert(it1, p);
    }

    std::string getTextureFilename(){
      return textureFileName;
    }

	GLfloat getMaterialShininess(){
		return mat_shi;
	}

    void addNormalPoint(GLfloat p){
      if (!normalVectorDefined){
        normalVectorDefined = true;
      }
      std::vector<GLfloat>::iterator it1 = normalVector.end();
      normalVector.insert(it1, p);
    }

    void addTexturePoint(GLfloat p){
      if (!textureVectorDefined){
        textureVectorDefined = true;
      }
      std::vector<GLfloat>::iterator it1 = textureVector.end();
      textureVector.insert(it1, p);
    }

    void setTexture( std::string textureFilePath ){
      textureFileName = textureFilePath;
	  textureVectorEnabled = true;
    }

    std::vector<GLfloat> getPointsVector(){
      return pointsVector;
    }

    std::vector<GLfloat> getNormalVector(){
      return normalVector;
    }

    std::vector<GLfloat> getTextureVector(){
      return textureVector;
    }

    void setMaterialDiffuse( float n_diffR , float n_diffG, float n_diffB , float n_diffA ){
      mat_diff[0] = n_diffR;
      mat_diff[1] = n_diffG;
      mat_diff[2] = n_diffB;
      mat_diff[3] = n_diffA;
      enableMaterial();
    }

    void setMaterialAmbient(float n_ambR, float n_ambG, float n_ambB, float n_ambA){
      mat_amb[0] = n_ambR;
      mat_amb[1] = n_ambG;
      mat_amb[2] = n_ambB;
      mat_amb[3] = n_ambA;
      enableMaterial();
    }

    void setMaterialSpecular(float n_specR, float n_specG, float n_specB , float n_specA){
      mat_spec[0] = n_specR;
      mat_spec[1] = n_specG;
      mat_spec[2] = n_specB;
      mat_spec[3] = n_specA;
      enableMaterial();
    }

	void setMaterialEmissive(float n_emiR, float n_emiG, float n_emiB, float n_emiA){
		mat_emi[0] = n_emiR;
		mat_emi[1] = n_emiG;
		mat_emi[2] = n_emiB;
		mat_emi[3] = n_emiA;
		enableMaterial();
	}

	void setMaterialShininess( float n_shi ){
		mat_shi = n_shi;
		enableMaterial();
	}


    bool isMaterialDefined(){
      return materialDefined;
    }

    bool isNormalVectorDefined(){
      return normalVectorDefined;
    }

    bool isTextureVectorDefined(){
      return textureVectorDefined;
    }

    bool isTextureVectorEnabled(){
      return textureVectorEnabled;
    }

    void enableTextureVector(){
      textureVectorEnabled = true;
    }

    void enableMaterial(){
      materialDefined = true;
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
        pElem=hRoot.FirstChild().Element();
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
