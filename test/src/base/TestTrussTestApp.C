//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "TestTrussTestApp.h"
#include "TestTrussApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
TestTrussTestApp::validParams()
{
  InputParameters params = TestTrussApp::validParams();
  return params;
}

TestTrussTestApp::TestTrussTestApp(InputParameters parameters) : MooseApp(parameters)
{
  TestTrussTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

TestTrussTestApp::~TestTrussTestApp() {}

void
TestTrussTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  TestTrussApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"TestTrussTestApp"});
    Registry::registerActionsTo(af, {"TestTrussTestApp"});
  }
}

void
TestTrussTestApp::registerApps()
{
  registerApp(TestTrussApp);
  registerApp(TestTrussTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
TestTrussTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  TestTrussTestApp::registerAll(f, af, s);
}
extern "C" void
TestTrussTestApp__registerApps()
{
  TestTrussTestApp::registerApps();
}
