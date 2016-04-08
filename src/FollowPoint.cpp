#include "FollowPoint.h"

FollowPoint::FollowPoint()
{
  m_Mesh.load("models/Bird.obj", true);
  m_Mesh.createVAO();
}

FollowPoint::~FollowPoint()
{

}

void FollowPoint::updatePosition(ngl::Vec3 _newPos)
{
  m_Position = _newPos;
}

void FollowPoint::draw()
{
  m_Mesh.draw();
}
