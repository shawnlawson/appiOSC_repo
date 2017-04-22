import math

SAMPLES = 256
OUTMAX = 255

import sys
if len(sys.argv) > 1:
    SAMPLES = int(sys.argv[1])
if len(sys.argv) > 2:
    OUTMAX = int(sys.argv[2])

angle_range = [9999, -9999]
sine_range = [99, -99]
rescaled_range = [9999, -9999]

for sample in range(SAMPLES):
    angle = (sample * 360.0) / SAMPLES
    sine = math.sin(math.radians(angle))
    rescaled = int(round(((sine + 1) * OUTMAX) / 2.0))

    angle_range[0] = min(angle_range[0], angle)
    angle_range[1] = max(angle_range[1], angle)
    sine_range[0] = min(sine_range[0], sine)
    sine_range[1] = max(sine_range[1], sine)
    rescaled_range[0] = min(rescaled_range[0], rescaled)
    rescaled_range[1] = max(rescaled_range[1], rescaled)

    # print "%4d" % sample,
    # print "%4d" % angle,
    # print "%7.3f" % sine,
    # print "%4d" % rescaled,
    # print "#" * rescaled,
    # print
    print str(rescaled)+",",

print "-"*60
print "Angle range:", angle_range
print "Sine range:", sine_range
print "Rescaled range:",rescaled_range