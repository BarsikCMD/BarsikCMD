#!/bin/bash
# Usage: ./build.sh [-s] <version>

CUSTOM_COMPRESSION=0

while getopts "s" opt; do
  case $opt in
    s) CUSTOM_COMPRESSION=1 ;;
    *) echo "Usage: $0 [-s] <version>"; exit 1 ;;
  esac
done

shift $((OPTIND - 1))
VERSION=$1

if [ -z "$VERSION" ]; then
  echo "Error: version not specified"
  echo "Usage: $0 [-s] <version>"
  exit 1
fi

PKG_DIR="barsik-cmd_${VERSION}_amd64"

mkdir -p "$PKG_DIR/DEBIAN"
mkdir -p "$PKG_DIR/usr/local/bin"

if [ ! -f "$PKG_DIR/DEBIAN/control" ]; then
  cat > "$PKG_DIR/DEBIAN/control" << CONTROL
Package: barsik-cmd
Version: $VERSION
Architecture: amd64
Maintainer: Your Name <your@email.com>
Description: BarsikCMD - CLI utility
CONTROL
fi

# Check binary exists
BINARY="$PKG_DIR/usr/local/bin/barsik-cmd"
if [ ! -f "$BINARY" ]; then
  echo "Error: binary not found at $BINARY"
  exit 1
fi

chmod 755 "$BINARY"
chmod 755 "$PKG_DIR/DEBIAN"
chmod 755 "$PKG_DIR/DEBIAN/postinst" 2>/dev/null

# Calculate installed size (in KB)
INSTALLED_SIZE=$(du -sk --exclude="$PKG_DIR/DEBIAN" "$PKG_DIR" | cut -f1)
if grep -q "^Installed-Size:" "$PKG_DIR/DEBIAN/control"; then
  sed -i "s/^Installed-Size: .*/Installed-Size: $INSTALLED_SIZE/" "$PKG_DIR/DEBIAN/control"
else
  echo "Installed-Size: $INSTALLED_SIZE" >> "$PKG_DIR/DEBIAN/control"
fi

if [ "$CUSTOM_COMPRESSION" -eq 1 ]; then
  echo "Compressor (gzip/xz/zstd/none):"
  read COMPRESSOR
  echo "Level (1-9):"
  read LEVEL
  dpkg-deb -Z"$COMPRESSOR" -z"$LEVEL" --build "$PKG_DIR"
else
  dpkg-deb --build "$PKG_DIR"
fi

echo "Done: ${PKG_DIR}.deb"