#pragma once

#include <mc_control/mc_controller.h>

#include <mc_tasks/CoMTask.h>

#include <mc_control/api.h>

namespace mc_control
{

struct MC_CONTROL_DLLAPI MCCoMController : public MCController
{
public:
  MCCoMController(std::shared_ptr<mc_rbdyn::RobotModule> robot, double dt);

  virtual void reset(const ControllerResetData & reset_data) override;

  /* Services */
  virtual bool move_com(const Eigen::Vector3d & v) override;
public:
  std::shared_ptr<mc_tasks::CoMTask> comTask;
};

}

SIMPLE_CONTROLLER_CONSTRUCTOR("CoM", mc_control::MCCoMController)