#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int readConsole( char **args, int index ) {
    
    char buf[512];
    int idx = 0, nail = 0;

    while( read( 0, buf + idx, 1 ) == 1 ) {
        if( buf[idx] == '\n' ) {
            buf[idx] = 0;
            if( buf[idx - 1] ) args[index++] = buf + nail;
            return idx;
        }

        if( buf[idx] == ' ' ) {
            buf[idx] = 0;
            if( buf[idx - 1] ) args[index++] = buf + nail;
        }
        else if( !buf[idx - 1] ) nail = idx;

        ++idx;
    }

    buf[idx] = 0;
    if( buf[idx - 1] ) args[index++] = buf + nail;

    return idx;
}

int main( int argc, char *argv[] ) {
    
    if( argc < 2 ) {
        fprintf( 2, "Usage: <cmd> | xargs <cmd>\n" );
        exit(1);
    }

    char *cmd = argv[1], *args[MAXARG];

    for( int i = 0; i < argc; ++i ) args[i] = argv[i + 1];
    while( readConsole( args, argc - 1 ) ) {
        if( !fork() ) {
            exec( cmd, args );
            fprintf( 2, "Exec error\n" );
            exit(1);
        }
        wait(0);
    }

    exit(0);
}