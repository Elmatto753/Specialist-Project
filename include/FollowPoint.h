#ifndef FOLLOWPOINT_H__
#define FOLLOWPOINT_H__

#include "Member.h"
#include <ngl/VAOPrimitives.h>

class FollowPoint : public Member
{
public:

  FollowPoint();

  ~FollowPoint();

  ngl::Vec3 getPosition() { return m_Position; }

  void updatePosition(ngl::Vec3 _newPos);

  void draw();

protected:

  //ngl::Vec3 m_Position;

};

#endif
