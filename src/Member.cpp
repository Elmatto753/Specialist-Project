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
