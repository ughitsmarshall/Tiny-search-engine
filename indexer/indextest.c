/* indextest.c - driverfile for indextest functionality
 * attempts to duplicate one index to another to prove full index
 * module functionality
 *
 * Marshall Carey-Matthews, 30 October 2024
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "hashtable.h"
#include "counters.h"

/* Call function to read from index file, then write
 * to new index and save to a copy file
 */
int main(const int argc, char* argv[]) {
  //make index from filepath
  index_t* theIndex = index_read(argv[1]);

  //save the index to a new file
  index_save(theIndex, argv[2]);

  index_delete(theIndex);
}

