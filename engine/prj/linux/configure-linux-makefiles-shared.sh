source var.sh

SCRIPT=$(readlink -f "$0")
SCRIPT_PATH=$(dirname "$SCRIPT")
PRJ_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/prj.makefiles-shared"

mkdir -p $PRJ_DIR
cd $PRJ_DIR

CMAKELISTS_DIR=$SCRIPT_PATH/../../
cmake $CMAKELISTS_DIR -G "Unix Makefiles"			\
	-DBUILD_SHARED_LIBS=ON