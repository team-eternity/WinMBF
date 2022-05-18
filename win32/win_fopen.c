//
//  Copyright(C) 2021 Roman Fomin
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
// DESCRIPTION:
//      unicode paths for fopen() on Windows

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <direct.h>
#ifdef _MSC_VER
#include <io.h>
#endif

static wchar_t* ConvertToUtf8(const char *str)
{
    wchar_t *wstr = NULL;
    int wlen = 0;

    wlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);

    if (!wlen)
    {
        return NULL;
    }

    wstr = malloc(sizeof(wchar_t) * wlen);

    if (!wstr)
    {
        return NULL;
    }

    if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, wlen) == 0)
    {
        free(wstr);
        return NULL;
    }

    return wstr;
}

FILE* D_fopen(const char *filename, const char *mode)
{
    FILE *f;
    wchar_t *wname = NULL;
    wchar_t *wmode = NULL;

    wname = ConvertToUtf8(filename);

    if (!wname)
    {
        return NULL;
    }

    wmode = ConvertToUtf8(mode);

    if (!wmode)
    {
        free(wname);
        return NULL;
    }

    f = _wfopen(wname, wmode);

    free(wname);
    free(wmode);

    return f;
}

int D_remove(const char *path)
{
    wchar_t *wpath = NULL;
    int ret;

    wpath = ConvertToUtf8(path);

    if (!wpath)
    {
        return 0;
    }

    ret = _wremove(wpath);

    free(wpath);

    return ret;
}

int D_rename(const char *oldname, const char *newname)
{
    wchar_t *wold = NULL;
    wchar_t *wnew = NULL;
    int ret;

    wold = ConvertToUtf8(oldname);

    if (!wold)
    {
        return 0;
    }

    wnew = ConvertToUtf8(newname);

    if (!wnew)
    {
        free(wold);
        return 0;
    }

    ret = _wrename(wold, wnew);

    free(wold);
    free(wnew);

    return ret;
}

int D_stat(const char *path, struct stat *buf)
{
    wchar_t *wpath = NULL;
    struct _stat wbuf;
    int ret;

    wpath = ConvertToUtf8(path);

    if (!wpath)
    {
        return 0;
    }

    ret = _wstat(wpath, &wbuf);

    buf->st_mode = wbuf.st_mode;

    free(wpath);

    return ret;
}

int D_open(const char *filename, int oflag)
{
    wchar_t *wname;
    int ret;

    wname = ConvertToUtf8(filename);

    if (!wname)
    {
        return 0;
    }

    ret = _wopen(wname, oflag);

    free(wname);

    return ret;
}

int D_access(const char *path, int mode)
{
    wchar_t *wpath;
    int ret;

    wpath = ConvertToUtf8(path);

    if (!wpath)
    {
        return 0;
    }

    ret = _waccess(wpath, mode);

    free(wpath);

    return ret;
}

int D_mkdir(const char *dirname)
{
    wchar_t *wdir;
    int ret;

    wdir = ConvertToUtf8(dirname);

    if (!wdir)
    {
        return 0;
    }

    ret = _wmkdir(wdir);

    free(wdir);

    return ret;
}

#endif
