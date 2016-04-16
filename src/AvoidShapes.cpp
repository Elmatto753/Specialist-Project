#include "AvoidShapes.h"

AvoidShapes::AvoidShapes()
{
  ShapeList.push_back(*this);
  std::cout<<"Made a shape";
}

AvoidShapes::~AvoidShapes()
{

}

SphereShape::SphereShape()
{
  m_Mesh.load("models/Sphere.obj", true);
  m_Mesh.createVAO();
  m_Position = ngl::Vec3(140.0f, 100.0f, 100.0f);
  std::cout<<"Made sphere";
}

SphereShape::~SphereShape()
{

}

PyramidShape::PyramidShape()
{
  m_Mesh.load("models/Pyramid.obj", true);
  m_Mesh.createVAO();
  m_Position = ngl::Vec3(100.0f, 100.0f, 140.0f);
}

PyramidShape::~PyramidShape()
{

}

WallShape::WallShape()
{
  m_Mesh.load("models/Wall.obj", true);
  m_Mesh.createVAO();
  m_Position = ngl::Vec3(60.0f, 100.0f, 100.0f);
}

WallShape::~WallShape()
{

}

TorusShape::TorusShape()
{
  m_Mesh.load("models/Torus.obj", true);
  m_Mesh.createVAO();
  m_Position = ngl::Vec3(100.0f, 100.0f, 60.0f);
}

TorusShape::~TorusShape()
{

}
