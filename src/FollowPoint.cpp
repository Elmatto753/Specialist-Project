#include "FollowPoint.h"

FollowPoint::FollowPoint()
{
  std::cout<<"trying to load ";
  m_Mesh.load("models/Bird.obj", true);
  m_Mesh.createVAO();
  std::cout<<"it has done me a load ";
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
