#ifndef AVOIDSHAPES_H__
#define AVOIDSHAPES_H__

#include "Member.h"
#include <ngl/Obj.h>

class AvoidShapes : public Member
{
public:

  AvoidShapes();
  ~AvoidShapes();

  std::vector<AvoidShapes> ShapeList;

protected:

};

class SphereShape : public AvoidShapes
{
public:
  SphereShape();
  ~SphereShape();


};

class PyramidShape : public AvoidShapes
{
public:
  PyramidShape();
  ~PyramidShape();


};

class WallShape : public AvoidShapes
{
public:
  WallShape();
  ~WallShape();


};

class TorusShape : public AvoidShapes
{
public:
  TorusShape();
  ~TorusShape();


};


#endif
