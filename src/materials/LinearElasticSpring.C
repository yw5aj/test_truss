#include "LinearElasticSpring.h"

registerMooseObject("TestTrussApp", LinearElasticSpring);

defineLegacyParams(LinearElasticSpring);

InputParameters
LinearElasticSpring::validParams()
{
  InputParameters params = SpringMaterial::validParams();
  params.addClassDescription("Computes the linear elastic strain for a spring element");
  return params;
}

LinearElasticSpring::LinearElasticSpring(const InputParameters & parameters)
  : SpringMaterial(parameters)
{
}

void
LinearElasticSpring::computeQpStrain()
{
  _total_stretch[_qp] = _current_length / _origin_length - 1.0;
  _elastic_stretch[_qp] = _total_stretch[_qp] - _thermal_expansion_coeff * (_T[_qp] - _T0);
}

void
LinearElasticSpring::computeQpStress()
{
  _axial_stress[_qp] = _youngs_modulus[_qp] * _elastic_stretch[_qp];
}
