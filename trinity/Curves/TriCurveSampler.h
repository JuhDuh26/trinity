// Copyright © 2026 CCP ehf.

#pragma once
#ifndef TriCurveSampler_h
#define TriCurveSampler_h

#include <ITriFunction.h>

// Samples a typed curve's value at any time through GetValueAt, without moving
// the curve's current value - so a curve shared with others, or one that fires
// events as it advances, is unaffected. (Curves may still update internal
// lookup caches, which never changes what they return.)
//
// Holds a raw interface pointer found once, so it is only valid while the
// source object is, and must be found again whenever the source changes.
class TriCurveSampler
{
public:
	// A sampler for source when valueEntry is its value attribute - a scalar,
	// vector, quaternion or color curve - or an empty sampler otherwise.
	static TriCurveSampler Find( IRoot* source, const Be::VarEntry* valueEntry );

	explicit operator bool() const
	{
		return m_kind != Kind::None;
	}

	// The curve's value at time, laid out from out.x as in its currentValue: a
	// scalar fills x, a vector xyz, a quaternion or color all four.
	void Sample( double time, Vector4& out ) const
	{
		switch( m_kind )
		{
		case Kind::Scalar:
			out.x = m_curve.scalar->GetValueAt( time );
			break;
		case Kind::Vector:
			m_curve.vector->GetValueAt( reinterpret_cast<Vector3*>( &out ), time );
			break;
		case Kind::Quaternion:
			m_curve.quaternion->GetValueAt( reinterpret_cast<Quaternion*>( &out ), time );
			break;
		case Kind::Color:
			m_curve.color->GetValueAt( reinterpret_cast<Color*>( &out ), time );
			break;
		case Kind::None:
			break;
		}
	}

private:
	enum class Kind : uint8_t
	{
		None,
		Scalar,
		Vector,
		Quaternion,
		Color
	};

	Kind m_kind = Kind::None;
	union
	{
		ITriScalarFunction* scalar;
		ITriVectorFunction* vector;
		ITriQuaternionFunction* quaternion;
		ITriColorFunction* color;
	} m_curve = { nullptr };
};

#endif // TriCurveSampler_h
