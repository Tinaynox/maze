source var.sh

SCRIPT_PATH=$(cd "$(dirname "$0")"; pwd)
PRJ_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/prj.xcode-osx-shared"

mkdir -p $PRJ_DIR
cd $PRJ_DIR

CMAKELISTS_DIR=$SCRIPT_PATH/../../
cmake $CMAKELISTS_DIR -G "Xcode"			\
	-DBUILD_SHARED_LIBS=ON