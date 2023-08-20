#!/bin/bash

# Set the root directory
ROOT_FOLDER="../"

# List of folders to ignore
IGNORED_FOLDERS=(
    "${ROOT_FOLDER}dependencies"
)

# Header text file
HEADER_FILE="${ROOT_FOLDER}LICENSEHEADER.TXT"

# Read the contents of the HEADER_FILE
HEADER_CONTENTS=$(cat "$HEADER_FILE")

# Iterate through the files recursively
find "$ROOT_FOLDER" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.cc" \) -print |
while IFS= read -r file; do
    ignore_file=false

    # Check if the file is in any of the ignored folders
    for ignored_folder in "${IGNORED_FOLDERS[@]}"; do
        if [[ "$file" == "$ignored_folder"* ]]; then
            ignore_file=true
            break
        fi
    done

    if [ "$ignore_file" = false ]; then
        # Read the existing file contents
        existing_contents=$(cat "$file")

        # Check if the file starts with #pragma once
        if [[ "$existing_contents" == "#pragma once"* ]]; then
            # Insert the license after #pragma once and a newline
            new_contents="#pragma once

${HEADER_CONTENTS}
${existing_contents#*#pragma once}"

        else
            # If there's no #pragma once, just prepend the license
            new_contents="${HEADER_CONTENTS}

${existing_contents}"
        fi

        # Write the new contents back to the file using redirection
        echo "$new_contents" > "$file"
    fi
done
