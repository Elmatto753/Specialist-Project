#include "Member.h"

Member::Member()
{
  std::cout<<"Member not deded \n";
}

Member::~Member()
{
  std::cout<<"Member deded \n";
}

void Member::draw()
{
  m_Mesh.draw();
}

void Member::update()
{

}

void Member::setPosition(ngl::Vec3 newPosition, bool replace)
{
  if(replace == true)
  {
    m_Position = newPosition;
  }

  else
  {
    m_Position += newPosition;
  }
}

void Member::setVelocity(ngl::Vec3 newVelocity, bool replace)
{
  if(replace == true)
  {
    m_Velocity = newVelocity;
  }

  else
  {
    m_Velocity += newVelocity;
  }
}

void Member::setxRot(float newX)
{
  m_xRot = newX;
}

void Member::setyRot(float newY)
{
  m_yRot = newY;
}

void Member::setForwardVector(ngl::Vec3 newFV)
{
  m_ForwardVector = newFV;
}

void Member::setSideVector(ngl::Vec3 newSV)
{
  m_SideVector = newSV;
}

void Member::calcVectors()
{
  // Calculate the bird's forward vector based on its angles of rotation
  m_ForwardVector = m_Position + ngl::Vec3(-(cos(m_yRot) * sin(m_xRot)),
                             -(cos(m_xRot)),
                             sin(m_yRot) * sin(m_xRot))*360;
  // Cross the forward vector with the constant up vector to get the side vector
  m_SideVector = m_ForwardVector.cross(ngl::Vec3(0.0f,1.0f,0.0f));

  if((m_SideVector.lengthSquared()) != 0.0f)
  {
    //m_forwardVector.normalize();
    m_SideVector.normalize();
  }
}

void Member::calcForwardVector()
{
  m_ForwardVector = m_SideVector.cross((ngl::Vec3(0.0f,1.0f,0.0f)));

}

void Member::calcSideVector()
{
  m_SideVector = m_ForwardVector.cross(ngl::Vec3(0.0f,1.0f,0.0f));

  if(m_SideVector.lengthSquared() != 0.0f)
  {
    //m_forwardVector.normalize();
    m_SideVector.normalize();
  }

}
