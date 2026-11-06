#!/data/data/com.termux/files/usr/bin/bash
# =============================================================================
# PUBG Zygisk Suite - One-Click Install Script
# =============================================================================
# This is the MAIN script - run this to compile and install the module
# Run this in Termux on your Android device with root
# =============================================================================

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}==========================================${NC}"
echo -e "${BLUE}  PUBG Zygisk Suite v2.0.0${NC}"
echo -e "${BLUE}  One-Click Install${NC}"
echo -e "${BLUE}==========================================${NC}"
echo ""

# Check if running in Termux
if [ ! -d /data/data/com.termux ]; then
    echo -e "${RED}ERROR: This script must be run in Termux!${NC}"
    echo ""
    echo "Install Termux from F-Droid:"
    echo "https://f-droid.org/en/packages/com.termux/"
    echo ""
    exit 1
fi

# Check for root
if [ ! -f /su/bin/su ]; then
    echo -e "${YELLOW}WARNING: Root access not detected!${NC}"
    echo "Some features may not work without root."
    echo ""
    read -p "Continue anyway? (y/n): " -n 1 -r
    echo ""
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Main menu
while true; do
    echo -e "${BLUE}==========================================${NC}"
    echo -e "${BLUE}  Main Menu${NC}"
    echo -e "${BLUE}==========================================${NC}"
    echo ""
    echo "1. Full Setup + Compile + Install (Recommended)"
    echo "2. Setup Termux environment only"
    echo "3. Compile module only"
    echo "4. Create ZIP for Magisk only"
    echo "5. Exit"
    echo ""
    read -p "Select option (1-5): " -n 1 -r
    echo ""
    
    case $REPLY in
        1)
            echo -e "${GREEN}[*] Starting full setup...${NC}"
            chmod +x setup-termux.sh
            ./setup-termux.sh
            
            echo -e "${GREEN}[*] Compiling module...${NC}"
            chmod +x build-module.sh
            ./build-module.sh
            
            echo -e "${GREEN}[*] Module compiled successfully!${NC}"
            echo ""
            echo -e "${GREEN}==========================================${NC}"
            echo -e "${GREEN}  INSTALLATION COMPLETE!${NC}"
            echo -e "${GREEN}==========================================${NC}"
            echo ""
            echo "Module file: pubg_full_suite.zip"
            echo ""
            echo "To install:"
            echo "  1. Copy pubg_full_suite.zip to /sdcard/Download/"
            echo "  2. Open Magisk -> Modules -> Install from storage"
            echo "  3. Select pubg_full_suite.zip"
            echo "  4. Reboot your device"
            echo "  5. Enable Zygisk in Magisk settings"
            echo ""
            echo -e "${YELLOW}IMPORTANT: Update offsets in src/main.cpp for your game version!${NC}"
            echo ""
            break
            ;;
        2)
            echo -e "${GREEN}[*] Setting up Termux environment...${NC}"
            chmod +x setup-termux.sh
            ./setup-termux.sh
            break
            ;;
        3)
            echo -e "${GREEN}[*] Compiling module...${NC}"
            chmod +x build-module.sh
            ./build-module.sh
            break
            ;;
        4)
            echo -e "${GREEN}[*] Creating ZIP package...${NC}"
            cd pubg_full_suite
            zip -r ../pubg_full_suite.zip pubg_full_suite -x "pubg_full_suite/src/build/*" "pubg_full_suite/.git/*"
            cd ..
            echo -e "${GREEN}[*] ZIP created: pubg_full_suite.zip${NC}"
            break
            ;;
        5)
            echo -e "${YELLOW}Exiting...${NC}"
            exit 0
            ;;
        *)
            echo -e "${RED}Invalid option!${NC}"
            ;;
    esac
done

echo ""
echo -e "${BLUE}==========================================${NC}"
echo -e "${BLUE}  Need help?${NC}"
echo -e "${BLUE}==========================================${NC}"
echo ""
echo "Read TERMUX-GUIDE.md for detailed instructions"
echo "Read BUILD.md for build and configuration options"
echo "Read README.md for feature list"
echo ""
