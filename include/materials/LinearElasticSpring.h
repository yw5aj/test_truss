#pragma once

#include "SpringMaterial.h"

class LinearElasticSpring;

template <>
InputParameters validParams<LinearElasticSpring>();

class LinearElasticSpring : public SpringMaterial
{
public:
  static InputParameters validParams();

  LinearElasticSpring(const InputParameters & parameters);

protected:
  virtual void computeQpStrain();
  virtual void computeQpStress();

private:

};
