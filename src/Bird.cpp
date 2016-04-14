#include "Bird.h"
#include "SwarmFactory.h"

Bird::Bird()
{
  m_Mesh.load("models/Bird.obj", true);
  m_Mesh.createVAO();
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
  m_Position = ngl::Vec3(dist(mt), dist(mt), dist(mt));
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


