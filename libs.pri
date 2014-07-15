CONFIG(release, debug|release) {
  LIBS += $$BUILDROOT/lib/libcore.a
  PRE_TARGETDEPS += $$BUILDROOT/lib/libcore.a
}

CONFIG(debug, debug|release) {
  LIBS += $$BUILDROOT/lib/libcored.a
  PRE_TARGETDEPS += $$BUILDROOT/lib/libcored.a
}
