#include <stdio.h>
#include <stdlib.h>

int main() {
    // Get the PATH environment variable
    char* path = getenv("PATH");
    
    // VULNERABLE: Exposing system PATH information (CWE-497)
    fprintf(stderr, "Error: cannot find executable on path: %s\n", path);
    
    // Also show current directory for more exposure
    char* cwd = getenv("CD"); // Windows current directory
    if (cwd == NULL) {
        cwd = "C:\\unknown";
    }
    fprintf(stderr, "Current directory: %s\n", cwd);
    
    // Show system info (additional exposure)
    char* username = getenv("USERNAME");
    char* computername = getenv("COMPUTERNAME");
    fprintf(stderr, "User: %s@%s\n", username, computername);
    
    return 0;
}