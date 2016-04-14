#include "Camera.h"

Camera::Camera()
{
  m_Position = ngl::Vec3(10.0f, 0.0f, 0.0f);
  m_Look = ngl::Vec3(1.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{

}

void Camera::setxRot(float newX)
{
  m_xRot = newX;
}

void Camera::setyRot(float newY)
{
  m_yRot = newY;
}

void Camera::moveCamera(float _forward, float _right)
{
  calcVectors();
  m_Position += _forward * m_forwardVector;
  m_Position += _right * m_sideVector;
}

void Camera::rotateCamera(float _up, float _right)
{
  // Increment the player's rotation by this amount
  m_xRot = (m_xRot + (_right / 500));
  m_yRot = (m_yRot + (_up / 500));
//  std::cout<<"Camera rotated ";

  m_Look = m_Position + ngl::Vec3(-(cos(m_yRot) * sin(m_xRot)),
                     -(cos(m_xRot)),
                     (sin(m_yRot) * sin(m_xRot)))*360;
}

void Camera::calcVectors()
{
  // Calculate the camera's forward vector based on its angles of rotation
  m_forwardVector = m_Position + ngl::Vec3(-(cos(m_yRot) * sin(m_xRot)),
                             -(cos(m_xRot)),
                             sin(m_yRot) * sin(m_xRot))*360;
  // Cross the forward vector with the constant up vector to get the side vector
  m_sideVector = m_forwardVector.cross(ngl::Vec3(0,1,0));

  if((m_sideVector.lengthSquared()) != 0.0f)
  {
    //m_forwardVector.normalize();
    m_sideVector.normalize();
  }
}
