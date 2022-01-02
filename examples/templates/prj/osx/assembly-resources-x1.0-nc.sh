SCRIPT_PATH=$(cd "$(dirname "$0")"; pwd)

source $MAZE_ENGINE_DIR/../examples/templates/prj/osx/var.sh

RESOURCES_X1_DIR="$PRJ_ROOT_DIR/resources/resources-x1.0/data/"
python3 "$MAZE_ENGINE_DIR/../examples/templates/resources/assembly_resources.py"  --src "$RESOURCES_DIR" --dst "$RESOURCES_X1_DIR" --tscale 1.0

RESOURCES_X1_NC_DIR="$PRJ_ROOT_DIR/resources/resources-x1.0-nc/data/"
python3 "$MAZE_ENGINE_DIR/../examples/templates/resources/make_data.py" --src "$RESOURCES_X1_DIR" --dst "$RESOURCES_X1_NC_DIR" --tools "$MAZE_ENGINE_DIR/../tools" --temp "$PRJ_ROOT_DIR/resources/.temp" --cache cache-win-x1.0-nc.pkl