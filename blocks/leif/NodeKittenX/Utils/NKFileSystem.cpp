//
//  NKFileSystem.cpp
//  NodeKittenX
//
//  Created by Leif Shackelford on 2/1/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#include "NKFileSystem.h"
#include "NKStringUtil.h"

#define MAX_PATH 256

#if (TARGET_OSX || TARGET_IOS)
#import "NKAppleDeviceTools.h"
#endif

string appDirectory(){
   
#if TARGET_EMSCRIPTEN
    return "Assets/";
#elif TARGET_WIN32
    char result[ MAX_PATH ];
    return std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
#elif TARGET_LINUX
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    return std::string( result, (count > 0) ? count : 0 );
#elif (TARGET_OSX || TARGET_IOS)
     return NKApple::appDirectory();
#endif
    
}
using namespace String;

/*!
	Replace the \ for / in a an absolute or relative file path.
 
	\param[in,out] filepath The file path to adjust.
 */
void adjust_file_path(string& filePath)
{
    unsigned long i = 0,
    l = filePath.length();
    
    while( i != l )
    {
        if( filePath[ i ] == '\\' ) filePath[ i ] = '/';
        ++i;
    }
}

/*!
	Open/Extract a file from disk and load it in memory.
	
	\param[in] filename The file to load in memory.
	\param[in] relative_path Determine if the filename is an absolute or relative path.
	
	\return Return a MEMORY structure pointer if the file is found and loaded, instead will return
	NULL.
 */

NKFile::NKFile(string& filename, bool relative_path){

#if !TARGET_ANDROID
    
    FILE *f;
    
    if( relative_path )
    {
        string path = appDirectory();
        
        nkLog("filesystm : %s \n", path);
        
        _filename = path + filename;
    }
    
    else _filename = filename;
    
    f = fopen( _filename.c_str(), "rb" );
    
    if( !f ) return;
    
    fseek( f, 0, SEEK_END );
    _size = ftell( f );
    fseek( f, 0, SEEK_SET );
    
    
    _buffer = ( unsigned char * ) calloc( 1, _size + 1 );
    fread( _buffer, _size, 1, f );
    _buffer[ _size ] = 0;
    
    
    fclose( f );
    
    
#else
    
    char fpath[ MAX_PATH ] = {""},
			 fname[ MAX_PATH ] = {""};
    
    unzFile		    uf;
    unz_file_info   fi;
    unz_file_pos    fp;
    
    strcpy( fpath, getenv( "FILESYSTEM" ) );
    
    uf = unzOpen( fpath );
    
    if( !uf ) return NULL;
    
    if( relative_path ) sprintf( fname, "assets/%s", filename );
    else strcpy( fname, filename );
    
    unzGoToFirstFile( uf );
    
    MEMORY *memory = ( MEMORY * ) calloc( 1, sizeof( MEMORY ) );
    
    unzGetFilePos( uf, &fp );
    
    if( unzLocateFile( uf, fname, 1 ) == UNZ_OK )
    {
        unzGetCurrentFileInfo(  uf,
                              &fi,
                              memory->filename,
                              MAX_PATH,
                              NULL, 0,
                              NULL, 0 );
        
        if( unzOpenCurrentFilePassword( uf, NULL ) == UNZ_OK )
        {
            memory->position = 0;
            memory->size	 = fi.uncompressed_size;
            memory->buffer   = ( unsigned char * ) realloc( memory->buffer, fi.uncompressed_size + 1 );
            memory->buffer[ fi.uncompressed_size ] = 0;
            
            while( unzReadCurrentFile( uf, memory->buffer, fi.uncompressed_size ) > 0 ){}
            
            unzCloseCurrentFile( uf );
            
            unzClose( uf );
            
            return memory;
        }
    }
    
    unzClose( uf );
    
    return NULL;
    
#endif
    
    nkLog("read file : %u bytes", _size);
}


/*!
	Close and free a previously initialized MEMORY stream.
	
	\param[in,out] memory A valid MEMORY structure pointer.
	
	\return Return a NULL MEMORY structure pointer.
 */

NKFile::~NKFile(){
    if( _buffer ) free( _buffer );
}


/*!
	Similar as the read method, this function read a chunk of bytes
	at starting from the current location of the cursor in the memory stream
	to a specific size. The result will be stored in the dst pointer.
	
	\param[in,out] memory A valid MEMORY structure pointer.
	\param[in,out] dst A void pointer to store the bytes that was read from this function.
	\param[in] size The number of bytes to read from the memory stream.
	
	\return Return the number of bytes that the function store in the dst pointer.
 */

unsigned long NKFile::read(void *dst, unsigned long size){
    if( ( _position + size ) > _size )
    { size = _size - _position; }
    
    memcpy( dst, &_buffer[ _position ], size );
    
    _position += size;
    
    return size;
}

/*!
	Insert text at a specific position in a memory stream.
	
	\param[in,out] memory A valid MEMORY structure pointer with a loaded buffer.
	\param[in] str The string of text to insert.
	\param[in] position The position (in bytes) to inser the text.
 */
void NKFile::insert(char *str, unsigned long position )
{
    unsigned long s1 = strlen( str ),
    s2 = _size + s1 + 1;
    
    char *buffer = ( char * )_buffer,
    *tmp	 = ( char * )calloc( 1, s2 );
    
    if( position )
    { strncpy( &tmp[ 0 ], &buffer[ 0 ], position ); }
    
    strcat( &tmp[ position ], str );
    
    strcat( &tmp[ position + s1 ], &buffer[ position ] );
    
    _size = s2;
    
    free( _buffer );
    _buffer = ( unsigned char * )tmp;
}

