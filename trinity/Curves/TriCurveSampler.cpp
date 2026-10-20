// Copyright © 2026 CCP ehf.

#include "StdAfx.h"
#include "TriCurveSampler.h"

#include <ITriFunction.h>

namespace
{
// Each goes through GetValueAt, which typed curves implement without touching
// the state UpdateValue keeps - so a curve shared with others, or one that
// fires events as it advances, is unaffected by being sampled.
void SampleScalar( IRoot* curve, double time, Vector4& out )
{
	out.x = ITriScalarFunctionPtr( BlueCastPtr( curve ) )->GetValueAt( time );
}

void SampleVector( IRoot* curve, double time, Vector4& out )
{
	ITriVectorFunctionPtr( BlueCastPtr( curve ) )->GetValueAt( reinterpret_cast<Vector3*>( &out ), time );
}

void SampleQuaternion( IRoot* curve, double time, Vector4& out )
{
	ITriQuaternionFunctionPtr( BlueCastPtr( curve ) )->GetValueAt( reinterpret_cast<Quaternion*>( &out ), time );
}

void SampleColor( IRoot* curve, double time, Vector4& out )
{
	ITriColorFunctionPtr( BlueCastPtr( curve ) )->GetValueAt( reinterpret_cast<Color*>( &out ), time );
}
}

TriCurveSampleFunc FindCurveSampleFunc( IRoot* source, const Be::VarEntry* valueEntry )
{
	if( valueEntry->mType == Be::FLOAT )
	{
		return ITriScalarFunctionPtr( BlueCastPtr( source ) ) ? SampleScalar : nullptr;
	}

	if( valueEntry->mType != Be::FLOATARRAY )
	{
		return nullptr;
	}

	switch( valueEntry->GetFloatArraySize() )
	{
	case 3:
		return ITriVectorFunctionPtr( BlueCastPtr( source ) ) ? SampleVector : nullptr;
	case 4:
		if( ITriQuaternionFunctionPtr( BlueCastPtr( source ) ) )
		{
			return SampleQuaternion;
		}
		return ITriColorFunctionPtr( BlueCastPtr( source ) ) ? SampleColor : nullptr;
	default:
		return nullptr;
	}
}
