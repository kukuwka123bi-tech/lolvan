#!/system/bin/sh
# Uninstall script for PUBG Zygisk Suite

MODDIR=${0%/*}

# Remove files from various locations
rm -f /data/adb/zygisk/pubg_full_suite.so
rm -f /data/adb/magisk/zygisk/pubg_full_suite.so
rm -f /data/adb/modules/pubg_full_suite/zygisk.so

# Remove module directory
rm -rf /data/adb/modules/pubg_full_suite
rm -rf "$MODDIR"

# Log uninstall
 echo "[PUBG Suite] Uninstalled" > /dev/kmsg 2>/dev/null || true