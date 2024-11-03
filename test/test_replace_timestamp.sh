#!/bin/bash

LOG_DIR="/home/oskar/.trashsys/log"

if [ -z "$1" ]; then
  echo "Usage: $0 <replacement_timestamp>"
  exit 1
fi

REPLACEMENT="$1"

if [ ! -d "$LOG_DIR" ]; then
  echo "Directory $LOG_DIR does not exist."
  exit 1
fi

for file in "$LOG_DIR"/*; do
  if [ -f "$file" ]; then
    echo "Replacing timestamp in: $file"
    awk -v replacement="$REPLACEMENT" '{
      # Replace the fifth field (timestamp) with the replacement string
      $5 = replacement
      print $0
    }' "$file" > "$file.tmp" && mv "$file.tmp" "$file"
  fi
done

echo "Done."
