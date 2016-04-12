#include "Bird.h"
#include "SwarmFactory.h"

Bird::Bird()
{
  m_Mesh.load("models/Bird.obj", true);
  m_Mesh.createVAO();
  m_Position = ngl::Vec3(0.0f, 0.0f, 0.0f);
  m_Velocity = ngl::Vec3(0.0f, 0.0f, 0.0f);
  std::cout<<"made une birde\n";
}

Bird::~Bird()
{
  std::cout<<"Bird deded \n";
}

void Bird::draw()
{
  m_Mesh.draw();
}

void Bird::update()
{

}


