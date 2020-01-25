#pragma once

#include "Material.h"

// Forward Declarations
class SpringMaterial;

template <>
InputParameters validParams<SpringMaterial>();

class SpringMaterial : public Material
{
public:
  static InputParameters validParams();

  SpringMaterial(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties();
  virtual void computeProperties();

  virtual void computeQpStrain() = 0;
  virtual void computeQpStress() = 0;

  std::vector<MooseVariable *> _disp_var, _rot_var;

  DenseMatrix<Real> _stiffness_matrix;

  unsigned int _ndisp, _nrot, _ndof;

  MaterialProperty<DenseVector<Real>> & _delta_disp;
  MaterialProperty<DenseVector<Real>> & _delta_rot;
  MaterialProperty<DenseVector<Real>> & _delta_dof;
};
