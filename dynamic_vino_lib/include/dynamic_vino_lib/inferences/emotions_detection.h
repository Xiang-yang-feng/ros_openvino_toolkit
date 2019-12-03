/*
 * Copyright (c) 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @brief A header file with declaration for EmotionsDetection Class
 * @file emotions_detection.h
 */
#ifndef DYNAMIC_VINO_LIB_INFERENCES_EMOTIONS_DETECTION_H
#define DYNAMIC_VINO_LIB_INFERENCES_EMOTIONS_DETECTION_H

#include <memory>
#include <string>
#include <vector>

#include "dynamic_vino_lib/engines/engine.h"
#include "dynamic_vino_lib/inferences/base_inference.h"
#include "dynamic_vino_lib/models/emotion_detection_model.h"
#include "inference_engine.hpp"
#include "opencv2/opencv.hpp"

namespace Outputs
{
class BaseOuput;
}
namespace dynamic_vino_lib
{
/**
 * @class EmotionResult
 * @brief Class for storing and processing emotion detection result.
 */
class EmotionsResult : public Result
{
 public:
  friend class EmotionsDetection;
  explicit EmotionsResult(const cv::Rect& location);
  /**
   * @brief Get the emotion label of the detected person.
   * @return The predictea emotion label.
   */
  std::string getLabel() const
  {
    return label_;
  }

 private:
  std::string label_ = "";
  float confidence_ = -1;
};

/**
 * @class EmotionDetection
 * @brief Class to load emotion detection model and perform emotion detection.
 */
class EmotionsDetection : public BaseInference
{
 public:
  using Result = dynamic_vino_lib::EmotionsResult;
  EmotionsDetection();
  ~EmotionsDetection() override;
  /**
   * @brief Load the emotin detection model.
   */
  void loadNetwork(std::shared_ptr<Models::EmotionDetectionModel>);
  /**
   * @brief Enqueue a frame to this class.
   * The frame will be buffered but not infered yet.
   * @param[in] frame The frame to be enqueued.
   * @param[in] input_frame_loc The location of the enqueued frame with respect
   * to the frame generated by the input device.
   * @return Whether this operation is successful.
   */
  bool enqueue(const cv::Mat&, const cv::Rect&) override;
  /**
   * @brief Start inference for all buffered frames.
   * @return Whether this operation is successful.
   */
  bool submitRequest() override;
  /**
   * @brief This function will fetch the results of the previous inference and
   * stores the results in a result buffer array. All buffered frames will be
   * cleared.
   * @return Whether the Inference object fetches a result this time
   */
  bool fetchResults() override;
  /**
   * @brief Get the length of the buffer result array.
   * @return The length of the buffer result array.
   */
  int getResultsLength() const override;
  /**
   * @brief Get the location of result with respect
   * to the frame generated by the input device.
   * @param[in] idx The index of the result.
   */
  const dynamic_vino_lib::Result* getLocationResult(int idx) const override;
  /**
   * @brief Get the name of the Inference instance.
   * @return The name of the Inference instance.
   */
  const std::string getName() const override;
  /**
   * @brief Show the observed detection result either through image window
   * or ROS topic.
   */
  void observeOutput(
      const std::shared_ptr<Outputs::BaseOutput>& output) override;

  const std::vector<cv::Rect> getFilteredROIs(
    const std::string filter_conditions) const override;
  
 private:
  std::shared_ptr<Models::EmotionDetectionModel> valid_model_;
  std::vector<Result> results_;
};
}  // namespace dynamic_vino_lib

#endif  // DYNAMIC_VINO_LIB_INFERENCES_EMOTIONS_DETECTION_H
