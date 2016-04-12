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

void Member::setVelocity(ngl::Vec3 newVelocity, bool replace)
{
  if(replace == true)
  {
    m_Velocity = newVelocity;
  }

  else
  {
    { m_Velocity += newVelocity; }
  }
}
