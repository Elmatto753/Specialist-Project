#include "Camera.h"

Camera::Camera()
{
  std::cout<<"test";
}

Camera::~Camera()
{

}


void Camera::moveCamera(float _Forward, float _Right)
{
  m_Position += _Forward * m_forwardVector;
  m_Position += _Right * m_sideVector;
}

void Camera::rotateCamera(float _Up, float _Right)
{

}

void Camera::calcVectors()
{
  // Calculate the player's forward vector based on their angles of rotation
  m_forwardVector = ngl::Vec3(-(cos(m_xRot) * sin(m_yRot)),
                              -(cos(m_yRot)),
                              sin(m_xRot) * sin(m_yRot));
  // Cross the forward vector with the constant up vector to get the side vector
  m_sideVector = m_forwardVector.cross(ngl::Vec3(0,1,0));

  if((m_sideVector.lengthSquared()) != 0.0f)
  {
    m_sideVector.normalize();
  }
}
