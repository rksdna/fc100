# Debain deploy script
# Usage: sh deploy.sh SOURCE_DIR DESTINATION_DIR

set -e

# Prepare

DEB_ARCHITECTURE="amd64"
DEB_QT_DIR="/usr/lib/x86_64-linux-gnu/qt5/"

DEB_SOURCE_DIR=$(readlink -f "$1")
DEB_DEPLOY_DIR=$(readlink -f "$2")

test -d "$DEB_SOURCE_DIR" -a -d "$DEB_DEPLOY_DIR"

DEB_BUILD_DIR=$(mktemp -d -p /tmp)
DEB_INSTALL_DIR=$(mktemp -d -p /tmp)

DEB_PRODUCT="fct"
DEB_VERSION="1.0.0.$(cd "$DEB_SOURCE_DIR" && git rev-list --count master)"

echo $DEB_PRODUCT $DEB_VERSION

DEB_FILE="$DEB_DEPLOY_DIR/$DEB_PRODUCT""_""$DEB_VERSION""_""$DEB_ARCHITECTURE.deb"
DEB_MAINTANER="rksdna <rksdna@mail.ru>"
DEB_WEBSITE="https://bitbucket.org/rksdna/fct"

# Build and install

"$DEB_QT_DIR/bin/qmake" \
    DEPLOY_PACKAGE="$DEB_PRODUCT" \
    DEPLOY_PRODUCT="$DEB_PRODUCT" \
    DEPLOY_VERSION="$DEB_VERSION" \
    -o "$DEB_BUILD_DIR/Makefile" \
    "$DEB_SOURCE_DIR"

(cd "$DEB_BUILD_DIR" && make INSTALL_ROOT="\"$DEB_INSTALL_DIR\"" -j install)

sed -i \
    -e "s|DEB_PRODUCT|$DEB_PRODUCT|g" \
    -e "s|DEB_VERSION|$DEB_VERSION|g" \
    -e "s|DEB_MAINTAINER|$DEB_MAINTANER|g" \
    -e "s|DEB_DATE|$(date -R)|g" \
    -e "s|DEB_YEAR|$(date +%Y)|g" \
    "$DEB_INSTALL_DIR/usr/share/doc/$DEB_PRODUCT/changelog" \
    "$DEB_INSTALL_DIR/usr/share/doc/$DEB_PRODUCT/copyright"

gzip -9 -n \
    "$DEB_INSTALL_DIR/usr/share/doc/$DEB_PRODUCT/changelog" \
    "$DEB_INSTALL_DIR/usr/share/man/man1/$DEB_PRODUCT.1"

# Deploy

tree "$DEB_INSTALL_DIR"

mkdir -p "$DEB_INSTALL_DIR/DEBIAN"
install -p -m644 "$DEB_SOURCE_DIR/deploy/debian/control" "$DEB_INSTALL_DIR/DEBIAN"

sed -i \
    -e "s|DEB_PRODUCT|$DEB_PRODUCT|g" \
    -e "s|DEB_VERSION|$DEB_VERSION|g" \
    -e "s|DEB_MAINTAINER|$DEB_MAINTANER|g" \
    -e "s|DEB_SIZE|$(du -s "$DEB_INSTALL_DIR" | cut -f1)|g" \
    -e "s|DEB_WEBSITE|$DEB_WEBSITE|g" \
    -e "s|DEB_ARCHITECTURE|$DEB_ARCHITECTURE|g" \
    "$DEB_INSTALL_DIR/DEBIAN/control"

fakeroot dpkg-deb --build "$DEB_INSTALL_DIR" "$DEB_FILE"
lintian "$DEB_FILE"

#Clean

rm -r -f "$DEB_BUILD_DIR" "$DEB_INSTALL_DIR"
