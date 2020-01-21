//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "LinearElasticTruss2.h"

registerMooseObject("TestTrussApp", LinearElasticTruss2);

defineLegacyParams(LinearElasticTruss2);

InputParameters
LinearElasticTruss2::validParams()
{
  InputParameters params = Truss2Material::validParams();
  params.addClassDescription("Computes the linear elastic strain for a truss2 element");
  params.addParam<Real>("thermal_expansion_coeff", 0.0, "Thermal expansion coefficient in 1/K");
  params.addParam<Real>("temperature_ref", 273, "Reference temperature for thermal expansion in K");
  params.addCoupledVar("temperature", 273, "Temperature in Kelvin");
  return params;
}

LinearElasticTruss2::LinearElasticTruss2(const InputParameters & parameters)
  : Truss2Material(parameters),
    _T(coupledValue("temperature")),
    _T0(getParam<Real>("temperature_ref")),
    _thermal_expansion_coeff(getParam<Real>("thermal_expansion_coeff"))
{
}

void
LinearElasticTruss2::computeQpStrain()
{
  _total_stretch[_qp] = _current_length / _origin_length - 1.0;
  _elastic_stretch[_qp] = _total_stretch[_qp] - _thermal_expansion_coeff * (_T[_qp] - _T0);
}

void
LinearElasticTruss2::computeQpStress()
{
  _axial_stress[_qp] = _youngs_modulus[_qp] * _elastic_stretch[_qp];
}
