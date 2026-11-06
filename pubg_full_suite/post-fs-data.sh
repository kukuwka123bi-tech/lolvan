#!/system/bin/sh
# Post-fs-data script for PUBG Zygisk Suite
# This runs after /data is mounted

MODDIR=${0%/*}

# Create necessary directories if they don't exist
mkdir -p /data/adb/modules/pubg_full_suite

# Copy zygisk library to proper location for Magisk Zygisk
if [ -f "$MODDIR/zygisk/arm64-v8a.so" ]; then
    cp -f "$MODDIR/zygisk/arm64-v8a.so" /data/adb/zygisk/pubg_full_suite.so 2>/dev/null || \
    cp -f "$MODDIR/zygisk/arm64-v8a.so" /data/adb/magisk/zygisk/pubg_full_suite.so 2>/dev/null || \
    cp -f "$MODDIR/zygisk/arm64-v8a.so" /data/adb/modules/pubg_full_suite/zygisk.so 2>/dev/null
fi

# Set permissions
chmod 644 /data/adb/zygisk/pubg_full_suite.so 2>/dev/null
chmod 644 /data/adb/magisk/zygisk/pubg_full_suite.so 2>/dev/null
chmod 644 /data/adb/modules/pubg_full_suite/zygisk.so 2>/dev/null

# Log that we've run
 echo "[PUBG Suite] post-fs-data executed" > /dev/kmsg 2>/dev/null || true