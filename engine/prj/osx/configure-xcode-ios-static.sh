source var.sh

SCRIPT_PATH=$(cd "$(dirname "$0")"; pwd)
PRJ_DIR="$PRJ_ROOT_DIR/$PROJECT_NAME/prj.xcode-ios-static"

mkdir -p $PRJ_DIR
cd $PRJ_DIR

CMAKELISTS_DIR=$SCRIPT_PATH/../../
cmake $CMAKELISTS_DIR -G "Xcode"														\
	-DBUILD_SHARED_LIBS=OFF																\
	-DCMAKE_TOOLCHAIN_FILE=$MAZE_ENGINE_DIR/cmake/toolchains/ios/ios.toolchain.cmake 	\
	-DIOS_PLATFORM=OS																	\
	-DIOS_ARCH=arm64																	\
	-DENABLE_ARC=FALSE																	\
	-DENABLE_BITCODE=TRUE																