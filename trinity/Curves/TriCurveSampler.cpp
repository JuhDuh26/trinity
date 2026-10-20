// Copyright © 2026 CCP ehf.

#include "StdAfx.h"
#include "TriCurveSampler.h"

#include <ITriFunction.h>

namespace
{
// Each goes through GetValueAt, which typed curves implement without touching
// the state UpdateValue keeps - so a curve shared with others, or one that
// fires events as it advances, is unaffected by being sampled.
void SampleScalar( IRoot* curve, double time, void* out )
{
	*static_cast<float*>( out ) = ITriScalarFunctionPtr( BlueCastPtr( curve ) )->GetValueAt( time );
}

void SampleVector( IRoot* curve, double time, void* out )
{
	ITriVectorFunctionPtr( BlueCastPtr( curve ) )->GetValueAt( static_cast<Vector3*>( out ), time );
}

void SampleQuaternion( IRoot* curve, double time, void* out )
{
	ITriQuaternionFunctionPtr( BlueCastPtr( curve ) )->GetValueAt( static_cast<Quaternion*>( out ), time );
}

void SampleColor( IRoot* curve, double time, void* out )
{
	ITriColorFunctionPtr( BlueCastPtr( curve ) )->GetValueAt( static_cast<Color*>( out ), time );
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
