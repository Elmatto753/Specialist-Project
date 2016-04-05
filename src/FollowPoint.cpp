#include "FollowPoint.h"

FollowPoint::FollowPoint()
{

}

FollowPoint::~FollowPoint()
{

}

void FollowPoint::updatePosition(ngl::Vec3 _newPos)
{
  m_position = _newPos;
}
