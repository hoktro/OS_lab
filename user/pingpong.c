#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main( int argc, char *argv[] ) {
    
    if( argc != 1 ) {
        fprintf( 2, "Usage: pingpong\n\n" );
        exit(1);
    }

    int parent_to_child[2], child_to_parent[2];
    
    pipe(parent_to_child);
    pipe(child_to_parent);

    if( fork() == 0 ) {
        char* buffer[10] = {0};
        if( read( parent_to_child[0], buffer, 4 ) > 0 ) fprintf( 1, "%d: received %s\n", getpid(), buffer );
        write( child_to_parent[1], "pong", 4 );
        exit(0);
    }

    char* _buffer[10] = {0};
    write( parent_to_child[1], "ping", 4 );
    if( read( child_to_parent[0], _buffer, 4 ) > 0 ) fprintf( 1, "%d: received %s\n", getpid(), _buffer );
    
    exit(0);
}