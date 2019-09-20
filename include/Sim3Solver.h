/*************************************************************************
 *
 *              Author: b51
 *                Mail: b51live@gmail.com
 *            FileName: Sim3Solver.h
 *
 *          Created On: Fri 20 Sep 2019 05:49:35 PM CST
 *     Licensed under The GPLv3 License [see LICENSE for details]
 *
 ************************************************************************/
/**
 * This file is part of ORB-SLAM2.
 *
 * Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University
 * of Zaragoza) For more information see <https://github.com/raulmur/ORB_SLAM2>
 *
 * ORB-SLAM2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ORB-SLAM2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIM3SOLVER_H
#define SIM3SOLVER_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "KeyFrame.h"

namespace ORB_SLAM2 {

class Sim3Solver {
 public:
  Sim3Solver(KeyFrame* keyframe_1, KeyFrame* keyframe_2,
             const std::vector<MapPoint*>& matched_points_1_in_2,
             const bool is_fixed_scale = true);

  void SetRansacParameters(double probability = 0.99, int min_inliers = 6,
                           int max_iterations = 300);

  cv::Mat find(std::vector<bool>& vbInliers12, int& n_inliers);

  cv::Mat iterate(int n_iterations, bool& is_no_more,
                  std::vector<bool>& is_inliers, int& n_inliers);

  cv::Mat GetEstimatedRotation();
  cv::Mat GetEstimatedTranslation();
  float GetEstimatedScale();

 protected:
  void ComputeCentroid(cv::Mat& P, cv::Mat& Pr, cv::Mat& C);

  void ComputeSim3(cv::Mat& P1, cv::Mat& P2);

  void CheckInliers();

  void Project(const std::vector<cv::Mat>& vP3Dw, std::vector<cv::Mat>& vP2D,
               cv::Mat Tcw, cv::Mat K);
  void FromCameraToImage(const std::vector<cv::Mat>& vP3Dc,
                         std::vector<cv::Mat>& vP2D, cv::Mat K);

 protected:
  // KeyFrames and matches
  KeyFrame* keyframe_1_;
  KeyFrame* keyframe_2_;

  std::vector<cv::Mat> X3Dsc1_;
  std::vector<cv::Mat> X3Dsc2_;
  std::vector<MapPoint*> map_points_1_;
  std::vector<MapPoint*> map_points_2_;
  std::vector<MapPoint*> matched_points_1_in_2_;
  std::vector<size_t> matched_indices_1_;
  std::vector<size_t> max_errors_1_;
  std::vector<size_t> max_errors_2_;

  int N_;
  int N1_;

  // Current Estimation
  cv::Mat R12i_;
  cv::Mat t12i_;
  float scale_12_i_;
  cv::Mat T12i_;
  cv::Mat T21i_;
  std::vector<bool> is_inliers_i_;
  int n_inliers_i_;

  // Current Ransac State
  int n_iterations_;
  std::vector<bool> is_best_inliers_;
  int n_best_inliers_;
  cv::Mat best_T12_;
  cv::Mat best_rotation_;
  cv::Mat best_translation_;
  float best_scale_;

  // Scale is fixed to 1 in the stereo/RGBD case
  bool is_fixed_scale_;

  // Indices for random selection
  std::vector<size_t> all_indices_;

  // Projections
  std::vector<cv::Mat> points_1_in_im_1_;
  std::vector<cv::Mat> points_2_in_im_2_;

  // RANSAC probability
  double ransac_probability_;

  // RANSAC min inliers
  int ransac_min_inliers_;

  // RANSAC max iterations
  int ransac_max_iterations_;

  // Threshold inlier/outlier. e = dist(Pi,T_ij*Pj)^2 < 5.991*sigma2
  // float thres_;
  // float sigma2_;

  // Calibration
  cv::Mat K1_;
  cv::Mat K2_;
};

}  // namespace ORB_SLAM2

#endif  // SIM3SOLVER_H
