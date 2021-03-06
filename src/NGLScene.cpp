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
  m_proj=ngl::perspective(90.0f,float(width()/height()),0.1,200);
  m_view=ngl::lookAt(cam.getPosition(), cam.getLook(), ngl::Vec3(0.0f, 1.0f, 0.0f));

  followSphere = new FollowPoint;
  avoidSphere = new SphereShape;
//  avoidPyramid = new PyramidShape;
  avoidWall = new WallShape;
//  avoidTorus = new TorusShape;
  ShapeStore.ShapeList.push_back(*avoidSphere);
//  ShapeStore.ShapeList.push_back(*avoidPyramid);
  ShapeStore.ShapeList.push_back(*avoidWall);
//  ShapeStore.ShapeList.push_back(*avoidTorus);

  m_text = new ngl::Text(QFont("Arial", 14));
  m_text->setScreenSize(width(), height());


  startTimer(10);

}

void NGLScene::switchFullScreen()
{
  // Start windowed, switch to fullscreen on every other call
  if(m_fullScreen %2 == 0)
  {
    showNormal();
  }
  else
  {
    showFullScreen();
  }
}

void NGLScene::loadToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Colour");

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  ngl::Mat4 R;

  shader->setRegisteredUniform("lightPos", m_lightPos);
  M=m_transform.getMatrix()/**m_mouseGlobalTX*/;
//  MV=  M*ngl::Mat4(cam.getSideVector().m_x, cam.getSideVector().m_y, cam.getSideVector().m_z, 0.0f,
//                   0.0f,1.0f,0.0f,0.0f,
//                   cam.getForwardVector().m_x, cam.getForwardVector().m_y, cam.getForwardVector().m_z, 0.0f,
//                   1.0f,1.0f,1.0f,1.0f);
  m_view = ngl::lookAt(cam.getPosition(), cam.getLook(), ngl::Vec3(0.0f, 1.0f, 0.0f));
//  R= m_transform.getRotation();
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
}

void NGLScene::updateMember(Member &io_toUpdate)
{
  ngl::Vec3 alignment = calcAlignment(io_toUpdate) * 10;
  ngl::Vec3 cohesion = /*followSphere->getPosition() - */calcCohesion(io_toUpdate) *10;
  ngl::Vec3 separation = calcSeparation(io_toUpdate);
  ngl::Vec3 newVelocity = (followSphere->getPosition()) -
                          (io_toUpdate.getPosition() +
                          (ngl::Vec3(alignment + cohesion + separation)));
  for(unsigned int i = 0; i<ShapeStore.ShapeList.size(); i++)
  {
    float shapeDist = calcDistance(getMemberPosition(ShapeStore.ShapeList[i]), io_toUpdate.getPosition());

    if(abs(shapeDist) < 8.0f)
    {
      newVelocity += (io_toUpdate.getPosition() - getMemberPosition(ShapeStore.ShapeList[i])) * 100;
    }
  }

  if(newVelocity.lengthSquared() != 0.0f)
  {
    newVelocity.normalize();
  }
  //std::cout<<"x = "<<newVelocity.m_x<<" y = "<<newVelocity.m_y<<" z = "<<newVelocity.m_z<<"\n";

//  if(newVelocity.m_x < 0.0f)
//  {
//    newVelocity.m_x = -newVelocity.m_x;
//  }
//  if(newVelocity.m_y < 0.0f)
//  {
//    newVelocity.m_y = -newVelocity.m_y;
//  }
//  if(newVelocity.m_z < 0.0f)
//  {
//    newVelocity.m_z = -newVelocity.m_z;
//  }

  float dist = ngl::Vec3(followSphere->getPosition() - io_toUpdate.getPosition()).length();

  if (dist > 15.0f)
  {
    io_toUpdate.setForwardVector(followSphere->getPosition());
    io_toUpdate.calcSideVector();
  }
  else
  {
    io_toUpdate.setSideVector(followSphere->getPosition());
    io_toUpdate.calcForwardVector();
  }

  io_toUpdate.getForwardVector().normalize();
  if(io_toUpdate.getForwardVector().m_x <0.0f)
  {
    io_toUpdate.setForwardVector(ngl::Vec3(-io_toUpdate.getForwardVector().m_x,
                                           io_toUpdate.getForwardVector().m_y,
                                           io_toUpdate.getForwardVector().m_z));
  }

  if(io_toUpdate.getForwardVector().m_y <0.0f)
  {
    io_toUpdate.setForwardVector(ngl::Vec3(io_toUpdate.getForwardVector().m_x,
                                           -io_toUpdate.getForwardVector().m_y,
                                           io_toUpdate.getForwardVector().m_z));
  }

  if(io_toUpdate.getForwardVector().m_z <0.0f)
  {
    io_toUpdate.setForwardVector(ngl::Vec3(io_toUpdate.getForwardVector().m_x,
                                           io_toUpdate.getForwardVector().m_y,
                                           -io_toUpdate.getForwardVector().m_z));
  }


  io_toUpdate.setVelocity(newVelocity * 0.0001f * (io_toUpdate.getForwardVector() + io_toUpdate.getPosition()) , false);

  if(io_toUpdate.getVelocity().m_x > 0.5f)
  {
    io_toUpdate.setVelocity(ngl::Vec3(0.5f,
                                      io_toUpdate.getVelocity().m_y,
                                      io_toUpdate.getVelocity().m_z),
                                      true);
  }

  else if(io_toUpdate.getVelocity().m_x < -0.5f)
  {
    io_toUpdate.setVelocity(ngl::Vec3(-0.5f,
                                      io_toUpdate.getVelocity().m_y,
                                      io_toUpdate.getVelocity().m_z),
                                      true);
  }

  if(io_toUpdate.getVelocity().m_y > 0.5f)
  {
    io_toUpdate.setVelocity(ngl::Vec3(io_toUpdate.getVelocity().m_x,
                                      0.5f,
                                      io_toUpdate.getVelocity().m_z),
                                      true);
  }

  else if(io_toUpdate.getVelocity().m_y < -0.5f)
  {
    io_toUpdate.setVelocity(ngl::Vec3(io_toUpdate.getVelocity().m_x,
                                      -0.5f,
                                      io_toUpdate.getVelocity().m_z),
                                      true);
  }

  if(io_toUpdate.getVelocity().m_z > 0.5f)
  {
    io_toUpdate.setVelocity(ngl::Vec3(io_toUpdate.getVelocity().m_x,
                                      io_toUpdate.getVelocity().m_y,
                                      0.5f),
                                      true);
  }

  else if(io_toUpdate.getVelocity().m_z < -0.5f)
  {
    io_toUpdate.setVelocity(ngl::Vec3(io_toUpdate.getVelocity().m_x,
                                      io_toUpdate.getVelocity().m_y,
                                      -0.5f),
                                      true);
  }

  //io_toUpdate.setVelocity(newVelocity * 0.0001f * io_toUpdate.getForwardVector() , false);

  io_toUpdate.setPosition(io_toUpdate.getVelocity(), false);
}

float NGLScene::calcDistance(ngl::Vec3 _vector1, ngl::Vec3 _vector2)
{
  float dX = _vector2.m_x - _vector1.m_x;
  float dY = _vector2.m_y - _vector1.m_y;
  float dZ = _vector2.m_z - _vector1.m_z;
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
    if(dist < 50.0f && dist != 0.0f )
    {
      alignVec += getMemberVelocity(*makerBirds.BirdID[i]);
      neighbours++;
    }
  }
  if(neighbours == 0)
  {
//    std::cout<<"No neighbours\n";
    return alignVec;
  }

  alignVec /= neighbours;
  if(alignVec.lengthSquared() != 0.0f)
  {
    alignVec.normalize();
  }
//  std::cout<<"Some neighbours\n";
  return alignVec;
}

ngl::Vec3 NGLScene::calcCohesion(Member &_toCalc)
{
  unsigned int neighbours = 0;
  ngl::Vec3 memPos = _toCalc.getPosition();
  ngl::Vec3 coVec = ngl::Vec3(0.0f, 0.0f, 0.0f);
  for(unsigned int i = 0; i < makerBirds.BirdID.size(); i++)
  {
    float dist = calcDistance(getMemberPosition(*makerBirds.BirdID[i]), memPos);
    if(dist < 50.0f && dist != 0.0f )
    {
      coVec += getMemberPosition(*makerBirds.BirdID[i]);
      neighbours++;
    }
  }
  if(!neighbours)
    return coVec;

  coVec /= neighbours;
  coVec = coVec - getMemberPosition(_toCalc);
  if(coVec.lengthSquared() != 0.0f)
  {
    coVec.normalize();
  }
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

  sepVec /= neighbours;
  sepVec = sepVec - getMemberPosition(_toCalc);
  sepVec *= -1;
  if(sepVec.lengthSquared() != 0.0f)
  {
    sepVec.normalize();
  }
  return sepVec;
}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);
  QString text;
  m_text->setColour(0,0,0);
  m_text->renderText(10,10,text);
  text.sprintf("Number of birds = %d", makerBirds.BirdID.size());
  m_text->renderText(10,25,text);
  m_lightPos = avoidWall->getPosition();
  m_transform.setPosition(followSphere->getPosition());
  m_transform.setRotation(0.0f, 0.0f, 0.0f);
  m_transform.setScale(2.0f, 2.0f, 2.0f);
  loadToShader();
  followSphere->draw();
  m_transform.setPosition(avoidSphere->getPosition());
  m_transform.setScale(3.0f, 3.0f, 3.0f);
  loadToShader();
  avoidSphere->draw();
//  m_transform.setPosition(avoidPyramid->getPosition());
//  m_transform.setScale(3.0f, 3.0f, 3.0f);
//  loadToShader();
//  avoidPyramid->draw();
  m_transform.setPosition(avoidWall->getPosition());
  loadToShader();
  avoidWall->draw();
//  m_transform.setPosition(avoidTorus->getPosition());
//  m_transform.setScale(3.0f, 3.0f, 3.0f);
//  loadToShader();
//  avoidTorus->draw();
  m_transform.setScale(1.0f, 1.0f, 1.0f);
//  std::cout <<"size " <<makerBirds.BirdID.size() << "\n";
  for(unsigned int i = 0; i < makerBirds.BirdID.size(); i++)
  {
    m_transform.setPosition(getMemberPosition(*makerBirds.BirdID[i]));
//    m_transform.setRotation(getMemberForwardVector(*makerBirds.BirdID[i]) + ngl::Vec3(90.0f, 0.0f, 0.0f));
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

  if(cam.getxRot()>=M_PI)
  {
    cam.setxRot(M_PI - 0.0001f);
  }

  if(cam.getxRot()<=0.0002f)
  {
    cam.setxRot(0.0001f);
  }


  // Increment the player's rotation by this amount
  cam.rotateCamera(o_mouseMovement.m_x, o_mouseMovement.m_y);

  // Return the mouse pointer to the screen centre and update
  QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
  QCursor::setPos(glob);

  cam.calcVectors();
  if(lockFollow == false)
  {
    followSphere->setPosition(cam.getLook(), true);
  }
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
  case Qt::Key_Space : makerBirds.Produce(); break;
  case Qt::Key_W : cam.moveCamera(0.05f, 0.0f);  break;
  case Qt::Key_S : cam.moveCamera(-0.05f, 0.0f); break;
  case Qt::Key_A : cam.moveCamera(0.0f, -0.1f); break;
  case Qt::Key_D : cam.moveCamera(0.0f, 0.1f);  break;
  case Qt::Key_F : m_fullScreen += 1; switchFullScreen(); break;
  case Qt::Key_L : lockFollow = true; break;
  case Qt::Key_P : lockFollow = false; break;
  default : break;
  }
  // finally update the GLWindow and re-draw

  loadToShader();

  update();
}
