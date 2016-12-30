#-----------------------------------------------------------------------------
# Unit tests
# Copyright (c) Paulo Custodio, 2015-2016
# License: http://www.perlfoundation.org/artistic_license_2_0
#-----------------------------------------------------------------------------

use Modern::Perl;
use Test::More;
use Path::Tiny;
use z80asm_config;

# Version

# Usage
call_z80asm("", <<OUT, "", 0);
$NAME $VERSION $COPYRIGHT

Usage: $PROG [options] {FILES|\@FILELIST}...
OUT

# Help
call_z80asm("-h", <<OUT, "", 0);
$NAME $VERSION $COPYRIGHT

Usage: $PROG [options] {FILES|\@FILELIST}...

Options:
  -h            Show help options
  -v            Be verbose
  -I=PATH       Add directory to include search path
  -L=PATH       Add directory to library search path
  -l            Create listing file.lis
  --IXIY        Swap IX and IY registers
OUT

done_testing();
exit 0;



# non-existent file
call_z80asm("nonexistent", <<OUT, <<ERR, 1);
$NAME version $VERSION $COPYRIGHT

OUT
Fatal error: nonexistent: $NOTFOUND
ERR

# file with syntax error
path("test.asm")->spew(<<END);
  xnop
END
call_z80asm("test.asm", <<OUT, <<ERR, 1);
$NAME version $VERSION $COPYRIGHT

OUT
test.asm:1:3: error: unexpected 'x'
      xnop
      ^
ERR

done_testing();
