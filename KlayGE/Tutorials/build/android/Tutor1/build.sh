#!/bin/bash

$ANDROID_NDK/ndk-build -j 3

ant debug
adb install -r bin/KlayGE_Tutor1-debug.apk