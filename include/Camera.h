#ifndef CAMERA_H__
#define CAMERA_H__

#include <ngl/Obj.h>

class Camera
{
public:

  Camera();

  ~Camera();

  void moveCamera(float _forward, float _right);
  void rotateCamera(float _up, float _right);

  ngl::Vec3 getForwardVector() { return m_forwardVector; }
  ngl::Vec3 getSideVector() { return m_sideVector; }

protected:

  ngl::Vec3 m_position;

  float m_xRot;
  float m_yRot;

  ngl::Vec3 m_forwardVector;
  ngl::Vec3 m_sideVector;

  void calcVectors();


};


#endif
