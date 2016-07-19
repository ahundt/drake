#include <iostream>
#include <random>
#include <vector>

#include "drake/solvers/trajectoryOptimization/direct_trajectory_optimization.h"
#include "drake/systems/trajectories/PiecewisePolynomial.h"
#include "gtest/gtest.h"

using namespace std;
using std::default_random_engine;
using std::vector;

namespace drake {
namespace solvers {
namespace {

typedef PiecewisePolynomial<double> PiecewisePolynomialType;

default_random_engine generator;

GTEST_TEST(TrajectoryOptimizationTest, DirectTrajectoryOptimizationTest) {
  const int kNumInputs(1);
  const int kNumStates(2);
  const int kNumTimeSamples(21);  // aka N.
  DirectTrajectoryOptimization directTraj(kNumInputs, kNumStates,
                                          kNumTimeSamples, -2, 3);

  const int t_init_in(7);
  VectorXd s_times{VectorXd::LinSpaced(kNumTimeSamples, 0, t_init_in)};
  vector<double> segment_times(
      s_times.data(), s_times.data() + s_times.rows() * s_times.cols());
  //  cout << "segmentTimes= " << endl;
  //  std::copy(segment_times.begin(), segment_times.end(),
  //    std::ostream_iterator<double>(std::cout, " "));

  const PiecewisePolynomialType inputs_u =
      PiecewisePolynomialType::random(kNumInputs, 1, 4, segment_times);
  const PiecewisePolynomialType states_x =
      PiecewisePolynomialType::random(kNumStates, 1, 6, segment_times);

  SolutionResult result = SolutionResult::kUnknownError;
  ASSERT_NO_THROW(result = directTraj.SolveTraj(
                      t_init_in, PiecewisePolynomialType(), states_x));
  EXPECT_EQ(result, SolutionResult::kSolutionFound) << "Result is an Error";

  ASSERT_NO_THROW(result = directTraj.SolveTraj(t_init_in, inputs_u, states_x));
  EXPECT_EQ(result, SolutionResult::kSolutionFound) << "Result is an Error";
}

}  // anonymous namespace
}  // namespace solvers
}  // namespace drake
