/*  _______         ____    __         ___    ___
 * \    _  \       \    /  \  /       \   \  /   /     '   '  '
 *  |  | \  \       |  |    ||         |   \/   |       .      .
 *  |  |  |  |      |  |    ||         ||\  /|  |
 *  |  |  |  |      |  |    ||         || \/ |  |       '  '  '
 *  |  |  |  |      |  |    ||         ||    |  |       .      .
 *  |  |_/  /        \  \__//          ||    |  |
 * /_______/edicated  \____/niversal  /__\  /____\usic /|  .  . astardisation
 *                                                    /  \
 *                                                   / .  \
 * playogg.c - Simple program to play OGG files.    / / \  \
 *                                                 | <  /   \_
 * Adapted from DUMB's player examples by Sven.    |  \/ /\   /
 *                                                  \_  /  > /
 *                                                    | \ / /
 *                                                    |  ' /
 *                                                     \__/
 */

#include <stdlib.h>
#include <allegro.h>
#include <aldumb.h>
#include <vorbis/vorbisfile.h>

#ifndef ALLEGRO_DOS
#include <string.h>
#endif

/* Note that your own programs should use <dumbogg.h> not "dumbogg.h". <>
 * tells the compiler to look in the compiler's default header directory,
 * which is where DUMB should be installed before you use it (make install
 * does this). Use "" when it is your own header file. This example uses ""
 * because DUMB might not have been installed yet when the makefile builds
 * it.
 */
#include "dumbogg.h"



#ifndef ALLEGRO_DOS
static int closed = 0;
static void closehook(void) { closed = 1; }
#else
#define closed 0
#endif

#ifdef ALLEGRO_WINDOWS
#define GFX_DUMBOGG_MODE GFX_GDI
#include <winalleg.h>
#define YIELD() Sleep(1)
#else
#define GFX_DUMBOGG_MODE GFX_AUTODETECT_WINDOWED
#ifdef ALLEGRO_UNIX
#include <sys/time.h>
static void YIELD(void)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1;
	select(0, NULL, NULL, NULL, &tv);
}
#else
#define YIELD() yield_timeslice()
#endif
#endif



static void usage(void)
{
	allegro_message(
		"Usage: playogg file.ogg\n"
		"This will play the .ogg file specified.\n"
		"You can control playback quality by editing dumb.ini.\n"
	);

	exit(1);
}



int main(int argc, char *argv[])
{
	DUH *duh;
	AL_DUH_PLAYER *dp;

	if (allegro_init())
		return 1;

	if (argc != 2)
		usage();

	set_config_file("dumbogg.ini");

	if (install_keyboard()) {
		allegro_message("Failed to initialise keyboard driver!\n");
		return 1;
	}

	set_volume_per_voice(0);

	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL)) {
		allegro_message("Failed to initialise sound driver!\n%s\n", allegro_error);
		return 1;
	}

	atexit(&dumb_exit);

	duh = dumb_load_ogg(argv[1], 1);
	if (!duh) {
		allegro_message("Failed to load %s!\n", argv[1]);
		return 1;
	}

	dumb_resampling_quality = get_config_int("sound", "dumb_resampling_quality", 4);

#ifndef ALLEGRO_DOS
	{
		const char *fn = get_filename(argv[1]);
		int w = strlen(fn);
		if (w < 22) w = 22;
		w = (w + 2) * 4;

		set_window_title("DUMBOGG - OGG player");

		if (set_gfx_mode(GFX_DUMBOGG_MODE, w*2, 80, 0, 0) == 0) {
			acquire_screen();
			textout_centre(screen, font, fn, w, 28, 14);
			textout_centre(screen, font, "Press any key to exit.", w, 44, 11);
			release_screen();
		}
	}

	set_window_close_hook(&closehook);
#endif

	set_display_switch_mode(SWITCH_BACKGROUND);

	dp = al_start_duh(duh, 2, 0, 1.0,
		get_config_int("sound", "buffer_size", 4096),
		get_config_int("sound", "sound_freq", 44100));

	for (;;) {
		if (keypressed()) {
			readkey();
			break;
		}

		if (al_poll_duh(dp) || closed)
			break;

		YIELD();
	}

	al_stop_duh(dp);

	unload_duh(duh);

	return 0;
}
END_OF_MAIN();

