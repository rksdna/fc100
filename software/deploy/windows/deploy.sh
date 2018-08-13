# Windows deploy script
# Usage: sh deploy.sh SOURCE_DIR DESTINATION_DIR

set -e

# Prepare

MSI_ARCHITECTURE="x86"
MSI_QT_DIR="/opt/i686-w64-mingw32-qt-5.5.1"
MSI_RT_DIR="/usr/lib/gcc/i686-w64-mingw32/5.3-win32"

MSI_SOURCE_DIR=$(readlink -f "$1")
MSI_DEPLOY_DIR=$(readlink -f "$2")

test -d "$MSI_SOURCE_DIR" -a -d "$MSI_DEPLOY_DIR"

MSI_BUILD_DIR=$(mktemp -d -p /tmp)
MSI_INSTALL_DIR=$(mktemp -d -p /tmp)

MSI_PRODUCT="fct"
MSI_VERSION="1.0.0.$(cd "$MSI_SOURCE_DIR" && git rev-list --count master)"

MSI_FILE="$MSI_DEPLOY_DIR/$MSI_PRODUCT""_""$MSI_VERSION""_""$MSI_ARCHITECTURE.msi"

# Build and install

"$MSI_QT_DIR/bin/qmake" \
    DEPLOY_PRODUCT="$MSI_PRODUCT" \
    DEPLOY_VERSION="$MSI_VERSION" \
    -o "$MSI_BUILD_DIR/Makefile" \
    "$MSI_SOURCE_DIR"

(cd "$MSI_BUILD_DIR" && make INSTALL_ROOT="\"$MSI_INSTALL_DIR\"" -j install)

install -p -m 644 "$MSI_RT_DIR/libstdc++-6.dll" "$MSI_INSTALL_DIR"
install -p -m 644 "$MSI_RT_DIR/libgcc_s_sjlj-1.dll" "$MSI_INSTALL_DIR"

install -p -m 644 "$MSI_QT_DIR/bin/Qt5Core.dll" "$MSI_INSTALL_DIR"
install -p -m 644 "$MSI_QT_DIR/bin/Qt5Gui.dll" "$MSI_INSTALL_DIR"
install -p -m 644 "$MSI_QT_DIR/bin/Qt5Widgets.dll" "$MSI_INSTALL_DIR"
install -p -m 644 "$MSI_QT_DIR/bin/Qt5SerialPort.dll" "$MSI_INSTALL_DIR"

mkdir -p "$MSI_INSTALL_DIR/imageformats"
install -p -m 644 "$MSI_QT_DIR/plugins/imageformats/qico.dll" "$MSI_INSTALL_DIR/imageformats"
install -p -m 644 "$MSI_QT_DIR/plugins/imageformats/qjpeg.dll" "$MSI_INSTALL_DIR/imageformats"

mkdir -p "$MSI_INSTALL_DIR/platforms"
install -p -m 644 "$MSI_QT_DIR/plugins/platforms/qwindows.dll" "$MSI_INSTALL_DIR/platforms"

mkdir -p "$MSI_INSTALL_DIR/translations"
install -p -m 644 "$MSI_QT_DIR/translations/qt_en.qm" "$MSI_INSTALL_DIR/translations"
install -p -m 644 "$MSI_QT_DIR/translations/qt_ru.qm" "$MSI_INSTALL_DIR/translations"
install -p -m 644 "$MSI_QT_DIR/translations/qtbase_en.qm" "$MSI_INSTALL_DIR/translations"
install -p -m 644 "$MSI_QT_DIR/translations/qtbase_ru.qm" "$MSI_INSTALL_DIR/translations"

# Deploy

tree "$MSI_INSTALL_DIR"

find "$MSI_INSTALL_DIR" | wixl-heat \
    -p "$MSI_INSTALL_DIR/" \
    --directory-ref=ApplicationFilesFolder \
    --component-group Files \
    --var var.MSI_INSTALL_DIR \
    | tee "$MSI_INSTALL_DIR/files.wxi"

wixl \
    -v \
    -D MSI_PRODUCT="$MSI_PRODUCT" \
    -D MSI_VERSION="$MSI_VERSION" \
    -D MSI_INSTALL_DIR="$MSI_INSTALL_DIR" \
    -o "$MSI_FILE" \
    "$MSI_SOURCE_DIR/deploy/windows/control.wxs" "$MSI_INSTALL_DIR/files.wxi"

#Clean

rm -r -f "$MSI_BUILD_DIR" "$MSI_INSTALL_DIR"
