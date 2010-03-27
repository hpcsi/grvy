#include<grvy.h>
#include<stdio.h>

int main()
{
  char *mask = "mymodel";	/* mask to include in message output */
  int std  = 0;			/* standard message (always include) */
  int info = 1;			/* info message                      */
  int dbg  = 2;			/* debug message                     */

  printf("\nMessages with default setting\n");

  GRVY_message(0,mask,"This is a message from mymodel");
  GRVY_message(1,mask,"This is an informational message from mymodel");
  GRVY_message(2,mask,"This is a debug message from mymodel");

  /* above will only output one message by default; now increasing to 
   * debug mode to get all messages (this can also be accomplished using
   * the GRVY_MESSAGE_MODE environment variable */

  printf("\nMessages with debug output setting\n");

  GRVY_message_mode(dbg);

  GRVY_message(0,mask,"This is a message from mymodel");
  GRVY_message(1,mask,"This is an informational message from mymodel");
  GRVY_message(2,mask,"This is a debug message from mymodel");

  return 0;
}

