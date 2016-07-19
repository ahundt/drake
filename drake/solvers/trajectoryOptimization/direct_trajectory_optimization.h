#pragma once

#include <cstddef>

#include <Eigen/Core>

#include "drake/drakeTrajectoryOptimization_export.h"
#include "drake/solvers/Optimization.h"
#include "drake/systems/trajectories/PiecewisePolynomial.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace drake {
namespace solvers {

/**
 * DirectTrajectoryOptimization is a class for direct method approaches
 * to trajectory optimization.
 *
 * This class assumes that there are a fixed number (N) time steps/samples, and
 * that the trajectory is discreteized into timesteps h (N-1 of these), state x
 * (N of these), and control input u (N of these).
 *
 * To maintain nominal sparsity in the optimization programs, this
 * implementation assumes that all constraints and costs are
 * time-invariant.
 *
 * TODO(Lucy-tri) This class is a WIP. 
 */
class DRAKETRAJECTORYOPTIMIZATION_EXPORT DirectTrajectoryOptimization {
 public:
 /**
  * @p trajectory_time_lower_bound Bound on total time for trajectory.
  * @p trajectory_time_upper_bound Bound on total time for trajectory.
  */
  DirectTrajectoryOptimization(const int num_inputs, const int num_states,
                               const int num_time_samples,
                               const int trajectory_time_lower_bound,
                               const int trajectory_time_upper_bound);
  // TODO(Lucy-tri) add param: time steps constant or independent.

  /**
   * Add constraint (or composite constraint) that is a function of the
   * state at the specified time or times.
   * @p constraint A CompositeConstraint.
   * @p time_index A cell array of time indices.
   *   ex1., time_index = {1, 2, 3} means the constraint is applied
   *   individually to knot points 1, 2, and 3. 
   *   ex2,. time_index = {{1 2}, {3 4}} means the constraint is applied to knot
   *   points 1 and 2 together (taking the combined state as an argument)
   *   and 3 and 4 together.
   */
  void AddStateConstraint(const Constraint& constraint, const int time_index);

  /**
   * Solve the nonlinear program and return the resulting trajectory.
   *
   * @p t_init Initial timespan for solution.
   *
   * @p traj_init_u Initial guess for trajectory for control input.
   *
   * @p traj_init_x Initial guess for trajectory for state input.
   */
  SolutionResult SolveTraj(int t_init,
                           const PiecewisePolynomial<double>& traj_init_u,
                           const PiecewisePolynomial<double>& traj_init_x);

 private:
  /**
   *  Evaluate the initial trajectories at the sampled times and construct the
   *  nominal initial vectors.
   *
   * @p t_init Initial timespan for solution.
   *
   * @p traj_init_u Initial guess for trajectory for control input.
   *
   * @p traj_init_x Initial guess for trajectory for state input.
   */
  void GetInitialVars(int t_init_in,
                      const PiecewisePolynomial<double>& traj_init_u,
                      const PiecewisePolynomial<double>& traj_init_x);

  const int num_inputs_;
  const int num_states_;
  const int N;  // Number of time samples

  OptimizationProblem opt_problem_;
  DecisionVariableView h_vars_;
  DecisionVariableView u_vars_;
  DecisionVariableView x_vars_;
};

}  // solvers
}  // drake
