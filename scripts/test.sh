rm -fr ./build/clover_test;
sh scripts/buildNoRun.sh;
cd build && ctest & cd ..;