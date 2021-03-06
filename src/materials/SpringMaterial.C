//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SpringMaterial.h"

// MOOSE includes
#include "Material.h"
#include "MooseMesh.h"
#include "MooseVariable.h"
#include "NonlinearSystem.h"

#include "libmesh/quadrature.h"

defineLegacyParams(SpringMaterial);

InputParameters
SpringMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addCoupledVar(
      "rotations", "The rotations appropriate for the simulation geometry and coordinate system");      
  params.addRequiredParam<std::vector<Real>>(
      "stiffness_coeffs", 
      "Components of upper triangular part of the stiffness matrix in column major order");

  return params;
}

SpringMaterial::SpringMaterial(const InputParameters & parameters)
  : Material(parameters),
    _delta_disp(declareProperty<DenseVector<Real>>("delta_disp")),
    _delta_rot(declareProperty<DenseVector<Real>>("delta_rot")),
    _delta_dof(declareProperty<DenseVector<Real>>("delta_dof"))
{
  const std::vector<VariableName> 
    & disp_vnames(getParam<std::vector<VariableName>>("displacements")),
    & rot_vnames(getParam<std::vector<VariableName>>("rotations"));
  _ndisp = disp_vnames.size();
  _nrot = rot_vnames.size();
  _ndof = _ndisp + _nrot;

  // fetch displacement & rotation variables
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp_var.push_back(&_fe_problem.getStandardVariable(_tid, disp_vnames[i]));
  for (unsigned int i = 0; i < _nrot; ++i)
    _rot_var.push_back(&_fe_problem.getStandardVariable(_tid, rot_vnames[i]));

  // Fill the stiffness matrix
  const std::vector<Real> & stiffness_coeffs = getParam<std::vector<Real>>("stiffness_coeffs");
  _stiffness_matrix.resize(_ndof, _ndof);
  unsigned int k = 0;  // Counter in the column-major ABAQUS-like vector
  for (unsigned int j = 0; j < _ndof; ++j)
    for (unsigned int i = 0; i <= j; ++i)
    {
      _stiffness_matrix(i, j) = stiffness_coeffs[k];
      ++k;
    }

}

void
SpringMaterial::initQpStatefulProperties()
{
  _delta_disp[_qp].resize(_ndisp);
  _delta_rot[_qp].resize(_nrot);
  _delta_dof[_qp].resize(_ndof);
}

void
SpringMaterial::computeProperties()
{
  // check for consistency of the number of element nodes
  mooseAssert(_current_elem->n_nodes() == 2, "Truss element needs to have exactly two nodes.");

  // fetch the two end nodes for _current_elem
  std::vector<const Node *> node;
  for (unsigned int i = 0; i < 2; ++i)
    node.push_back(_current_elem->node_ptr(i));

  // fetch the solution for the two end nodes
  NonlinearSystemBase & nonlinear_sys = _fe_problem.getNonlinearSystemBase();
  const NumericVector<Number> & sol = *nonlinear_sys.currentSolution();

  DenseVector<Real> disp0, disp1, rot0, rot1;
  for (unsigned int i = 0; i < _ndisp; ++i)
  {
    disp0(i) = (sol(node[0]->dof_number(nonlinear_sys.number(), _disp_var[i]->number(), 0)));
    disp1(i) = (sol(node[1]->dof_number(nonlinear_sys.number(), _disp_var[i]->number(), 0)));
  }
  for (unsigned int i = 0; i < _nrot; ++i)
  {
    rot0(i) = (sol(node[0]->dof_number(nonlinear_sys.number(), _rot_var[i]->number(), 0)));
    rot1(i) = (sol(node[1]->dof_number(nonlinear_sys.number(), _rot_var[i]->number(), 0)));
  }

  // calculate delta of the dof
  _delta_disp[_qp] = disp1;
  _delta_disp[_qp] -= disp0;
  _delta_rot[_qp] = rot1;
  _delta_rot[_qp] -= rot0;

  for (unsigned int i = 0; i < _ndisp; ++i)
    _delta_dof[_qp](i) = _delta_disp[_qp](i);

  for (unsigned int i = _ndisp; i < _ndof; ++i)
    _delta_dof[_qp](i) = _delta_rot[_qp](i - _ndisp);


  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    computeQpStrain();
    computeQpStress();
  }
}
