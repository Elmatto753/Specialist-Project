#include "Bird.h"

Bird::Bird()
{
  m_Mesh.load("models/Bird.obj", true);
  m_Mesh.createVAO();
  std::cout<<"made une birde\n";
}

Bird::~Bird()
{
  std::cout<<"Bird deded \n";
}

void Bird::draw()
{

}

void Bird::update()
{
  m_Mesh.draw();
}
