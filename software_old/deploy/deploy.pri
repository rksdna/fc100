isEmpty(DEPLOY_PRODUCT): DEPLOY_PRODUCT = "fc100"
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
    $$PWD/windows/deploy.sh \
    $$PWD/windows/i686-w64-mingw32-qt-5.5.1.sh \
    $$PWD/windows/control.wxs \
    $$PWD/windows/changelog.txt \
    $$PWD/windows/license.txt

linux {
    target.path = /usr/bin
    translations.path = /usr/share/fc100

    docs.path = /usr/share/doc/fc100
    docs.files = \
        $$PWD/debian/copyright \
        $$PWD/debian/changelog

    mans.path = /usr/share/man/man1
    mans.files = \
        $$PWD/debian/fc100.1

    icons.path = /usr/share/icons
    icons.files = \
        $$PWD/debian/fc100.png

    shortcuts.path = /usr/share/applications
    shortcuts.files = \
        $$PWD/debian/fc100.desktop

    INSTALLS += target translations docs mans icons shortcuts
}

win32 {
    RC_LANG = 1033
    RC_CODEPAGE = 1252
    RC_ICONS = $$PWD/windows/fc100.ico

    QMAKE_TARGET_PRODUCT = $$DEPLOY_PRODUCT
    QMAKE_TARGET_COMPANY = $$DEPLOY_COMPANY
    QMAKE_TARGET_COPYRIGHT = "[copyright here]"
    QMAKE_TARGET_DESCRIPTION = "[description here]"

    target.path = /
    translations.path = /

    INSTALLS += target translations
}

DISTFILES += \
    $$PWD/debian/fc100.1 \
    $$PWD/debian/fc100.desktop \
    $$PWD/debian/fc100.png \
    $$PWD/windows/fc100.ico
