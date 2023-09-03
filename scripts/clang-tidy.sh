list_cpp_h_files() {
    if [ $# -lt 1 ]; then
        echo "Usage: $0 <relative_path> [<exclude_path1> <exclude_path2> ...]"
        exit 1
    fi

    starting_folder="$1"
    shift

    exclude_paths=()
    while [ $# -gt 0 ]; do
        exclude_paths+=("-not" "-path" "$1")
        shift
    done

    find "$starting_folder" "${exclude_paths[@]}" -type f \( -name "*.cc" -o -name "*.cpp" -o -name "*.h" \)
}


clang-tidy -p build \
-extra-arg=-std=c++20 \
-extra-arg=-I/usr/include/c++/13.2.1/ \
-extra-arg=-I/usr/lib/clang/15.0.7/include \
-extra-arg=-I/usr/local/include \
-extra-arg=-I/usr/include \
$(list_cpp_h_files ./src) $(list_cpp_h_files ./includes) 
