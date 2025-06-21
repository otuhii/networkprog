#!/bin/bash

# Check if directory argument is provided
if [ -z "$1" ]; then
    echo "Error: Missing directory argument."
    echo "Usage: $0 /path/to/zipfiles"
    exit 1
fi

ZIPDIR="$1"

# Check if the directory exists and is accessible
if [ ! -d "$ZIPDIR" ]; then
    echo "Error: Directory '$ZIPDIR' does not exist or is not accessible."
    exit 1
fi

# Create the 'music' folder inside ZIPDIR to organize extracted files
MUSICDIR="$ZIPDIR/music"
mkdir -p "$MUSICDIR" || { echo "Error: Cannot create or access '$MUSICDIR'."; exit 1; }

cd "$ZIPDIR" || { echo "Error: Cannot change directory to '$ZIPDIR'."; exit 1; }

# Loop over all zip files in the directory  .
for zip_file in *.zip; do  .
    # Skip if no zip files found///
    [ -e "$zip_file" ] || { echo "No zip files found in $ZIPDIR."; break; }

    # Extract folder name by removing .zip extension
    folder="${zip_file%.zip}"

    echo "Processing '$zip_file' into folder '$MUSICDIR/$folder'..."

    # Create the folder inside music directory
    mkdir -p "$MUSICDIR/$folder"

    # Extract zip file into the folder inside music
    7z e "$zip_file" -o"$MUSICDIR/$folder" || { echo "Failed to unzip $zip_file"; continue; }

    # Optional: remove the original zip file after successful extraction
    # rm "$zip_file"

    # Count subdirectories inside the extracted folder
    subdirs_count=$(find "$MUSICDIR/$folder" -mindepth 1 -maxdepth 1 -type d | wc -l)

    # If exactly one subdirectory, move its contents up and remove the subdirectory
    if [ "$subdirs_count" -eq 1 ]; then
        subfolder=$(find "$MUSICDIR/$folder" -mindepth 1 -maxdepth 1 -type d)
        echo "Single subfolder '$subfolder' found inside '$MUSICDIR/$folder', flattening structure..."
        mv "$subfolder"/* "$MUSICDIR/$folder"/
        rmdir "$subfolder"
    fi

    echo "Finished processing '$zip_file'."
done
