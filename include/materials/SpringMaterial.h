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

  const std::string _base_name;
  ColumnMajorMatrix _stiffness_matrix;

  unsigned int _ndisp, _nrot, _ndof;

  MaterialProperty<Real> & _total_stretch;
  MaterialProperty<Real> & _elastic_stretch;
  MaterialProperty<Real> & _axial_stress;
  MaterialProperty<Real> & _e_over_l;

  Real _origin_length;
  Real _current_length;
};
