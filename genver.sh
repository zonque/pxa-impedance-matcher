#!/bin/sh

if head=`git rev-parse --verify HEAD 2>/dev/null`; then

	printf "#ifndef _VERSION_H_ \n"
	printf "#define _VERSION_H_ \n\n"
	printf "#define VERSION \""

	# generate the version info based on the tag
	(git describe || git describe --tags || git describe --all --long) \
		2>/dev/null | tr -d '\n'

	# Are there uncommitted changes?
	git update-index --refresh --unmerged > /dev/null
	if git diff-index --name-only HEAD | grep -v "^scripts/package" \
	    | read dummy; then
		printf '%s' -dirty
	fi

	printf "\"\n"
	printf "\n#endif\n"
fi
