# /brief Read file and print content to console
# /param[in] INPUT_FILE Path to file
file(READ "${INPUT_FILE}" FILE_OUTPUT)
message("Print content of file: ${INPUT_FILE}")
message("${FILE_OUTPUT}")
