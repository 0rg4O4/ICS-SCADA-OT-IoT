#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=============================================================================
// Cylera PE-DICOM Polyglot - EXTRACT AND RUN
//=============================================================================

typedef struct {
    unsigned char *data;
    size_t size;
    char *filename;
} DICOMFile;

int read_dicom_file(const char *filename, DICOMFile *dcm) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    
    fseek(f, 0, SEEK_END);
    dcm->size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    dcm->data = (unsigned char *)malloc(dcm->size);
    if (!dcm->data) {
        fclose(f);
        return 0;
    }
    
    fread(dcm->data, 1, dcm->size, f);
    fclose(f);
    
    dcm->filename = _strdup(filename);
    return 1;
}

void extract_pe_from_polyglot(unsigned char *data, size_t size) {
    printf("\n=== Extracting PE from polyglot ===\n");
    
    // Check for MZ header
    if (size < 2 || data[0] != 'M' || data[1] != 'Z') {
        printf("[-] No MZ header found\n");
        return;
    }
    
    printf("[+] Found MZ header at offset 0\n");
    
    // Get PE header offset
    int pe_offset = *(int *)(data + 0x3C);
    printf("[+] PE header offset: 0x%X\n", pe_offset);
    
    if (pe_offset + 4 >= size) {
        printf("[-] PE header offset out of bounds\n");
        return;
    }
    
    // Verify PE signature
    if (data[pe_offset] != 'P' || data[pe_offset+1] != 'E' ||
        data[pe_offset+2] != 0 || data[pe_offset+3] != 0) {
        printf("[-] Invalid PE signature\n");
        return;
    }
    
    printf("[+] Valid PE signature found\n");
    
    // Get the size of the PE image
    int size_of_image = *(int *)(data + pe_offset + 0x50);
    printf("[+] Size of image: 0x%X bytes\n", size_of_image);
    
    // Extract the PE to a file
    char *exe_filename = "extracted_polyglot.exe";
    FILE *f = fopen(exe_filename, "wb");
    if (!f) {
        printf("[-] Failed to create output file\n");
        return;
    }
    
    // Write the entire file (it's already a valid PE)
    fwrite(data, 1, size, f);
    fclose(f);
    
    printf("[+] Saved extracted PE to %s (%zu bytes)\n", exe_filename, size);
    
    // Check for Cylera strings in the file
    const char *patterns[] = {"Cylera", "PE-DICOM", "d00rt", "cylera.com", "Markel Picado", NULL};
    
    printf("\n=== Searching for Cylera strings in PE ===\n");
    for (int p = 0; patterns[p]; p++) {
        for (size_t i = 0; i < size - strlen(patterns[p]); i++) {
            if (memcmp(data + i, patterns[p], strlen(patterns[p])) == 0) {
                printf("[+] Found '%s' at offset 0x%zX\n", patterns[p], i);
                
                // Print context
                printf("    Context: ");
                for (size_t j = (i > 20 ? i - 20 : 0); 
                     j < i + 50 && j < size; j++) {
                    unsigned char c = data[j];
                    if (c >= 0x20 && c <= 0x7E)
                        putchar(c);
                    else if (c == '\n' || c == '\r')
                        putchar(' ');
                    else
                        printf("\\x%02X", c);
                }
                printf("\n\n");
            }
        }
    }
    
    printf("\n=== EXECUTION OPTIONS ===\n");
    printf("[1] Run the extracted PE now (will show message box)\n");
    printf("[2] Just save the file (for manual analysis)\n");
    printf("Choice: ");
    
    int choice = getchar();
    if (choice == '1') {
        printf("\n[*] Launching extracted PE...\n");
        printf("[*] Looking for Cylera message box...\n\n");
        
        // Execute the file
        ShellExecuteA(NULL, "open", exe_filename, NULL, NULL, SW_SHOW);
        
        printf("[+] Executed %s\n", exe_filename);
        printf("[!] Check for a message box titled 'Cylera, Inc'\n");
    } else {
        printf("\n[+] File saved as %s\n", exe_filename);
        printf("[!] Run it manually with: %s\n", exe_filename);
    }
}

void check_pixel_data(unsigned char *data, size_t size) {
    printf("\n=== Checking pixel data ===\n");
    
    // Check if it's all 0x10 (image data)
    int non_10_count = 0;
    for (size_t i = 0; i < (size > 1000 ? 1000 : size); i++) {
        if (data[i] != 0x10) non_10_count++;
    }
    
    if (non_10_count < 10) {
        printf("[+] Pixel data appears to be valid image data (mostly 0x10)\n");
        printf("[!] This is NOT the compressed payload - it's the actual DICOM image\n");
    }
}

int main(int argc, char *argv[]) {
    printf("\n=== Cylera PE-DICOM Polyglot - EXTRACTOR ===\n\n");
    
    if (argc < 2) {
        printf("Usage: %s <pedicom-cylera.dcm>\n", argv[0]);
        return 1;
    }
    
    // Load the polyglot file
    DICOMFile dcm;
    if (!read_dicom_file(argv[1], &dcm)) {
        printf("[-] Failed to read %s\n", argv[1]);
        return 1;
    }
    
    printf("[+] Loaded %s (%zu bytes)\n", argv[1], dcm.size);
    
    // Check if it has MZ header
    if (dcm.size > 2 && dcm.data[0] == 'M' && dcm.data[1] == 'Z') {
        printf("[+] File has MZ header - polyglot confirmed\n");
        
        // Check for DICM marker at offset 128
        if (dcm.size > 132 && 
            dcm.data[128] == 'D' && dcm.data[129] == 'I' &&
            dcm.data[130] == 'C' && dcm.data[131] == 'M') {
            printf("[+] File has DICM marker - valid DICOM\n");
        }
        
        // Extract and run the PE
        extract_pe_from_polyglot(dcm.data, dcm.size);
        
    } else {
        printf("[-] No MZ header found - not a polyglot PE\n");
        
        // Check pixel data instead
        for (size_t i = 0; i < dcm.size - 8; i++) {
            if (dcm.data[i] == 0xE0 && dcm.data[i+1] == 0x7F &&
                dcm.data[i+2] == 0x10 && dcm.data[i+3] == 0x00) {
                
                unsigned int len = *(unsigned int *)(dcm.data + i + 4);
                if (i + 8 + len <= dcm.size) {
                    printf("[+] Found pixel data at offset 0x%zX\n", i + 8);
                    check_pixel_data(dcm.data + i + 8, len);
                }
            }
        }
    }
    
    // Cleanup
    free(dcm.data);
    free(dcm.filename);
    
    printf("\n=== Done ===\n");
    return 0;
}