rm -fr ./build/clover_test;
touch build/.gitkeep
cmake -B build -G "Unix Makefiles" -DCLOVER_BUILD_TESTS=ON;
make -C build;

# cd build && ctest --rerun-failed --output-on-failure && cd ..;
cd build && ctest && cd ..;