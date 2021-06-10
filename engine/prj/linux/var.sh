SCRIPT=$(readlink -f "$0")
SCRIPT_PATH=$(dirname "$SCRIPT")
#set SCRIPT=`readlink -f "$0"`
#set SCRIPT_PATH=`dirname "$SCRIPT"`

echo SCRIPT_PATH=$SCRIPT_PATH

PROJECT_NAME="maze"
MAZE_ENGINE_DIR="$SCRIPT_PATH/../.."
PRJ_ROOT_DIR="$MAZE_ENGINE_DIR/../prj"