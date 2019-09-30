/*
 * Copyright 2015-2019 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

/*! Interface used to load observers */

#include <mc_observers/api.h>
#include <mc_rtc/GUIState.h>
#include <mc_rtc/log/Logger.h>

namespace mc_control
{
struct MCController;
} // namespace mc_control

namespace mc_observers
{

struct MC_OBSERVER_DLLAPI Observer
{
  Observer(const std::string & name, double dt, const mc_rtc::Configuration & config = {});
  virtual ~Observer();
  virtual const std::string & name() const;
  double dt() const;

  /*! \brief Reset estimator.
   *
   * \param ctl Controller reference (const). Used to access information about
   * the robot and the controller (anchor frame, contacts, desired contact
   * force, etc).
   */
  virtual void reset(const mc_control::MCController & ctl) = 0;

  /*! \brief Compute observer state
   *
   * \param ctl Controller reference (const). Used to access information about
   * the robot and the controller (anchor frame, contacts, desired contact
   * force, etc).
   */
  virtual bool run(const mc_control::MCController & ctl) = 0;

  /*! \brief Update the real robot state from the observed state
   *
   * \param ctl Controller reference (const). Used to access information about
   * the robot and the controller (anchor frame, contacts, desired contact
   * force, etc).
   *
   * \param robot Robot state to write to. Each controller is expected to update
   * the real robot instance with its estimates. The pipeline will only call the
   * updateRobot() function if requested by the user.
   */
  virtual void updateRobot(const mc_control::MCController & ctl, mc_rbdyn::Robots & realRobots) = 0;

  /*! \brief Add observer to the logger.
   *
   * Default implementation does nothing, each observer implementation is
   * responsible for logging its own data by overriding this function
   */
  virtual void addToLogger(const mc_control::MCController &, mc_rtc::Logger &) {}
  /*! \brief Remove observer from logger
   *
   * Default implementation does nothing, each observer implementation is
   * responsible for removing all logs entry that it added.
   */
  virtual void removeFromLogger(mc_rtc::Logger &) {}
  /*! \brief Add observer information the GUI.
   *
   * Default implementation does nothing, each observer implementation is
   * responsible for adding its own elements to the GUI. Default observers will
   * be shown under the tab "Observers->observer name".
   */
  virtual void addToGUI(const mc_control::MCController &, mc_rtc::gui::StateBuilder &) {}
  /*! \brief Remove observer from gui
   *
   * Default implementation removes the category Observers->observer name
   */
  virtual void removeFromGUI(mc_rtc::gui::StateBuilder &);

protected:
  std::string name_;
  double dt_;
};

using ObserverPtr = std::shared_ptr<mc_observers::Observer>;

} // namespace mc_observers

/* Set of macros to assist with the writing of an Observer */

#ifdef WIN32
#  define OBSERVER_MODULE_API __declspec(dllexport)
#else
#  if __GNUC__ >= 4
#    define OBSERVER_MODULE_API __attribute__((visibility("default")))
#  else
#    define OBSERVER_MODULE_API
#  endif
#endif

#define EXPORT_OBSERVER_MODULE(NAME, TYPE)                                                    \
  extern "C"                                                                                  \
  {                                                                                           \
    OBSERVER_MODULE_API void MC_RTC_OBSERVER_MODULE(std::vector<std::string> & names)         \
    {                                                                                         \
      names = {NAME};                                                                         \
    }                                                                                         \
    OBSERVER_MODULE_API void destroy(mc_observers::Observer * ptr)                            \
    {                                                                                         \
      delete ptr;                                                                             \
    }                                                                                         \
    OBSERVER_MODULE_API unsigned int create_args_required()                                   \
    {                                                                                         \
      return 3;                                                                               \
    }                                                                                         \
    OBSERVER_MODULE_API mc_observers::Observer * create(const std::string & name,             \
                                                        const double & dt,                    \
                                                        const mc_rtc::Configuration & config) \
    {                                                                                         \
      return new TYPE(name, dt, config);                                                      \
    }                                                                                         \
  }
