# A command line editor in C

This was a first year project in my computer architecture module. 

> This editor, like many-other UNIX command-line editors, provides a command-line user interface and allows the user to display and operate on files in their current working directory.
> - File operations: creating, copying, deleting, saving, and displaying them on a user-interface.
> - Line operations: Real-time editing- insertion and deletion.
> - Creates log files for each file edited and saved with the editor, accessible through command line flags.
> - Additionally, provides a spelling check feature where misspellings are highlighted on the user-interface when the user presses `ctrl-f`.


## Editor controls and flags

```
/******************************************************************************
*                                  Editor                                     *
******************************************************************************/

CONTROLS
-------------------------------------------------------------------------------

Key                        meaning
ctrl-q                     quit
ctrl-x                     help message
ctrl-s                     save buffer
ctrl-k                     delete line
ctrl-h                     delete a characters
ctrl-f                     spell checker
ctrl-c                     copy file
ctrl-d                     delete file



COMMAND LINE FLAGS 
-------------------------------------------------------------------------------

flag                       meaning
--help                     view this file
--append <filname> string  append a string to filename.txt
--log <filename>           view the change log of filename.txt

```


