#include "FollowPoint.h"

FollowPoint::FollowPoint()
{
  std::cout<<"trying to load ";
  m_Mesh.load("models/Sphere.obj", true);
  m_Mesh.createVAO();
//  m_Position = ngl::Vec3(0.1, -0.1, 1.0);
  std::cout<<"it has done me a load ";
}

FollowPoint::~FollowPoint()
{

}

//void FollowPoint::updatePosition(ngl::Vec3 _newPos)
//{
//  m_Position = _newPos;
//}

void FollowPoint::draw()
{
  m_Mesh.draw();
}
