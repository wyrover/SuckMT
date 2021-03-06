//=========================================================================
//                 Copyright (C)1999-2012 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   If you reuse code from SuckMT you are required to put a notice of 
//   this fact in both your manual and about box.
//
//=========================================================================

#ifndef __DuplicatesChecker_H__
#define __DuplicatesChecker_H__
    
//-------------------------------------------------------------------------

class DuplicatesChecker; // Forward Declaration

//-------------------------------------------------------------------------

#include <fstream>
#include <stdio.h>  
#include <stdlib.h>  
#include "Abortable.h"
#include "IniFile.h"
#include "NEWSArticle.h"

//-------------------------------------------------------------------------
// This class reads the specified value from the specified IniFile
// and logs this event by itself when a matching query is performed.

class DuplicatesChecker: 
        public Abortable
{
public: 
    DuplicatesChecker(IniFile *settings);

    ~DuplicatesChecker();

    bool
    DoWeNeedToDownloadThisArticle(NEWSArticle * article);

    void
    ArticleHasBeenStored(NEWSArticle * article);

    void
    ArticleHasBeenKilled(NEWSArticle * article);
        
private:
    omni_mutex valuesMutex;    
    
    // Message-ID storage system
    string     fRestartFileName;
    ofstream   fRestartFile;
    omni_mutex fRestartFileMutex;

    // The string is the messageID and 
    // the bool is always true except when not yet present
    map <string,bool>   fAllMessageIDs;
    map <string,bool>   fRestartMessageIDs;
    omni_mutex          fMessageDBmutex;
};

//-------------------------------------------------------------------------

#endif // __HEADERCHECKER_H__

// End of the file DuplicatesChecker.h
//=========================================================================
