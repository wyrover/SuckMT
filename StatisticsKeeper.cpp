//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : StatisticsKeeper.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:43 $
//  $Revision: 1.3 $
//  $RCSfile: StatisticsKeeper.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include "StatisticsKeeper.h"
 
//--------------------------------------------------------------------

StatisticsKeeper * statisticsKeeper = NULL;

//--------------------------------------------------------------------

// The specified time indicated the statistics should be printer automagically
void 
KeepStatistics(long milliseconds)
{
    if (statisticsKeeper != NULL)
        EndStatistics();
    
    statisticsKeeper = new StatisticsKeeper(milliseconds);
}

//--------------------------------------------------------------------

void 
EndStatistics()
{
    if (statisticsKeeper == NULL)
        return;
        
    statisticsKeeper->Abort();
    statisticsKeeper->join(NULL);
    statisticsKeeper = NULL;
}

//--------------------------------------------------------------------
StatisticsKeeper::StatisticsKeeper(long milliseconds): Printable("StatisticsKeeper")
{
    valuesModified = true;
    fMilliseconds = milliseconds;
    if (fMilliseconds > 0)
        start_undetached();
}

//--------------------------------------------------------------------

StatisticsKeeper::~StatisticsKeeper()
{
    // Nothing to do
}

//-------------------------------------------------------------------------

void* 
StatisticsKeeper::run_undetached(void* /*arg*/)
{
    unsigned long sleepTime = fMilliseconds * 1000;
    while (KeepRunning())
    {
        sleep(0,sleepTime);    
        if(valuesModified)
        {
            unsigned long killed  = GetNumericValue("Articles Killed");
            unsigned long total   = GetNumericValue("Articles Present");
            unsigned long stored  = GetNumericValue("Articles Written");
            unsigned long skipped = GetNumericValue("Articles Skipped");
            unsigned long qlen    = GetNumericValue("Command Queue Length");
            unsigned long done    = killed+stored+skipped;
            float precentage_done = (total==0?0.0:(((float)(done))/((float)total))*100.0);

            // I couldn't get the stream formatting to do what I wanted
            // So let's do this the traditional way.
            char percentage[100];
            sprintf(percentage,"%3.0f",precentage_done);

            cout 
                << "Q(" << setw(5) << qlen <<")," // Queue length
                << "Total("  << setw(5) << total  <<")," // Total
                << "Done (" << percentage <<"%)={" // Killed
                << "Stor("  << setw(5) << stored  <<")," // Stored
                << "Skip(" << setw(5) << skipped <<")," // Skipped
                << "Kill("  << setw(5) << killed  <<")}   \r" // Killed
                << flush;
            
            valuesModified = false;
        }
    }

    cout << endl << flush;

    return NULL;
}


//--------------------------------------------------------------------

void
StatisticsKeeper::SetValue(string name, string value)
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;
    stringValues[name] = value;
}

//--------------------------------------------------------------------

void
StatisticsKeeper::SetValue(string name, long value)
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;
    numericValues[name] = value;
}

//--------------------------------------------------------------------

void
StatisticsKeeper::AddValue(string name, long value)
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;

    numericValues[name] = numericValues[name] + value;
}

//--------------------------------------------------------------------

string 
StatisticsKeeper::GetStringValue(string name)
{
    omni_mutex_lock lock(valuesMutex);

    return stringValues[name];
}

//--------------------------------------------------------------------

long 
StatisticsKeeper::GetNumericValue(string name)
{
    omni_mutex_lock lock(valuesMutex);

    return numericValues[name];
}

//--------------------------------------------------------------------

void 
StatisticsKeeper::Print(ostream &os)
{
    omni_mutex_lock lock(valuesMutex);

    map <string,string>::const_iterator stringValuesIter;

    os << "Statistic values= " << endl << "{" << endl;

    for(stringValuesIter  = stringValues.begin();
        stringValuesIter != stringValues.end();
        stringValuesIter ++)
    {
        os << "\t" << stringValuesIter->first 
           << " \t= \"" << stringValuesIter->second << "\"" << endl;
    }

    map <string,long>::const_iterator   numericValuesIter;

    for(numericValuesIter  = numericValues.begin();
        numericValuesIter != numericValues.end();
        numericValuesIter ++)
    {
        os << "\t" << numericValuesIter->first 
           << " \t= \"" << numericValuesIter->second << "\"" << endl;
    }
    
    os << "}" << endl;
}

//--------------------------------------------------------------------

void 
STAT_SetValue(string name, string value)
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->SetValue(name,value);
}

//--------------------------------------------------------------------

void 
STAT_SetValue(string name, long value)
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->SetValue(name,value);
}

//--------------------------------------------------------------------

void 
STAT_AddValue(string name, long value)
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->AddValue(name,value);
}

//--------------------------------------------------------------------

string
STAT_GetStringValue(string name)
{
    if (statisticsKeeper == NULL)
        return "Statistics are disabled";
    return statisticsKeeper->GetStringValue(name);
}

//--------------------------------------------------------------------

long
STAT_GetNumericValue(string name)
{
    if (statisticsKeeper == NULL)
        return -1;
    return statisticsKeeper->GetNumericValue(name);
}

//--------------------------------------------------------------------

// End of the file StatisticsKeeper.cpp
//=========================================================================
