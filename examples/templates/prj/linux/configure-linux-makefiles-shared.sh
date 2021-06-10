PRJ_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/prj.makefiles"

mkdir -p $PRJ_DIR
cd $PRJ_DIR

cmake $CMAKELISTS_DIR -G "Unix Makefiles"			\
	-DBUILD_SHARED_LIBS=ON