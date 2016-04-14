#ifndef SWARMFACTORY_H__
#define SWARMFACTORY_H__

#include <iostream>
#include <stdio.h>
#include <memory>
#include "Bird.h"

class SwarmFactory
{
public:

  virtual void Produce() = 0;

  // Implementing make_unique, as it doesn't exist until C++14
  template<typename T, typename ...Args>
  std::unique_ptr<T> make_unique(Args&& ...args) const
  {
    return std::unique_ptr<T>( new T(std::forward<Args>(args)... ) );
  }

protected:
  SwarmFactory();

  ~SwarmFactory();

};

class BirdFactory : public SwarmFactory
{
public:

  void Produce() override
  {
    BirdID.push_back(std::unique_ptr<Bird>{make_unique<Bird>()});
  }

  BirdFactory();

  ~BirdFactory();

  std::vector<std::unique_ptr<Bird>> BirdID;

protected:



};


#endif
