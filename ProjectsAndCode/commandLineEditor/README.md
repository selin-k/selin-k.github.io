# command line editor

This was a first year project in my computer architecture module. 

This editor, like many-other UNIX command-line editors, provides a command-line user interface and allows the user to display and operate on files in their current working directory.
- File operations: creating, copying, deleting, saving, and displaying them on a user-interface.
- Line operations: Real-time editing- insertion and deletion.
- Creates log files for each file edited and saved with the editor, accessible through command line flags.
- Additionally, provides a spelling check feature where misspellings are highlighted on the user-interface when the user presses `ctrl-f`.

**High level design**
![Design of editor.](https://github.com/selin-k/selin-k.github.io/blob/main/ProjectsAndCode/media/design.PNG)

[View on GitHub](https://github.com/selin-k/selin-k.github.io/tree/main/ProjectsAndCode/commandLineEditor)

## Execution

```
gcc -o editor editor.c spell.c dictionary.c -std=c99 -std=gnu99
./editor
```

**notes:**

`./editor --help` for control and flag information.

`spell.c` has a hardcoded definition that is the path to the "`large.txt`" file.
This is a dictionary. Please keep it in the same working directory as the editor 
executable to avoid issues with the spelling checker feature.


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

This editor indeed was heavily inspired by the `kilo` editor and that has been referenced in my documentation.


<iframe src="documentation_final.pdf" width="100%" height="500px">
    </iframe>
    
    
Testing the editor:
[![Testing the editor](https://github.com/selin-k/selin-k.github.io/blob/main/ProjectsAndCode/media/video.jpg?raw=true)](https://www.youtube.com/watch?v=-pAq43psZnk)

