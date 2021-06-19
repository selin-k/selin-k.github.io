/******************************************************************************
*                                  Editor                                     *
******************************************************************************/


/** Links **/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>


/** Definitions **/
#define HIDE_CURSOR "\x1b[?25l", 6
#define SHOW_CURSOR "\x1b[?25h", 6
#define HOME_CURSOR "\x1b[H", 3
#define CLEAR_SCREEN "\x1b[2J", 4
#define ERASE_IN_LINE "\x1b[K", 3
#define CTRL_KEY(k) ((k) & 0x1f)
#define ESC 0x001b
#define BACKSPACE 127
#define TABS 8
#define INIT_CURSOR "\x1b[%d;%dH", (E.cy - E.rowoff) + 1, (E.rx - E.coloff) + 1
/** Init cursor initializes the cursor within limits of the read file and window size **/

/** The control keys **/
enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  PAGE_UP,
  PAGE_DOWN
};

/** Used for the spell checker **/
enum wordType {
  NORMAL = 0,
  MISSPELLED
};

/** Holds each row of a read file **/
typedef struct rows {
  int size;       /* Size of chars */
  char *chars;    /* String of data, a single row */
  int rsize;      /* Size of rendered row */
  char *render;   /* The rendered string of data */
  unsigned char *hl;
} rows;


/** Gloal editor data **/
struct editorData {
  int cx, cy, rx;              /** The cursor coordinates **/
  int rowoff, coloff;          /** Window offset values **/
  int screenrows, screencols;  /** Window size **/
  int numrows;                 /** Number of lines read to buffer **/
  rows *row;                   /** The array of row structures **/
  char *filename;              /** Name of loaded file **/
  char statusmsg[128];         /** Status bar message **/
  time_t statusmsg_time;       /** Timer for message bar **/
  bool modified;               /** Records if buffer is modified **/
  struct termios terminal;     /** Terminal properties **/
  bool highlight;              /** Spell checker highlighter **/
  int start,end;               /** Misspelled words range **/
};

/** Global declarations **/
struct editorData E;
struct editorBuffer editorBuffer;
void modifyTerminal();
void initialize();
void args(int argc, char *argv[]);
int readKey();
void processKeypress();
void getWindowSize();
void showChangeLog(char *filename);
void appendChangeLog(char *buf, int len);
void scroll();
void die(const char *s);

void displayScreen();
void displayRows(struct editorBuffer *ab);
void displayStatusBar(struct editorBuffer *ab);
void displayMessageBar(struct editorBuffer *ab);
void setMessage(const char *fmt, ...);
void writeRow(int index, char *line, size_t len);
void renderRow(rows *row);

void loadFile(char*);
void deleteFile();
void copyFile();

void appendLine(char *filename, char *s);
char* prompter();
void highlightWords(rows *row);


/** Starting point **/
int main(int argc, char *argv[]) {
  modifyTerminal(); 
  initialize();
  args(argc, argv);

  setMessage("Ctrl-Q = QUIT | Ctrl-X = HELP | Ctrl-S = SAVE | Ctrl-F = SPELLCHECK | Ctrl-C = COPY FILE | Ctrl-D = DELETE FILE");

  /** Editor screen flow **/
  while (1) {
    displayScreen();
    processKeypress();
  }

  return 0;
}


/******************************************************************************
*                            Editor Components                                *
******************************************************************************/

/** Initialize the editor data **/
void initialize() {
  E.modified = false;
  E.highlight = false;
  E.start = -1;
  E.end = -1;
  E.cx = 0;
  E.cy = 0;
  E.rx = 0;
  E.rowoff = 0;
  E.coloff = 0;
  E.numrows = 0;
  E.row = NULL;
  E.filename = NULL;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;
  getWindowSize();
  E.screenrows -= 2; // for the bottom two status bars
}

/**
 * Given the arguments that the user passed from the 
 * command line, parses them and directs the editor.
 * Possible flags: --help, --append, --log.
 * @param argc the number of arguments passed
 * @param argv the array of passed arguments
 */ 
void args(int argc, char *argv[]) {
  char *message = malloc(80);
  if (argc == 2) {
    if (strstr(argv[1], "--")!=NULL) {
      loadFile("help.txt");
    } else {
      loadFile(argv[1]);
    }
  }

  if (argc > 2) {
    if (strcmp(argv[1], "--log")==0) {
      showChangeLog(argv[2]);
    } else if (strcmp(argv[1], "--append")==0) {
      if (argc == 4) {
        appendLine(argv[2], argv[3]);
        exit(0);
      }
      else
        loadFile("help.txt");
    } else {
      message = "Flag not found, Try '--help'\r\n";
      write(STDOUT_FILENO, message, strlen(message));
      exit(0);
    }
  } 

 }


/******************************************************************************
*                          The Editing Buffer                                 *
******************************************************************************/

/** This structure is the editing buffer, it consists of 
 * a character array and its length */
typedef struct editorBuffer {
  char *b;
  int len;
} ebuffer;

#define ABUF_INIT {NULL, 0}

/**
 * Given a string of some length, write it to the editing 
 * buffer structure.
 * @param editorBuffer is the editing buffer.
 * @param s is a string to be written to the buffer.
 * @param len is the length of the string to be written.
 */
void bufferWrite(struct editorBuffer *ab, const char *s, int len) {
  /** Reallocate space for the string to be written **/
  char *new = realloc(ab->b, ab->len + len);

  if (new == NULL) return;
  /** If reallocation was successful, copy the string to the buffer
   *  array **/
  memcpy(&new[ab->len], s, len); // copy s to the buffer.
  ab->b = new;
  ab->len += len;
}

/**
 * Given the buffer structure, free the pointer. Hence, deletes
 * buffer contents.
 * @param editorBuffer is the editing buffer.
 */
void bufferFree(struct editorBuffer *ab) {
  free(ab->b);
}



/******************************************************************************
*                               Display                                       *
******************************************************************************/

/**
 * displayScreen is called after every processed key input.
 * This refreshes the screen and writes the updated editing
 * buffer to the terminal screen.
 */ 
void displayScreen() {
  scroll();

  /** Initialize the editing buffer **/
  struct editorBuffer ab = ABUF_INIT; 
  /** Hide the cursor and initialize position **/
  bufferWrite(&ab, HIDE_CURSOR);
  bufferWrite(&ab, HOME_CURSOR);
  /** Write the modified rows to the buffer and 
   * draw the screen with status/message bars **/
  displayRows(&ab);
  displayStatusBar(&ab);
  displayMessageBar(&ab);

  /**  Print the hidden cursor then show it **/
  char buf[32];
  snprintf(buf, sizeof(buf), INIT_CURSOR);
  bufferWrite(&ab, buf, strlen(buf));
  bufferWrite(&ab, SHOW_CURSOR);
  
  /** Write the buffer to standard output and free buffer **/
  write(STDOUT_FILENO, ab.b, ab.len);
  bufferFree(&ab);
}

/**
 * Uses ioctl to get window size properties into
 * a winsize structure. Then saves the row and column
 * length of the window to 2 global variables screencols
 * and screenrows.
 */
void getWindowSize() {
    struct winsize win;
    
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &win) == -1 || win.ws_col == 0) {
        /** Error Handling **/
        die("ioctl error");
    } else{
        /** The winsize struct will be filled with info about
         *  terminal width and height **/
        E.screencols=win.ws_col; // store in editorData
        E.screenrows=win.ws_row;
    }
}

/**
 * Given the editing buffer, append a status bar to the
 * bottom of the screen when called by displayScreen. 
 * The status bar must display the number of lines read,
 * the name of the file, and the position of the user's cursor.
 * @param editorBuffer is the editing buffer.
 */ 
void displayStatusBar(struct editorBuffer *ab) {
  /** Invert colors for the status bar **/
  bufferWrite(ab, "\x1b[7m", 4);
  /** Initialize space for status bar strings **/
  char status[80];
  char rstatus[80];

  /** Display <name of file> -- <length of file> **/
  int len = snprintf(status, sizeof(status), "[ %s - READ %d LINES ]",
    E.filename ? E.filename : "[No Name]", E.numrows);

  /** Display the current line the user is on **/
  int rlen = snprintf(rstatus, sizeof(rstatus), "LINE %d \t", E.cy + 1);


  /** Append the status messages to the editing buffer **/
  
  bufferWrite(ab, status, len); // append status message to bar

  /** Append spaces until the end of the window screen to show
   *  the white background **/
  while (len < E.screencols) {
    if ( E.screencols - len  == rlen) {
      /** At the right end of the status bar display the current 
       * line the user is on **/
      bufferWrite(ab, rstatus, rlen);
      break;
    } else {
      bufferWrite(ab, " ", 1);
      len++;
    }
  }
  bufferWrite(ab, "\x1b[m", 3); // turn off color inversion
}

/**
 * Displays a message bar below the status bar. This
 * must be displayed only for 5 seconds.
 * @param editorBuffer is the editing buffer
 */ 
void displayMessageBar(struct editorBuffer *ab) {
  bufferWrite(ab, ERASE_IN_LINE);
  int msglen = strlen(E.statusmsg);
  /** If message exceeds window length, don't display the 
   * extra characters **/
  if (msglen > E.screencols) msglen = E.screencols;
  /** If message is less than 5 seconds old display it **/
  if (time(NULL) - E.statusmsg_time < 5)
    bufferWrite(ab, E.statusmsg, msglen);
}

/**
 * Sets a 5 second message on the message bar.
 * Can be passed multiple parameters of type char,
 * is a variadic function.
 * @param fmt* the string message to be displayed.
 */ 
void setMessage(const char *fmt, ...) {
    va_list ap;
    va_start(ap,fmt);
    vsnprintf(E.statusmsg,sizeof(E.statusmsg),fmt,ap);
    va_end(ap);
    E.statusmsg_time = time(NULL);
}

/**
 * For a requested arrow key, move the cursor on the
 * screen depending on the bounds of the loaded file.
 * @param key is the specific arrow key
 */ 
void moveCursor(int key) {
    /** Check if current cursor is out of bounds **/
    rows *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

    switch (key) {
    case ARROW_LEFT:
        if (E.cx != 0) {
          /** If not trying to go left out of the editor
           *  window, move left **/
          E.cx--;
        } else if (E.cy > 0) { 
          /** Circle back to end of last row if user 
           * presses out of screen **/
          E.cy--;
          E.cx = E.row[E.cy].size;
        }
        break;
    case ARROW_RIGHT:
      if (row) {
        /** If not out of bounds of the row*/
        if (E.cx < row->size){
          /** If not out of bounds of editor window scope
           * move right **/
          E.cx++;
        } else if (E.cx == row->size) {
          /** If the cursor is at end of row, set cursor
           *  to beginning of next row **/
          E.cy++;
          E.cx = 0;
        }
        break;
      }
    case ARROW_UP: 
      /** Don't move further up than page limit 1,1, **/
      if (E.cy != 0) {
        E.cy--; // move up
      }
      break;
    case ARROW_DOWN:
      /** Don't move further down than total number 
       * of rows in file **/
      if (E.cy < E.numrows) {
        E.cy++; // move down
      }
      break;
  }

    /** Set row again, incase the cursor changes **/
    row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy]; 
    int rowlen = row ? row->size : 0; // get length of row 
    /** if current position is past length of row don't
     *  display further **/
    if (E.cx > rowlen) 
      E.cx = rowlen;
    
}

/**
 * Depending on the requested position of the cursor, 
 * simulates a scrolling movement.
 */ 
void scroll() {
    /** Set the rendered cursor to its position **/
    E.rx = 0;
    if (E.cy < E.numrows) {
        for (int i = 0; i < E.cx; i++) {
            if (E.row[E.cy].chars[i] == '\t')
            /** If there is a tab encountered, set rx
             * to next tab stop **/
                E.rx += (TABS - 1) - (E.rx % TABS);
            E.rx++; // moves cursor to next character
        }
    }

  /** Scroll up **/
  if (E.cy < E.rowoff) {
    /** If the cursor is above the visible window **/
    E.rowoff = E.cy; /** scroll up by setting current
    display index to that index **/
  }
  /** Scroll down **/
  else if (E.cy >= (E.rowoff + E.screenrows) ) {
    /** If the cursor is below the visible window **/
    E.rowoff = E.cy - E.screenrows + 1; // scroll down
  }
  /** Scroll left **/
  if (E.rx < E.coloff) {
    E.coloff = E.rx;
  }
  /** Scroll right **/
  else if (E.rx >= E.coloff + E.screencols) {
    E.coloff = E.rx - E.screencols + 1;
  }
}


/**
 * For every line being read of a file, at loadFile, writeRow
 * writes them to the rows structure. Then, makes a call to
 * render each line.
 * @param index is the current line being read from the loaded file.
 * @param line is the contents of the line being read from the file.
 * @param len is the length of line.
 */
void writeRow(int index, char *line, size_t len) {
  if (index < 0 || index > E.numrows) return;
  /** Extend size of the rows string array by 1 for every
   *  row read **/
  E.row = realloc(E.row, sizeof(rows) * (E.numrows + 1));
  memmove(&E.row[index + 1], &E.row[index], sizeof(rows) 
    * (E.numrows - index));

  /** Fill in the row structure for the current row in file **/
  E.row[index].size = len; 
  E.row[index].chars = malloc(len + 1); 
  memcpy(E.row[index].chars, line, len); 

  /** Initialize rsize and render and then make a call to
   *  render the currently read line **/
  E.row[index].rsize = 0;
  E.row[index].render = NULL;
  E.row[index].hl = NORMAL;
  renderRow(&E.row[index]);

  /** Keep a record of the number of lines read, display
   *  on status bar **/
  E.numrows++;
  E.modified = true;
}

/**
 * Given a row from the array of rows, renders the row data
 * to be displayed with consistent tabs on the terminal screen.
 * @param row is a row from the rows structure array
 */ 
void renderRow(rows *row) {
  int tabs = 0;
  /** count number of tabs in the row string **/
  for (int i = 0; i < row->size; i++)
    if (row->chars[i] == '\t') tabs++;

  free(row->render);
  /** allocate memory to rendered row with size of text + 8 
   * characters for each tab in row**/
  row->render = malloc(row->size + 1 + tabs*(TABS - 1));

  int idx = 0;
  for (int j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') {
      /** append space if there is a tab encountered, until
       *  a tab stop, which is 8 characters later **/
      row->render[idx++] = ' ';
      while (idx % TABS != 0) row->render[idx++] = ' ';
    } else {
      row->render[idx++] = row->chars[j];
    }
  }
  /** set the null character and size of the rendered row **/
  row->render[idx] = '\0';
  row->rsize = idx;

  highlightWords(row);
}

/**
 * Writes the rendered rows to the editing buffer to be displayed
 * on the terminal screen.
 * @param editorBuffer the editing buffer
 */ 
void displayRows(struct editorBuffer *ab) {
    /** Iterate through all the window screen rows **/
  for (int y = 0; y < E.screenrows; y++) {
    /** index of line to be displayed on screen, takes into account
     *  moving out of visible editor window **/
    int filerow = y + E.rowoff; 

    if (filerow < E.numrows) {
      /** Make sure current row is not past the total number of 
       * rows in file **/
      int len;
      /** If user tries to display past end of line, display nothing **/
      if ((len = E.row[filerow].rsize - E.coloff) < 0) len = 0;
      /** if user tries to display out of window scope, display last
       * possible line and don't go out of scope **/
      if (len > E.screencols) len = E.screencols;
      /** append rendered row to buffer to be displayed **/
      char *c = &E.row[filerow].render[E.coloff];
      unsigned char *hl = &E.row[filerow].hl[E.coloff];
      for (int j = 0; j < len; j++) {
         if (hl[j] == NORMAL) {
          bufferWrite(ab, "\x1b[m", 3);
          bufferWrite(ab, &c[j], 1);
        } else {
          bufferWrite(ab, "\x1b[7m", 4);
          bufferWrite(ab, &c[j], 1);
        }
      }
      bufferWrite(ab, "\x1b[m", 3);
    }

    bufferWrite(ab, ERASE_IN_LINE);
    bufferWrite(ab, "\r\n", 2); // append a return and end of line
  }
}


/******************************************************************************
*                          Editing/Control                                    *
******************************************************************************/

/**
 * Given a row in the rows array, insert the character
 * in a specific position in this row.
 * This position is indicated by the user's cursor.
 * @param c the character to be inserted.
 * @param row is the row to be inserted into.
 */ 
void insertCharToRow(rows *row, int c) {
  /** Check if the cursor is out of bounds **/
	if (E.cx < 0 || E.cx > row->size) E.cx = row->size;
  /** Reallocate memory to rows, 2 bytes for the new
   * character and the null byte **/
	row->chars = realloc(row->chars, row->size + 2);

  /** Move the data to the cursor's right by 1 **/
  memmove(&row->chars[E.cx + 1], &row->chars[E.cx], row->size - E.cx + 1);
  /** Insert the character in the cursor's position
   * and render the row **/
  row->chars[E.cx] = c;
  row->size++;
  renderRow(row);
  E.cx++; /** move cursor to the right hence next insert
   won’t overwrite **/
}

/**
 * Get a character from user key input and 
 * call insertCharToRow to modify the row structure.
 * @param c is the user input character.
 */ 
void insertChar(int c) {
  /** Set modified to true hence, the user must
   * be prompted to save the buffer before quitting **/
  E.modified = true;
	if (E.cy == E.numrows)
    /** If the user inserts a character to a
     * newline, add a newline to rows **/
		writeRow(E.numrows, "",0);
	insertCharToRow(&E.row[E.cy], c); // insert character to rows
}

/** Insert a newline depending on the position of
 * the cursor.
 */
void insertNewline() {
  if (E.cx == 0) {
    /** At the head of a row **/
    writeRow(E.cy, "", 0); // insert a new row
  } else {
    /** Anywhere else within a row **/
    rows *row = &E.row[E.cy]; // get the row structure

    /** Write the contents of the current row
     * to one row down, starting from current position
     * of cursor **/
    writeRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
    /** Update the current row and render it**/
    row = &E.row[E.cy];
    row->size = E.cx;
    row->chars[row->size] = '\0';
    renderRow(row);
  }
  /** Move the cursor to the head of the bottom row **/
  E.cy++;
  E.cx=0;
}

/**
 * Given a file, appends a passed in string.
 * This is done through the command line only.
 * @param filename is the file to be appended to.
 * @param s is the string to append.
 */ 
void appendLine(char *filename, char *s) {
  /** Open the given file **/
  FILE *fp = fopen(filename,"a");
  if (fp==NULL) die("fopen failed");

  /** Append the string and close **/
  fprintf(fp, "%s\n", s);
  fclose(fp);
  char message[80] =  "Operation successful.\r\n";
  write(STDOUT_FILENO, message, strlen(message));
}


/**
 * Given a row delete a specific character that
 * the cursor is pointing to.
 * @param row is the row to be deleted from.
 */ 
void deleteCharinRow(rows *row) {
  /** Check whether the cursor is out of bounds **/
  if (E.cx-1 < 0 || E.cx-1 >= row->size) return;

  /** Overlap the text to the right of the cursor
   * with the current position in order to overwrite
   * the byte to be deleted. **/
  memmove(&row->chars[E.cx - 1], &row->chars[E.cx],
    row->size - (E.cx - 1));

  /** Update the row structure, and render **/
  row->size--;
  renderRow(row);
  E.cx--; // move the cursor up
  E.modified = true;
}


/**
 * Moves up the preceding rows up by 1, relative
 * to the current row.
 */ 
void moveUp() {
  /** Check if cursor is out of bounds **/
  if (E.cy < 0 || E.cy >= E.numrows) return;

  /** Get current row **/
  rows *row = &E.row[E.cy];

  /** free the current row **/
  free(row->render); 
  free(row->chars);

  /** Move the rows below up by 1 **/
  memmove(&E.row[E.cy], &E.row[E.cy + 1],
    sizeof(rows) * (E.numrows - E.cy - 1)); 
  E.numrows--; 
}


/**
 * When the user deletes at the head of a row, move
 * the contents of the current row up to the previous one.
 * The move all following rows up by 1.
 * @param row is the previous row.
 * @param s is the contents of the current row.
 * @param len is the size of s.
 */ 
void deleteMoveUp(rows *row, char *s, size_t len) {
  /** Set the cursor the end of the previous line **/
  E.cx = E.row[E.cy-1].size;

  /** Reallocate memory for the previous row to include
   *  the characters being appended from current row **/
  row->chars = realloc(row->chars, row->size + len + 1);

  /** Copy the characters in the current row to the end
   *  of the previous row **/
  memcpy(&row->chars[row->size], s, len);

  /** Update the current row and render it **/
  row->size += len;
  row->chars[row->size] = '\0';
  renderRow(row);

  /** Move all the following rows up by 1 **/
  moveUp();
  E.cy--; 
}

/**
 * Deletes a character in a specific position.
 */ 
void deleteChar() {
  /** Check if cursor is out of bounds **/
  if ( (E.cy == E.numrows) || (E.cx == 0 && E.cy == 0) )
    return;

  /** Get current row **/
  rows *row = &E.row[E.cy];
  if ( E.cx > 0 ) {
    /** If the cursor is in the middle of some 
     * row just delete the character**/
    deleteCharinRow(row);
  } else {
    /** If the cursor is trying to delete at the
     * head of a row, move the row up **/
    deleteMoveUp(&E.row[E.cy - 1], row->chars, row->size);
  }
}

/**
 * Delete the contents of the current row
 * entirely when the user presses CTRL-K.
 */ 
void deleteLine() {
  /** Get current row **/
  rows *row = &E.row[E.cy];

  /** Set the cursor to the size of the row and call
   *  deleteCharInRow until the entire row is deleted **/
  E.cx = row->size;
  while(row->size>0 && E.cy<E.numrows) deleteCharinRow(row);
  /** Move up all following rows **/
  if (E.cy<E.numrows) {
    moveUp();
  }
}


/******************************************************************************
*                                 File I/O                                    *
******************************************************************************/

/**
 * Given a filename, loads the file contents to row structures.
 * The row structures are then displayed on the editing buffer.
 * @param filename, name of the file to be loaded to display
 */ 
void loadFile(char *filename) {
  /**   Set the filename globally **/
  free(E.filename);
  E.filename = strdup(filename);

  /** Open given file to read it **/
  FILE *fp = fopen(filename, "r"); 
  if (!fp) return;

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  /** Until the end of the file, read each line and append to
   *  rows **/
  while ( (linelen = getline(&line, &linecap, fp)) != -1 ) {
  /** Remove empty lines in between read lines **/
  if (linelen > 0 && (line[linelen - 1] == '\n' 
    || line[linelen - 1] == '\r'))
      linelen--;
  writeRow(E.numrows, line, linelen);
  }

  /** Clean up **/
  free(line);
  fclose(fp);
  // If the file is just loaded, it isn't modified.
  E.modified = false;
}


/**
 * Given the rows structure that displays all the rows to the
 * terminal screen, convert all to a single string.
 * @param length is the total length of all the
 * rows in the rows structure combined.
 */ 
char *rowsToString(int *length) {
  int totlen = 0;
  /** Get the length of all the rows lengths
   * combined **/
  for (int i = 0; i < E.numrows; i++)
    totlen += E.row[i].size + 1;
  *length = totlen;

  /** Allocate space to store the string in **/
  char *string = malloc(totlen);
  char *p = string; // a pointer to the string

  for (int j = 0; j < E.numrows; j++) {
    /** Iterate over the rows and copy every row to the
     * allocated string location **/
    memcpy(p, E.row[j].chars, E.row[j].size);
    /** move pointer to the start of next row **/
    p += E.row[j].size; 
    *p = '\n';
    p++;
  }
  return string;
}

/**
 * Save all the rows currently in the buffer into
 * a file.
 * If filename wasn't previously specified, prompt
 * user.
 */ 
void saveFile() {
  /** Check if the file has a name, if not prompt **/
  if (E.filename == NULL) E.filename = prompter();

  /** Convert the rows structure, and take a single string **/
  int len;
  char *buf = rowsToString(&len);

  /** Open the file, if doesn't exist create it. **/
  int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    /** If the file opens **/
    if (ftruncate(fd, len) != -1) {
      /** Truncate to the length of the rows **/
      if (write(fd, buf, len) == len) {
        /** Write to the file **/
        appendChangeLog(buf, len); // Append to the change log
        /** Clean up **/
        close(fd);
        free(buf);
        E.modified = false;
        setMessage("Saved successfully.");
        return;
      }
    }
    /** Unsucessfull truncate **/
    close(fd);
  }
  /** File didn't open **/
  free(buf);
  setMessage("Error:", strerror(errno));
}

/**
 * Prompts the user when they try to quit
 * without saving.
 */ 
void saveQuit() {
  setMessage("Do you want to save before you quit? y/n");
  displayScreen();
  int c = readKey();
  if (c == 'y' || c == 'Y') saveFile();
}

/**
 * Prompt the user to request information, displayed
 * on the message bar.
 */ 
char* prompter() {
  /** Allocate some space for the user input **/
  int bufsize = 128; // initial size of input string
  char *usrinput = malloc(bufsize);
  int size = 0;
  usrinput[0] = '\0';

  while (1) {
    /** Display the prompt on the message bar **/
    setMessage("Enter filename: %s", usrinput);
    displayScreen();

    int c = readKey(); // read the user keys
    /** Enter key/return ends the prompt **/
    if (c == '\r') {
        break;
    } else if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
      /** Allow the user to delete **/
      if (size != 0) usrinput[--size] = '\0';
    } else if (!(iscntrl(c))) {
      /** If not a control key, take it **/
      if (size == bufsize - 1){
        /** Reallocate if user exceeds initial size **/
        bufsize *= 2;
        usrinput = realloc(usrinput, bufsize);
      }
      usrinput[size++] = c;
      usrinput[size] = '\0';
    }
  }
  return usrinput;
}

/**
 * Copy the currently open file to a new file. The
 * name of the new file will be prompted. If the current
 * file isn't saved, user will be prompted to save.
 */ 
void copyFile() {
  /** First check if the current buffer is saved and has a filename **/
  if (E.modified) saveFile();

  /** Prompt for destination filename **/
  char* dest = prompter();
  char* source = strdup(E.filename);
  if (strcmp(source, dest)==0) {
    setMessage("Cannot copy a file to itself");
    return;
  } 

  /** Open source to read and dest to write to **/
  FILE *sourcef = fopen(source, "r");
  FILE *destf = fopen(dest, "w");
  if (!sourcef || !destf) {
    die("Files failed to open");
  }

  /** Copy every character one by one to dest until
  *  the end of the source file **/
  int c;
  while((c = fgetc(sourcef)) != EOF) fputc(c, destf);
  setMessage("%s was sucessfully saved to %s.", source, dest);
  /** Clean up file pointers **/
  fclose(sourcef);
  fclose(destf);
}

/**
 * Delete the current file. If buffer was never
 * saved, it can't be deleted.
 */ 
void deleteFile(){
  /** Check if buffer was saved **/
  if (E.filename) {
    remove(E.filename);
    write(STDOUT_FILENO, CLEAR_SCREEN);
    write(STDOUT_FILENO, HOME_CURSOR);
    exit(0);
  } else{
     setMessage("File doesn't exist.");
  }
 
}


/******************************************************************************
*                               Change Log                                    *
******************************************************************************/

/**
 * Given a filename, creates its log filename.
 * @param filename is the file to be logged.
 */ 
char* changeLogFilename(char *filename) {
  char *logfilename = strdup(filename);
  int index;
  char *p;
  if(strstr(logfilename, ".")!=NULL){
    p = strchr(logfilename, '.');
    index = (int) (p - logfilename);

  }
  /** Get rid of .txt extension and concatenate
   * the .log extension then return **/
  logfilename[strlen(filename) - index] = '\0'; 
  strcat(logfilename,".log"); 
  return logfilename;
}

#define SPACE 4
#define BAR_LENGTH 20
#define NEWLINE fputs("\n", fp);

/**
 * For a given buffer, creates or appends to a 
 * log file with a timestamp and the number of 
 * written lines.
 * @param buf is the file buffer to be logged
 * @param len is the length of the buffer.
 */ 
void appendChangeLog(char *buf, int len){
  /** For the current file, create a .log file **/
  char *filename = changeLogFilename(E.filename);
  char *rows = malloc(50);
  time_t t; 
  time(&t); // get the current time.

  FILE *fp = fopen(filename, "ab");

  /** Draw a bar on top of every appended
   * log **/
  for (int i=0;i<BAR_LENGTH;i++)
    fputs("#####", fp);
  NEWLINE

  /** Append a timestamp for every log save **/
  fputs("Time of save: ", fp);
  fputs(ctime(&t), fp);

  /** Append the current file to the log with, 
   * even amounts of space in between **/
  for (int i=1; i<=(SPACE*2)-1;i++) {
    if (i % SPACE == 0)
      fputs(buf, fp);
    else
      NEWLINE
  }

  /** Append the number of lines written **/
  fputs("Total lines written: ", fp);
  sprintf(rows, "%d", E.numrows);
  fputs(rows, fp);
  NEWLINE

  /** Clean up **/
  fclose(fp);
}

/**
 * Given a file, opens the corresponding log.
 * @param filename is the file that the log belongs to.
 */ 
void showChangeLog(char *filename) {
    char *logfilename = changeLogFilename(filename);
    loadFile(logfilename);
}


/******************************************************************************
*                            Spelling Checker                                 *
******************************************************************************/

#include "spell.h"

/**
 * Iterates over each row of the buffer and sends
 * each to the spell checker one by one.
 * If there are misspellings, gets their start and 
 * end index then highlights and displays them on the 
 * screen.
 */ 
void spellCheck() {
  int totalmissed=0;
  if (loadDictionary() == 1) die ("Error loading dictionary. Please check README.");

  for (int i=0; i<E.numrows; i++) {
    /** Send each row of the buffer to the spell checker
     *  function**/
    int miss = spellChecker(E.row[i].chars, E.row[i].size,0);
    totalmissed+=miss;
    for (int j=0; j<miss;j++) {
      /** For each misspelled word in the row, 
       * gets its start and end index on the row **/
      E.highlight = true;
      E.start = getMisspellings(j).start;
      E.end = getMisspellings(j).end;

      /** Highlights the misspelled words by 
       * rendering the entire row and then 
       * displays the row on the screen **/
      renderRow(&E.row[i]);
    }
  }

  if (totalmissed > 0)
    setMessage("The misspelled words are highlighted. Found %d.", totalmissed);
  else
    setMessage("There were no misspelled words found.");
  

  /** Clean up **/
  unloadDictionary();
  E.highlight = false;
  E.start = -1;
  E.end = -1;
}

/**
 * Given a row, checks the wordType and accordingly
 * highlights misspelled words.
 * @param row is the row to be highlighted
 */ 
void highlightWords(rows *row) {
  /** Allocate space to the highlighting field **/
  row->hl = realloc(row->hl, row->rsize);

  if (!E.highlight)
    /** If not in highlight mode, set to NORMAL **/
    memset(row->hl, NORMAL, row->rsize);

  for (int i = 0; i < row->rsize; i++) {
    /** Iterate over the entire row and where
     * the misspelled word starts and ends, set 
     * to MISSPELLED **/
    if (i >= E.start && i <= E.end) {
      row->hl[i] = MISSPELLED;
    }
  }
}



/******************************************************************************
*                          Key Input Processing                               *
******************************************************************************/

/**
 * Given a specific message, clears the terminal
 * screen and prints a specific error message.
 * @param s is the reason for error.
 */ 
void die(const char *s) {
  write(STDOUT_FILENO, CLEAR_SCREEN);
  write(STDOUT_FILENO, HOME_CURSOR);
  perror(s);
  write(STDOUT_FILENO, "\r\n", 3);
  exit(1);
}

/**
 * Is called at exit, sets the original terminal
 * attributes back.
 */ 
void restoreTerminal() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.terminal) == -1)
    die("tcsetattr");
}

/**
 * First gets the original terminal attributes to a termios structure.
 * Creates an instance of it, and modifies its attributes to 
 * accept non-canonical mode of input. At exit calls restoreTerminal.
 */ 
void modifyTerminal() { 
  /** Get the terminal's initial attributes **/
  if (tcgetattr(STDIN_FILENO, &E.terminal) == -1) die("tcgetattr");
  atexit(restoreTerminal); // at exit restore initial attributes

  /** Modify the terminal attributes to change the mode of input **/
  struct termios newterminal = E.terminal;
  newterminal.c_iflag &= ~(BRKINT | ICRNL | IXON); /** input flags **/
  newterminal.c_lflag &= ~(ECHO | ICANON | ISIG); /** local flags **/
  newterminal.c_cc[VMIN] = 0; /** control characters **/
  newterminal.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newterminal) == -1) die("tcsetattr");

}

/**
 * Waits on standard input until a byte is read from
 * the user. This is called from processKeypresses, hence
 * every input is processed immediately.
 * @return the read character from stdin.
 */ 
int readKey() {
  int nread;
  char c;
  /** Loop until there is a valid byte to read from stdin **/
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  /** Deal with escape sequences/control keys **/
  if (c == ESC) {
    char seq[3]; // space to read the rest of the control key

    /** Try to read 2 bytes, individually, into seq**/
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return ESC;
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return ESC;

    /** Deal with two types of sequences e.g. ^[0~ and ^[A **/
    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return ESC;
        if (seq[2] == '~') {
          switch (seq[1]) {
            /** LINUX/RXVT CONSOLE **/
            case '3': return DEL_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;

          }
        }
      } else {
        switch (seq[1]) {
            /** FREEBSD CONSOLE **/
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;

        }
      }
    } 

    return ESC;
  } else {
    /** If not a control character, return read 
     * literal byte **/
    return c; 

  }
}


/** 
 * Recieves a byte from readKey, depending on the
 * user input calls specific functions to fulfill
 * user requests
 * */
void processKeypress() {
  int c = readKey();

  switch (c) {  
    case CTRL_KEY('q'):
      if (E.modified){
        saveQuit();
      }
      write(STDOUT_FILENO, CLEAR_SCREEN);
      write(STDOUT_FILENO, HOME_CURSOR);
      exit(0);
      break;
    case CTRL_KEY('x'):
      setMessage("Ctrl-Q = QUIT | Ctrl-X = HELP | Ctrl-S = SAVE | Ctrl-F = SPELLCHECK | Ctrl-C = COPY FILE | Ctrl-D = DELETE FILE");
      break;
    case CTRL_KEY('s'):
      saveFile();
      break;
    case CTRL_KEY('k'):
      deleteLine();
      break;
    case CTRL_KEY('f'):
      spellCheck();
      break;
    case CTRL_KEY('c'):
      copyFile();
      break;
    case CTRL_KEY('d'):
      deleteFile();
      break;
    case ESC:
      break;
    case '\r':
    case '\n':
      insertNewline();
      break;
    case BACKSPACE:
    case DEL_KEY:
    case CTRL_KEY('h'):
      deleteChar();
      break;
    case PAGE_UP:
        /** set vertical index of cursor to offset
         *  position to simulate movement of page
         *  to that position **/
        E.cy = E.rowoff;
        break;
    case PAGE_DOWN:
        /** set vertical index of cursor to bottom
         *  of page and never out of the window **/
        E.cy = E.rowoff + E.screenrows - 1;
        if (E.cy > E.numrows) E.cy = E.numrows;
        break;
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      moveCursor(c);
      break;
    default:
      insertChar(c);
      break;
  }
}