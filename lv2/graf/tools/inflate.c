
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define ZLIB_CHUNK_SIZE			0x1000

static int zlib_inflate(FILE *fin, FILE *fout);

static const char *zlib_error2str(int error);

static void usage(const char *progname);

int main(int argc, char **argv)
{
	FILE *fin, *fout;
	int result;

	if (argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}

	fin = fopen(argv[1], "r");
	if (fin == NULL)
	{
		fprintf(stderr, "couldn't open input file '%s'\n", argv[1]);
		exit(1);
	}

	fout = fopen(argv[2], "w");
	if (fin == NULL)
	{
		fprintf(stderr, "couldn't open output file '%s'\n", argv[2]);
		exit(1);
	}

	result = zlib_inflate(fin, fout);
	if (result != Z_OK)
	{
		fprintf(stderr, "zlib error %s\n", zlib_error2str(result));
		exit(1);
	}

	fclose(fin);
	fclose(fout);

	exit(0);
}

static int zlib_inflate(FILE *fin, FILE *fout)
{
	z_stream strm;
	unsigned char bin[ZLIB_CHUNK_SIZE];
	unsigned char bout[ZLIB_CHUNK_SIZE];
	int nleft, result;

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;

	result = inflateInit(&strm);
	if (result != Z_OK)
		return result;

	do
	{
		strm.avail_in = fread(bin, 1, ZLIB_CHUNK_SIZE, fin);
        if (ferror(fin))
		{
			inflateEnd(&strm);
			return Z_ERRNO;
        }

        if (strm.avail_in == 0)
			break;

		strm.next_in = bin;

		do
		{
			strm.avail_out = ZLIB_CHUNK_SIZE;
			strm.next_out = bout;

			result = inflate(&strm, Z_NO_FLUSH);
			if (result == Z_STREAM_ERROR)
				return result;

			switch (result)
			{
				case Z_NEED_DICT:
					result = Z_DATA_ERROR;
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&strm);
					return result;
					break;
			}

			nleft = ZLIB_CHUNK_SIZE - strm.avail_out;

			if ((fwrite(bout, 1, nleft, fout) != nleft) || ferror(fout))
			{
				inflateEnd(&strm);
				return Z_ERRNO;
			}
		} while (strm.avail_out == 0);
	} while (result != Z_STREAM_END);

	inflateEnd(&strm);

	if (result != Z_STREAM_END)
		return Z_DATA_ERROR;

	return Z_OK;
}

static const char *zlib_error2str(int error)
{
	static char buf[64];

	switch (error)
	{
		case Z_ERRNO:
			return "Z_ERRNO";
		break;

		case Z_STREAM_ERROR:
			return "Z_STREAM_ERROR";
		break;

		case Z_DATA_ERROR:
			return "Z_DATA_ERROR";
		break;

		case Z_MEM_ERROR:
			return "Z_MEM_ERROR";
		break;

		case Z_VERSION_ERROR:
			return "Z_VERSION_ERROR";
		break;

		default:
			sprintf(buf, "%d", error);
			return buf;
		break;
	}
}

static void usage(const char *progname)
{
	fprintf(stderr, "usage: %s <input file> <output file>\n", progname);
}
