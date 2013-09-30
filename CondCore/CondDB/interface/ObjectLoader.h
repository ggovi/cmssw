#ifndef CondCore_CondDB_ObjectLoader_h
#define CondCore_CondDB_ObjectLoader_h
//
// Package:     CondDB
// Class  :     ObjectLoader
// 
/**\class ObjectLoader ObjectLoader.h CondCore/CondDB/interface/ObjectLoader.h
   Description: 
*/
//
// Author:      Giacomo Govi
// Created:     September 2013
//
//
#include "CondCore/CondDB/interface/Utils.h" 
//
#include <boost/shared_ptr.hpp>

namespace conddb {

  class SingleTypeLoaderBase {
  public:
    SingleTypeLoaderBase(){}
    virtual ~SingleTypeLoaderBase(){}
    //virtual boost::shared_ptr<void> load( const InputStreamer& streamer  ) = 0; 
  };

  template <typename T> class SingleTypeLoader: public  SingleTypeLoaderBase {
  public:
    SingleTypeLoader():
      SingleTypeLoaderBase(){}
    //boost::shared_ptr<void> load( const InputStreamer& streamer ) {
    //  return streamer.read<T>();
    //}
  };

  //static std::map<std::string,SingleTypeLoaderBase*> s_loaders_map;

  template <typename Archive> class ObjectLoader {
  public:
    ObjectLoader( Archive& arch ):
      m_archive( arch ){
    }
    template <typename T>
    boost::shared_ptr<T> load( const std::string& objectTypeName ){
      //if( demangledName( typeid(T) )== objectTypeName ){
      //	
      //} else {
	// polymorphism not yet supported...
      //	throwException( )
      //}
      boost::shared_ptr<T> obj( new T );
      m_archive >> obj;
      return obj;
    }
  private:
    Archive& m_archive;
  };

}

#endif
