INCLUDEPATH += ../
BUILDROOT = ../build

CONFIG(release, debug|release) {
  OBJECTS_DIR = $$BUILDROOT/$$APPNAME/obj/release
  DESTDIR = $$BUILDROOT/bin/release
}

CONFIG(debug, debug|release) {
  OBJECTS_DIR = $$BUILDROOT/$$APPNAME/obj/debug
  DESTDIR = $$BUILDROOT/bin/debug
}
