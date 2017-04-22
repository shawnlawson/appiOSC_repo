import math

SAMPLES = 256
OUTMAX = 255

import sys
if len(sys.argv) > 1:
    SAMPLES = int(sys.argv[1])
if len(sys.argv) > 2:
    OUTMAX = int(sys.argv[2])

section = SAMPLES // 4
for direction in (1, -1):
    for i in range(section):
        tri = i * (1.0/section) * direction
        rescaled = int(round(((tri + 1) * OUTMAX) / 2.0))
        print str(rescaled)+",",
    for i in range(section):
        tri = (1.0 - (i * (1.0/section))) * direction
        rescaled = int(round(((tri + 1) * OUTMAX) / 2.0))
        print str(rescaled)+",",
