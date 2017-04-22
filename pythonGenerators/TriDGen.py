import math

SAMPLES = 256
OUTMAX = 255

import sys
if len(sys.argv) > 1:
    SAMPLES = int(sys.argv[1])
if len(sys.argv) > 2:
    OUTMAX = int(sys.argv[2])


for sample in range(SAMPLES):
    tri = 1.0 - (float(sample) / float(SAMPLES))
    rescaled = int(round((tri * OUTMAX)))
    print str(rescaled)+",",
