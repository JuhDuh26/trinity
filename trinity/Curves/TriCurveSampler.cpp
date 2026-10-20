// Copyright © 2026 CCP ehf.

#include "StdAfx.h"
#include "TriCurveSampler.h"

TriCurveSampler TriCurveSampler::Find( IRoot* source, const Be::VarEntry* valueEntry )
{
	// The interface is looked up once here rather than on every sample. The raw
	// pointer stays valid for as long as source does: the smart pointers only
	// hold their own reference while we look.
	TriCurveSampler sampler;

	if( valueEntry->mType == Be::FLOAT )
	{
		if( ITriScalarFunctionPtr scalar = BlueCastPtr( source ) )
		{
			sampler.m_kind = Kind::Scalar;
			sampler.m_curve.scalar = scalar.p;
		}
	}
	else if( valueEntry->mType == Be::FLOATARRAY && valueEntry->GetFloatArraySize() == 3 )
	{
		if( ITriVectorFunctionPtr vector = BlueCastPtr( source ) )
		{
			sampler.m_kind = Kind::Vector;
			sampler.m_curve.vector = vector.p;
		}
	}
	else if( valueEntry->mType == Be::FLOATARRAY && valueEntry->GetFloatArraySize() == 4 )
	{
		if( ITriQuaternionFunctionPtr quaternion = BlueCastPtr( source ) )
		{
			sampler.m_kind = Kind::Quaternion;
			sampler.m_curve.quaternion = quaternion.p;
		}
		else if( ITriColorFunctionPtr color = BlueCastPtr( source ) )
		{
			sampler.m_kind = Kind::Color;
			sampler.m_curve.color = color.p;
		}
	}

	return sampler;
}
