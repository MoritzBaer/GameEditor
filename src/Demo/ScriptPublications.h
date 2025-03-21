#pragma once

#include "Demo/Scripts.h"

#define SCRIPT_PUBLICATIONS                                                                 \
    SCRIPT_PUBLISH(Demo::SpinnyScript, S_PUBLISH(rotationSpeed))                              \
    SCRIPT_PUBLISH(Demo::BobbyScript, S_PUBLISH_RANGE(bobbingAmplitude, 0.001f, 1.0f, 0.001f))