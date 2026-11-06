#!/data/data/com.termux/files/usr/bin/bash
# =============================================================================
# PUBG Zygisk Suite - Build Script
# =============================================================================
# This script compiles the PUBG Zygisk module on your Android device
# Run this in Termux after running setup-termux.sh
# =============================================================================

echo "=========================================="
echo "  PUBG Zygisk Suite - Build Script"
echo "=========================================="
echo ""

# Check NDK
if [ ! -d "$NDK_HOME" ]; then
    echo "ERROR: NDK not found!"
    echo "Run setup-termux.sh first!"
    exit 1
fi

echo "[1/5] Setting up build environment..."

# Create build directory
cd src
mkdir -p build
cd build

echo "[2/5] Running CMake..."
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$NDK_HOME/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-26 \
    -DANDROID_STL=c++_static \
    -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "ERROR: CMake failed!"
    echo "Check if NDK is properly installed"
    exit 1
fi

echo "[3/5] Compiling..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

echo "[4/5] Copying library..."
cp libpubg_suite.so ../../zygisk/arm64-v8a.so

if [ ! -f ../../zygisk/arm64-v8a.so ]; then
    echo "ERROR: Library not found!"
    exit 1
fi

echo "[5/5] Creating module zip..."
cd ../..
zip -r pubg_full_suite.zip pubg_full_suite -x "pubg_full_suite/src/build/*" "pubg_full_suite/.git/*"

echo ""
echo "=========================================="
echo "  Build complete!"
echo "=========================================="
echo ""
echo "Module created: pubg_full_suite.zip"
echo ""
echo "To install:"
echo "  1. Copy pubg_full_suite.zip to your device storage"
echo "  2. Open Magisk -> Modules -> Install from storage"
echo "  3. Select pubg_full_suite.zip"
echo "  4. Reboot your device"
echo "  5. Enable Zygisk in Magisk settings"
echo ""
echo "IMPORTANT: Update offsets in src/main.cpp for your game version!"
echo ""
