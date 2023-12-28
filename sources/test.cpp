#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

int main()
{
    // Deleting a file
    const char* filename = "dir/";
    if (remove(filename)) {
        perror("Error deleting file");
    } else {
        printf("File '%s' deleted successfully\n", filename);
    }
    return 0;
}