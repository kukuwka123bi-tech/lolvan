#!/system/bin/sh
# Service script for PUBG Zygisk Suite
# This runs in late_start service context

MODDIR=${0%/*}

# Wait for Zygisk to be fully initialized
while [ ! -f /sbin/magisk ]; do
    sleep 1
done

# Ensure our library is in the right place
if [ -f "$MODDIR/zygisk/arm64-v8a.so" ]; then
    # For Magisk Zygisk, libraries should be in /data/adb/zygisk/
    cp -f "$MODDIR/zygisk/arm64-v8a.so" /data/adb/zygisk/pubg_full_suite.so
    chmod 644 /data/adb/zygisk/pubg_full_suite.so
    
    # Also copy to module directory for persistence
    cp -f "$MODDIR/zygisk/arm64-v8a.so" /data/adb/modules/pubg_full_suite/zygisk.so
    chmod 644 /data/adb/modules/pubg_full_suite/zygisk.so
fi

# Log startup
 echo "[PUBG Suite] Service started - Zygisk module loaded" > /dev/kmsg 2>/dev/null || true

# Keep running (Magisk will manage this as a service)
while true; do
    sleep 3600
done