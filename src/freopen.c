/*
 * WCRT  -  Win32API CRT
 *
 * stdio freopen
 *
 * Copyright (c) 2003-2004 by Joergen Ibsen / Jibz
 * All Rights Reserved
 *
 * http://www.ibsensoftware.com/
 *
 * This software is provided 'as-is', without any express
 * or implied warranty.  In no event will the authors be
 * held liable for any damages arising from the use of
 * this software.
 *
 * Permission is granted to anyone to use this software
 * for any purpose, including commercial applications,
 * and to alter it and redistribute it freely, subject to
 * the following restrictions:
 *
 * 1. The origin of this software must not be
 *    misrepresented; you must not claim that you
 *    wrote the original software. If you use this
 *    software in a product, an acknowledgment in
 *    the product documentation would be appreciated
 *    but is not required.
 *
 * 2. Altered source versions must be plainly marked
 *    as such, and must not be misrepresented as
 *    being the original software.
 *
 * 3. This notice may not be removed or altered from
 *    any source distribution.
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <stdio.h>

#include <stdlib.h>

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
    HANDLE hFile;
    DWORD dwAccess;
    DWORD dwCreate;

    if (stream)
    {
	if (stream->hFile != INVALID_HANDLE_VALUE) CloseHandle(stream->hFile);
    } else {
	stream = (FILE *) malloc(sizeof(FILE));
	if (stream == NULL) return NULL;
    }

    stream->hFile = INVALID_HANDLE_VALUE;
    stream->unget = -1;

    // BUGBUG: mode never checked for NULL or length
    switch (mode[0])
    {
    case 'r':
	dwAccess = GENERIC_READ;
	dwCreate = OPEN_EXISTING;
	break;
    case 'w':
	dwAccess = GENERIC_WRITE;
	dwCreate = CREATE_ALWAYS;
	break;
    case 'a':
	dwAccess = GENERIC_READ | GENERIC_WRITE;
	dwCreate = OPEN_ALWAYS;
	break;
    default:
	free(stream);
	return NULL;
    }

    if (mode[1] == '+')	dwAccess = GENERIC_READ | GENERIC_WRITE;

    hFile = CreateFile(filename,
		       dwAccess,
		       FILE_SHARE_READ | FILE_SHARE_WRITE,
		       NULL,
		       dwCreate,
		       0,
		       NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
	free(stream);
	return NULL;
    }

    if (mode[0] == 'a') SetFilePointer(hFile, 0, NULL, FILE_END);

    stream->hFile = hFile;

    return stream;
}
