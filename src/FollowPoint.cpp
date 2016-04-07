#include "FollowPoint.h"

FollowPoint::FollowPoint()
{
  m_Mesh.load("models/Sphere.obj", true);
  m_Mesh.createVAO();
  m_Position = ngl::Vec3(1.0f, 1.0f, 1.0f);
}

FollowPoint::~FollowPoint()
{

}

void FollowPoint::updatePosition(ngl::Vec3 _newPos)
{
  m_Position = _newPos;
}
