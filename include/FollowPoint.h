#ifndef FOLLOWPOINT_H__
#define FOLLOWPOINT_H__

#include <ngl/VAOPrimitives.h>

class FollowPoint
{
public:

  FollowPoint();

  ~FollowPoint();

  void updatePosition(ngl::Vec3 _newPos);

protected:

  ngl::Vec3 m_position;

};

#endif
