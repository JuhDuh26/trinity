// Copyright © 2026 CCP ehf.

#pragma once
#ifndef Tr2CurveSegmentSearch_h
#define Tr2CurveSegmentSearch_h

#include <algorithm>

// Index i of the key segment [keys[i], keys[i + 1]) that contains t, for a curve
// with at least two keys. Falls back to the last segment when none contains t.
//
// cachedSegment is a hint, updated to the segment found: curves sampled at
// steadily moving times find their segment at or next to the cached one. Each
// caller sampling at its own times - a curve's own UpdateValue, or a binding
// with a time offset - should keep its own hint, or they keep moving each
// other's and every lookup has to search.
template <typename Keys>
size_t FindCurveSegment( const Keys& keys, float t, size_t& cachedSegment )
{
	const size_t count = keys.size();
	const auto contains = [&]( size_t i ) { return t >= keys[i].m_time && t < keys[i + 1].m_time; };

	if( cachedSegment + 1 < count )
	{
		if( contains( cachedSegment ) )
		{
			return cachedSegment;
		}
		if( cachedSegment + 2 < count && contains( cachedSegment + 1 ) )
		{
			return ++cachedSegment;
		}
		if( cachedSegment > 0 && contains( cachedSegment - 1 ) )
		{
			return --cachedSegment;
		}
	}

	// Keys are kept sorted by time, so binary search for the segment.
	const auto next = std::upper_bound( keys.begin(), keys.end(), t, []( float time, const auto& key ) { return time < key.m_time; } );
	const size_t found = size_t( next - keys.begin() );
	if( found > 0 && found < count && contains( found - 1 ) )
	{
		return cachedSegment = found - 1;
	}

	// Keys edited out of order without being re-sorted: try every segment.
	for( size_t i = 0; i + 1 < count; ++i )
	{
		if( contains( i ) )
		{
			return cachedSegment = i;
		}
	}
	return cachedSegment = count - 2;
}

#endif // Tr2CurveSegmentSearch_h
