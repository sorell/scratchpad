#!/bin/sh
UP_TO_COMMIT=$1
PATH_TO_FIND=$(if [ "$2"z != ""z ]; then echo -n $2; else echo "."; fi)

if [ $# -lt 1 ]; then 
	echo "This script browses through git commits up to UP_TO_COMMIT listing changed filenames matching pattern PATH_TO_FIND"
	echo
	echo "Usage: $(basename $0) UP_TO_COMMIT [PATH_TO_FIND]"
	echo -e "\tUP_TO_COMMIT      Look up to this commit. Can be f.ex. HEAD~20, hash, tag or branch name."
	echo -e "\tPATH_TO_FIND      Pattern used to match changed files. Leave empty for any."
	exit 1
fi

echo "Looking for '$PATH_TO_FIND' up to $UP_TO_COMMIT from here"
echo

hash=
lasthash=$(git rev-parse $UP_TO_COMMIT) || exit 1
steps=1

while [ "$hash"z != "$lasthash"z ]; do
	hash=$(git rev-parse HEAD~$steps) || exit 1
	changedFiles=$(git --no-pager diff --name-only HEAD~$steps HEAD~$((steps-1)) | grep $PATH_TO_FIND | wc -l)
	if [ $changedFiles -ne 0 ]; then 
		echo "HEAD~$((steps-1))  $hash  Use 'git diff $(git rev-parse --short HEAD~$steps) $(git rev-parse --short HEAD~$((steps-1)))' to see changes"
		git --no-pager diff --name-only HEAD~$steps HEAD~$((steps-1)) | grep $PATH_TO_FIND
		echo
	fi
	steps=$((steps+1))
done
