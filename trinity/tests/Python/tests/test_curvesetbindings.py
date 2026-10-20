# Copyright © 2026 CCP ehf.

import unittest

import trinity


def _LinearCurve():
    # currentValue == time over [0, 10]
    curve = trinity.Tr2CurveScalar()
    curve.AddKey(0.0, 0.0, trinity.Tr2CurveInterpolation.LINEAR)
    curve.AddKey(10.0, 10.0, trinity.Tr2CurveInterpolation.LINEAR)
    return curve


def _Bind(curveSet, source, sourceAttribute, timeOffset=0.0):
    destination = trinity.Tr2FloatParameter()
    binding = trinity.TriValueBinding()
    binding.sourceObject = source
    binding.sourceAttribute = sourceAttribute
    binding.destinationObject = destination
    binding.destinationAttribute = 'value'
    binding.timeOffset = timeOffset
    curveSet.bindings.append(binding)
    return destination


def _ApplyAt(curveSet, time):
    curveSet.scaledTime = time
    curveSet.Apply()


class TestCurveSetBindingTimeOffset(unittest.TestCase):
    def setUp(self):
        self.curve = _LinearCurve()
        self.curveSet = trinity.TriCurveSet()
        self.curveSet.curves.append(self.curve)

    def test_zero_offset_copies_curve_set_time(self):
        destination = _Bind(self.curveSet, self.curve, 'currentValue')
        _ApplyAt(self.curveSet, 5.0)
        self.assertAlmostEqual(destination.value, 5.0, places=4)

    def test_offset_samples_curve_at_shifted_time(self):
        lagging = _Bind(self.curveSet, self.curve, 'currentValue', timeOffset=2.0)
        leading = _Bind(self.curveSet, self.curve, 'currentValue', timeOffset=-1.5)
        _ApplyAt(self.curveSet, 5.0)
        self.assertAlmostEqual(lagging.value, 3.0, places=4)
        self.assertAlmostEqual(leading.value, 6.5, places=4)

    def test_offset_binding_leaves_shared_curve_alone(self):
        _Bind(self.curveSet, self.curve, 'currentValue', timeOffset=2.0)
        plain = _Bind(self.curveSet, self.curve, 'currentValue')
        _ApplyAt(self.curveSet, 5.0)
        self.assertAlmostEqual(self.curve.currentValue, 5.0, places=4)
        self.assertAlmostEqual(plain.value, 5.0, places=4)

    def test_offset_binding_leaves_curve_of_another_curve_set_alone(self):
        owner = trinity.TriCurveSet()
        curve = _LinearCurve()
        owner.curves.append(curve)
        _ApplyAt(owner, 8.0)

        destination = _Bind(self.curveSet, curve, 'currentValue', timeOffset=2.0)
        _ApplyAt(self.curveSet, 5.0)

        self.assertAlmostEqual(destination.value, 3.0, places=4)
        self.assertAlmostEqual(curve.currentValue, 8.0, places=4)

    def test_offset_wraps_into_looped_time_range(self):
        destination = _Bind(self.curveSet, self.curve, 'currentValue', timeOffset=2.0)
        self.curveSet.SetTimeRange(0.0, 4.0, True)
        _ApplyAt(self.curveSet, 1.0)
        # 1 - 2 = -1 wraps to 3 inside [0, 4)
        self.assertAlmostEqual(destination.value, 3.0, places=4)

    def test_offset_without_range_is_not_wrapped(self):
        destination = _Bind(self.curveSet, self.curve, 'currentValue', timeOffset=2.0)
        _ApplyAt(self.curveSet, 7.0)
        self.assertAlmostEqual(destination.value, 5.0, places=4)

    def test_offset_on_non_curve_source_copies_value_as_is(self):
        source = trinity.Tr2FloatParameter()
        source.value = 42.0
        destination = _Bind(self.curveSet, source, 'value', timeOffset=2.0)
        _ApplyAt(self.curveSet, 5.0)
        self.assertAlmostEqual(destination.value, 42.0, places=4)

    def test_offset_on_curve_attribute_other_than_current_value_is_ignored(self):
        self.curve.timeOffset = 0.25
        destination = _Bind(self.curveSet, self.curve, 'timeOffset', timeOffset=2.0)
        _ApplyAt(self.curveSet, 5.0)
        self.assertAlmostEqual(destination.value, 0.25, places=4)


if __name__ == '__main__':
    unittest.main()
