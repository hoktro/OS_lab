#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void recursive( int *p ) {
    
    close(p[1]);

    int prime;
    if( read( p[0], &prime, 4 ) <= 0 ) exit(0);
    fprintf( 1, "prime %d\n", prime );

    int _p[2];
    pipe(_p);

    if( !fork() ) recursive(_p);
    int num;
    while( read( p[0], &num, 4 ) == 4 ) if( num % prime ) write( _p[1], &num, 4 );
    
    close(p[0]);

    close(_p[0]);
    close(_p[1]);

    wait(0);
    exit(0);
}

int main( int argc, char *argv[] ) {
    
    if( argc != 1 ) {
        fprintf( 2, "Usage: primes" );
        exit(1);
    }

    int p[2];
    pipe(p);

    for( int i = 2; i <= 35; ++i ) write( p[1], &i, sizeof(i) );

    if( !fork() ) recursive(p);

    close(p[0]);
    close(p[1]);
    wait(0);
    exit(0);
}