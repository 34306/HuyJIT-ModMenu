#export THEOS=/var/mobile/theos
ARCHS = arm64
DEBUG = 0
FINALPACKAGE = 1
FOR_RELEASE = 1

include $(THEOS)/makefiles/common.mk

# ناوی دایبلەکە لێرە دەگۆڕدرێت بۆ HARA_IOS
TWEAK_NAME = HARA_IOS

$(TWEAK_NAME)_FRAMEWORKS = UIKit Foundation Security QuartzCore CoreGraphics CoreText AVFoundation Accelerate GLKit SystemConfiguration GameController

$(TWEAK_NAME)_CCFLAGS = -std=c++11 -fno-rtti -fno-exceptions -DNDEBUG
$(TWEAK_NAME)_CFLAGS = -fobjc-arc -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-value

# لێرەدا هەموو فایلە زیادەکانی وەک مۆد مینیۆ و ESP لادراوە بۆ ئەوەی تەنها ڕیکلامەکە بێت
$(TWEAK_NAME)_FILES = Tweak.mm

include $(THEOS_MAKE_PATH)/tweak.mk
