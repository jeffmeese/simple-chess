INCLUDEPATH += ../

CONFIG(release, debug|release) {
    OBJECTS_DIR = build/release
    DESTDIR = ../bin/release
    LIBS += -L../lib/release
}

CONFIG(debug, debug|release) {
    OBJECTS_DIR = build/debug
    DESTDIR = ../bin/debug
    LIBS += -L../lib/debug
}
