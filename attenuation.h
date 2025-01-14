#ifndef ATTENUATION_H
#define ATTENUATION_H

struct Attenuation {
  float constant;
  float linear;
  float quadratic;
};

// https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
const Attenuation ATTENUATION_3250 = { 1.0f, 0.0014f, 0.000007f };

const Attenuation ATTENUATION_600 = { 1.0f, 0.007f, 0.0002f };

const Attenuation ATTENUATION_325 = { 1.0f, 0.014f, 0.0007f };

const Attenuation ATTENUATION_200 = { 1.0f, 0.022f, 0.0019f };

const Attenuation ATTENUATION_160 = { 1.0f, 0.027f, 0.0028f };

const Attenuation ATTENUATION_100 = { 1.0f, 0.045f, 0.0075f };

const Attenuation ATTENUATION_65 = { 1.0f, 0.07f, 0.017f };

const Attenuation ATTENUATION_50 = { 1.0f, 0.09f, 0.032f };

const Attenuation ATTENUATION_32 = { 1.0f, 0.14f, 0.07f };

const Attenuation ATTENUATION_20 = { 1.0f, 0.22f, 0.20f };

const Attenuation ATTENUATION_13 = { 1.0f, 0.35f, 0.44f };

const Attenuation ATTENUATION_7 = { 1.0f, 0.7f, 1.8f };

#endif
