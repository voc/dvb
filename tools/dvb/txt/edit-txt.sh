#!/bin/bash

if [ $# -ne 1 ]
then
	echo "usage: $0 foo.plain" >&2
	exit 1
fi

xdg-open $(echo -n 'http://editor.teletext40.com/#0:'; plain2edit < $1)
