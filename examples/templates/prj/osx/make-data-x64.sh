BUILD_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/build.x64"

mkdir -p $BUILD_DIR
cd $BUILD_DIR

source $MAZE_ENGINE_DIR/../examples/templates/prj/osx/assembly-resources-x1.0-nc.sh
python3 $MAZE_ENGINE_DIR/../tools/scripts/assets/copy_resources.py --src $RESOURCES_X1_NC_DIR --dst $BUILD_DIR/data
rsync -r  $EXAMPLES_LIB_DIR/x64 $BUILD_DIR