#include "TestTrussApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
TestTrussApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  return params;
}

TestTrussApp::TestTrussApp(InputParameters parameters) : MooseApp(parameters)
{
  TestTrussApp::registerAll(_factory, _action_factory, _syntax);
}

TestTrussApp::~TestTrussApp() {}

void
TestTrussApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"TestTrussApp"});
  Registry::registerActionsTo(af, {"TestTrussApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
TestTrussApp::registerApps()
{
  registerApp(TestTrussApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
TestTrussApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  TestTrussApp::registerAll(f, af, s);
}
extern "C" void
TestTrussApp__registerApps()
{
  TestTrussApp::registerApps();
}
