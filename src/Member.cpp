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
