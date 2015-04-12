//
//  NKFileSystem.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 2/1/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __NodeKittenX__NKFileSystem__
#define __NodeKittenX__NKFileSystem__

#include "NKTypes.h"

//! Structure that allows you to manipulate memory stream.
class NKFile
{
    //! The filename used by the MEMORY structure.
    string			_filename;
    
    //! The file size in bytes.
    unsigned long	_size {0};
    
    //! The position of the cursor within the memory buffer.
    unsigned long	_position {0};
    
    //! The memory buffer.
    unsigned char	*_buffer {nullptr};
    
public:
    
    NKFile(string& filename, bool relative_path);
    ~NKFile();
    
    unsigned long size(){return _size;};
    string filename(){return _filename;};
    unsigned char	*data(){return _buffer;};
    
    unsigned long read(void *dst, unsigned long size );
    void insert(char *str, unsigned long position );
    
};

// Utility



#endif /* defined(__NodeKittenX__NKFileSystem__) */

