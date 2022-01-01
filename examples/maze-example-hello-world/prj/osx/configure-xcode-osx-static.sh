source var.sh

SCRIPT_PATH=$(cd "$(dirname "$0")"; pwd)

CMAKELISTS_DIR=$SCRIPT_PATH/../../
source ../../../templates/prj/osx/configure-xcode-osx-static.sh