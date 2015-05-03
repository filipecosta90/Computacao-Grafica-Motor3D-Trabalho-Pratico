/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef testeGLEW_bezier_hpp
#define testeGLEW_bezier_hpp

#include "point.hpp"

/**Computes factorial*/
int fact( int k){
  if(k==0 || k==1){
    return 1;
  }
  else{
    return k * fact(k-1);
  }
}

/**Computes Bernstain
 *parameter int i - the i-th index
 *parameter int n - the total number of points
 *parameter float t - the value of parameter t , between 0 and 1
 **/
float Berstain( int i, int n, float t){
  return fact(n) / (fact(i) * fact(n-i))* pow(t, i) * pow(1-t, n-i);
}

/**Computes a point's coordinates for a value of t
 *parameter float t - a value between 0 and 1
 *parameter std::vector<points> - an vector of [x,y,z] coodinates. The initial points
 **/
Point getBezierAtTime( float t, std::vector<Point> points){
  Point r ( 0.0f , 0.0f , 0.0f) ;
  int n = points.size()-1;
  for(int i=0; i <= n; i++){
    r.x += points[i].x * Berstain( i, n, t);
    r.y += points[i].y * Berstain( i, n, t);
    r.z += points[i].z * Berstain (i, n, t);
  }
  return r;
}

#endif
