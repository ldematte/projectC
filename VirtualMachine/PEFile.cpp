
#include "PEFile.h"
#include <dbghelp.h>

HMODULE PEFile::openFile(const std::string& filename)
{
  return ::LoadLibraryEx(filename.c_str(), NULL, 0);
}

bool PEFile::importBindings(HMODULE module)
{

  // build parser
 

  // $$$ first attempt to import from the resources, ELSE do all of this export
  // $$$ table stuff... store the digest in with the resource data 

  // set to local image by default
  if ( module == NULL )
  {
    module = ::GetModuleHandle( NULL );
  }

  // Prep the import.

  // Find the export table.

  // get headers
  const BYTE* imageBase = reinterpret_cast <const BYTE*> ( module );
  const IMAGE_DOS_HEADER* dosHeader = reinterpret_cast <const IMAGE_DOS_HEADER*> ( module );
  const IMAGE_NT_HEADERS* winHeader = reinterpret_cast <const IMAGE_NT_HEADERS*> (
        imageBase + dosHeader->e_lfanew );

 
  // find the export data directory
  const IMAGE_DATA_DIRECTORY& exportDataDir
        = winHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ];

    DWORD exportRva  = exportDataDir.VirtualAddress;
    DWORD exportSize = exportDataDir.Size;

 

    // see if it exists

    if ( exportRva == 0 )
    {
      return ( false );   // nope
    }

 

    // find the export directory

    DWORD exportBegin = exportRva;
    DWORD exportEnd = exportBegin + exportSize;

    const IMAGE_EXPORT_DIRECTORY* exportDir
        = reinterpret_cast <const IMAGE_EXPORT_DIRECTORY*> ( imageBase + exportBegin );

 
    // find the subtables
    const DWORD* funcTable
        = reinterpret_cast <const DWORD*> ( imageBase + exportDir->AddressOfFunctions    );

    const DWORD* nameTable
        = reinterpret_cast <const DWORD*> ( imageBase + exportDir->AddressOfNames        );

    const WORD * ordinalTable
        = reinterpret_cast <const WORD *> ( imageBase + exportDir->AddressOfNameOrdinals );

 
    // Iterate over all the exported functions. 

    const DWORD* nameTableBegin = nameTable;
    const DWORD* nameTableEnd   = nameTableBegin + exportDir->NumberOfNames;
    const DWORD* nameTableIter;

    const WORD * ordinalTableIter = ordinalTable; 
    bool success = true;

    for (nameTableIter = nameTableBegin; nameTableIter != nameTableEnd; ++nameTableIter, ++ordinalTableIter )
    {
        const char* functionName
            = reinterpret_cast <const char*> ( imageBase + *nameTableIter );

        DWORD functionAddress = funcTable[ *ordinalTableIter ];
        // $ special: skip if it's just a string

        /*
        if (strncmp( "??_C@_", functionName, 6 ))
        {
            continue;
        } 
        */

        // this will be false if it's a forwarding export (unlikely but
        // whatever...)

        if ( (functionAddress < exportBegin) || (functionAddress >= exportEnd) )
        {
            // get address to function call
            functionAddress += reinterpret_cast <DWORD> ( imageBase );

 
            // $ NOTE $ this code is VERY VC++ version specific. it knows about
            //          how the compiler will export a function via a relative
            //          jump (probably for easy relocations). if this pattern
            //          changes in the future, this code will need to be
            //          updated.


            // it probably points to a jump indirection - redirect to actual
            // function

            if ( *reinterpret_cast <const BYTE*> ( functionAddress ) == 0xE9 /*jmp*/ )
            {
                // cool, figure out relative jump address (function + sizeof op)
                ++functionAddress;
                DWORD offset = *reinterpret_cast <const DWORD*> ( functionAddress );
                functionAddress += 4 + offset;
            }


            // attempt to import it
            std::cout << functionName << std::endl;  

            char unmangledName[1024];
            
            DWORD dwRet = UnDecorateSymbolName(functionName, unmangledName, sizeof(unmangledName), UNDNAME_COMPLETE);

            if (dwRet)
              std::cout << unmangledName << std::endl;  

        }

    }

    // Postprocess.
    // We reconstruct function hierarchy, classes etc. 



    /*
    const ClassSpec* eventSpec = FindEventNamespace();

    eVarType skritObjectType = FindSkritObject()->m_Type;

 

    // postprocess global functions

    PostProcess( m_GlobalFunctions );

 

    // fix up classes

    {

        ClassByNameMap::iterator i,

            begin = m_Classes.begin(), end = m_Classes.end();

        for ( i = begin ; i != end ; ++i )

        {

            ClassSpec& spec = *i->second;

 

            gpassert( !(spec.m_Flags & ClassSpec::FLAG_POSTPROCESSED) );

            spec.PostProcess();

            spec.m_Flags |= ClassSpec::FLAG_POSTPROCESSED;

 

            m_Digest = spec.AddExtraDigest( m_Digest );

        }

    }

 

    // postprocess our enums

    {

        EnumIndex::iterator i,

            begin = m_EnumVarTypeIndex.begin(), end = m_EnumVarTypeIndex.end();

        for ( i = begin ; i != end ; ++i )

        {

            (*i)->PostProcess();

        }

 

        begin = m_IrregularEnums.begin(), end = m_IrregularEnums.end();

        for ( i = begin ; i != end ; ++i )

        {

            (*i)->PostProcess();

        }

    }

 

    // postprocess SKRIT_IMPORT and DECLARE_EVENT functions

    {

        FunctionByAddrMap::iterator i,

            begin = m_Functions.begin(), end = m_Functions.end();

        for ( i = begin ; i != end ; ++i )

        {

            FunctionSpec& spec = i->second;

            if (   (spec.m_ParamSpecs.size() >= 2)      // object + function name

                && (spec.m_ParamSpecs[ 0 ].m_Type.m_Type == skritObjectType)

                && (spec.m_ParamSpecs[ 1 ].m_Type.IsCString()) )

            {

                spec.m_Flags |= FunctionSpec::FLAG_HIDDEN

                             | FunctionSpec::FLAG_SKRIT_IMPORT;

            }

 

            if ( spec.m_Parent == eventSpec )

            {

                spec.m_Flags |= FunctionSpec::FLAG_EVENT;

#               if ( !GP_RETAIL )

                {

                    if ( !spec.m_Name.same_no_case( "On", 2 ) )

                    {

                        gpwarningf(( "FuBi consistency warning for event function "

                                     "'%s' - events should be of the form "

                                     "'OnEvent'\n",

                                     spec.m_Name.c_str() ));

                    }

                }

#               endif // !GP_RETAIL

            }

        }

    }

 

    // final renaming

    ReplaceNameInIndex( m_Classes );

    ReplaceNameInIndex( m_Enums );

    m_RenameComplete = true;

 

    // check for overloads in event namespace - not supported

#   if ( !GP_RETAIL )

    {

        FunctionByNameIndex::const_iterator

                i,

                begin = eventSpec->m_Functions.begin(),

                end   = eventSpec->m_Functions.end();

        for ( i = begin ; i != end ; ++i )

        {

            // check for overloads

            if ( i->second.size() != 1 )

            {

                gperrorf(( "%s namespace contains overloaded function '%s' - not "

                           "supported by FuBi/Skrit\n",

                               eventSpec->m_Name.c_str(), i->first.c_str() ));

                success = false;

            }

            const FunctionSpec* funcSpec = *i->second.begin();

 

            // check for vararg

            if ( funcSpec->m_Flags & FunctionSpec::FLAG_VARARG )

            {

                gperrorf(( "%s namespace cannot contain functions using a "

                           "variable argument list ('%s')\n",

                               eventSpec->m_Name.c_str(), i->first.c_str() ));

                success = false;

            }

 

            // first param must be skrit object

            if ( funcSpec->m_ParamSpecs.empty() )

            {

                gperrorf(( "Event function '%s' must at least take a Skrit object "

                           "as its first param\n",

                               i->first.c_str() ));

                success = false;

            }

            else

            {

                const TypeSpec& firstParam

                    = funcSpec->m_ParamSpecs.begin()->m_Type;

                if (   (firstParam.m_Type != skritObjectType)

                    || !(firstParam.m_Flags & (TypeSpec::FLAG_POINTER |

                                               TypeSpec::FLAG_HANDLE |

                                               TypeSpec::FLAG_REFERENCE)) )

                {

                    gperrorf(( "Event function '%s' must at least take a Skrit "

                               "object as its first param\n",

                                   i->first.c_str() ));

                    success = false;

                }

            }

        }

    }

#   endif // !GP_RETAIL

 

    // check to make sure that all canonical rpc's are accounted for

#   if ( GP_DEBUG )

    {

        FunctionIndex::const_iterator begin = m_FunctionsBySerialID.begin(),

                                      end   = begin + CanonicalRpcSpec::ms_Count;

        gpassert( std::find( begin, end, (FunctionSpec*)NULL ) == end );

    }

#   endif // GP_DEBUG

 

    // check to make sure we've used all our replacements

#   if ( GP_DEBUG )

    {

        if ( !TestOptions( OPTION_NO_STRICT ) )

        {

            NameReplaceSpec::ReplaceColl coll;

            NameReplaceSpec::GetUnusedReplacements( coll );

 

            if ( !coll.empty() )

            {

                ReportSys::AutoReport autoReport( &gErrorContext );

                gperrorf(( "Found unused global FuBi name replacement specs:\n" ));

                ReportSys::AutoIndent autoIndent( &gErrorContext );

 

                NameReplaceSpec::ReplaceColl::const_iterator i,

                    begin = coll.begin(), end = coll.end();

                for ( i = begin ; i != end ; ++i )

                {

                    gperrorf(( "%s -> %s\n", i->first, i->second ));

                }

            }

        }

    }

#   endif // GP_DEBUG

 

// Done.

 

    // import all done - no more data

    m_ImportComplete = true;

 

    // verify that everything ok

    GPDEBUG_ONLY( CheckSanity() );

 
*/
    // done

    return ( success );

}

