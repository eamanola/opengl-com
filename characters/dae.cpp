#include "dae.h"
#include <glm/glm.hpp>
#include "../shader-utils.h"

Dae::Dae() : Character("assets/dae-runner/model.dae")
{
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
  model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  model = glm::scale(model, glm::vec3(0.25f));
  setModelMatrix(model);

  // Animation staticPose;
  // const float tps = 30.f;
  // staticPose.ticksPerSecond = tps;

  // staticPose.boneTransforms["Lower_Arm_L"] = BoneTransforms {
  //   .positionTimestamps = { 0.f * tps, 1.f * tps, 2.f * tps },
  //   .positions = {
  //     { 0.f, 0.f, 0.f },
  //     { 0.f, 1.f, 0.f },
  //     { 0.f, 0.f, 0.f }
  //   },
  // };
  // staticPose.boneTransforms["Upper_Arm_L"] = BoneTransforms {
  //   .positionTimestamps = { 0.f * tps, 1.f * tps, 2.f * tps },
  //   .positions = {
  //     { 0.f, 0.f, 0.f },
  //     { 1.f, 1.f, 0.f },
  //     { 0.f, 0.f, 0.f }
  //   },

  //   .rotationTimestamps = { 0.f * tps, 1.f * tps, 2.f * tps },
  //   .rotations = {
  //     glm::quat(glm::radians(-45.f), glm::vec3(0.f, 0.f, 1.f)),
  //     glm::quat(glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f)),
  //     glm::quat(glm::radians(-45.f), glm::vec3(0.f, 0.f, 1.f)),
  //   },
  // };
  // staticPose.duration = 2.f * tps;
  // unsigned int index = mModel.addAnimation(staticPose);
  // setAnimation(index);
}
