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
  MV=  M*ngl::Mat4(cam.getSideVector().m_x, cam.getSideVector().m_y, cam.getSideVector().m_z, 0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   cam.getForwardVector().m_x, cam.getForwardVector().m_y, cam.getForwardVector().m_z, 0.0f,
                   1.0f,1.0f,1.0f,1.0f);
  MVP=  MV*m_proj;
  normalMatrix=MV;
  normalMatrix.inverse();
//  shader->setShaderParamFromMat4("MV",MV);
//  shader->setShaderParamFromMat4("MVP",MVP);
//  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
//  shader->setShaderParamFromMat4("M",M);
  shader->setUniform("MVP",MVP);
  shader->setShaderParam4f("Colour", 1.0f, 0.1f, 0.1f, 1.0f);
}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);
  m_transform.setPosition(cam.getPosition());
  loadToShader();
  followSphere->draw();

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  // Get the amount the mouse has moved
  ngl::Vec2 o_mouseMovement = ngl::Vec2(width()/2 - _event->x(), height()/2 - _event->y());
  if(cam.getRotation().m_y>M_PI)
  {
    o_mouseMovement.m_y = -0.0001;
  }

  if(cam.getRotation().m_y<0.0001)
  {
    o_mouseMovement.m_y = 0.001;
  }
  // Increment the player's rotation by this amount
  cam.rotateCamera(o_mouseMovement.m_x, o_mouseMovement.m_y);
  // Return the mouse pointer to the screen centre and update
  QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
  QCursor::setPos(glob);

  cam.calcVectors();
  std::cout<<"x is "<<cam.getForwardVector().m_x<<"\n";
  std::cout<<"y is "<<cam.getForwardVector().m_y<<"\n";
//  followSphere->updatePosition(cam.getForwardVector() * 4);
//  followSphere.draw();

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
  case Qt::Key_Space : makerBirds.Produce(); break;
  case Qt::Key_W : cam.moveCamera(0.1f, 0.0f); break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}
