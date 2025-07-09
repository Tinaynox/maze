#!/bin/bash
SCRIPT_PATH=$(cd "$(dirname "$0")"; pwd)
cd $SCRIPT_PATH
source var.sh


RESOURCES_DIR=$SCRIPT_PATH/../../resources
source ../../../templates/prj/osx/make-data-x64.sh

read -s -n 1 -p "Press any key to continue..."