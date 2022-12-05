#pragma once
#include <Eigen/Core>
#include <opencv4/opencv2/core.hpp>

namespace pcdless::graph_segment
{
struct Histogram
{
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  Histogram(int bin = 10) : bin(bin) { data = Eigen::MatrixXf::Zero(3, bin); }

  Eigen::MatrixXf eval() const
  {
    float sum = data.sum();
    if (sum < 1e-6f) throw std::runtime_error("invalid division");
    return data / sum;
  }

  void add(const cv::Vec3b & rgb)
  {
    for (int ch = 0; ch < 3; ++ch) {
      int index = std::clamp(static_cast<int>(rgb[ch] * bin / 255.f), 0, bin - 1);
      data(ch, index) += 1.0f;
    }
  }
  const int bin;
  Eigen::MatrixXf data;

  static float eval_histogram_intersection(const Eigen::MatrixXf & a, const Eigen::MatrixXf & b)
  {
    float score = 0;
    for (int c = 0; c < a.cols(); c++) {
      for (int r = 0; r < a.rows(); r++) {
        score += std::min(a(r, c), b(r, c));
      }
    }
    return score;
  };

  static float eval_bhattacharyya_coeff(const Eigen::MatrixXf & a, const Eigen::MatrixXf & b)
  {
    float score = 0;
    for (int c = 0; c < a.cols(); c++) {
      for (int r = 0; r < a.rows(); r++) {
        score += std::sqrt(a(r, c) * b(r, c));
      }
    }
    return score;
  };
};

}  // namespace pcdless::graph_segment