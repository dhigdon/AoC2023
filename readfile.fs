\ Read File for AoC by Dan Higdon

marker readfile

variable read-file-handle

: close-read ( -- )
   read-file-handle @ ?dup if
      close-file throw
      0 read-file-handle !
   then ;

\ Opens a file for reading. File must exist
: open-read: ( "name" -- )
   32 parse r/o open-file abort" File not found"
   close-read  read-file-handle ! ;

\ Read the next line into PAD.
: line-read ( -- addr n flag )
   pad dup 80 read-file-handle @ read-line throw ;

\ A test and example of how to use these words
: cat:
   open-read:
   begin line-read while type cr repeat 2drop
   close-read ;

