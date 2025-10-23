include <stdio.h>
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

    // convert the file descriptor to an elf handle
    elf = elf_begin(fd_in, ELF_C_READ, NULL);

    // get the index of the string table 
    elf_getshdrstrndx(elf, &shstrndx);

    // go thru the sections
    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        // get the section header
        shdr = elf64_getshdr(scn);

        // get name of the section
        name = elf_strptr(elf, shstrndx, shdr->sh_name);

        // check if the section is .text
        if (strcmp(name, ".text") == 0) {
            // open the output file
            fd_out = open("out_libelf", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

            // read the section and write it to the file
            while ((data = elf_getdata(scn, data)) != NULL) {
                write(fd_out, data->d_buf, data->d_size);
            }

         close(fd_out);
        }
    }
    elf_end(elf);
    close(fd_in);   
}   
