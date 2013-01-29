

#include "RTSysExportTable.h"


bool RTSysExportTable::match(NativeFunction ne, tag_t id)
{
  //search for *proc

  NameToIdMap::iterator it;
  it = name2Id.find(ne.name);

  if (it == name2Id.end())
  {
    //raise error: no primitive available in this VM
    return false;
  }

  ne.proc = (*it).second;

  //che bisogno ho io a run time dei tipi
  //parametri? in realta' non mi servono,
  //li ho gia' sullo stack quando eseguo una CALL

  //create a new entry
  id2Func[id] = ne;
  return true;
}



RTSysExportTable RTSysExportTableSingleton;

