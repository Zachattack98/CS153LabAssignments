#include "types.h"
#include "stat.h"
#include "user.h"

// Prevent this function from being optimized, which might give it closed form
#pragma GCC push_options
#pragma GCC optimize ("O0")
static int
recurse(int n)
{
  if(n == 0)
    return 0;
  return n + recurse(n - 1);
}
#pragma GCC pop_options

//modification of the memory layout, accounting the changes for adding a page fault
int
main(int argc, char *argv[])
{
  int k = argc;
  int n, m;

  // output the pointer type data of the # of arguments (argc)
  // check whether or not the modifications to tha address space are correct
  printf(1, "Pointer type: %p\n", &k);

  if(argc != 2){
    if(argc == 1) {
      printf(1, "Error!! No applicable levels inputted!\n");
    }
    else {
      printf(1, "Usage: %s levels\n", argv[0]);
    }
    exit();
  }

  n = atoi(argv[1]);
  printf(1, "Lab 3: Recursing %d levels\n\n", n);
  m = recurse(n);
  printf(1, "Lab 3: Yielded a value of %d\n", m);
  exit();
}