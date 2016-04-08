#include "Bird.h"

Bird::Bird()
{
  m_Mesh.load("models/Bird.obj", true);
  m_Mesh.createVAO();
  std::cout<<"made une birde\n";
}

Bird::~Bird()
{

}

void Bird::draw()
{

}

void Bird::update()
{

}
