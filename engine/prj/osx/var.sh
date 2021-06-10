#SCRIPT=$(readlink -f "$0")
#SCRIPTPATH=$(dirname "$SCRIPT")
#set SCRIPT=`readlink -f "$0"`
#set SCRIPTPATH=`dirname "$SCRIPT"`
SCRIPT_PATH=$(cd "$(dirname "$0")"; pwd)

PROJECT_NAME="maze"
MAZE_ENGINE_DIR="$SCRIPT_PATH/../.."
PRJ_ROOT_DIR="$MAZE_ENGINE_DIR/../prj"