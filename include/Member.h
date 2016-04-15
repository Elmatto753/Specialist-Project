#ifndef MEMBER_H__
#define MEMBER_H__

#include <iostream>
#include <ngl/Obj.h>

class Member
{
public:

  Member();

  ~Member();

  virtual void draw();

  virtual void update();

  ngl::Vec3 getPosition() { return m_Position; }
  ngl::Vec3 getVelocity() { return m_Velocity; }
  void setPosition(ngl::Vec3 newPosition, bool replace);
  void setVelocity(ngl::Vec3 newVelocity, bool replace);

  ngl::Vec3 getForwardVector() { return m_ForwardVector; }
  ngl::Vec3 getSideVector() { return m_SideVector; }

  void setForwardVector(ngl::Vec3 newFV);
  void setSideVector(ngl::Vec3 newSV);

  float getxRot() { return m_xRot; }
  float getyRot() { return m_yRot; }

  void setxRot(float newX);
  void setyRot(float newY);

  void calcVectors();
  void calcForwardVector();
  void calcSideVector();

protected:

  ngl::Obj m_Mesh;
  ngl::Vec3 m_Position;
  ngl::Vec3 m_Velocity;
  ngl::Vec3 m_ForwardVector;
  ngl::Vec3 m_SideVector;

  float m_xRot;
  float m_yRot;



};

#endif
