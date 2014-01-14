#pragma once

#include "ofMain.h"

#ifdef TARGET_OSX
#undef err_local
#endif

#include "cl.hpp"

#define OFX_CL_BEGIN_NAMESPACE namespace ofx { namespace CL {
#define OFX_CL_END_NAMESPACE } }

OFX_CL_BEGIN_NAMESPACE

bool reportError(cl_int err);

OFX_CL_END_NAMESPACE

namespace ofxCL = ofx::CL;