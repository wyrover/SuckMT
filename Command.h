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

#ifndef __COMMAND_H__
#define __COMMAND_H__

//-------------------------------------------------------------------------

class Command; // Forward Declaration

//-------------------------------------------------------------------------

#include "CommandHandler.h"
#include "Abortable.h"

//-------------------------------------------------------------------------

class Command : public Abortable
{
public:
    Command();
    virtual ~Command();
    virtual bool Execute(CommandHandler * currentHandler) = 0;
};

//-------------------------------------------------------------------------

#endif // __COMMAND_H__

// End of the file Command.h
//=========================================================================
