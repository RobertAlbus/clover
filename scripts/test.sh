rm -fr ./build/clover_test;
sh scripts/buildNoRun.sh;
# cd build && ctest --rerun-failed --output-on-failure && cd ..;
cd build && ctest && cd ..;