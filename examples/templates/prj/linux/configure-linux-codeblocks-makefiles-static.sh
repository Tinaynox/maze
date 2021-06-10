PRJ_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/prj.codeblocks-makefiles-static"

mkdir -p $PRJ_DIR
cd $PRJ_DIR

cmake $CMAKELISTS_DIR -G "CodeBlocks - Unix Makefiles"			\
	-DBUILD_SHARED_LIBS=OFF