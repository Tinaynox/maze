PRJ_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/prj.codeblocks-makefiles-shared"

mkdir -p $PRJ_DIR
cd $PRJ_DIR

cmake $CMAKELISTS_DIR -G "CodeBlocks - Unix Makefiles"			\
	-DBUILD_SHARED_LIBS=ON