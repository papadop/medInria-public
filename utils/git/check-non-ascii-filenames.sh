#!/bin/sh

against=$1

# If you want to allow non-ascii filenames set this variable to true.

allownonascii=$(git config hooks.allownonascii)

# Cross platform projects tend to avoid non-ascii filenames; prevent
# them from being added to the repository. We exploit the fact that the
# printable range starts at the space character and ends with tilde.

if [ "$allownonascii" != "true" ] &&
	# Note that the use of brackets around a tr range is ok here, (it's
	# even required, for portability to Solaris 10's /usr/bin/tr), since
	# the square bracket bytes happen to fall in the designated range.
	test "$(git diff --cached --name-only --diff-filter=A -z $against |
	  LC_ALL=C tr -d '[ -~]\0')"
then
	echo "Error: Attempt to add a non-ascii file name."
	echo
	echo "This can cause problems if you want to work"
	echo "with people on other platforms."
	echo
	echo "To be portable it is advisable to rename the file ..."
	echo
	echo "If you know what you are doing you can disable this"
	echo "check using:"
	echo
	echo "  git config hooks.allownonascii true"
	echo
	exit 1
fi
