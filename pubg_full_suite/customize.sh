#!/system/bin/sh
SKIPUNZIP=1

# PUBG Zygisk Suite - Custom Install Script
ui_print " "
ui_print "========================================"
ui_print "   PUBG Zygisk Suite v2.0.0"
ui_print "   by dj"
ui_print "========================================"
ui_print " "

# Extract files
ui_print "- Extracting module files..."
unzip -o "$ZIPFILE" -x 'META-INF/*' -d "$MODPATH" >&2

# Set permissions
ui_print "- Setting permissions..."
set_perm_recursive "$MODPATH" 0 0 0755 0644
set_perm "$MODPATH/customize.sh" 0 0 0755
set_perm "$MODPATH/post-fs-data.sh" 0 0 0755 2>/dev/null || true
set_perm "$MODPATH/service.sh" 0 0 0755 2>/dev/null || true
set_perm "$MODPATH/zygisk/arm64-v8a.so" 0 0 0644 2>/dev/null || true

# Check for Zygisk
if [ ! -f /sbin/magisk ]; then
    ui_print "!!! Magisk not found! This module requires Magisk with Zygisk enabled."
    abort "Please install Magisk first and enable Zygisk in Magisk settings."
fi

ui_print " "
ui_print "- Module installed successfully!"
ui_print "- Reboot to activate."
ui_print " "