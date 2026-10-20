// Copyright © 2026 CCP ehf.

#pragma once
#ifndef TriCurveSampler_h
#define TriCurveSampler_h

// Evaluates a curve at a time into out, without changing the curve's own state.
// The value is laid out from out.x, as in the curve's currentValue: a scalar
// fills x, a vector xyz, a quaternion or color all four.
typedef void ( *TriCurveSampleFunc )( IRoot* curve, double time, Vector4& out );

// How to sample source at an arbitrary time when valueEntry is its value
// attribute - a scalar, vector, quaternion or color curve - or nullptr if
// source is not a curve of the matching type.
TriCurveSampleFunc FindCurveSampleFunc( IRoot* source, const Be::VarEntry* valueEntry );

#endif // TriCurveSampler_h
