//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : IniFile.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:28 $
//  $Revision: 1.3 $
//  $RCSfile: IniFile.h,v $
//  $Author: niels $
//=========================================================================

#ifndef INIFILE_H
#define INIFILE_H

//-------------------------------------------------------------------------

#include <map>
#include <list>
#include <string>
#include <iostream.h>
#include "omnithread.h"

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define map    std::map
#define list   std::list
#define string std::string
#endif

//-------------------------------------------------------------------------

class IniFile 
{
//======
public: 
    //------------------------------------------------------
    // Constructor that automagically reads the specified file
    // If no file was specified none will be read.
    // If a file is specified, all settings will be automagically
    // written back to the same file when destroying the object.
	IniFile(string filename = "");
	~IniFile();

    //------------------------------------------------------
    // Read the specified file into the settings database
    // if erase_old_values == true then first erase everything
    // Returns true if file succesfully read
    //         false otherwise (like : syntax error / no such file)
    bool
    ReadFile(string filename);

    //------------------------------------------------------
    // Write all settings to the specified file
    // Returns true if file succesfully written
    //         false otherwise.
    bool
    WriteFile(string filename);

    //------------------------------------------------------
    // Prints all settings to the specified stream
    // Returns true if file succesfully written
    //         false otherwise.
    bool
    Print(ostream & os);

    //------------------------------------------------------
    // Set the specified setting in the specified section
    // If the section doesn't exist it is created
    // Returns true if succes
    // Returns false in case of error.
    bool
    SetValue(string section, string name, string value);

    //------------------------------------------------------
    // Set the specified setting in the specified section
    // If the section doesn't exist it is created
    // Returns true if succes
    // Returns false in case of error.
    bool
    SetValue(string section, string name, long value);

    //------------------------------------------------------
    // Get the specified setting from the specified section
    // The value is returned in the value parameter
    // Returns true if the setting exists --> value is filled
    // Returns false if the setting does not exist --> value is unchanged
    bool
    GetValue(string section, string name, string &value);

    //------------------------------------------------------
    // Get the specified setting from the specified section
    // The value is returned in the value parameter
    // Returns true if the setting exists and it is a number 
    //                          --> value is filled
    // Returns false if the setting does not exist or not a number 
    //                          --> value is unchanged
    bool
    GetValue(string section, string name, long &value);

    //------------------------------------------------------
    // Get the list of all the variables in the specified section
    // The list of names is returned in the variableNames parameter
    // Returns true if the section exists --> value is filled
    // Returns false if the section does not exist
    //                          --> value is unchanged
    bool
    GetVariableNames(string section,list<string> &variableNames);

    //------------------------------------------------------
    // Get the list of all the sections
    // The list of names is returned in the sectionNames parameter
    // Returns true if succes --> value is filled
    // Returns false in case of error --> value is unchanged
    bool
    GetSectionNames(list<string> &sectionNames);


    //------------------------------------------------------
    // Add the specified section
    // Returns true if created, false if already existed
    bool
    CreateSection(string section);

    //------------------------------------------------------
    // Add the specified number to the specified setting 
    // If the section doesn't exist it is created
    // If the setting doesn't exist it is created
    // If the existing setting is a number the value will be added
    // If the existing setting is NOT a number false is returned
    // Returns true if succes
    // Returns false in case of error.
    bool
    AddValue(string section, string name, long value);

    //------------------------------------------------------
    // Set lowest value of the specified and the current value.
    // If the section doesn't exist it is created
    // If the setting doesn't exist it is created
    // If the existing setting is a number the value will be added
    // If the existing setting is NOT a number false is returned
    // Returns true if succes
    // Returns false in case of error.
    bool
    MinValue(string section, string name, long value);

    //------------------------------------------------------
    // Set highest value of the specified and the current value.
    // If the section doesn't exist it is created
    // If the setting doesn't exist it is created
    // If the existing setting is a number the value will be added
    // If the existing setting is NOT a number false is returned
    // Returns true if succes
    // Returns false in case of error.
    bool
    MaxValue(string section, string name, long value);

    //------------------------------------------------------
    // Erase the specified setting
    // Returns true if erased, false if not present.
    bool
    EraseValue(string section, string name);

    //------------------------------------------------------
    // Erase the specified section and all settings in that section
    // Returns true if erased, false if not present.
    bool
    EraseSection(string section);

    //------------------------------------------------------
    // Erase all sections and all settings in them
    // Returns true if erased, false if already empty.
    bool
    EraseEverything();

//======
private:
    //------------------------------------------------------
    // Private members that are actually used for the storage 
    omni_mutex valuesMutex;    

    class Section: public map<string,string>
    {
        public:
            // Because the map<> will put all string members
            // in alphabetical order we need an extra storage 
            // to keep the current order
            list<string> variablesOrder;
            unsigned int maxVariableNameLen;
    };
    map <string,Section*> fSections;
    
    // Because the map<> will put all string members
    // in alphabetical order we need an extra storage 
    // to keep the current order
    list<string> sectionsOrder;
    
    // The currently loaded filename
    string fFilename;
    
    //------------------------------------------------------
    // Get the specified section
    // Returns NULL or a pointer to the specified section
    Section *
    GetSection(string section);

    //------------------------------------------------------
    // Add the specified section
    // Returns NULL or a pointer to the specified section
    Section *
    AddSection(string section);

    //------------------------------------------------------
    // Set the specified setting in the specified section
    // If the section doesn't exist it is created
    // Returns true if succes
    // Returns false in case of error.
    bool
    SetValue(Section * theSection, string name, string value);

    //------------------------------------------------------
    // Set the specified setting in the specified section
    // If the section doesn't exist it is created
    // Returns true if succes
    // Returns false in case of error.
    bool
    SetValue(Section * theSection, string name, long value);

    //------------------------------------------------------
    // Get the specified setting from the specified section
    // The value is returned in the value parameter
    // Returns true if the setting exists --> value is filled
    // Returns false if the setting does not exist --> value is unchanged
    bool
    GetValue(Section * theSection, string name, string &value);

    //------------------------------------------------------
    // Get the specified setting from the specified section
    // The value is returned in the value parameter
    // Returns true if the setting exists and it is a number 
    //                          --> value is filled
    // Returns false if the setting does not exist or not a number 
    //                          --> value is unchanged
    bool
    GetValue(Section * theSection, string name, long &value);

//======
};

#endif

// End of the file IniFile.h
//=========================================================================
