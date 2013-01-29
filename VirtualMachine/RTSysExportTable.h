
#ifndef RT_SYS_EXPORT_TABLE_H_INCLUDED
#define RT_SYS_EXPORT_TABLE_H_INCLUDED

#include "../Singleton.h"
#include "../NativeFunction.h"
#include "../CodeHeader.h"

#include <string>
#include <map>
#include <string.h>


#define Q(string) #string
#define EXPORT(x) { RTSysExportTable::GetSingleton().insert(Q(x), (void*)(x)); }




class RTSysExportTable : public Singleton<RTSysExportTable>
{
private:
  typedef std::map<std::string, void*> NameToIdMap;
  typedef std::map<tag_t, NativeFunction> IdToFuncMap;

  NameToIdMap name2Id;
  IdToFuncMap id2Func;

public:

  void insert(const std::string& name, void* func)
  {
    name2Id[name] = func;
  }

  bool match(NativeFunction ne, tag_t id);

  NativeFunction& get(const FunctionEntry& fe)
  {
    return id2Func[fe.nativeID];
  }
};


#endif //RT_SYS_EXPORT_TABLE_H_INCLUDED

