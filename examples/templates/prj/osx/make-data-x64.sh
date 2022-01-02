SCRIPT_PATH=$(cd "$(dirname "$0")"; pwd)

BUILD_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/build.x64"

mkdir -p $BUILD_DIR
cd $BUILD_DIR

source $SCRIPT_PATH/assembly-resources-x1.0-nc.sh
python3 $MAZE_ENGINE_DIR/../examples/templates/resources/copy_resources.py --src $RESOURCES_X1_NC_DIR --dst $BUILD_DIR/data
rsync -r $EXAMPLES_LIB_DIR/x64 $BUILD_DIR