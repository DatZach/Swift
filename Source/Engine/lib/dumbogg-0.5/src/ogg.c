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
 * ogg.c - Code for using Ogg Vorbis streams        / / \  \
 *         with DUMB.                              | <  /   \_
 *                                                 |  \/ /\   /
 * Written by Sven_                                 \_  /  > /
 *                                                    | \ / /
 *                                                    |  ' /
 *                                                     \__/
 */

#include <stdlib.h>
#include <string.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <dumb.h>
#include "dumbogg.h"



#define SIGTYPE_OGG DUMB_ID('O','G','G',' ')



typedef struct OGG_SIGNAL OGG_SIGNAL;
typedef struct OGG_SAMPINFO OGG_SAMPINFO;



struct OGG_SAMPINFO
{
	OggVorbis_File stream;
	int bitstream, n_channels, stream_channels, loop;
};



struct OGG_SIGNAL
{
	char *filename;
	long length, average_bitrate;
	int channels, frequency, loop;
};



static long ogg_render_samples(void *sampinfo, float volume,
                               float delta, long size, sample_t **samples);
static void *ogg_start_samples(DUH *duh, void *signal, int n_channels, long pos);
static void ogg_end_samples(void *sampinfo);
static void ogg_unload_signal(void *signal);



static DUH_SIGTYPE_DESC sigtype_ogg = {
	SIGTYPE_OGG,
	NULL,				/* entheh says I don't need you */
	&ogg_start_samples,
	NULL,				/* don't need this either */
	&ogg_render_samples,
	&ogg_end_samples,
	&ogg_unload_signal
};




/* dumb_load_ogg(): loads an OGG file into a DUH struct, returning a pointer
 * to the DUH struct. When you have finished with it, you must pass the
 * pointer to unload_duh() so that the memory can be freed.
 * Note that I don't use DUMBFILES .. they're dumb ;)
 */
DUH *dumb_load_ogg(const char *filename, int loop)
{
	int n;
	FILE *file;
	void *signal;
	double time;
	OggVorbis_File ov;
	DUH_SIGTYPE_DESC *descptr = &sigtype_ogg;


#if 0
	if(!exists(filename))
		return NULL;
#endif


	/* open the file */
	file = fopen(filename,"rb");
	if (!file) return NULL;


	/* let oggvorbis take over the file */
	n = ov_open(file, &ov, NULL,0);
	if(n < 0)
	{
		fclose(file);
		return NULL;
	}


	/* allocate the signal */
	signal = malloc(sizeof(OGG_SIGNAL));
	if (!signal) {
		ov_clear(&ov);
	}
	((OGG_SIGNAL*)signal)->filename = malloc(sizeof(*((OGG_SIGNAL*)signal)->filename) * (strlen(filename) + 1));
	strcpy(((OGG_SIGNAL*)signal)->filename,filename);


	/* Store information about it for retrieval later */
	time = ov_time_total(&ov,-1);
	if(time == OV_EINVAL)
	{
		ov_clear(&ov);
		free(signal);
		return NULL;
	}

	{
		vorbis_info *vi = ov_info(&ov, -1);
		((OGG_SIGNAL*)signal)->length = ov_pcm_total(&ov, -1);
		((OGG_SIGNAL*)signal)->average_bitrate = ov_bitrate(&ov,-1);
		((OGG_SIGNAL*)signal)->channels = vi->channels;
		((OGG_SIGNAL*)signal)->frequency = vi->rate;
		((OGG_SIGNAL*)signal)->loop = loop;
	}


	/* close the OggVorbis_File stream */
	ov_clear(&ov);


	return make_duh(((OGG_SIGNAL*)signal)->length, 1, &descptr, &signal);
}




/* ogg_render_samples:
 * Rewritten and works accurately now :)
 */
static long ogg_render_samples(void *sampinfo, float volume,
                               float delta, long size, sample_t **samples)
{
	long total_samples_read=0;
	OggVorbis_File *ov = &((OGG_SAMPINFO*)sampinfo)->stream;

	(void)delta; // !!

	while(total_samples_read < size)
	{
		float **pcm;
		long temp = ov_read_float(ov, &pcm, size - total_samples_read, NULL);

		// Note: delta is ignored!
		// Note: delta?
		// <HappyFun> I heard that Note: delta is ignored!

		if (temp <= 0) {
			if (((OGG_SAMPINFO*)sampinfo)->loop)
				ov_pcm_seek(ov, 0);
			else
				return total_samples_read;
		}

		if (ov->vi->channels == ((OGG_SAMPINFO*)sampinfo)->n_channels) {
			int i, j;
			float mul = 32768.0f * volume;
			for (j = 0; j < ((OGG_SAMPINFO*)sampinfo)->n_channels; j++) {
				sample_t *dst = samples[j] + total_samples_read;
				float *src = pcm[j];
				for (i = 0; i < temp; i++)
					dst[i] += (int)(src[i] * mul);
			}
		} else {
			int i, j, k;
			float mul = 32768.0f * volume / ov->vi->channels;
			for (k = 0; k < ov->vi->channels; k++) {
				float *src = pcm[k];
				for (j = 0; j < ((OGG_SAMPINFO*)sampinfo)->n_channels; j++) {
					sample_t *dst = samples[j] + total_samples_read;
					for (i = 0; i < temp; i++)
						dst[i] += (int)(src[i] * mul);
				}
			}
		}

		total_samples_read += temp;

#if 0
		temp = _dumb_ogg_read_samples(ov, size - total_samples_read, ((OGG_SAMPINFO*)sampinfo)->n_channels, samples, &((OGG_SAMPINFO*)sampinfo)->bitstream, total_samples_read, volume);
		if(temp == 0)
		{
			/* check if the user requested that this stream loop */
			if(((OGG_SAMPINFO*)sampinfo)->loop)
				ov_pcm_seek(ov, 0);
			else
				return total_samples_read;
		}
		total_samples_read += temp;
#endif
	}

	return total_samples_read;
}



/* ogg_start_samples:
 * 
 */
static void *ogg_start_samples(DUH *duh, void *signal, int n_channels, long pos)
{
	FILE *file;
	void *sampinfo;

	(void)duh;

	/* Open the file pointer */
	file = fopen( ((OGG_SIGNAL*)signal)->filename, "rb");

	if (!file)
		return NULL;

	/* Allocate the sample info */
	sampinfo = malloc(sizeof(OGG_SAMPINFO));
	if(!sampinfo) {
		fclose(file);
		return NULL;
	}


	/* Open the stream */
	if(ov_open(file, &((OGG_SAMPINFO*)sampinfo)->stream, NULL, 0) < 0)
	{
		free(sampinfo);
		fclose(file);
		return NULL;
	}


	/* Save some information */
	((OGG_SAMPINFO*)sampinfo)->n_channels = n_channels;
	((OGG_SAMPINFO*)sampinfo)->loop = ((OGG_SIGNAL*)signal)->loop;


	/* Seek to the position the user requested */
	ov_pcm_seek( &((OGG_SAMPINFO*)sampinfo)->stream,pos );


	return sampinfo;
}





/* ogg_end_samples:
 * closes the vorbis file and frees memory
 */
static void ogg_end_samples(void *sampinfo)
{
	if(sampinfo)
	{
		/* close the sample stream */
		ov_clear( &((OGG_SAMPINFO*)sampinfo)->stream );
		
		free(sampinfo);
	}

	return;
}



/* ogg_unload_signal:
 * Frees memory allocated for the duh signal
 */
static void ogg_unload_signal(void *signal)
{
	if(signal)
	{
		if( ((OGG_SIGNAL*)signal)->filename)
			free( ((OGG_SIGNAL*)signal)->filename);

		free(signal);
	}

	return;
}



