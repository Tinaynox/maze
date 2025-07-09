source var.sh

SCRIPT=$(readlink -f "$0")
SCRIPT_PATH=$(dirname "$SCRIPT")

RESOURCES_DIR=$SCRIPT_PATH/../../resources
source ../../../templates/prj/linux/make-data-x64.sh