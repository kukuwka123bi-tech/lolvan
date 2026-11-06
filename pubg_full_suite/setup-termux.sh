#!/data/data/com.termux/files/usr/bin/bash
# =============================================================================
# PUBG Zygisk Suite - Termux Setup Script
# =============================================================================
# This script sets up Termux environment for compiling the PUBG module
# Run this in Termux on your Android device
# =============================================================================

echo "=========================================="
echo "  PUBG Zygisk Suite - Termux Setup"
echo "=========================================="
echo ""

# Check if running in Termux
if [ ! -d /data/data/com.termux ]; then
    echo "ERROR: This script must be run in Termux!"
    echo "Install Termux from F-Droid: https://f-droid.org/en/packages/com.termux/"
    exit 1
fi

# Update packages
echo "[1/5] Updating packages..."
pkg update -y

# Install required packages
echo "[2/5] Installing required packages..."
pkg install -y \
    git \
    cmake \
    clang \
    make \
    python \
    wget \
    unzip \
    openjdk-17

# Install Android NDK
echo "[3/5] Installing Android NDK..."
NDK_VERSION="r26b"
NDK_DIR="$HOME/android-ndk"
NDK_ZIP="android-ndk-${NDK_VERSION}-linux.zip"

if [ ! -d "$NDK_DIR" ]; then
    wget -q https://dl.google.com/android/repository/${NDK_ZIP} -O /tmp/${NDK_ZIP}
    unzip -q /tmp/${NDK_ZIP} -d $HOME
    mv $HOME/android-ndk-${NDK_VERSION} $NDK_DIR
    rm /tmp/${NDK_ZIP}
    echo "NDK installed to: $NDK_DIR"
else
    echo "NDK already installed"
fi

# Set environment variables
echo "[4/5] Setting up environment..."
echo "export NDK_HOME=$NDK_DIR" >> $HOME/.bashrc
echo "export ANDROID_NDK=$NDK_HOME" >> $HOME/.bashrc
echo "export PATH=\"$PATH:$NDK_HOME\"" >> $HOME/.bashrc
source $HOME/.bashrc

# Clone or update the repository
echo "[5/5] Setting up repository..."
if [ -d "pubg-zygisk-suite" ]; then
    cd pubg-zygisk-suite
    git pull
else
    git clone https://github.com/kukuwka123bi-tech/lolvan.git pubg-zygisk-suite
    cd pubg-zygisk-suite
fi

cd pubg_full_suite

echo ""
echo "=========================================="
echo "  Setup complete!"
echo "=========================================="
echo ""
echo "To compile the module, run:"
echo "  ./build-module.sh"
echo ""
echo "After compilation, install via Magisk:"
echo "  1. Copy pubg_full_suite.zip to your device"
echo "  2. Open Magisk -> Modules -> Install from storage"
echo "  3. Select pubg_full_suite.zip"
echo "  4. Reboot"
echo ""
