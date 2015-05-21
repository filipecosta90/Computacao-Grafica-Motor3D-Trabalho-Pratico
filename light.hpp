/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _LIGHT_H_INCLUDED_
#define _LIGHT_H_INCLUDED_
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

class Light {
  public:
    std::string type;
    Point position = Point ( 0.0f , 0.0f , 0.0f ) ;

    Light ( std::string typeN , Point positionN ) {
      type = typeN;
	  position = positionN;
    }
};

#endif /* _MODEL_H_INCLUDED_ */