#pragma once

#include "required.h"

void flycamera(
    float * _Nonnull cam_vec3, float * _Nonnull cam_quat,
    float * _Nonnull cam_m3x3, float look_mult, float move_mult, float x_delta,
    float y_delta, float key_f, float key_l, float key_b, float key_r,
    float key_u, float key_d);
