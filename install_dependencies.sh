#!/bin/sh
set -e
cd "`dirname "$0"`"

if which apt-get; then

	apt-get install libpng12-dev libjpeg-dev libfreetype6-dev libglew-dev portaudio19-dev libcurl4-openssl-dev cmake g++

elif which port; then

	port install cmake portaudio +universal

else

	echo "FATAL ERROR: Unsupported system" >&2
	exit 1

fi
