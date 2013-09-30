#ifndef CondCore_CondDB_Streamer_h
#define CondCore_CondDB_Streamer_h
//
// Package:     CondDB
// Class  :     Streamer
// 
/**\class Streamer Streamer.h CondCore/CondDB/interface/Streamer.h
   Description: functions for streaming the payload objects.  
*/
//
// Author:      Giacomo Govi
// Created:     May 2013
//
//

#include "CondCore/CondDB/interface/Binary.h"
#include "CondCore/CondDB/interface/Exception.h" 
#include "CondCore/CondDB/interface/ObjectLoader.h"
#include "CondCore/CondDB/interface/Utils.h" 
//
#include <sstream>
#include <iostream>
//
// temporarely
#include <boost/shared_ptr.hpp>

class TBufferFile;
class TClass;

namespace conddb {

  // Archives for the streaming based on ROOT.

  // output
  class RootOutputArchive {
  public:
    explicit RootOutputArchive( std::ostream& destination );

    template <typename T>
    RootOutputArchive& operator<<( const T& instance );
  private:
    // type and ptr of the object to stream
    void write( const std::type_info& sourceType, const void* sourceInstance);
  private:
    // here is where the write function will write on...
    std::ostream& m_buffer;
  };

  template <typename T> inline RootOutputArchive& RootOutputArchive::operator<<( const T& instance ){
    write( typeid(T), &instance );
    return *this;
  }

  // input
  class RootInputArchive {
  public:
    explicit RootInputArchive( const std::stringbuf& source );

    virtual ~RootInputArchive();

    template <typename T>
    RootInputArchive& operator>>( T& instance );
  private:
    friend class ObjectLoader<RootInputArchive>;
    // type and ptr of the object to restore
    void read( const TClass* destinationClass, void* destinationInstance ) const ;
    void read( const std::type_info& destinationType, void* destinationInstance) const ;
  private:
    // copy of the input stream. is referenced by the TBufferFile.
    std::string m_buffer;
    TBufferFile* m_streamer = nullptr;
  };

  template <typename T> inline RootInputArchive& RootInputArchive::operator>>( T& instance ){
    read( typeid(T), &instance );
    return *this;
  }

  template <> class ObjectLoader<RootInputArchive> {
  public:
    ObjectLoader( RootInputArchive& arch ):
      m_archive( arch ){
    }
    template <typename T>
    boost::shared_ptr<T> load( const std::string& objectTypeName ){
      return boost::static_pointer_cast<T>( load( typeid(T), objectTypeName ) );
    }

    boost::shared_ptr<void> load( const std::type_info& destinationType, const std::string& objectTypeName );
      
  private:
    
    RootInputArchive& m_archive;
  };

  // Generic streaming classes. Currently based on root. Could be a template class?

  class OutputStreamer {
  public:
    OutputStreamer();

    template <typename T> void write( const T& payload );

    const Binary& data() const;
  private:
    Binary m_data;
  };

  template <typename T> inline void OutputStreamer::write( const T& payload ){
    // save data to buffer
    std::stringbuf buffer;
    { 
      std::ostream s(&buffer);
      RootOutputArchive oa(s);
      oa << payload;
    } 
    m_data.copy( buffer.str() );
  }

  class InputStreamer {
  public:
    InputStreamer( const std::string& payloadType, const Binary& payloadData );

    template <typename T> boost::shared_ptr<T> read() const;

  private:
    std::string m_objectType;
    std::stringbuf  m_buffer;
  }; 

  template <typename T> inline boost::shared_ptr<T> InputStreamer::read() const {
    // for the moment we fail if types don't match... later we will check for base types...
    if( demangledName( typeid(T) )!= m_objectType ) throwException(std::string("Type mismatch, target object is type \"")+m_objectType+"\"",
								   "OutputStreamer::read" );
    boost::shared_ptr<T> payload;
    {
      RootInputArchive ia(m_buffer);
      ObjectLoader<RootInputArchive> loader(ia);  
      boost::shared_ptr<void> obj = loader.load<T>( m_objectType );
      payload = boost::static_pointer_cast<T>( obj );
    }
    return payload;
  }

}
#endif
