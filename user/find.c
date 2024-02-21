#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char* fmtname( char* path ) {

    static char buf[DIRSIZ + 1]; // Each name has fixed size
    char *pointer;

    pointer = path + strlen( path );
    while( pointer >= path && *pointer != '/' ) --pointer;
    ++pointer; // The popsition starting the name of file

    if( strlen(pointer) >= DIRSIZ ) return pointer;
    memmove( buf, pointer, strlen(pointer) );
    memset( buf + strlen(pointer), ' ', DIRSIZ - strlen(pointer) ); // Blank-padding
    return buf;
}

void diving( char *path, char *fname ) {
    
    char buf[512], *pointer;
    int fd;
    struct dirent de;
    struct stat st;

    if( ( fd = open( path, O_RDONLY ) ) < 0 ) {
        fprintf( 2, "Cannot open %s\n", path );
        return;
    }

    if( fstat( fd, &st ) < 0 ) {
        fprintf( 2, "Cannot stat %s\n", path );
        close(fd);
        return;
    }

    if( st.type != T_DIR ) {
        fprintf( 2, "Usage: find <DIRECTORY> <FILENAME>\n");
        close(fd);
        return;
    }

    if( strlen(path) + 1 + DIRSIZ + 1 > sizeof buf ) {
        printf("Path too long\n");
        close(fd);
        return;
    }

    strcpy( buf, path );
    pointer = buf + strlen(buf);
    *pointer++ = '/';

    while( read( fd, &de, sizeof(de) ) == sizeof(de) ) {
        if( !de.inum ) continue;
        memmove( pointer, de.name, DIRSIZ );
        pointer[DIRSIZ] = 0;
        if( stat( buf, &st ) < 0 ) {
            printf("Cannot stat %s\n", buf );
            continue;
        }

        char *name = fmtname(buf);
        if( st.type == T_DEVICE ) continue;
        if( st.type == T_DIR ) {
            if( name[0] == '.' ) continue;
            diving( buf, fname );
            continue;
        }
        if( !strcmp( name, fname ) ) printf( "%s\n", buf );
    }

    close(fd);
}

int main( int argc, char *argv[] ) {
    
    if( argc != 3 ) {
        fprintf( 2, "Usage: find\n" );
        exit(1);
    }

    char *path = argv[1];
    char fname[DIRSIZ];

    memmove( fname, argv[2], strlen( argv[2] ) );
    memset( fname + strlen(argv[2]), ' ', DIRSIZ - strlen(argv[2]) );

    diving( path, fname );

    exit(0);
}