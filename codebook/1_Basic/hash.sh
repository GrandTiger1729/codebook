"Useful for verifying that there aren't mistypes."
cpp $1 -dD -P -fpreprocessed | tr -d '[:space:]' | md5sum | cut -c-6