
#include <stdio.h> 
#include <string.h>

#define NOB_IMPLEMENTATION

// Define if you want to do release mode (which atm just stops the windows app opening a console)
//#define BUILD_RELEASE

//#define TARGET_WIN32
#define TARGET_UNIX

#include "nob.h"

#define PROGRAM_NAME "Castle"

/* All paths assume target is created in the top level folder. */
/* That is, to distribute, just delete all the C files n shit lmao */

int main(int argc, char **argv) {

	NOB_GO_REBUILD_URSELF(argc, argv);

	Nob_Cmd cmd = { 0 };
	
	//nob_mkdir_if_not_exists("build");

	#ifdef TARGET_UNIX
		nob_cmd_append(&cmd, "clang", "-o", PROGRAM_NAME);
	#endif
	#ifdef TARGET_WIN32
		nob_cmd_append(&cmd, "mingw", "-o", PROGRAM_NAME);
	#endif

	nob_cmd_append(&cmd, "-g");

	// Get all files out of src dir
	/*
 	* What the fuck is going on with this? Why does it shit all over my path?
 	* We may never know.
 	*
	Nob_File_Paths source_paths;
	nob_read_entire_dir("src", &source_paths);

	for (int i=0; i<source_paths.count; i++) {
		const char* filename = source_paths.items[i];
		// TODO: recursively go into subdirs and append them to the command
		if (strcmp(filename, ".") == 0) { continue; }
		if (strcmp(filename, "..") == 0) { continue; }

		printf("[FILE] src/%s\n", filename);
		//if (nob_get_file_type(filename) == NOB_FILE_DIRECTORY) { continue; }

		// It seems kind of wasteful to make a whole new stringbuilder for this every time
		Nob_String_Builder st = { 0 };
		nob_sb_appendf(&st, "%s", filename);
		nob_sb_append_null(&st);
		nob_cmd_append(&cmd, st.items);
    	nob_sb_free(st);
	}
	*/

	nob_cmd_append(&cmd, "src/main.c");
	nob_cmd_append(&cmd, "src/gameobjects.c");
	nob_cmd_append(&cmd, "src/collisions.c");

	// Include
	nob_cmd_append(&cmd, "-Iinclude");


	// Libraries
	#if defined(TARGET_UNIX)
		nob_cmd_append(&cmd, "-L./lib/linux");
		nob_cmd_append(&cmd, "-Wl,-rpath,./lib/linux");
		nob_cmd_append(&cmd, "-lraylib");

	#elif defined(TARGET_WIN32)
		nob_cmd_append(&cmd, "-L./lib/win32");
		nob_cmd_append(&cmd, "-Wl,-rpath,./lib/win32");
		nob_cmd_append(&cmd, "-lraylibdll");
		#if defined(BUILD_RELEASE)
			nob_cmd_append(&cmd, "-mwindows");
		#endif

	#endif


	// Other stuff
	nob_cmd_append(&cmd, "-D", "ASSETS_PATH=\"./assets/\"");
	
	#if defined(TARGET_UNIX)
		nob_cmd_append(&cmd, "-lm");
	#endif

	nob_cmd_run_sync(cmd);

	return 0;
}


