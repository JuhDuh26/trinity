// Copyright © 2023 CCP ehf.

#pragma once
#ifndef ITr2ValueBinding_H
#define ITr2ValueBinding_H

BLUE_INTERFACE( ITr2ValueBinding ) :
	public IRoot
{
	virtual void CopyValue() = 0;

	// Time offset, in seconds, that this binding applies to its source curve
	// before copying the value. Zero for bindings that always read the source
	// as it stands.
	virtual float GetTimeOffset() const
	{
		return 0.0f;
	}

	// Copies the value with the source curve evaluated at sampleTime, then puts
	// the curve back to restoreTime so that a curve shared by several bindings
	// is left as its curve set left it. Only called for bindings that report a
	// non-zero time offset.
	virtual void CopyValueSampledAt( double sampleTime, double restoreTime )
	{
		CopyValue();
	}
};

#endif