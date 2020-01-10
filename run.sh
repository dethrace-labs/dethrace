#! /bin/bash
set -eu

# Required environment variables:
#   DETHRACE_ROOT_DIR:
#   - Carmageddon root directory (containing DATA directory)
#   - eg DETHRACE_ROOT_DIR=/home/jeff/carmageddon run.sh
#

CODE_DIR=$(pwd)
ln -s $CODE_DIR ${DETHRACE_ROOT_DIR}
${CODE_DIR}/src/DETHRACE/build/c1
