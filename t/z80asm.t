#-----------------------------------------------------------------------------
# Unit tests
# Copyright (c) Paulo Custodio, 2015-2016
# License: http://www.perlfoundation.org/artistic_license_2_0
#-----------------------------------------------------------------------------

use Modern::Perl;
use Test::More;
use Path::Tiny;
use z80asm_config;

# Usage
call_z80asm("", <<OUT, "", 0);
$PROGNAME version $VERSION $COPYRIGHT

Usage: $PROGNAME [options] input_files
OUT

# non-existent file
call_z80asm("nonexistent", <<OUT, <<ERR, 1);
$PROGNAME version $VERSION $COPYRIGHT

OUT
Fatal error: nonexistent: $NOTFOUND
ERR

# file with syntax error
path("test.asm")->spew(<<END);
  xnop
END
call_z80asm("test.asm", <<OUT, <<ERR, 1);
$PROGNAME version $VERSION $COPYRIGHT

OUT
test.asm:1:3: error: unexpected 'x'
      xnop
      ^
ERR

done_testing();
