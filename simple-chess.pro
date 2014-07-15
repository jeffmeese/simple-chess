TEMPLATE = subdirs

SUBDIRS += \
    console \
    gui \
    core

console.depends = core
gui.depends = core
