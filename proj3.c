#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int debugMode = 0;
#define TRUE 1
#define FALSE -1
 
typedef struct mazeStruct
{
 char **arr;  /* pointer to a char pointer array */
 int xsize, ysize;  /* keeps track of the size of the board */
 int xstart, ystart;  /* keeps track of the start positions*/
 int xend, yend;  /* keeps track of the end positions*/
} maze;

/* stack struct */
typedef struct stack
{
  int xpos, ypos;    /* variables to keep track of current position in the stack */
  struct stack *next;   /* pointer to next position in the stack */
}stack;

void initializeStack( maze maze )
{
  /* set the sizes of the x & y positions */
  maze.xsize = 0;
  maze.ysize = 0;

  /* set the start positions */
  maze.xstart = 0;
  maze.ystart = 0;
  
}

void clear( maze m1 )
{
  int i;
  for ( i = 0; i < m1.xsize; i++ )
  {
    free( m1.arr[i] );
  }
  free(m1.arr);
}

/* check if the stack is mepty */
int empty( stack *head )
{
  /* if empty then return true */
  if ( head == NULL )
  {
    return TRUE;
  }
  /* else false */
  else
  {
    return FALSE;
  }
}

/* push the values into the stack */
void push( stack **head, int xpos, int ypos, int debugMode )
{
  /* create a temp node */
  stack *tmp = ( stack* ) malloc (sizeof(stack));

  /* assign the s & y positions */
  tmp->xpos = xpos;
  tmp->ypos = ypos;

  /* set the pointers */
  tmp->next = *head;
  *head = tmp;

  /* check for debugMode */
  if ( debugMode == TRUE )
  {
    printf("Values pushed into stack are:  X:%d  Y:%d\n", (*head)->xpos, (*head)->ypos );
  }
}

/* pop the values from the stack */
void pop( stack **head, int debugMode )
{
  stack *tmp = *head;
  /* check if i debugMode */
  if ( debugMode == TRUE )
  {
    printf("Values being popped from stack are: %d, %d\n", tmp->xpos, tmp->ypos);
  }

  /* check if the list is empty */
  if ( empty( *head ) != TRUE )
  {
    /* pop from the stack */
    *head = (*head)->next;
    free(tmp);
    return;
  }
  else
  {
    /* the stack is empty */
    printf("***ERROR: CAN'T POP FROM EMPTY STACK***\n");
  }
}

/* returns the top values of the indices of the stack */
int top( stack *head, int *xpos, int *ypos )
{
  /* check if empty */
  //* if = empty *//
  if ( empty( head ) == TRUE )
  {
    *xpos = head->xpos;
    *ypos = head->ypos;
  }
  ///* else = not empty *///
  else
  {
    /* traverse through stack */
    while ( head != NULL )
    {
      /* increment node head */
      head = head->next;
    }
    *xpos = head->xpos;
    *ypos = head->ypos;
  }
}

void printMaze( maze m1 )
{
  int i, j;
  for (i = 0; i < m1.xsize+2; i++)
    {
     for (j = 0; j < m1.ysize+2; j++)
       printf (" %c ", m1.arr[i][j]);
     printf("\n");
    }
}

void displayStack(stack *head)
{
	stack *temp = head;
	/* check is the stack is empty */
	if ( empty( head ) == TRUE)
  {
		printf("\n*** EMPTY STACK ***\n");
		return;
	}
	
	while ( temp->next != NULL )
  {
		printf("(%d,%d)<-", temp->xpos, temp->ypos);
		temp = temp->next;
	}
	printf("(%d,%d)\n", temp->xpos, temp->ypos);
}

int main (int argc, char **argv)
{
  maze m1;

  /* initialize mazes with values all set to 0 */
  initializeStack( m1 );
 
  int xpos, ypos;
  /* variables to keep track of positions through board */
  int xCurr, yCurr;
  int i,j;

  /* set the head of the stack to NULL */
  stack *head = NULL;
 
  FILE *src;

  /* allocate space */
  m1.arr = (char**) malloc ( sizeof(char*) * (m1.xsize + 2 ));
  //visited.arr = (char**) malloc ( sizeof(char*) * (visited.xsize + 2 ));
  //m1.arr = (char*) malloc (sizeof(char*) * ( m1.xsize + 2 ));
  /* allocate space for the array of the board */
  for ( i = 0; i < m1.xsize + 2; i++ )
  {
    m1.arr[i] = (char*) malloc (sizeof(char) * ( m1.ysize + 2));
    //m1.arr[i] = (char) malloc (sizeof(char) * ( m1.ysize + 2));
  }
 
  /* verify the proper number of command line arguments were given */
  if(argc < 2) {
     printf("Usage: %s <input file name>\n", argv[0]);
     exit(-1);
  }

  /* verify debugMode */
  for ( i = 0; i < argc; i++ )
  {
    if ( strcmp(argv[i], "-d" ) == 0 )
    {
      printf("***DEBUG MODE IS ON***\n");
      debugMode = TRUE;
    }
  }

  /* Try to open the input file. */
  if ( ( src = fopen( argv[1], "r" )) == NULL )
  {
    printf ( "Can't open input file: %s", argv[1] );
    exit(-1);
  }
 
  /* read in the size, starting and ending positions in the maze */
  fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
  fscanf (src, "%d %d", &m1.xstart, &m1.ystart);
  fscanf (src, "%d %d", &m1.xend, &m1.yend);
  /* print them out to verify the input */
  printf ("size: %d, %d\n", m1.xsize, m1.ysize);
  printf ("start: %d, %d\n", m1.xstart, m1.ystart);
  printf ("end: %d, %d\n", m1.xend, m1.yend);
 
 /* make sure sizes are correct */
 if ( m1.xsize < 1 || m1.ysize < 1 )
 {
   printf("\n*** STANDARD ERROR: Invalid sizes, must be greater than 0 ***\n");
   exit(1);
 }
 /* make sure that start positions are not greater than the sizes */
 if ( m1.xstart > m1.xsize || m1.ystart > m1.ysize )
 {
   if ( m1.xstart > m1.xsize )
   {
     printf("\n*** STANDARD ERROR: row %d is outside range from 1 to %d ***\n", m1.xstart, m1.xsize);
   }
   else if ( m1.ystart > m1.ysize )
   {
     printf("\n*** STANDARD ERROR: column %d is outside range from 1 to %d ***\n", m1.ystart, m1.ysize);
   }
   exit(1);
 }
  /* initialize the maze to empty */
  for (i = 0; i < m1.xsize+2; i++)
     for (j = 0; j < m1.ysize+2; j++)
       m1.arr[i][j] = 'U';
 
  /* mark the borders of the maze with *'s */
  for (i=0; i < m1.xsize+2; i++)
    {
     m1.arr[i][0] = 'V';
     m1.arr[i][m1.ysize+1] = 'V';
    }
  for (i=0; i < m1.ysize+2; i++)
    {
     m1.arr[0][i] = 'V';
     m1.arr[m1.xsize+1][i] = 'V';
    }
 
  /* mark the starting and ending positions in the maze */
  m1.arr[m1.xstart][m1.ystart] = 's';
  m1.arr[m1.xend][m1.yend] = 'e';
           
  /* mark the blocked positions in the maze with *'s */
  while (fscanf (src, "%d %d", &xpos, &ypos) != EOF)
    {
     m1.arr[xpos][ypos] = '*';
     if ( m1.arr[m1.xstart][m1.ystart] != 's' || m1.arr[m1.xend][m1.yend] != 'e')
     {
       printf("\n*** STANDARD ERROR: CANNOT OVERWRITE START OR END POSITIONS ***\n");
       exit(1);
     }
    }
 
  /* print out the initial maze */
  printMaze( m1 );
  
  /* set the current positions equal to that start positions */
  xCurr = m1.xstart;
  yCurr = m1.ystart;
  
  /* push the start positions to the stack */
  push( &head, xCurr, yCurr, debugMode );

  while ( xCurr <= m1.xsize && yCurr <= m1.ysize )
  {
    /* check that we found the end */
    if ( (head->xpos == m1.xend) && (head->ypos == m1.yend)  )
    {
      break;
    }
    /* movement to the right */
    //* y position incremented *//
    if ( m1.arr[xCurr][yCurr+1] == 'U')
    {
      yCurr++;
      push( &head, xCurr, yCurr, debugMode );
      m1.arr[xCurr][yCurr] = 'V';
      continue;
    }
    /* movement up */
    //* x position decremented *//
    else if ( m1.arr[xCurr-1][yCurr] == 'U' )
    {
      xCurr--;
      push( &head, xCurr, yCurr, debugMode );
      m1.arr[xCurr][yCurr] = 'V';
      continue;
    }
    /* movement to the left */
    //* y position decremented *//
    else if ( m1.arr[xCurr][yCurr-1] == 'U' )
    {
      yCurr--;
      push( &head, xCurr, yCurr, debugMode );
      m1.arr[xCurr][yCurr] = 'V';
      continue;
    }
    /* movement down */
    //* x position incremented *//
    else if ( m1.arr[xCurr+1][yCurr] == 'U' )
    {
      xCurr++;
      push( &head, xCurr, yCurr, debugMode );
      m1.arr[xCurr][yCurr] = 'V';
      continue;
    }
    /* No positions available */
    else
    {
      pop( &head, debugMode );
      /* check for no solution */
      if ( empty( head ) == TRUE )
      {
        //continue;
        break;
      }
      xCurr = head->xpos;
			yCurr = head->ypos;
    }
    
    if ( xCurr == m1.xend && yCurr == m1.yend )
      {
        break;
      }
    //break;
    //continue;
  }

  /* maze no solution */
  if ( empty( head ) != TRUE )
  {
    printf("\nMaze has no possible solution\n");
    printf("xEnd is: %d     xCurr is %d\n", m1.xend, xCurr );
    printf("yEnd is: %d     yCurr is %d\n", m1.yend, yCurr );
    exit(1);
  }
  //* maze solution *//
  else
  {
    printf("\n*** SOLUTION ***\n");
    /* print maze */
    printMaze( m1 );
    /* display stack */
    //displayStack( head );
    /* exit program */
    exit(1);
  }
  clear( m1 );
}