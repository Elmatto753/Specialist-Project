#ifndef SWARMFACTORY_H__
#define SWARMFACTORY_H__

#include <iostream>
#include <stdio.h>
#include <memory>
#include "Bird.h"

class SwarmFactory
{
public:

  virtual std::unique_ptr<Member> Produce() const = 0;

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

  virtual std::unique_ptr<Member> Produce() const override
  {
    return std::unique_ptr<Member>{make_unique<Bird>()};
  }

  BirdFactory();

  ~BirdFactory();

protected:

  std::vector<std::unique_ptr<Member>> BirdID;

};


#endif
