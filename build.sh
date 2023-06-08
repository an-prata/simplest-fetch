#!/usr/bin/bash

SOURCE_DIR="src"
PROJECT_NAME="simplest-fetch"
SOURCE_FILES=$(find "./$SOURCE_DIR/" -name "*.c")
HEADER_FILES=$(find "./$SOURCE_DIR/" -name "*.h")

mkdir build -p

for FILE in $SOURCE_FILES; do
    gcc -Wall -Wextra -c $FILE -o "build/$(basename $FILE).o"

    if [ "$?" != "0" ]; then
        exit 1
    fi
done

OBJECT_FILES=$(find "./build/" -name "*.o" -printf "%p ")

gcc $OBJECT_FILES -o "./build/$PROJECT_NAME"

if [ "$?" != "0" ]; then
    exit 1
fi

