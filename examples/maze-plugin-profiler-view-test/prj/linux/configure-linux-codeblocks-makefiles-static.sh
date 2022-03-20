source var.sh

SCRIPT=$(readlink -f "$0")
SCRIPT_PATH=$(dirname "$SCRIPT")

CMAKELISTS_DIR=$SCRIPT_PATH/../../
source ../../../templates/prj/linux/configure-linux-codeblocks-makefiles-static.sh