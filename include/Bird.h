#ifndef BIRD_H__
#define BIRD_H__
#include "Member.h"
#include <random>

class Bird : public Member
{
public:

  Bird();

  ~Bird();

  void draw();

  void update();

  ngl::Vec3 calcAlignment();
  ngl::Vec3 calcCohesion();
  ngl::Vec3 calcSeparation();



protected:

  ngl::Vec3 m_Velocity;

};

#endif
