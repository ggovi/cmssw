#include "CondCore/CondDB/interface/Streamer.h"
#include "CondCore/CondDB/interface/Exception.h"
#include "CondCore/CondDB/interface/Utils.h"
#include "FWCore/PluginManager/interface/PluginCapabilities.h"
//
#include <sstream>
// root includes 
#include "TBufferFile.h"
#include "TClass.h"
#include "Cintex/Cintex.h"

namespace conddb {

  static bool cintexInitialized = false;
  // initialize Cintex and load dictionary when required
  TClass* lookUpDictionary( const std::type_info& sourceType ){
    if (!cintexInitialized) {
      ROOT::Cintex::Cintex::Enable();
      cintexInitialized = true;
    }
    TClass* rc = TClass::GetClass(sourceType);
    if( !rc ){
      static std::string const prefix("LCGReflex/");
      std::string name = demangledName(sourceType);
      edmplugin::PluginCapabilities::get()->load(prefix + name);
      rc = TClass::GetClass(sourceType);
    }
    return rc;
  }

  // initialize Cintex and load dictionary when required
  TClass* lookUpDictionary( const std::string& typeName ){
    if (!cintexInitialized) {
      ROOT::Cintex::Cintex::Enable();
      cintexInitialized = true;
    }
    TClass* rc = TClass::GetClass(typeName.c_str());
    if( !rc ){
      static std::string const prefix("LCGReflex/");
      edmplugin::PluginCapabilities::get()->load(prefix + typeName);
      rc = TClass::GetClass( typeName.c_str() );
    }
    return rc;
  }

  // functor to inject in the shared_ptr to enable the deallocation
  class RootDeleter{

    public:
    RootDeleter( TClass* rclass ):
      m_rclass( rclass ){
    }

    RootDeleter( const RootDeleter& rhs ):
      m_rclass( rhs.m_rclass ){
    }

    ~RootDeleter(){}

    void operator()( void* ptr ){
      m_rclass->Destructor( ptr );
    }
    
  private:
    TClass* m_rclass;
    
  };


}

conddb::RootOutputArchive::RootOutputArchive( std::ostream& dest ):
  m_buffer( dest ){
} 

void conddb::RootOutputArchive::write( const std::type_info& sourceType, const void* sourceInstance){
  TClass* r_class = lookUpDictionary( sourceType );
  if (!r_class) throwException( "No ROOT class registered for \"" + demangledName(sourceType)+"\"", "RootOutputArchive::write");
  TBufferFile buffer(TBufferFile::kWrite);
  buffer.InitMap();
  buffer.StreamObject(const_cast<void*>(sourceInstance), r_class);
  // copy the stream into the target buffer
  m_buffer.write( static_cast<const char*>(buffer.Buffer()), buffer.Length() ); 
}

conddb::RootInputArchive::RootInputArchive( const std::stringbuf& source ):
  m_buffer( source.str() ),
  m_streamer( new TBufferFile( TBufferFile::kRead, m_buffer.size(), const_cast<char*>(m_buffer.c_str()), kFALSE ) ){
  m_streamer->InitMap();
}

conddb::RootInputArchive::~RootInputArchive(){
  delete m_streamer;
}

void conddb::RootInputArchive::read( const TClass* destinationClass, void* destinationInstance ) const {
  m_streamer->StreamObject(destinationInstance, destinationClass);     
}

void conddb::RootInputArchive::read( const std::type_info& destinationType, void* destinationInstance) const {
  TClass* r_class = lookUpDictionary( destinationType );
  if (!r_class) throwException( "No ROOT class registered for \"" + demangledName(destinationType) +"\"","RootInputArchive::read");
  read( r_class, destinationInstance);
}

boost::shared_ptr<void> conddb::ObjectLoader<conddb::RootInputArchive>::load( const std::type_info& destinationType, 
									      const std::string& objectTypeName ){
  TClass* destClass = lookUpDictionary( destinationType );
  TClass* objClass = lookUpDictionary( objectTypeName );
  if( !objClass->InheritsFrom( destClass ) ) 
    throwException( "Destination type and object type are unrelated.","ObjectLoader<RootInputArchive>::load" );
  void* obj = objClass->New();
  m_archive.read( destClass, obj );
  return boost::shared_ptr<void>( obj, RootDeleter( destClass ) );
}

conddb::OutputStreamer::OutputStreamer():
  m_data(){
}

const conddb::Binary& conddb::OutputStreamer::data() const{
  return m_data;
}

conddb::InputStreamer::InputStreamer( const std::string& payloadType, const Binary& payloadData ):
  m_objectType( payloadType ),
  m_buffer(){
  std::ostream s(&m_buffer);
  s.write( static_cast<const char*>(payloadData.data()), payloadData.size() );
}

