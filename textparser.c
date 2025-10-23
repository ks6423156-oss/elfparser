#include <stdio.h>
#include <libelf.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void main() {
    Elf *elf = NULL;
    Elf_Scn *scn = NULL;
    Elf_Data *data = NULL;
    Elf64_Shdr *shdr = NULL;
    char *name = NULL;
    int fd_in = 0, fd_out = 0;
    size_t shstrndx = 0, n = 0;

elf_version(EV_CURRENT);

fd_in = open("main", O_RDONLY, 0);
    
elf = elf_begin(fd_in, ELF_C_READ, NULL);
//calling 64bit elf section header 
Elf64_Ehdr *ehdr = elf64_getehdr(elf);
if (ehdr->e_shstrndx == SHN_XINDEX) {
    Elf_Scn *scn0 = elf_getscn(elf, 0);
    GElf_Shdr shdr0;
    gelf_getshdr(scn0, &shdr0);
    shstrndx = shdr0.sh_link;
} else {
    shstrndx = ehdr->e_shstrndx;
}
while ((scn = elf_nextscn(elf, scn)) != NULL) {
        // Retrieve the section header
        shdr = elf64_getshdr(scn);

        // Get the name of the section
        name = elf_strptr(elf, shstrndx, shdr->sh_name);

        // Check if the section is .text
        if (strcmp(name, ".text") == 0) {
            // Open the binary output file
            fd_out = open("out_libelf", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

            // Read the section and write it to the file
            while ((data = elf_getdata(scn, data)) != NULL) {
                write(fd_out, data->d_buf, data->d_size);
            }

            // Close the file descriptor of output file
            close(fd_out);
        }
    }
    // Release the ELF descriptor
    elf_end(elf);
    // Close main
    close(fd_in);   
}   
