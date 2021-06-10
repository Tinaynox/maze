


PRJ_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/prj.xcode-osx-shared"

mkdir -p $PRJ_DIR
cd $PRJ_DIR

cmake $CMAKELISTS_DIR -G "Xcode"			\
	-DBUILD_SHARED_LIBS=ON