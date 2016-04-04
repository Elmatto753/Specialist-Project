#ifndef CAMERA_H__
#define CAMERA_H__

#include <ngl/Obj.h>

class Camera
{
public:

  Camera();

  ~Camera();

protected:

  ngl::Vec3 m_Position;

  float m_xRot;
  float m_yRot;

  ngl::Vec3 m_forwardVector;
  ngl::Vec3 m_sideVector;

  void moveCamera(float _Forward, float _Right);
  void rotateCamera(float _Up, float _Right);

  void calcVectors();


};


#endif
