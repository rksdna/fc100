isEmpty(DEPLOY_PRODUCT): DEPLOY_PRODUCT = "fct"
isEmpty(DEPLOY_VERSION): DEPLOY_VERSION = "0.0.0.$$system(git rev-list --count HEAD)"

VERSION = $$DEPLOY_VERSION

DEFINES += \
    DEPLOY_PRODUCT="\"\\\"$$DEPLOY_PRODUCT\\\"\"" \
    DEPLOY_VERSION="\"\\\"$$DEPLOY_VERSION\\\"\""

OTHER_FILES += \
    $$PWD/debian/deploy.sh \
    $$PWD/debian/control \
    $$PWD/debian/copyright \
    $$PWD/debian/changelog \
    $$PWD/debian/fct.1 \
    $$PWD/debian/fct.desktop \
    $$PWD/debian/fct.png \
    $$PWD/windows/deploy.sh \
    $$PWD/windows/i686-w64-mingw32-qt-5.5.1.sh \
    $$PWD/windows/control.wxs \
    $$PWD/windows/changelog.txt \
    $$PWD/windows/license.txt \
    $$PWD/windows/fct.ico

linux {
    target.path = /usr/bin
    translations.path = /usr/share/fct

    docs.path = /usr/share/doc/fct
    docs.files = \
        $$PWD/debian/copyright \
        $$PWD/debian/changelog

    mans.path = /usr/share/man/man1
    mans.files = \
        $$PWD/debian/fct.1

    icons.path = /usr/share/icons
    icons.files = \
        $$PWD/debian/fct.png

    shortcuts.path = /usr/share/applications
    shortcuts.files = \
        $$PWD/debian/fct.desktop

    INSTALLS += target translations docs mans icons shortcuts
}

win32 {
    RC_LANG = 1033
    RC_CODEPAGE = 1252
    RC_ICONS = $$PWD/windows/fct.ico

    QMAKE_TARGET_PRODUCT = $$DEPLOY_PRODUCT
    QMAKE_TARGET_COMPANY = $$DEPLOY_COMPANY
    QMAKE_TARGET_COPYRIGHT = "[copyright here]"
    QMAKE_TARGET_DESCRIPTION = "[description here]"

    target.path = /
    translations.path = /

    INSTALLS += target translations
}
