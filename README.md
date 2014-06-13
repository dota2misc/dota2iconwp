Requires CMake to compile.

Requires Boost, OpenCV.

To compile wallpaper generator,

	cd dota2iconwall
	mkdir build
	cd build
	cmake ..
	make

To fetch icons

	cd dota2iconwall/src
	python fetch_icons.py ../icons

To make wallpaper

	cd dota2iconwall/build
	./dota2iconwall ../icons