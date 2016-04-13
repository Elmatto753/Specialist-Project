#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <iostream>

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Swarm");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}


void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  // Initialise and set up the shaders with NGL
  ngl::ShaderLib *shader =ngl::ShaderLib::instance();

  shader->createShaderProgram("Colour");
  shader->attachShader("ColourVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("ColourFragment",ngl::ShaderType::FRAGMENT);

  shader->loadShaderSource("ColourVertex","shaders/VertShader.glsl");
  shader->loadShaderSource("ColourFragment","shaders/FragShader.glsl");

  shader->compileShader("ColourVertex");
  shader->compileShader("ColourFragment");

  shader->attachShaderToProgram("Colour","ColourVertex");
  shader->attachShaderToProgram("Colour","ColourFragment");
  shader->linkProgramObject("Colour");
  (*shader)["Colour"]->use();
  shader->autoRegisterUniforms("Colour");
  shader->printProperties();
  shader->printRegisteredUniforms("Colour");

  shader->setRegisteredUniform("Colour",1.0f,1.0f,1.0f,0.0f);
  shader->setRegisteredUniform("lightPos",m_lightPos);
  shader->setRegisteredUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);

  // Create the projection matrix
  m_proj=ngl::perspective(90.0f,float(width()/height()),0.1,20);
  m_view=ngl::lookAt(cam.getPosition(), cam.getLook(), ngl::Vec3(0.0f, 1.0f, 0.0f));

  followSphere = new FollowPoint;

  startTimer(10);

}

void NGLScene::loadToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Colour");

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix()/**m_mouseGlobalTX*/;
//  MV=  M*ngl::Mat4(cam.getSideVector().m_x, cam.getSideVector().m_y, cam.getSideVector().m_z, 0.0f,
//                   0.0f,1.0f,0.0f,0.0f,
//                   cam.getForwardVector().m_x, cam.getForwardVector().m_y, cam.getForwardVector().m_z, 0.0f,
//                   1.0f,1.0f,1.0f,1.0f);
  m_view = ngl::lookAt(cam.getPosition(), cam.getLook(), ngl::Vec3(0.0f, 1.0f, 0.0f));
  MV = M*m_view;
  MVP = MV*m_proj;
  normalMatrix=MV;
  normalMatrix.inverse();
//  shader->setShaderParamFromMat4("MV",MV);
//  shader->setShaderParamFromMat4("MVP",MVP);
//  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
//  shader->setShaderParamFromMat4("M",M);
  shader->setUniform("MVP",MVP);
  shader->setShaderParam4f("Colour", 1.0f, 0.1f, 0.1f, 1.0f);
}

void NGLScene::drawMember(Member &_toDraw)
{
  _toDraw.draw();
  std::cout<<"drawn\n";
}

void NGLScene::updateMember(Member &io_toUpdate)
{
  ngl::Vec3 alignment = calcAlignment(io_toUpdate);
  ngl::Vec3 cohesion = calcCohesion(io_toUpdate);
  ngl::Vec3 separation = calcSeparation(io_toUpdate);
  std::cout<<"alignment = "<<alignment.m_x<<"\n";
  std::cout<<"cohesion = "<<cohesion.m_x<<"\n";
  std::cout<<"separation = "<<separation.m_x<<"\n";
  io_toUpdate.setVelocity(ngl::Vec3(alignment + cohesion + separation)/200, false);
  std::cout<<"Velocity "<<io_toUpdate.getVelocity().m_x<<"\n";
  io_toUpdate.setPosition(io_toUpdate.getVelocity(), false);
}

float NGLScene::calcDistance(ngl::Vec3 _vector1, ngl::Vec3 _vector2)
{
  float dX = _vector2.m_x - _vector1.m_x;
  float dY = _vector2.m_y - _vector1.m_y;
  float dZ = _vector2.m_z - _vector1.m_z;
  std::cout<<"returning "<<sqrt(dX*dX + dY*dY + dZ*dZ)<<"\n";
  return sqrt(dX*dX + dY*dY + dZ*dZ);
}

ngl::Vec3 NGLScene::calcAlignment(Member &_toCalc)
{
  unsigned int neighbours = 0;
  ngl::Vec3 alignVec = ngl::Vec3(0.0f, 0.0f, 0.0f);
  ngl::Vec3 memPos = _toCalc.getPosition();
  for(unsigned int i = 0; i < makerBirds.BirdID.size(); i++)
  {
    float dist = calcDistance(getMemberPosition(*makerBirds.BirdID[i]), memPos);
    std::cout<<"BirdID pos = "<<getMemberPosition(*makerBirds.BirdID[i]).m_x<<" "<<getMemberPosition(*makerBirds.BirdID[i]).m_y<<" "<<getMemberPosition(*makerBirds.BirdID[i]).m_z<<"\n";

    std::cout<<"dist = "<<dist<<"\n";
    if(dist < 50.0f && dist != 0.0f )
    {
      alignVec += getMemberVelocity(*makerBirds.BirdID[i]);
      neighbours++;
      std::cout<<"You've got mail \n";
    }
  }
  if(neighbours == 0)
  {
    std::cout<<"No neighbours\n";
    return alignVec;
  }

  alignVec /= neighbours;
  if(alignVec.length() != 0.0f)
  {
    alignVec.normalize();
  }
  std::cout<<"Some neighbours\n";
  return alignVec;
}

ngl::Vec3 NGLScene::calcCohesion(Member &_toCalc)
{
  unsigned int neighbours = 0;
  ngl::Vec3 coVec = ngl::Vec3(0.0f, 0.0f, 0.0f);
  ngl::Vec3 memPos = _toCalc.getPosition();
  for(unsigned int i = 0; i < makerBirds.BirdID.size(); i++)
  {
    float dist = calcDistance(getMemberPosition(*makerBirds.BirdID[i]), memPos);
    if(dist < 50.0f && dist != 0.0f )
    {
      coVec += getMemberPosition(*makerBirds.BirdID[i]);
      neighbours++;
    }
  }
  if(neighbours == 0)
  {
    return coVec;
  }

  coVec /= neighbours;
  coVec = coVec - getMemberPosition(_toCalc);
  coVec.normalize();
  return coVec;
}

ngl::Vec3 NGLScene::calcSeparation(Member &_toCalc)
{
  unsigned int neighbours = 0;
  ngl::Vec3 sepVec = ngl::Vec3(0.0f, 0.0f, 0.0f);
  ngl::Vec3 memPos = _toCalc.getPosition();
  for(unsigned int i = 0; i < makerBirds.BirdID.size(); i++)
  {
    float dist = calcDistance(getMemberPosition(*makerBirds.BirdID[i]), memPos);
    if(dist < 50.0f && dist != 0.0f )
    {
      sepVec.m_x += memPos.m_x - getMemberPosition(*makerBirds.BirdID[i]).m_x;
      sepVec.m_y += memPos.m_y - getMemberPosition(*makerBirds.BirdID[i]).m_y;
      sepVec.m_z += memPos.m_z - getMemberPosition(*makerBirds.BirdID[i]).m_z;
      neighbours++;
    }
  }
  if(neighbours == 0)
  {
    return sepVec;
  }

  std::cout<<"neighbours: "<<neighbours<<"\n";
  sepVec /= neighbours;
  sepVec = sepVec - getMemberPosition(_toCalc);
  sepVec *= -1;
  sepVec.normalize();
  return sepVec;
}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);
  m_transform.setPosition(followSphere->getPosition());
//  loadToShader();
//  followSphere->draw();
  for(unsigned int i = 0; i < makerBirds.BirdID.size(); i++)
  {
    m_transform.setPosition(getMemberPosition(*makerBirds.BirdID[i]));
    loadToShader();
    drawMember(*makerBirds.BirdID[i]);
    updateMember(*makerBirds.BirdID[i]);
  }
//  std::cout<<"cam "<<cam.getPosition().m_x<<"\n";
//  std::cout<<"sphere "<<followSphere->getPosition().m_x<<"\n";

}

void NGLScene::timerEvent(QTimerEvent *)
{
  update();
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  // Get the amount the mouse has moved
  ngl::Vec2 o_mouseMovement = ngl::Vec2(width()/2 - _event->x(), height()/2 - _event->y());
  if(cam.getForwardVector().m_y>=1.0f)
  {
    cam.rotateCamera(-0.01f, 0.0f);
  }

  if(cam.getForwardVector().m_y<=-1.0f)
  {
    cam.rotateCamera(0.01f, 0.0f);
  }
  // Increment the player's rotation by this amount
  cam.rotateCamera(o_mouseMovement.m_x, o_mouseMovement.m_y);
  // Return the mouse pointer to the screen centre and update
  QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
  QCursor::setPos(glob);

  cam.calcVectors();
  std::cout<<"x is "<<cam.getForwardVector().m_x<<"\n";
  std::cout<<"y is "<<cam.getForwardVector().m_y<<"\n";
//  followSphere->updatePosition(cam.getForwardVector() * 20);
//  followSphere->draw();

  update();
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space : makerBirds.Produce(); std::cout<<"registered \n "; break;
  case Qt::Key_W : cam.moveCamera(0.1f, 0.0f);  break;
  case Qt::Key_S : cam.moveCamera(-0.1f, 0.0f); break;
  case Qt::Key_A : cam.moveCamera(0.0f, -0.1f); break;
  case Qt::Key_D : cam.moveCamera(0.0f, 0.1f);  break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  loadToShader();

  update();
}
