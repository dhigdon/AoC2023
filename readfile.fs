\ Read File for AoC by Dan Higdon

marker readfile

variable read-file-handle
256 constant input-buffer-len
create input-buffer input-buffer-len allot

: close-read ( -- )
   read-file-handle @ ?dup if close-file throw
   0 read-file-handle !  then ;

\ Opens a file for reading. File must exist
: open-read: ( "name" -- )
   32 parse r/o open-file abort" File not found"
   close-read  read-file-handle ! ;

\ Read the next line into input-buffer
\ Returns the string read or throws an error
: line-read ( -- addr n flag )
   input-buffer dup input-buffer-len 
   read-file-handle @ read-line throw ;

\ Call xt on each line of the currently open file
\ xt has the signature ( addr n -- )
: process ( xt -- )
   >r begin line-read while
      r@ catch abort" Error reading file"
   repeat 2drop r> drop ;

\ A test and example of how to use these words
: type-cr ( addr n -- ) type cr ;
: cat: ( name -- )
   open-read: ['] type-cr process close-read ;

