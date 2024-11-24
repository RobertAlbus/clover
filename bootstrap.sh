#!/bin/bash

# --------------------------------
COPYRIGHT_HOLDER_NAME="Rob W. Albus"
COPYRIGHT_YEAR="2024"
PROJECT_NAME="my_project"
# -------------------------------- 

SEARCH_STRING="cpp_boilerplate"
REPLACE_STRING="$PROJECT_NAME"
EXCLUDES=( ".git" "build" ".vscode" ".cache" "bootstrap.sh")

export SEARCH_STRING REPLACE_STRING

PERL_SUBSTITUTION='
    my $search = $ENV{SEARCH_STRING};
    my $replace = $ENV{REPLACE_STRING};
    s{
        ($search)
    }{
        ($1 eq uc($1)) ? uc($replace) :
        ($1 eq ucfirst(lc($1))) ? ucfirst($replace) :
        lc($replace)
    }gexi
'

build_prune_conditions() {
    local PRUNE_CONDITIONS=()
    for EXCLUDE in "${EXCLUDES[@]}"; do
        PRUNE_CONDITIONS+=( -name "$EXCLUDE" -o )
    done
    unset 'PRUNE_CONDITIONS[${#PRUNE_CONDITIONS[@]}-1]'
    echo "${PRUNE_CONDITIONS[@]}"
}

filter_by_depth() {
    input_files="$1"
    target_depth="$2"
    
    echo "$input_files" | while read -r file; do
        depth=$(echo "$file" | awk -F"/" '{print NF-1}')
        if [ "$depth" -eq "$target_depth" ]; then
            echo "$file"
        fi
    done
}

get_files_by_depth() {
    files=$(find . -type d \( $(build_prune_conditions) \) -prune -o -type f -print)
    filter_by_depth "$files" "$1"
}

get_folders_by_depth() {
    files=$(find . -type d \( $(build_prune_conditions) \) -prune -o -type d -print)
    filter_by_depth "$files" "$1"
}

MAX_DEPTH=$(find . \( $(build_prune_conditions) \) -prune -o -printf '%d\n' | sort -n | tail -1)
for (( D=1; D<=MAX_DEPTH; D++ )); do
    get_folders_by_depth "$D" | \
    while read -r dir; do
        newdir=$(echo "$dir" | perl -pe "$PERL_SUBSTITUTION")
        if [ "$dir" != "$newdir" ]; then
            mv "$dir" "$newdir"
        fi
    done

    get_files_by_depth "$D" | \
    while read -r file; do
        newfile=$(echo "$file" | perl -pe "$PERL_SUBSTITUTION")
        if [ "$file" != "$newfile" ]; then
            mv "$file" "$newfile"
        fi
    done

    get_files_by_depth "$D" | \
    while read -r file; do
        if file "$file" | grep -qE 'text|source|script'; then
            perl -i -pe "$PERL_SUBSTITUTION" "$file"
        fi
    done
done

sed -i "s/COPYRIGHT_HOLDER_NAME/$COPYRIGHT_HOLDER_NAME/g" LICENSE
sed -i "s/COPYRIGHT_YEAR/$COPYRIGHT_YEAR/g" LICENSE

rm "$0"

git reset --soft $(git rev-list --max-parents=0 HEAD)
git add -A
git commit --amend --no-edit
