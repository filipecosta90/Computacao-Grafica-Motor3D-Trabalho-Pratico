/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779 
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _ENGINE_H_INCLUDED_
#define _ENGINE_H_INCLUDED_
#define _USE_MATH_DEFINES

#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include <memory>    // for std::unique_ptr

#include "group.hpp"
#include "model.hpp"
#include "light.hpp"

#define TIXML_USE_STL

typedef void(*func_t)();

class Engine {
  public:
    std::string ceneName;
    std::vector<std::unique_ptr<Group>> groupVector;
    std::vector<std::unique_ptr<Group>>::iterator groupIt;
    std::vector<std::unique_ptr<Light>> lightsVector;
    std::vector<std::unique_ptr<Light>>::iterator lightsIt;
    GLuint arrayVBOS[100];
    GLuint arrayNormalVBOS[100];
    GLuint arrayTexturesVBOS[100];
    int sizeArrayVBOS[100];
    int sizeArrayNormalVBOS[100];
    int sizeArrayTextureVBOS[100];
    int posArrayVBOS = 0;
	unsigned int texID[100];
	ILuint ima[100];

    float pitchX = 0.0f;
    float headingY = 0.0f;
    float roolZ = 0.0f;
    float camPosX = 0.0f;
    float camPosY = 0.0f;
    float camPosZ = 20.0f;


    float initPX, initHY, initRZ, initCX, initCY, initCZ;
    int drawMode = 1, initMode;
    bool animateX = false, animateY = false , animateZ = false, animateBezier = false;

    Engine( float rx, float hy, float pz, float cx, float cy, float cz, int dmode ){
      pitchX = initPX = rx;
      headingY = initHY = hy;
      roolZ = initRZ = pz;
      camPosX = initCX = cx;
      camPosY = initCY = cy;
      camPosZ = initCZ = cz;
      drawMode = initMode = dmode;
    }

    void resetCameraParameters (){
      pitchX = initPX;
      headingY = initHY;
      roolZ = initRZ;
      camPosX = initCX;
      camPosY = initCY;
      camPosZ = initCZ;
      drawMode = initMode;
    }

    void loadGroup (  TiXmlElement* gElem , Group* g1 ){
      for (; gElem !=NULL ;  gElem=gElem->NextSiblingElement()) {
        const char *childType=gElem->Value();
        if( strcmp ( childType,  "translacao2f" ) == 0 ){
          float tX, tY, tZ;
          gElem->QueryFloatAttribute("X", &tX);
          gElem->QueryFloatAttribute("Y", &tY);
          gElem->QueryFloatAttribute("Z", &tZ);
          g1->groupTranslate2f(tX, tY, tZ);
        }
        if( strcmp ( childType,  "translacao" ) == 0 ){
          float animationTime;
          gElem->QueryFloatAttribute("tempo", &animationTime);
          g1->setAnimationTime(animationTime);
          // block: pontos translacao
          TiXmlHandle oRoot(gElem);
          TiXmlElement* pElem;
          pElem=oRoot.FirstChild( "ponto" ).Element();
          bool isFirstBezierPoint = true;
          for( ; pElem; pElem=pElem->NextSiblingElement()){
            float bezierX, bezierY, bezierZ;
            pElem->QueryFloatAttribute("X", &bezierX);
            pElem->QueryFloatAttribute("Y", &bezierY);
            pElem->QueryFloatAttribute("Z", &bezierZ);

            //saving initial position -- to show object if the user dont press animate button
            if ( isFirstBezierPoint ){
              g1->groupTranslate2f(bezierX, bezierY, bezierZ);
              isFirstBezierPoint = false;
            }

            std::unique_ptr<Point> uniquePointPointer ( new Point ( bezierX , bezierY , bezierZ ) );
            g1->addBezierPoint(*uniquePointPointer);
          }
        }
        if( strcmp ( childType,  "rotacao2f" ) == 0 ){
          float angle, rX, rY, rZ;
          gElem->QueryFloatAttribute("angulo", &angle);
          gElem->QueryFloatAttribute("eixoX", &rX);
          gElem->QueryFloatAttribute("eixoY", &rY);
          gElem->QueryFloatAttribute("eixoZ", &rZ);
          g1->groupRotate2f( angle, rX, rY, rZ);
        }
        if( strcmp ( childType,  "rotacao" ) == 0 ){
          float time, rX, rY, rZ;
          gElem->QueryFloatAttribute("tempo", &time);
          gElem->QueryFloatAttribute("eixoX", &rX);
          gElem->QueryFloatAttribute("eixoY", &rY);
          gElem->QueryFloatAttribute("eixoZ", &rZ);
          g1->groupSetRotate( time, rX, rY, rZ);
        }
        if( strcmp ( childType,  "escala" ) == 0 ){
          float eX, eY, eZ;
          gElem->QueryFloatAttribute("X", &eX);
          gElem->QueryFloatAttribute("Y", &eY);
          gElem->QueryFloatAttribute("Z", &eZ);
          g1->groupScale(eX, eY, eZ);
        }
        if( strcmp ( childType,  "modelos" ) == 0 ){
          // block: modelo
          TiXmlHandle oRoot(gElem);
          TiXmlElement* pElem;
          pElem=oRoot.FirstChild( "modelo" ).Element();
          std::string objectFile;
          std::string objectName;
          std::string textureFilePath;
          float diffR, diffG, diffB;
          int diffR_status, diffG_status, diffB_status;

          for( ; pElem; pElem=pElem->NextSiblingElement()){
            objectFile = pElem->Attribute("ficheiro");
            objectName = pElem->Attribute("nome");
            std::cout << "Reading: "<< objectName << " from " <<  objectFile << "\n";
            std::unique_ptr<Model> uniqueModelPointer ( new Model (objectName , objectFile) );

            // querie for texture
            pElem->QueryStringAttribute("textura", &textureFilePath);
            if ( textureFilePath.length() > 0 ){
              std::cout << "\tTexture File: "<< textureFilePath << "\n";
			  uniqueModelPointer->enableTextureVector();
              uniqueModelPointer->setTexture( textureFilePath );
            }

            // querie for rgb diffuse
            diffR_status = pElem->QueryFloatAttribute("diffR", &diffR);
            diffG_status = pElem->QueryFloatAttribute("diffG", &diffG);
            diffB_status = pElem->QueryFloatAttribute("diffB", &diffB);
            if ( diffR_status == TIXML_SUCCESS && diffG_status == TIXML_SUCCESS && diffG_status == TIXML_SUCCESS ){
              std::cout << "\tDifuse color: R:"<< diffR << " G:"  << diffG << " B:" << diffB << "\n";
              uniqueModelPointer->setRGBDiffuse( diffR , diffG, diffB );
            }

            //load
            uniqueModelPointer->load();
            g1->addModel(*uniqueModelPointer);
          }
        }
        if( strcmp ( childType,  "grupo" ) == 0 ){
          TiXmlHandle gRoot(gElem);
          TiXmlElement* iElem;
          iElem=gRoot.FirstChildElement().Element();
          std::unique_ptr<Group> g0 (new Group(*g1));
          loadGroup(iElem, &(*g0));
          groupVector.push_back( std::move(g0));
        }
      }
    }

    void load( std::string ceneFilename ){
      TiXmlDocument doc(ceneFilename.c_str());
      //se o ficheiro nao existir
      if (!doc.LoadFile()) {
        std::cout << "The file: " << ceneFilename << " doesn't exist!\n";
        //return;
      }
      TiXmlHandle hDoc(&doc);
      TiXmlElement* hElem;
      TiXmlHandle hRoot(0);
      hElem=hDoc.FirstChildElement().Element();
      if (!hElem) return;
      ceneName=hElem->Attribute("nome");
      std::cout << "Rendering: " << ceneName <<"\n";
      hRoot=TiXmlHandle(hElem);
      // block: grupo ou luzes
      hElem=hRoot.FirstChildElement().Element();
      int i = 0;
      for (; hElem !=NULL ;  hElem=hElem->NextSiblingElement()) {
        const char *groupType = hElem->Value();
        if (strcmp(groupType, "grupo") == 0){
          TiXmlHandle gRoot(hElem);
          TiXmlElement* gElem;
          gElem = gRoot.FirstChildElement().Element();
          std::unique_ptr<Group> uniqueGroup(new Group(std::to_string(i)));
          loadGroup(gElem, &(*uniqueGroup));
          groupVector.push_back(std::move(uniqueGroup));
        }
        if (strcmp(groupType, "luzes") == 0){
          TiXmlHandle lRoot(hElem);
          TiXmlElement* lElem;

          lElem = lRoot.FirstChild("luz").Element();
          for (; lElem !=NULL ;  lElem=lElem->NextSiblingElement()) {
            float position[4];
            float ambientLight[4];
            float diffuseLight[4];
            float specularLight[4];
            std::string lightType;
            // position of light
            lElem->QueryFloatAttribute("posX", &position[0]);
            lElem->QueryFloatAttribute("posY", &position[1]);
            lElem->QueryFloatAttribute("posZ", &position[2]);
            lElem->QueryFloatAttribute("posV", &position[3]);
            // ambient light
            lElem->QueryFloatAttribute("ambR", &ambientLight[0]);
            lElem->QueryFloatAttribute("ambG", &ambientLight[1]);
            lElem->QueryFloatAttribute("ambB", &ambientLight[2]);
            lElem->QueryFloatAttribute("ambA", &ambientLight[3]);
            // diffuse light
            lElem->QueryFloatAttribute("diffR", &diffuseLight[0]);
            lElem->QueryFloatAttribute("diffG", &diffuseLight[1]);
            lElem->QueryFloatAttribute("diffB", &diffuseLight[2]);
            lElem->QueryFloatAttribute("diffA", &diffuseLight[3]);
            // specular light
            lElem->QueryFloatAttribute("specR", &specularLight[0]);
            lElem->QueryFloatAttribute("specG", &specularLight[1]);
            lElem->QueryFloatAttribute("specB", &specularLight[2]);
            lElem->QueryFloatAttribute("specA", &specularLight[3]);
            lightType = lElem->Attribute("tipo");
            std::cout << "Adding light of type: " << lightType << "\tposition: " << position <<
              "\tambient: " << ambientLight[0]  << 
              "\tdiffuse: " << diffuseLight <<
              "\tspecular: " << specularLight << "\n";
            std::unique_ptr<Light> uniqueLight(new Light( lightType ));

            uniqueLight->setAmbientLight(ambientLight);
            uniqueLight->setDiffuseLight(diffuseLight);
            uniqueLight->setSpecularLight(specularLight);
            uniqueLight->setPosition(position);

            lightsVector.push_back(std::move(uniqueLight));
          }
        }
        i++;
      }
    }

    //função retirada dos apontamentos do professor
    void changeSize(int w, int h) {

      // Prevent a divide by zero, when window is too short
      // (you cant make a window with zero width).
      if (h == 0)
        h = 1;

      // compute window's aspect ratio 
      float ratio = w * 1.0 / h;

      // Set the projection matrix as current
      glMatrixMode(GL_PROJECTION);
      // Load Identity Matrix
      glLoadIdentity();

      // Set the viewport to be the entire window
      glViewport(0, 0, w, h);

      // Set perspective
      gluPerspective(45.0f, ratio, 10.0f, 1000000.0f);

      // return to the model view matrix mode
      glMatrixMode(GL_MODELVIEW);
    }

    void prepairObjects(void){
      std::vector<Model> modelsVector;
      std::vector<Model>::iterator modelIt;

      groupIt = groupVector.begin();

      // iterate through every group of scene
      for ( ; groupIt != groupVector.end(); ++groupIt){
        //iterate through every model of that group
        modelsVector = (*groupIt)->getModels();
        modelIt = modelsVector.begin();
        for (; modelIt != modelsVector.end(); ++modelIt){
          std::vector<GLfloat> pointsVector = modelIt->getPointsVector();
          glGenBuffers(1, &arrayVBOS[posArrayVBOS]);
          glBindBuffer(GL_ARRAY_BUFFER, arrayVBOS[posArrayVBOS]);
          glBufferData(GL_ARRAY_BUFFER,
              pointsVector.size()*sizeof(GLfloat),
              pointsVector.data(),
              GL_STATIC_DRAW);
          sizeArrayVBOS[posArrayVBOS] = pointsVector.size();

          // normal vector
          if (modelIt->isNormalVectorDefined()){
            std::vector<GLfloat> normalVector = modelIt->getNormalVector();
            glGenBuffers(1, &arrayNormalVBOS[posArrayVBOS]);
            glBindBuffer(GL_ARRAY_BUFFER, arrayNormalVBOS[posArrayVBOS]);
            glBufferData(GL_ARRAY_BUFFER,
                normalVector.size()*sizeof(GLfloat),
                normalVector.data(),
                GL_STATIC_DRAW);
            sizeArrayNormalVBOS[posArrayVBOS] = normalVector.size();
          }

		  // texture vector
		  if (modelIt->isTextureVectorDefined() && modelIt->isTextureVectorEnabled() ){
			  std::vector<GLfloat> textureVector = modelIt->getTextureVector();
			  glGenBuffers(1, &arrayTexturesVBOS[posArrayVBOS]);
			  glBindBuffer(GL_ARRAY_BUFFER, arrayTexturesVBOS[posArrayVBOS]);
			  glBufferData(GL_ARRAY_BUFFER,
				  textureVector.size()*sizeof(GLfloat),
				  textureVector.data(),
				  GL_STATIC_DRAW);
			  sizeArrayTextureVBOS[posArrayVBOS] = textureVector.size();

			  unsigned int tw, th;
			  unsigned char *texData;

			  ilGenImages(1, &ima[posArrayVBOS]); // unsigned int ima[...] 
			  ilBindImage(ima[posArrayVBOS]);
			  ilLoadImage((ILstring) modelIt->getTextureFilename().c_str());

			  tw = ilGetInteger(IL_IMAGE_WIDTH);
			  th = ilGetInteger(IL_IMAGE_HEIGHT);
			  printf("%d %d", tw, th);
			  ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			  texData = ilGetData();

			  glGenTextures(1, &texID[posArrayVBOS]); 
			  glBindTexture(GL_TEXTURE_2D, texID[posArrayVBOS]);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

		  }

          glBindBuffer(GL_ARRAY_BUFFER, arrayVBOS[posArrayVBOS]);
          posArrayVBOS++;
        }
      }
    }

    void initGL(void){

      // alguns settings para OpenGL
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
      glEnable(GL_TEXTURE_2D);
      //glEnable(GL_NORMALIZE);
      lightsIt = lightsVector.begin();
      for (int lightN = 0; lightsIt != lightsVector.end(); ++lightsIt, lightN++){
        std::cout << "enabling light: " << lightN << "\n";
        glEnable(GL_LIGHT0);
      }
      // init
	  glEnableClientState(GL_NORMAL_ARRAY);
      glEnableClientState(GL_VERTEX_ARRAY);
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);


      prepairObjects();
    }

    void drawObjects( int time ) {
      groupIt = groupVector.begin();
      lightsIt = lightsVector.begin();
      for (int lightN = 0 ; lightsIt != lightsVector.end(); ++lightsIt, lightN++){
        glLightfv(GL_LIGHT0, GL_POSITION, (*lightsIt)->getPosition());
        glLightfv(GL_LIGHT0, GL_AMBIENT, (*lightsIt)->getAmbientLight());
        glLightfv(GL_LIGHT0, GL_DIFFUSE, (*lightsIt)->getDiffuseLight());
        glLightfv(GL_LIGHT0, GL_SPECULAR, (*lightsIt)->getSpecularLight());
      }
      for ( int pos = 0 ; groupIt != groupVector.end(); ++groupIt ){

        //if animation is set ON
        if( animateBezier ){
          (*groupIt)->groupRotateAnimate( float ( time ) );
          (*groupIt)->groupBezierTranslate( float ( time ) );
        }

        //load camera transformation into the matrix
        (*groupIt)->loadTransformations(pitchX, headingY, roolZ,
            camPosX, camPosY, camPosZ);

        std::vector<Model> modelsVector;
        std::vector<Model>::iterator modelIt;

        modelsVector = (*groupIt)->getModels();
        modelIt = modelsVector.begin();
        for (; modelIt != modelsVector.end(); ++modelIt, pos++){
            glBindBuffer(GL_ARRAY_BUFFER, arrayVBOS[pos]);

		  if (modelIt->isTextureVectorEnabled()){
			  glBindTexture(GL_TEXTURE_2D, texID[pos]);
		  }

          glVertexPointer(3, GL_FLOAT, 0, 0);
          if (modelIt->isNormalVectorDefined()){
          
            glBindBuffer(GL_ARRAY_BUFFER, arrayNormalVBOS[pos]);
            glNormalPointer(GL_FLOAT, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
    
          }

		  if (modelIt->isTextureVectorEnabled()){
			  glBindBuffer(GL_ARRAY_BUFFER, arrayTexturesVBOS[pos]);
			  glTexCoordPointer(2, GL_FLOAT, 0, 0);

		  }

          //draw
       //   std::cout << "drawing: \n",
            //glBindBuffer(GL_ARRAY_BUFFER, arrayVBOS[pos]);
          glDrawArrays(GL_TRIANGLES, 0, sizeArrayVBOS[pos]);
        }

        //reset textures
        glBindTexture(GL_TEXTURE_2D, 0);
      }
    }

    void renderScene(void) {
      int time;

      glClearColor(0.0f,0.0f,0.0f,0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
      time = glutGet(GLUT_ELAPSED_TIME);

      // extra animate x,y,z axis
      if (animateX) {
        pitchX++;
        if (pitchX > 360.0f)
          pitchX = -360.0f;
      }
      if (animateY) {
        headingY++;
        if (headingY > 360.0f)
          headingY = -360.0f;
      }
      if (animateZ) {
        roolZ++ ;
        if (roolZ > 360.0f)
          roolZ = -360.0f;
      }

      drawObjects( time );

      glutSwapBuffers();
    }

    // special keys processing function -- função retirada do blog do professor e alterada
    void processKeys(int key, int xx, int yy)
    {
      switch (key) {

        case GLUT_KEY_RIGHT:
          headingY += 5.0f;
          if (headingY > 360.0f)
            headingY = -360.0f;
          break;

        case GLUT_KEY_LEFT:
          headingY -= 5.0f;
          if (headingY < -360.0f)
            headingY = 360.0f;
          break;

        case GLUT_KEY_UP:
          pitchX += 5.0f;
          if (pitchX > 360.0f)
            pitchX = -360.0f;
          break;

        case GLUT_KEY_DOWN:
          pitchX -= 5.0f;
          if (pitchX < -360.0f)
            pitchX = 360.0f;
          break;
      }
      glutPostRedisplay();
    }

    void processKeyboard (unsigned char key, int x, int y)
    {
      switch (key) {
        // switch rendering modes (fill -> wire -> point)
        case '1' : //fill mode
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          glEnable(GL_DEPTH_TEST);
          glEnable(GL_CULL_FACE);
          break;
        case '2' : //wireframe mode
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
          glDisable(GL_DEPTH_TEST);
          glDisable(GL_CULL_FACE);
          break;
        case '3' : //point mode
          glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
          glDisable(GL_DEPTH_TEST);
          glDisable(GL_CULL_FACE);
          break;

        case 'w' : camPosZ -= 50;  break;
        case 's' : camPosZ += 50;  break;

        case 'a' : camPosX -= 50;  break;
        case 'd' : camPosX += 50;  break;

        case 'q' : camPosY -= 50;  break;
        case 'e' : camPosY += 50;  break;

        case 'g' : roolZ -= 1;  break;
        case 't' : roolZ += 1;  break;

        case 'r' : resetCameraParameters();  break;
        case 'x' : if ( animateX ) animateX = false; else animateX = true;  break;
        case 'y' : if ( animateY ) animateY = false; else animateY = true;  break;
        case 'z' : if ( animateZ ) animateZ = false; else animateZ = true;  break;
        case 'b' : if ( animateBezier ) animateBezier = false; else animateBezier = true;  break;

        case '0' : exit(EXIT_SUCCESS);  break;

      }

      glutPostRedisplay();
    }
};

#endif /* _ENGINE_H_INCLUDED_ */
