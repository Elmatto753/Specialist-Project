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

protected:

  ngl::Obj m_Mesh;
  ngl::Vec3 m_Position;


};

#endif
