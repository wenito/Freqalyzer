source defines

echo -e "\033[4;34mBuilding project\n\033[0m"
gcc -g -L../lib -I../inc ../src/freqalyzerv1.c -o $OUTPUT_NAME -lfmod -lSDL -lSDL_image
echo -e "\033[4;34mDone\n\033[0m"

