/**
 * Copyright (c) 2016-present, Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "caffe2/operators/conv_op.h"
#include "caffe2/operators/conv_op_impl.h"
#include "caffe2/operators/conv_pool_op_base.h"

namespace caffe2 {

REGISTER_CPU_OPERATOR(ConvGradient, ConvGradientOp<float, CPUContext>);
OPERATOR_SCHEMA(ConvGradient).NumInputs(2, 3).NumOutputs(1, 3);

REGISTER_CPU_OPERATOR(Conv1DGradient, ConvGradientOp<float, CPUContext>);
OPERATOR_SCHEMA(Conv1DGradient).NumInputs(2, 3).NumOutputs(1, 3);

REGISTER_CPU_OPERATOR(Conv2DGradient, ConvGradientOp<float, CPUContext>);
OPERATOR_SCHEMA(Conv2DGradient).NumInputs(2, 3).NumOutputs(1, 3);

REGISTER_CPU_OPERATOR(Conv3DGradient, ConvGradientOp<float, CPUContext>);
OPERATOR_SCHEMA(Conv3DGradient).NumInputs(2, 3).NumOutputs(1, 3);

class GetConvGradient : public GradientMakerBase {
  using GradientMakerBase::GradientMakerBase;
  vector<OperatorDef> GetGradientDefs() override {
    CAFFE_ENFORCE(def_.input_size() == 3 || def_.input_size() == 2);

    ArgumentHelper argsHelper(def_);

    auto compute_dX = !argsHelper.GetSingleArgument<bool>("no_gradient_to_input", 0);

    if (def_.input_size() == 3) {
      if (compute_dX) {
        return SingleGradientDef(
            def_.type() + "Gradient",
            "",
            vector<string>{I(0), I(1), GO(0)},
            vector<string>{GI(1), GI(2), GI(0)});
      } else {
        return SingleGradientDef(
            def_.type() + "Gradient",
            "",
            vector<string>{I(0), I(1), GO(0)},
            vector<string>{GI(1), GI(2)});
      }
    } else {
      if (compute_dX) {
        return SingleGradientDef(
            def_.type() + "Gradient",
            "",
            vector<string>{I(0), I(1), GO(0)},
            vector<string>{GI(1), GI(0)},
            vector<Argument>{MakeArgument<int>("no_bias", 1)});
      } else {
        return SingleGradientDef(
            def_.type() + "Gradient",
            "",
            vector<string>{I(0), I(1), GO(0)},
            vector<string>{GI(1)},
            vector<Argument>{MakeArgument<int>("no_bias", 1)});
      }
    }
  }
};
REGISTER_GRADIENT(Conv, GetConvGradient);
REGISTER_GRADIENT(Conv1D, GetConvGradient);
REGISTER_GRADIENT(Conv2D, GetConvGradient);
REGISTER_GRADIENT(Conv3D, GetConvGradient);

} // namespace caffe2
